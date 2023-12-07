# LAST MODIFIED : Feb 27, 2023
# COMPILER      : gcc 12.2.1
# MAKE PROGRAM  : gnu make
# PLATFORM      : linux
# FUNCTION      : make iup program
# REF: https://www.tecgraf.puc-rio.br/iup/en/tutorial/

CC := gcc
# CFLAGS := -Wall -g3
# LIBS  := -static-libgcc -lgdi32
# OUT   := $(patsubst %.c, %.exe, $(wildcard *.c))
IUP_PATH := /home/zy/bear/iup/iup-3.30_Linux54_64_lib/
LIB_PATH := $(IUP_PATH)
INC_PATH := $(IUP_PATH)/include

NAME := ply
SRC  := $(wildcard $(NAME)*.c)
OBJ  := $(patsubst %.c, %.o, $(SRC))

# use dynamic
all: compile
	$(CC) -o $(NAME) $(OBJ) -liup -L$(LIB_PATH)

compile:
	$(CC) -c $(SRC) -I$(INC_PATH)

# use static

.PHONY: clean
clean:
	rm $(OBJ) $(NAME)

