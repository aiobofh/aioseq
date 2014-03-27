#
# gcovr.mk
#

ifeq (${Q},@)
  gcovfilter= | egrep -v '^File' | egrep -v '^-' | grep -v '100%' || true
endif

.PRECIOUS: gcovr
#gcovr:
#	$(V)wget "https://software.sandia.gov/trac/fast/export/2825/gcovr/trunk/scripts/gcovr"

fuu: $(subst _test.cc,_test,$(wildcard *_test.cc))
	../tools/disttest . /mnt/nfs/aio/.disstest $^
	$(Q)gcovr -r . -e '.*google.*' -e '/usr.*' -e '.*_test.cc' -e '.*mock_.*.hh' -e '.*_interface.hh'


.PRECIOUS: coverage.xml
coverage.xml: $(subst _test.cc,_test.xml,$(wildcard *_test.cc))
	$(Q)gcovr -r . -e '.*google.*' -e '/usr.*' -e '.*_test.cc' -e '.*mock_.*.hh' -e '.*_interface.hh' -x > $@

.PHONY: check
check: coverage.xml
	${Q}gcovr -r . -e '.*google.*' -e '/usr.*' -e '.*_test.cc' -e '.*mock_.*.hh' -e '.*_interface.hh' ${gcovfilter}

gcovr-clean:
	$(Q)rm -rf coverage.xml *.gcda *.gcno

gcovr-clean-all: gcovr-clean
