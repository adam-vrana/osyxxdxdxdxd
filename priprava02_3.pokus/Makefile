CXX = g++
TARGET = gennum
LIBNAME = generator
LIBFILE = lib$(LIBNAME).a

CPPFLAGS += -g
LDFLAGS += -L. 
LDLIBS += -l$(LIBNAME)

all: $(TARGET)

# clean binary files
clean:
	rm -rf $(TARGET) $(LIBFILE) *.o

# creating static library 
$(LIBFILE): gen_int.o gen_float.o gen_hexa.o gen_bin.o
	@echo "\nCreating static library...\n"
	ar r $@ $^
	@echo "\nStatic library created: '$(LIBFILE)'.\n"

# creating application
$(TARGET): $(TARGET).cpp $(LIBFILE)
	@echo "\nCreating application...\n"
	$(CXX) $(CPPFLAGS) $< $(LDFLAGS) $(LDLIBS) -o $@
	@echo "\nApplication '$(TARGET)' is ready.\n"