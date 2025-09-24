CXX = g++
CPPFLAGS = -g

# Targets
TARGET_GEN = gennum
TARGET_VER = vernum

# Generator library settings
GEN_LIBNAME = generator
GEN_LIBFILE = lib$(GEN_LIBNAME).a
GEN_LDFLAGS = -L./gen
GEN_LDLIBS = -l$(GEN_LIBNAME)

# Vernum checker library settings
VER_LIBNAME = checker
VER_LDFLAGS = -L../dir1
VER_LDLIBS = -l$(VER_LIBNAME)

# Build all targets
all: $(TARGET_GEN) $(TARGET_VER)

# Clean all binary files
clean:
	rm -rf $(TARGET_GEN) $(TARGET_VER) $(GEN_LIBFILE) gen/*.o vernum/*.o

# Generator targets
gen: $(TARGET_GEN)

# Creating generator static library 
$(GEN_LIBFILE): gen/gen_int.o gen/gen_float.o gen/gen_hexa.o gen/gen_bin.o
	@echo "\nCreating static library...\n"
	ar r gen/$@ $^
	@echo "\nStatic library created: 'gen/$(GEN_LIBFILE)'.\n"

# Creating generator application
$(TARGET_GEN): gen/$(TARGET_GEN).cpp $(GEN_LIBFILE)
	@echo "\nCreating generator application...\n"
	cd gen && $(CXX) $(CPPFLAGS) $(TARGET_GEN).cpp -L. -l$(GEN_LIBNAME) -o $(TARGET_GEN)
	@echo "\nApplication '$(TARGET_GEN)' is ready in gen/ directory.\n"

# Vernum targets
ver: $(TARGET_VER)

# Creating vernum application (uses checker library)
$(TARGET_VER): vernum/$(TARGET_VER).cpp
	@echo "\nCreating vernum application...\n"
	cd vernum && $(CXX) $(CPPFLAGS) $(TARGET_VER).cpp $(VER_LDFLAGS) $(VER_LDLIBS) -o $(TARGET_VER)
	@echo "\nApplication '$(TARGET_VER)' is ready in vernum/ directory.\n"
	@echo "Run it with:\n  export LD_LIBRARY_PATH=../dir1\n"

# Individual object file targets for generator
gen/%.o: gen/%.cpp
	@echo "Compiling $<..."
	cd gen && $(CXX) $(CPPFLAGS) -c $(notdir $<) -o $(notdir $@)

.PHONY: all clean gen ver