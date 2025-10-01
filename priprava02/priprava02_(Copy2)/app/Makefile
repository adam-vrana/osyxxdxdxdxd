CXX = g++
CPPFLAGS = -g

# cíle
TARGETS = vernum gennum

# knihovna checker (jen pro vernum)
LIBNAME = checker
LDFLAGS ?= -L../dir1
LDLIBS = -l$(LIBNAME)

all: $(TARGETS)

clean:
	rm -rf $(TARGETS) *.o

# vernum používá knihovnu checker
vernum: vernum.cpp
	@echo "\nCreating application $@...\n"
	$(CXX) $(CPPFLAGS) $< $(LDFLAGS) $(LDLIBS) -o $@
	@echo "\nApplication '$@' is ready.\n"
	@echo "Run it with:\n  export LD_LIBRARY_PATH=../dir1\n"

# gennum používá vlastní zdrojáky
gennum: gennum.cpp gen_int.cpp gen_float.cpp gen_hexa.cpp gen_bin.cpp
	@echo "\nCreating application $@...\n"
	$(CXX) $(CPPFLAGS) $^ -o $@
	@echo "\nApplication '$@' is ready.\n"
