LIBNAME = checker
LIBFILE = lib$(LIBNAME).so

CPPFLAGS += -g -fPIC

ifeq ($(shell uname -m),x86_64) 
 CPPFLAGS += -fPIC
endif

all: $(LIBFILE)

# clean binary files
clean:
	rm -rf $(LIBFILE) *.o

# creating dynamic library 
$(LIBFILE): check_float.o
	@echo "\nCreating dynamic library...\n"
	g++ $(CPPFLAGS) $^ -shared $(LDFLAGS) -o $@
	@echo "\nDynamic library created: '$(LIBFILE)'.\n"