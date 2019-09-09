# build all programs with names like `membug[0-9]`
MEMBUG_PROGRAMS = $(patsubst %.cc,%,$(sort $(wildcard membug[0-9].cc)))
SIMPLE_PROGRAMS = $(MEMBUG_PROGRAMS) objects names stl0 stl1 stl2
PROGRAMS = $(SIMPLE_PROGRAMS) testinsert0
ALLPROGRAMS = $(PROGRAMS) greet inv

all: $(PROGRAMS)

LIBS = -lm

include ../common/rules.mk

%.o: %.cc $(BUILDSTAMP)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEPCFLAGS) $(O) -o $@ -c $<

$(SIMPLE_PROGRAMS): %: %.o
	$(CXX) $(CXXFLAGS) $(O) -o $@ $^ $(LIBS)

testinsert0: qslib.o allowexec.o testinsert0.o
	$(CXX) $(CXXFLAGS) $(O) -o $@ $^ $(LIBS)

greet: greet.o
	$(CXX) $(CXXFLAGS) $(O) -o $@ $^ $(LIBS)

inv: inv.o
	$(CXX) $(CXXFLAGS) $(O) -o $@ $^ $(LIBS)

clean: clean-main
clean-main:
	$(call run,rm -f $(ALLPROGRAMS) *.o core *.core,CLEAN)
	$(call run,rm -rf out $(DEPSDIR) *.dSYM)

.PHONY: all clean clean-main
