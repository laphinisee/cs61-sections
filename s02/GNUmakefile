PROGRAMS = $(sort $(patsubst fun%.cc,fun%,$(wildcard fun[0-9][0-9].cc)) $(patsubst fun%.s,fun%,$(wildcard fun[0-9][0-9].s)))
ALLPROGRAMS = $(PROGRAMS)
FULLSOURCES = $(patsubst fun%.cc,fun%.s,$(sort $(wildcard fun[0-9][0-9].cc)))

all: $(PROGRAMS) $(FULLSOURCES)

O = s
PIE ?= 0
include ../common/rules.mk

%.o: %.cc $(BUILDSTAMP)
	$(CXX) $(CPPFLAGS) $(filter-out -g,$(CXXFLAGS)) $(O) $(DEPCFLAGS) -o $@ -c $<

%.s: %.cc $(BUILDSTAMP)
	$(CXX) $(CPPFLAGS) $(filter-out -g,$(CXXFLAGS)) $(O) $(DEPCFLAGS) -o $@ -S $<
	$(call cleanasm,$@)

fundriver.o: fundriver.cc $(BUILDSTAMP)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(O) $(DEPCFLAGS) -o $@ -c $<

%.o: %.s $(BUILDSTAMP)
	$(CXX) -o $@ -c $<

fun%: fun%.o fundriver.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(O) -o $@ $^


clean:
	rm -rf $(DEPSDIR)
	rm -f $(ALLPROGRAMS) *.o $(FULLSOURCES)

.PHONY: all clean
