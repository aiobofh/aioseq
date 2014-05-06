
#
# Google Test and Google Mock subversion trunk urls.
#

Q=@
export LINES
export COLUMNS

CXXVERSION=$(shell $(CXX) --version | head -1 | sed -r 's/.* //g;s/\.[0-9]*$$//g')

ifeq ($(CXXVERSION), '4.8')
  CXXFLAGS = -std=c++11
endif

ifneq ($(MAKECMDGOALS),aioseq)
  CFLAGS += -Werror -Wall -pedantic -std=c++11 -g -fprofile-arcs -ftest-coverage
  include gtest.mk
  include gmock.mk
  include gcovr.mk
else
  CFLAGS += -Werror -Wall -pedantic -std=c++11 -g -DMAIN=main
endif

ifeq ($(DEBUG),true)
  CFLAGS += -DDEBUG
endif

all: check

include doxygen.mk

.SUFFIXES:

check-syntax: $(CHK_SOURCES)
	$(CXX) $(CFLAGS) $(GTEST_CFLAGS) $(GMOCK_CFLAGS) -c $(CHK_SOURCES); \
	rm -rf $(CHK_SOURCES)

.PRECIOUS: %.d
%.d: %.cc mock_client_primitive.hh $(addprefix mock_,$(filter-out mock_%.hh debug.hh error.hh test.hh vector_of.hh,$(wildcard *.hh)))
	$(Q)$(CXX) -MM -MG $(CFLAGS) $< | sed -r 's/mock_mock[^ $$]*//g' | grep -v -e '^ \\'>> $@

.PRECIOUS: %.dh
mock_%.dh: %.hh
	$(Q)../tools/mock_dependencies.sh $< | sed -r 's/mock_mock[^ $$]*//g' > $@

.PRECIOUS: %.o
%.o: %.cc
	$(Q)$(CXX) -o $@ -c $< $(CFLAGS) $(GTEST_CFLAGS) $(GMOCK_CFLAGS)

#aioseq-pattern-editor: $(subst .cc,.o,$(filter-out $(wildcard *test.cc),$(wildcard *.cc)))
#	$(Q)$(CXX) -o $@ $^

#aioseq: $(subst .cc,.o,$(filter-out %test.cc,$(wildcard *.cc)))
aioseq: $(subst .cc,.o,$(filter-out $(wildcard *test.cc),$(wildcard *.cc)))
	echo $^
	$(Q)$(CXX) -o $@ $^

clean: gtest-clean gmock-clean gcovr-clean doxygen-clean
	$(Q)rm -rf *.o *.gch *.d *.dh *~ GPATH GRTAGS GSYMS GTAGS aioseq *_test Doxyfile* *.doxygen.log

clean-all: gtest-clean-all gmock-clean-all gcovr-clean-all clean

info:
	echo $(CXXVERSION)
	echo "Foobar"

ifneq ($(MAKECMDGOALS),clean)
  ifneq ($(filter-out %.d,$(MAKECMDGOALS)),)
    -include $(addprefix mock_,$(filter-out vector_of.dh,error.dh,debug.dh,mock_.dh,$(subst .hh,.dh,$(filter-out mock_%.hh,$(wildcard *.hh)))))
    -include $(subst .cc,.d,$(wildcard *.cc))
  endif
endif
