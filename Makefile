# This Makefile is intended for Windows platform!
# To create a corresponding one for other OS, just check the batch commands

OBJDIR	:= obj
SRCDIR	:= .
CC 		:= gcc
CFLAGS 	:= -g -ansi		#must add -Wall later
LDFLAGS := 				#for libraries
OBJ 	:= $(addprefix $(OBJDIR)/, \
		   assembler.o \
		   cgen.o \
		   builder.o )
TARGET 	:= builder

all: $(TARGET)

$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	del /s /q *.o
