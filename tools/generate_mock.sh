#!/usr/bin/env bash
#
#
#

usage() {
    echo "USAGE: $0 [-h] | <in.hh> <gmock_gen.py>"
    echo ""
    echo " -h, --help  Show this help text."
    echo ""
    echo "This tool will create a mock_in.hh file if possible."
}

#
# Transform some_thing_like_this to SomeThingLikeThis
#
underscore2camelcase() {
    intext=$1
    outtext=$(echo $intext | sed -re 's/_([a-z])/\u\1/g' -e 's/^([a-z])/\u\1/g')
    echo -n "${outtext}"
}

#
# Transform foo_bar.hh into FooBar (With the assumption that the file name
# reflects a class name, otherwise we're screwed).
#
filename2classname() {
    file=$1
    name=$(basename $file .hh)
    if [ "$name" == "" ]; then
        name=$(basename $file .h)
    fi
    if [ "$name" == "" ]; then
        echo "ERROR: No .h or .hh file provided as input." >&2
        exit 1
    fi
    class=$(underscore2camelcase $name)
    echo -n "${class}"
}

#
# Find the class *Interface { ... } block and store it in a separate file fore
# faster access to it.
#
extract_interface_class_from_file_to_file() {
    header_file=$1
    interface_name=$2
    output_file=$3
    # First try to determine if there is an interface at all in the file.
    found=$(egrep "^class ${interface_name}" ${header_file})
    if [ "$found" != "" ]; then
        offset=$(egrep -n "^class ${interface_name}" ${header_file} | \
            cut -d':' -f1)

        toffset=$[$(wc -l ${header_file} | cut -d' ' -f1)-${offset}+1]

        length=$(tail -${toffset} ${header_file} | egrep -n '^};' | \
            head -1 | cut -d':' -f1)

        tail -${toffset} ${header_file} | head -${length} > ${output_file}
    else
#        echo "ERROR: Not able to generate mock, no ${interface_name} found" >&2
        exit 1
    fi
}

#
# Try to find the VECTOR_OF() macro and extract the list name as plural.
#
search_for_plural_name_in_file() {
    header_file=$1
    plural=$(egrep -e '^VECTOR_OF\(' ${header_file} | cut -d',' -f2 | \
        sed -e 's/ //g')

    echo "${plural}"
}

#
# Tries to extract the inherited "other" interfaces in an interface to make a
# list of mocks that need to be generated.
#
translate_inheritances_in_interface_to_mocks() {
    file=$1
    last_inherit_line=$(grep -n '{' $file | head -1 | cut -d':' -f1)
    mocks=$(head -$last_inherit_line $file \
        | sed -e 's/.*class.*://g;s/virtual //g;s/{//g;s/Interface//g;s/public /public Mock/g;s/\s*$//g' \
        | tr -d "\n" | sed 's/ \{1,\}/ /g' | sed -e 's/^class.*//g')

    if [ "$mocks" != "" ]; then
        mocks="$mocks "
    fi

    echo -n "${mocks}"
}

#
# Transform #include "foo.hh" into #include "mock_foo.hh" in most cases.
#
translate_includes_in_file_to_mock_file_names() {
    header_file=$1
    include_files=$(egrep '^#include "' ${header_file} \
        | egrep -v '(error.hh|vector_of.hh)' | sed -e 's/ "/ "mock_/g' \
        | sed -e 's/#include //g;s/"//g' | tr '\n' ' ')

    echo ${include_files}
}

