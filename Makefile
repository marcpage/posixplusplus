CXX=clang++
CPPFLAGS=-std=c++14 -Wall -Weffc++ -Wextra -Wshadow -Wwrite-strings -Werror
CPPFLAGS+=-fno-optimize-sibling-calls -O0 -g
CPPFLAGS+=-fsanitize=address -fsanitize-address-use-after-scope -fsanitize=undefined
SOURCEDIR=src/tests
OUTPUTDIR=bin

$(OUTPUTDIR)/test_Exception.d:$(SOURCEDIR)/test_Exception.cpp
	mkdir -p $(OUTPUTDIR)
	$(CXX) -MM $(CPPFLAGS) $< | sed -e 's,\($*\)\.o[ :]*,\1 $@: ,g' > $@

$(OUTPUTDIR)/test_Exception:$(SOURCEDIR)/test_Exception.cpp $(OUTPUTDIR)/test_Exception.d
	mkdir -p $(OUTPUTDIR)
	echo $< -> $@
	$(CXX) $< $(CPPFLAGS) -o $@

test:$(OUTPUTDIR)/test_Exception
	./$(OUTPUTDIR)/test_Exception

clean:
	rm -Rf $(OUTPUTDIR)

-include $(OUTPUTDIR)/test_exception.d