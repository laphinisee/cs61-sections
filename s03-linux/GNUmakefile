# build all programs with names like `membug[0-9]`
PROGRAMS = quotes attackme attackme.unsafe

all: $(PROGRAMS)

O = 1
include ../common/rules.mk

UNSAFEFLAGS := -U_FORTIFY_SOURCE -fno-stack-protector -no-pie

%.o: %.cc $(BUILDSTAMP)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEPCFLAGS) $(O) -o $@ -c $<

%.unsafe.o: %.cc $(BUILDSTAMP)
	$(CXX) $(CPPFLAGS) $(UNSAFEFLAGS) $(CXXFLAGS) $(DEPCFLAGS) $(O) -o $@ -c $<

%.unsafe.s: %.cc GNUmakefile
	@ARGS=$$(grep '^//!' $< | sed 's/.*! *//'); \
	  CXXFLAGS="`echo "$(UNSAFEFLAGS) $(CXXFLAGS) -O1" | sed 's/ *-g */ /'`"; \
	  if test -z "$$ARGS"; then ARGS="$(DEFAULT_ASM_CXXFLAGS)"; fi; \
	  $(call xrun,$(CXX) $$CXXFLAGS $$ARGS -o $@ -S $<,COMPILE -S $$ARGS $<) && { $(call cleanasm,$@); }


quotes: %: %.o
	$(CXX) $(CXXFLAGS) $(O) -o $@ $^ $(LIBS)

attackme: attackme.o
	@$(call xrun,$(CXX) $(CXXFLAGS) $(O) $(LDFLAGS) -o $@ $^ $(LIBS),LINK $@)

attackme.unsafe: attackme.unsafe.o
	@$(call xrun,$(CXX) $(CXXFLAGS) $(UNSAFEFLAGS) $(O) $(LDFLAGS) -o $@ $^ $(LIBS),LINK $@)


clean: clean-main
clean-main:
	$(call run,rm -f $(ALLPROGRAMS) *.o core *.core,CLEAN)
	$(call run,rm -rf out $(DEPSDIR) *.dSYM)

.PHONY: all clean clean-main