#
# This will use the gmock_gen.py script on an extracted interface to produce a
# mock-up of it, and hopefully all inherited interfaces as well (just grabbing
# MOCK_METHOD-blocks from other mock_*.hh files generated earlier. Here you need
# to keep track of which files are needed when.
#
generate_mock_from_interface_to_file() {

    gmock_gen=$1
    interface_file=$2
    interface=$3
    mock_file=$4

    shift; shift; shift; shift

    include_mocks=$@

    $gmock_gen $interface_file \
        | sed -e 's/^};//' -e 's/>\*/\*>\*/g' -e "s/Interface :/ :/" -e "s/{/{\n\npublic:/" -e "s/ public://" > $mock_file

    echo "  /*
   * Interface compatible constructors (if any).
   */
" >> $mock_file
    egrep "^ +$interface\(.*\)" $interface_file \
        | sed -r 's/^( +)(.+)Interface\(/\1Mock\2\(/g' >> $mock_file

    echo "
  /*
   * Mocks from all inherrited interfaces (if any).
   */" >> $mock_file
    for i in $include_mocks; do
        class=$(filename2classname $i)
        interface_class=$(echo "${class}Interface" | sed -s 's/Mock//g')
        match=$(egrep "${interface_class}" ${interface_file})
        if [ "$match" == "" ]; then
#            echo "DEBUG: No ${interface_class} inherritances found ${interface_file}" >&2
            continue
        fi
        echo "
  // ${class}
" >> $mock_file
        # Hmm.. Are mocks always 2 lines long???
#        test -f $i || (echo "ERROR: $i needed for $mock_file" >&2; exit 1)
        test -f $i && grep -A1 MOCK_M $i | grep -v Die | grep -v '-' >> $mock_file
    done
    echo "
};
" >> $mock_file
}

#
# This is a nifty one. Generate a Fake-class from an implementation. If there are
# virtual methods in the implementation they are all mockable using this script,
# enabling you to unit-test methods in 100% isolation from eachother.
#
extract_fake_class_from_file_to_file() {

    header_file=$1
    class_name=$2
    template_name=$3
    output_file=$4
    offset=0

    skip=0

    # First try to determine if there is an interface at all in the file.
    found_template=$(egrep "^class ${template_name} " ${header_file})
    found_class=$(egrep "^class ${class_name} " ${header_file})
    if [ "$found_template" != "" ]; then

        offset=$(egrep -n "^class ${template_name} " ${header_file} \
            | cut -d':' -f1)

        constructor=$template_name
    elif [ "$found_class" != "" ]; then

        offset=$(egrep -n "^class ${class_name} " ${header_file} \
            | cut -d':' -f1)

        constructor=$class_name
    else
        echo "// No implementation found in ${header_file} will not create Fake." >> ${output_file}
        return
    fi

    friend_macro=$(generate_friend_macro_from_file_name ${header_file})

    toffset=$[$(wc -l ${header_file} | cut -d' ' -f1)-${offset}+1]

    length=$(tail -${toffset} ${header_file} | egrep -n '^};' | \
        head -1 | cut -d':' -f1)

    # Extract the class-virtuals and comment-out protection.
    echo "
class Fake${class_name} : ${class_name}Mock {

" > ${output_file}.tmp
    tail -${toffset} ${header_file} | head -${length} | \
        sed -e 's/private:/\/\/private:/;s/protected:/\/\/protected:/;s/public:/\/\/public:/' | awk 'BEGIN {
         RS = "\n" ;
         FS = "" ;
         d = 0 ;
     }

     {
         for (i=1; i<=NF; i++)
             if ($i == "{") {
                 d++ ;
                 if (d == 1) printf "{\n"
             } else
             if ($i == "}") {
                 d-- ;
                 if (d == 0) printf "}"
             } else
             if (d == 1)
                 printf "%s", $i ;
         if (d == 1) printf "\n"
     }' | egrep '\/\/.*:|virtual' \
         | grep -v 'virtual ~' \
         | sed -e 's/).*$/) = 0\;/g' >> ${output_file}.tmp
    echo '  '"$constructor"'() {}' >> ${output_file}.tmp
    echo "" >> ${output_file}.tmp
    echo "};" >> ${output_file}.tmp

    echo "/**
 * Fake-version of ${class_name} to be able to unit-test every method in a nice
 * way, mocking out all others. Use method call by ::${class}Mock.method_name on
 * an object of Fake${class_name}.
 */" > ${output_file}
    $gmock_gen ${output_file}.tmp | \
#        sed -e 's/^};//' -e 's/>\*/\*>\*/g' -e "s/Interface :/ :/" -e "s/{/{\n\npublic:/" -e "s/ public://" -e 's/class Mock/class /' -e 's/public Fake//' -e 's/ {/Mock {/' -e 's/public:/'"${friend_macro}"'\n\npublic:/'>> ${output_file}
        sed -e 's/^};//' -e 's/>\*/\*>\*/g' -e "s/Interface :/ :/" -e "s/{/{\n\npublic:/" -e "s/ public://" -e 's/class Mock/class /' -e 's/public Fake//' -e 's/ {/ {/' -e 's/public:/'"${friend_macro}"'\n\npublic:/'>> ${output_file}

    # Find the constructors of the original class and copy them.
    echo "  /*
   * Implementation compatible constructors (if any).
   */
