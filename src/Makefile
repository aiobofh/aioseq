#
# Makefile for aioseq.
#

.SUFFIXES:

WGET=wget
UNZIP=unzip
CPPCHECK=cppcheck
CXX=g++
GCOV=gcov
DOXYGEN=doxygen

CFLAGS=-Igtest-1.6.0/include -Igmock-1.6.0/include -Werror -fprofile-arcs -ftest-coverage

gcovr:
	@$(WGET) "https://software.sandia.gov/trac/fast/export/2825/gcovr/trunk/scripts/gcovr"

gtest-1.6.0.zip:
	@$(WGET) "https://googletest.googlecode.com/files/gtest-1.6.0.zip"

gtest-1.6.0: gtest-1.6.0.zip
	@$(UNZIP) -o $<

gmock-1.6.0.zip:
	@$(WGET) "https://googlemock.googlecode.com/files/gmock-1.6.0.zip"

gmock-1.6.0: gmock-1.6.0.zip
	@$(UNZIP) -o $<

gtest.o: gtest-1.6.0/src/gtest-all.cc gtest-1.6.0
	@$(CXX) -o $@ -c $< -Igtest-1.6.0 -Igtest-1.6.0/include

gmock.o: gmock-1.6.0/src/gmock-all.cc gmock-1.6.0
	@$(CXX) -o $@ -c $< -Igmock-1.6.0 -Igmock-1.6.0/include -Igtest-1.6.0/include

gtest_main.o: gtest-1.6.0/src/gtest_main.cc gtest-1.6.0
	@$(CXX) -o $@ -c $< -Igtest-1.6.0 -Igtest-1.6.0/include

-include $(wildcard *.d)

.PRECIOUS: %.o
%.o: %.cpp Makefile gcovr
	@$(CPPCHECK) -q --error-exitcode=1 $<
	@$(CXX) -o $@ -c $< $(CFLAGS) -MD

.PRECIOUS: %Test
%Test: %Test.o %.o gtest.o gtest_main.o gmock.o
	@$(CXX) -o $@ $^ -lpthread -lgcov

check: $(subst Test.cpp,Test,$(wildcard *Test.cpp))
	@for i in $^; do echo $$i:; echo; ./$$i --gtest_color=yes --gtest_output=xml:$$i.xml; echo; $(GCOV) `echo $$i | sed 's/Test//'`.cpp | head -2; echo; done
	@python gcovr -e '.*gtest-1.6.0.*' -e '.*gmock-1.6.0.*' -e '/usr.*' -e '.*Test.cpp' -x > coverage.xml
	@python gcovr -e '.*gtest-1.6.0.*' -e '.*gmock-1.6.0.*' -e '/usr.*' -e '.*Test.cpp'

doc:
	@$(DOXYGEN) Doxyfile

clean:
	@rm -rf *.d *.o *Test *~ *.xml *.gcov *.gcda *.gcno doc

clean-all: clean
	@rm -rf gtest* gmock*
