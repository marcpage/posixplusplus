MINIMUM_CODE_COVERAGE=80
CXX=g++
CPPFLAGS=-std=c++14 -Wall -Weffc++ -Wextra -Wshadow -Wwrite-strings -Werror
CPPFLAGS+=-Wpedantic -pedantic-errors -Wdisabled-optimization
CPPFLAGS+=-Wcast-align -Wcast-qual
CPPFLAGS+=-Wfloat-equal -Wformat=2
CPPFLAGS+=-Wmissing-declarations -Wmissing-include-dirs
CPPFLAGS+=-Wpointer-arith -Wredundant-decls -Wsequence-point
CPPFLAGS+=-Wswitch -Wundef -Wunreachable-code
CPPFLAGS+=-Wunused-but-set-parameter -Wwrite-strings -Wctor-dtor-privacy
CPPFLAGS+=-fno-optimize-sibling-calls -fprofile-arcs -ftest-coverage -O0 -g
CPPFLAGS+=-fsanitize=address -fsanitize-address-use-after-scope -fsanitize=undefined
CPPFLAGS+=-fno-inline
CPPFLAGS+=-Winit-self -Wold-style-cast -Woverloaded-virtual
CPPFLAGS+=-Wsign-conversion -Wsign-promo
CPPFLAGS+=-Wstrict-overflow=5 -Wswitch-default -Wunused
SOURCEDIR=src/tests
OUTPUTDIR=bin
SOURCES=$(wildcard $(SOURCEDIR)/test_*.cpp)
TESTS=$(subst test_,,$(basename $(notdir $(SOURCES))))
COV_TOOL=$(OUTPUTDIR)/coverage
COV_SOURCE=$(SOURCEDIR)/coverage.cpp

$(COV_TOOL):$(COV_SOURCE)
	@mkdir -p $(OUTPUTDIR)
	@echo "$< -> $@"
	@$(CXX) $< $(CPPFLAGS) -o $@

define HANDLE_TEST
$(OUTPUTDIR)/$(1)/$(1):$(SOURCEDIR)/test_$(1).cpp
	@echo
	@mkdir -p $(OUTPUTDIR)/$(1)
	@echo "$(SOURCEDIR)/test_$(1).cpp -> $(OUTPUTDIR)/$(1)/$(1)"
	@$(CXX) $(SOURCEDIR)/test_$(1).cpp $(CPPFLAGS) -o $(OUTPUTDIR)/$(1)/$(1)

$(1):$(OUTPUTDIR)/$(1)/$(1) $(COV_TOOL)
	@echo
	@./$(OUTPUTDIR)/$(1)/$(1)
	@gcov $(OUTPUTDIR)/$(1)/*.gcno > $(OUTPUTDIR)/$(1)/$(1).log
	@mv *.gcov $(OUTPUTDIR)/$(1)/
	@$(COV_TOOL) $(1) $(OUTPUTDIR)/$(1)/$(1).log $(OUTPUTDIR)/$(1)/$(1).h.gcov $(MINIMUM_CODE_COVERAGE)

endef

$(foreach test,$(TESTS),$(eval $(call HANDLE_TEST,$(test))))

# Default target
test: $(TESTS)

clean:
	@$(CXX) --version
	@gcov --version
	@rm -Rf $(OUTPUTDIR)