" >> ${output_file}
    IFS="
"
    for i in $(egrep '^ +'"$constructor"'\(.*\)' ${header_file}); do
        i=$(echo $i | sed 's/\:.*$/;/g')
#        i=$(echo $i | sed 's/'"$constructor"'/'"$class"'Mock/g;s/ *:/;/g')
        i=$(echo $i | sed 's/'"$constructor"'/'"$class"'/g;s/ *:/;/g')
#        echo "DEBUG: $i" >&2
        args=$(echo $i | sed -e 's/.*\((.*)\).*[;|:]/\1/g' -e 's/(//g;s/)//g' \
            -e 's/[^\,]*\s//g' -e 's/,/, /g')
#        echo "DEBUG: args: $args" >&2
#            | sed -r 's/^( +)'"$constructor"'\(/\1Fake'"$class"'\(/g' \
        echo $i \
            | sed -r 's/^( +).*\(/\1Fake'"$class"'\(/g' \
            | sed -r 's/\((.*)\).*[;|:]/(\1) : '"$class"'\('"$args"') {}/g' >> ${output_file}
#            | sed -r 's/\((.*)\).*[;|:]/(\1) : '"$class"'Mock\('"$args"') {}/g' >> ${output_file}
    done

#    rm -f ${output_file}.tmp

#    echo "${class}* base_class() { return dynamic_cast<${class}*>(this); }" >> ${output_file}

    echo "
};" >> ${output_file}
}

#
# Transform foo_bar.hh into _FOO_BAR_HH_
#
generate_include_guard_from_file_name() {
    echo "_$1_" | sed -e 's/\./_/g' | tr [:lower:] [:upper:]
}

#
# Transfor foo_bar.hh into FOO_BAR_FRIENDS
#
generate_friend_macro_from_file_name() {
    echo "$1" | sed -e 's/.hh/_friends/g' | tr [:lower:] [:upper:]
}

#
# Make a fake-base-class (usffixed by Mock) to enable the user to access methods
# in the original code, bypassing the fake, for example when testing a method
# in isolation.
#
generate_fake_base_class() {
    header_file=$1
    class=$2
    template=$3
    output_file=$4
    grep -Pzo '(?s)^typedef\s+'"$template"'\<.*;' ${header_file} | \
        sed -r 's/(<)(.*)(,|>)/\1Mock\2\3/g;s/^( +)(.*)(,|>)/\1Mock\2\3/g;s/(, *)(.*)([,|>])/\1Mock\2\3/g;s/> +(.*);/> \1Mock;/' > ${output_file}
    if [ "$(cat $output_file)" == "" ]; then
#        echo "DEBUG: Looking for 'class ${class} ' in ${header_file}" >&2
        found=$(grep "class ${class} " ${header_file})
        if [ "$found" == "" ]; then
            echo "// No typedef class possible" > ${output_file}
        else
            echo "typedef ${class} ${class}Mock;" > ${output_file};
        fi
    fi
}

header_file=$1
gmock_gen=$2

if [ "${header_file}" == "" ] || [ "${gmock_gen}" == "" ]; then
    echo "ERROR: No arguments provided, -h or --help for more information" >&2
    exit 1
fi

tmp_interface_file="/tmp/${header_file}.interface"
tmp_interface_mock_file="/tmp/${header_file}.interface_mock"
tmp_faketype_file="/tmp/${header_file}.faketype"
tmp_fake_file="/tmp/${header_file}.fake"

#
# Do some name magic to get the class name in CamelCase and try to find the
# plural version of the class name in CamelCase as well.
#
class_name=$(filename2classname ${header_file})
plural=$(search_for_plural_name_in_file ${header_file})

#
# Extract the interface class from the header file to a temproary file.
#
interface_name="${class_name}Interface"
extract_interface_class_from_file_to_file ${header_file} ${interface_name} \
    ${tmp_interface_file}

