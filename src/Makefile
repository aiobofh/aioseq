
#
# Google Test and Google Mock subversion trunk urls.
#

Q=@
ifneq ($(MAKECMDGOALS),aioseq)
  CFLAGS += -Werror -Wall -pedantic -std=c++11 -g -fprofile-arcs -ftest-coverage
else
  CFLAGS += -std=c++11 -O2
endif

ifeq ($(DEBUG),true)
  CFLAGS += -DDEBUG
endif

export LINES
export COLUMNS

all: check

include gtest.mk
include gmock.mk
include gcovr.mk
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

aioseq-pattern-editor: $(filter-out $(wildcard *test.cc),$(subst .cc,.o,$(wildcard *.cc)))
	$(Q)$(CXX) -o $@ $^

aioseq: $(subst .cc,.o,$(filter-out %test.cc,$(wildcard *.cc)))
	$(Q)echo $^
	$(Q)$(CXX) -o $@ $^

clean: gtest-clean gmock-clean gcovr-clean doxygen-clean
	$(Q)rm -rf *.o *.d *.dh *~ GPATH GRTAGS GSYMS GTAGS aioseq aioseq-pattern-editor *_test Doxyfile* *.doxygen.log

clean-all: gtest-clean-all gmock-clean-all gcovr-clean-all clean

info:
	echo "Foobar"

ifneq ($(MAKECMDGOALS),clean)
  ifneq ($(filter-out %.d,$(MAKECMDGOALS)),)
    -include $(addprefix mock_,$(filter-out vector_of.dh,error.dh,debug.dh,mock_.dh,$(subst .hh,.dh,$(filter-out mock_%.hh,$(wildcard *.hh)))))
    -include $(subst .cc,.d,$(wildcard *.cc))
  endif
endif
