OBJ=obj
dummy := $(shell test -d ${OBJ} || mkdir ${OBJ})
SRC=src
Z3INCLUDE=include
OCAMLPATH=ocaml-3.12.1
LIBPATH=${OCAMLPATH}/byterun
LIBNAME=camlrund
BIN=bin
dummy := $(shell test -d ${BIN} || mkdir ${BIN})

CCFLAGS= -g -Wall -Wextra -Wno-unused-parameter -I${Z3INCLUDE} -std=c++0x
LIBS= -lboost_program_options
CC=clang++ ${CCFLAGS} `llvm-config --cppflags` 
CSTDLIBCC=clang -O3 -Wall -Wextra -Wno-unused-parameter -I${Z3INCLUDE}

OBJECTS=$(OBJ)/Context.o $(OBJ)/GenBlock.o $(OBJ)/GenFunction.o $(OBJ)/GenModule.o $(OBJ)/GenModuleCreator.o $(OBJ)/Instructions.o $(OBJ)/SimpleContext.o

all: main

ocaml_runtime: ${OCAMLPATH}/config/Makefile ${OCAMLPATH}/config/m.h ${OCAMLPATH}/config/s.h
	cd ${LIBPATH} && make && make libcamlrund.a && rm main.d.o;

${OCAMLPATH}/config/Makefile ${OCAMLPATH}/config/m.h ${OCAMLPATH}/config/s.h:
	cd ${OCAMLPATH} && ./configure

${OBJ}/%.o: ${SRC}/%.cpp ${OCAMLPATH}/config/m.h ${OCAMLPATH}/config/s.h
	${CC} -c -o $@ $<

stdlib:
	${CSTDLIBCC} -S -emit-llvm -o ${BIN}/StdLib.ll ${SRC}/CStdLib.c
	${CSTDLIBCC} -std=c++0x -S -emit-llvm -o ${BIN}/ZamSimpleInterpreter.ll ${SRC}/zsi/ZamSimpleInterpreter.cpp


main: $(OBJECTS) ocaml_runtime stdlib
	${CC} ${LIBS} -rdynamic -L${LIBPATH} -o ${BIN}/Z3 ${SRC}/main.cpp $(OBJECTS) ${LIBPATH}/*.d.o ${LIBPATH}/prims.o -lcurses `llvm-config --ldflags --libs bitreader asmparser core jit native ipo`


clean:
	rm ${OBJ}/* -rf;

cleanall: clean
	cd ${OCAMLPATH} && make clean;

