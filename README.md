Z3 README
=========

This is the readme for the Z3 Project, a Zam to LLVM compiler and (hopefully) runtime.

The idea is to reuse ocaml's runtime and builtins, but to plug in an LLVM backed native code generator.

A word of disclaimer
--------------------

This project is nothing but ideas at the moment, and most of them are not fixed yet. This github space is meant as a discussion space for the time being, and this disclaimer will be removed when runnable and useful code is put in this repo. *For the moment, all code you'll find here is scraps and implementation ideas*

Building the project
--------------------

The default Makefile is configured to use clang, but should work with gcc too.

What does it do for the moment
------------------------------

- It uses the ocaml runtime (based in the byterun/ folder of the ocaml distribution) to parse an ocaml bytecode file.
- Processes the code to rebuild functions based on the ZAM bytecode.
- Emits and eventually jit compile/run llvm IR for a (yet) limited subset of the ZAM instruction set.
