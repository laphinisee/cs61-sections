PROGRAMS = matrixmultiply $(patsubst %.cc,%,$(wildcard matrixmultiply-*.cc))

all: $(PROGRAMS) $(DATAFILE)

ALLPROGRAMS = $(PROGRAMS)

O = 2
include ../common/rules.mk
-include config.mk

%.o: %.cc $(BUILDSTAMP)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEPCFLAGS) $(O) -o $@ -c $<

matrixmultiply: matrixmultiply.o
	$(CXX) $(CXXFLAGS) $(O) $(LDFLAGS) -o $@ $^ $(LIBS)

matrixmultiply%: matrixmultiply%.o
	$(CXX) $(CXXFLAGS) $(O) $(LDFLAGS) -o $@ $^ $(LIBS)


clean:
	rm -f $(ALLPROGRAMS) *.o
	rm -rf $(DEPSDIR) *.dSYM

.PHONY: all always clean
