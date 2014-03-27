#
# doxygen.mk
#
# Targets to generate doxygen output both for the whole project or for just one
# file in a fast way.
#

.PRECIOUS: Doxyfile
Doxyfile: ../tools/doxyfile_generator.sh
	$(Q)../tools/doxyfile_generator.sh > $@

.PRECIOUS: %.cc.doxyfile
%.cc.doxyfile: %.cc Doxyfile
	$(Q)../tools/doxyfile_dependency_generator.sh $< > $@

.PRECIOUS: %.cc.doxygen.log
%.cc.doxygen.log: %.cc.doxyfile
	$(Q)doxygen $< >/dev/null; \
	grep 'Warning:' $@ >/dev/null && (cat $@; rm $@) || true

doc: Doxyfile
	$(Q)doxygen $< >/dev/null

doxygen-clean:
	$(Q)rm -rf *.doxygen.log *.doxyfile Doxyfile html latex man
