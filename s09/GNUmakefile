PROGRAMS = sample01 sample02 sample03 onetwothree bathroom1 bathroom2
all: $(PROGRAMS)

WANT_TSAN = 1
include ../common/rules.mk
LIBS = -lpthread

%.o: %.cc $(BUILDSTAMP)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEPCFLAGS) $(O) -o $@ -c $<

$(PROGRAMS): %: %.o
	$(CXX) $(CXXFLAGS) $(O) -o $@ $^ $(LIBS)

clean:
	$(call run,rm -f $(PROGRAMS) *.o *.dSYM *.core,CLEAN)

.PHONY: all clean
