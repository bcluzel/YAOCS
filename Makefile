TARGET 		= client

CC 		= gcc
LINKER 	= gcc -o

CFLAGS 		= -Wall 
LFLAGS 		= -Wall -I. 

SRCDIR 		= src
OBJDIR 		= out
BINDIR 		= bin

SOURCES 	:= $(wildcard $(SRCDIR)/*.c)
INCLUDES 	:= $(wildcard $(SRCDIR)/*.h)
OBJECTS 	:= $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm 	        = rm -f

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(OBJECTS) $(LFLAGS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	@$(rm) $(OBJECTS)

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
