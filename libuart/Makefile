CROSS_COMPILE:=gcc
APP_DIR=$(shell pwd)/..
PRO_CFLAGS ?=-O2 -Wall

ifndef BUILD_TIME
BUILD_TIMESTAMPFLG=-DBUILD_TIMESTAMP=$(shell date +%Y%m%d%H%M%SLL)
else
BUILD_TIMESTAMPFLG=-DBUILD_TIMESTAMP=$(BUILD_TIME)LL
endif

FLAGS_LIB=
CFLAGS = 


SRC=$(wildcard *.c) 
HEAD=$(wildcard *.h)

#object
OBJ=libuart.a
all: $(OBJ)
$(OBJ):$(SRC) $(HEAD)
	$(CROSS_COMPILE) -static  -o $@ \
	$(SRC) \
	$(CFLAGS) \
	$(FLAGS_LIB) \
	$(PRO_CFLAGS) \
	$(BUILD_TIMESTAMPFLG)
clean:
	rm  $(OBJ) -rf
