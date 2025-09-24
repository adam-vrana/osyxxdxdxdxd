
TARGET = vernum
LIBNAME = checker

CPPFLAGS += -g
# Default to dir1, can override: make LDFLAGS="-L../dir2"
LDFLAGS ?= -L../dir1
LDLIBS += -l$(LIBNAME)

all: $(TARGET)

# clean binary files
clean:
	rm -rf $(TARGET) *.o

# creating application (library must exist in LD_LIBRARY_PATH)
$(TARGET): $(TARGET).cpp
	@echo "\nCreating application...\n"
	g++ $(CFLAGS) $< $(LDFLAGS) $(LDLIBS) -o $@
	@echo "\nApplication '$(TARGET)' is ready.\n"
	@echo "Set path to library: 'export LD_LIBRARY_PATH=../dir1' or 'export LD_LIBRARY_PATH=../dir2' !\n"