#
# gmock.mk
#
# Makefile for building Google-Mock related files.
#

GMOCKURL=http://googlemock.googlecode.com/svn/trunk/
GMOCK=googlemock-read-only
GMOCK_CFLAGS=-I$(GMOCK) -I$(GMOCK)/include -I$(GTEST)/include
GMOCK_GEN=$(GMOCK)/scripts/generator/gmock_gen.py

#
# The Google Mock or gmock framework is downloaded if not already done so that
# the code can be compiled with a fresh version of gmock.
#
.PRECIOUS: $(GMOCK)/src/gmock-all.cc
$(GMOCK)/src/gmock-all.cc: $(GTEST)/src/gtest-all.cc
	$(Q)svn checkout $(GMOCKURL) $(GMOCK) && touch $@

#
# The gmock.o file is used when linking an executable containing test cases
# written using the gmock API.
#
gmock.o: $(GMOCK)/src/gmock-all.cc
	$(Q)$(CXX) -o $@ -c $^ $(GMOCK_CFLAGS)

mock_client.hh: mock_pattern_client.hh mock_track_client.hh mock_part_client.hh

#
# Generate mocks for all *_interface.hh files.
#
.PRECIOUS: mock_%.hh
mock_%.hh: %.hh ../tools/generate_mock.sh $(GMOCK)/src/gmock-all.cc
	$(Q)../tools/generate_mock.sh $< $(GMOCK_GEN) >$@

gmock-clean:
	$(Q)rm -rf mock_*.hh

gmock-clean-all:
	$(Q)rm -rf $(GMOCK)
