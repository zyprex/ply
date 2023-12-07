# LAST MODIFIED : Jan 31, 2023
# COMPILER      : MinGW-w64 gcc 8.1.0
# MAKE PROGRAM  : mingw32-make.exe
# PLATFORM      : Windows
# FUNCTION      : make iup program
# REF: https://www.tecgraf.puc-rio.br/iup/en/tutorial/

CC := gcc
# CFLAGS := -Wall -g3 -mwindows
# LIBS  := -static-libgcc -lgdi32
# OUT   := $(patsubst %.c, %.exe, $(wildcard *.c))
IUP_PATH := e:\Xprogram\cgui\IUP
LIB_PATH := $(IUP_PATH)\iup-3.3.0
INC_PATH := $(IUP_PATH)\iup-3.3.0\include
# LIBS_PATH := .\iup-3.3.0s
# INCS_PATH := .\iup-3.3.0s\include
NAME := ply
SRC  := $(wildcard $(NAME)*.c)
OBJ  := $(patsubst %.c, %.o, $(SRC))

# use dynamic
all: clean compile
	$(CC) -o $(NAME).exe $(OBJ) -liupimglib -liup -L$(LIB_PATH) -mwindows

compile:
	$(CC) -c $(SRC) -I$(INC_PATH)


# use static
# all_static:
# $(CC) -o $(NAME).exe $(NAME).c -static -I$(INCS_PATH) -liup -lgdi32 -lcomdlg32 -lcomctl32 -luuid -loleaut32 -lole32 -L$(LIBS_PATH) -L$(IUP_PATH)\iup-3.3.0s\etc

.PHONY: clean
clean:
	del $(OBJ) $(NAME).exe

