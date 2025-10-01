CXX = g++
CPPFLAGS = -g

# cíl
TARGET = vernum

# knihovna checker
LIBNAME = checker
LDFLAGS ?= -Ldir1
LDLIBS = -l$(LIBNAME)

all: $(TARGET)

clean:
	rm -rf $(TARGET) *.o

# vernum používá knihovnu checker
vernum: vernum.cpp
	@echo "\nCreating application $@...\n"
	$(CXX) $(CPPFLAGS) $< $(LDFLAGS) $(LDLIBS) -o $@
	@echo "\nApplication '$@' is ready.\n"
	@echo "Run it with:\n  export LD_LIBRARY_PATH=dir1\n"