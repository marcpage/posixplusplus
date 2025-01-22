CXX=clang++
CPPFLAGS=-std=c++14 -Wall -Weffc++ -Wextra -Wshadow -Wwrite-strings -Werror
CPPFLAGS+=-fno-optimize-sibling-calls -fprofile-arcs -ftest-coverage -O0 -g
CPPFLAGS+=-fsanitize=address -fsanitize-address-use-after-scope -fsanitize=undefined
SOURCEDIR=src/tests
OUTPUTDIR=bin

$(OUTPUTDIR)/test_PsxException.d:$(SOURCEDIR)/test_PsxException.cpp
	@mkdir -p $(OUTPUTDIR)
	@$(CXX) -MM $(CPPFLAGS) $< | sed -e 's,\($*\)\.o[ :]*,\1 $@: ,g' > $@

$(OUTPUTDIR)/test_PsxException:$(SOURCEDIR)/test_PsxException.cpp $(OUTPUTDIR)/test_PsxException.d
	@mkdir -p $(OUTPUTDIR)
	@echo "$< -> $@"
	@$(CXX) $< $(CPPFLAGS) -o $@

test:$(OUTPUTDIR)/test_PsxException
	@./$(OUTPUTDIR)/test_PsxException
	@gcov bin/*.gcno > $(OUTPUTDIR)/coverage.log
	@mkdir -p $(OUTPUTDIR)/coverage
	@mv *.gcov $(OUTPUTDIR)/coverage
	@cat $(OUTPUTDIR)/coverage/PsxException.h.gcov | grep -e '#####:' | grep -ve '// NOTEST' || true
	@cat $(OUTPUTDIR)/coverage/PsxException.h.gcov | grep -e '// NOTEST' | grep -ve '#####:' || true

clean:
	@rm -Rf $(OUTPUTDIR)

-include $(OUTPUTDIR)/test_exception.d