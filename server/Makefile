TARGET 		= YAOCS_server

CC 		= gcc
LINKER 	= gcc -o


SRCDIR 		= src
OBJDIR 		= out
BINDIR 		= bin
LIBDIR		= ../libs

CFLAGS 		= -Wall -I$(LIBDIR)
LFLAGS 		= -Wall -I. 

SOURCES 	:= $(wildcard $(SRCDIR)/*.c) 
SOURCES_LIB := $(wildcard $(LIBDIR)/*.c)
INCLUDES 	:= $(wildcard $(SRCDIR)/*.h) 
INCLUDES_LIB := $(wildcard $(LIBDIR)/*.h)
OBJECTS 	:= $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
OBJECTS_LIB 	:= $(SOURCES_LIB:$(LIBDIR)/%.c=$(OBJDIR)/%.o)

rm 	        = rm -f

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS) $(OBJECTS_LIB)
	@$(LINKER) $@ $(OBJECTS) $(OBJECTS_LIB) $(LFLAGS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
$(OBJECTS_LIB): $(OBJDIR)/%.o : $(LIBDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	@$(rm) $(OBJECTS) $(OBJECTS_LIB)

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