#
# Extract the implementatino class from the header file to temporary file.
#
template_name="${class_name}Template"

#
# Copy and rename all the inheritancees into inheritances of mocks from the
# interface class.
#
inheritance_mocks=$(translate_inheritances_in_interface_to_mocks \
    ${tmp_interface_file})

#
# Extract all the includes and try to figure out which are needed to be include
# but with the mock_-prefix in the generated mock-file.
#
include_files=$(translate_includes_in_file_to_mock_file_names ${header_file})

mock_name="Mock${class_name}"

#
# Output the generated mock-up to a temporary file with all inherited methods
# mocked as well, and some other neat stuff, like APi compatible constructors.
#
generate_mock_from_interface_to_file ${gmock_gen} ${tmp_interface_file} ${interface_name} ${tmp_interface_mock_file} ${include_files}

#
# Make a *Mock type definition to make it possible to bypass fake-mock-calls and
# write it to a temporary file.
#
generate_fake_base_class ${header_file} ${class_name} ${template_name} ${tmp_faketype_file}

#generate_fake_from_header_file ${gmock_gen} ${header_file}
extract_fake_class_from_file_to_file ${header_file} ${class_name} ${template_name} ${tmp_fake_file}

friend_macro=$(generate_friend_macro_from_file_name ${header_file})

include_guard=$(generate_include_guard_from_file_name "mock_${header_file}")

echo "/**
 * @file mock_${header_file}
 *
 * This is a generated mock-up and fake-up of the ${header_file} file.
 *
 * NOTE: DO NOT EDIT BY HAND!
 */
#ifndef ${include_guard}
#define ${include_guard}

/*
 * Failsafe, overridden by setting a list of friend classes in the test-suite.
 */
#ifndef ${friend_macro}
#define ${friend_macro}
#endif

#include <gmock/gmock.h>
#include \"${header_file}\" // Everything in this file is based on this."
IFS=" "
for i in ${include_files}; do
    echo "#include \"${i}\" // $(echo ${i} | sed -e 's/mock_//g')"
done
if [ "$plural" != "" ]; then
    echo '#include "vector_of.hh"'
fi
echo "

/**
 * Mock-up based on the ${interface_name} pure virtual interface.
 */
$(cat ${tmp_interface_mock_file})


/**
 * Convenience type to be able to call real method via the Fake class via class
 * name.
 */
$(cat ${tmp_faketype_file})


$(cat ${tmp_fake_file})
"
if [ "$plural" != "" ]; then
    echo "

/**
 * Mock-up based on the ${plural}Interface pure virtual interface.
 */
class Mock${plural} : virtual public ${plural}Interface,
                      virtual public vector<Mock${class_name}*> {

public:

  Mock${plural}() : vector<Mock${class_name}*>() {}

};


class Mock${plural}Die : public Mock${plural} {

public:

  MOCK_METHOD0(Die, void());
  virtual ~Mock${plural}Die() { Die(); };

};"
fi
echo ""
echo "/**"
echo " * Fast way to create mockups of this class with the factory template."
echo " */"
grep -Pzo '(?s)^typedef.*FactoryTemplate\<.*;' $header_file | sed -r 's/(<)(.*)(,|>)/\1Mock\2\3/g;s/^( +)(.*)(,|>)/\1Mock\2\3/g;s/(, *)(.*)([,|>])/\1Mock\2\3/g;s/> +(.*);/> Mock\1;/'

echo ""
echo "/**"
echo " * Fast way to create real objects of this class but with everything else"
echo " * mocked up with the factory template."
echo " */"
grep -Pzo '(?s)^typedef.*FactoryTemplate\<.*;' $header_file | sed -r 's/(<)(.*)(,|>)/\1Mock\2\3/g;s/^( +)(.*)(,|>)/\1Mock\2\3/g;s/(, *)(.*)([,|>])/\1Mock\2\3/g;s/> +(.*);/> \1Mock;/' | sed -e 's/<Mock/</g'

echo "
#endif
"

#rm -f ${tmp_interface_file}
#rm -f ${tmp_interface_mock_file}
#rm -f ${tmp_faketype_file}
#rm -f ${tmp_fake_file}

exit 0
