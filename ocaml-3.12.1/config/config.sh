# generated by ./configure 
if [ "x$PREFIX" = "x" ]; then PREFIX="/usr/local"; fi
if [ "x$BINDIR" = "x" ]; then BINDIR="${PREFIX}/bin"; fi
if [ "x$LIBDIR" = "x" ]; then LIBDIR="${PREFIX}/lib/ocaml"; fi
if [ "x$STUBLIBDIR" = "x" ]; then STUBLIBDIR="${LIBDIR}/stublibs"; fi
if [ "x$MANDIR" = "x" ]; then MANDIR="${PREFIX}/man"; fi
if [ "x$MANEXT" = "x" ]; then MANEXT="1"; fi
if [ "x$RANLIB" = "x" ]; then RANLIB="ranlib"; fi
if [ "x$RANLIBCMD" = "x" ]; then RANLIBCMD="ranlib"; fi
if [ "x$SHARPBANGSCRIPTS" = "x" ]; then SHARPBANGSCRIPTS="true"; fi
if [ "x$BNG_ARCH" = "x" ]; then BNG_ARCH="ia32"; fi
if [ "x$BNG_ASM_LEVEL" = "x" ]; then BNG_ASM_LEVEL="2"; fi
if [ "x$PTHREAD_LINK" = "x" ]; then PTHREAD_LINK="-cclib -lpthread"; fi
if [ "x$X11_INCLUDES" = "x" ]; then X11_INCLUDES=""; fi
if [ "x$X11_LINK" = "x" ]; then X11_LINK="not found"; fi
if [ "x$DBM_INCLUDES" = "x" ]; then DBM_INCLUDES="not found"; fi
if [ "x$DBM_LINK" = "x" ]; then DBM_LINK="not found"; fi
if [ "x$TK_DEFS" = "x" ]; then TK_DEFS=""; fi
if [ "x$TK_LINK" = "x" ]; then TK_LINK=""; fi
if [ "x$LIBBFD_LINK" = "x" ]; then LIBBFD_LINK=""; fi
if [ "x$BYTECC" = "x" ]; then BYTECC="gcc"; fi
if [ "x$BYTECCCOMPOPTS" = "x" ]; then BYTECCCOMPOPTS="-fno-defer-pop -Wall -D_FILE_OFFSET_BITS=64 -D_REENTRANT"; fi
if [ "x$BYTECCLINKOPTS" = "x" ]; then BYTECCLINKOPTS=" -Wl,-E"; fi
if [ "x$BYTECCLIBS" = "x" ]; then BYTECCLIBS=" -lm  -ldl -lcurses -lpthread"; fi
if [ "x$BYTECCRPATH" = "x" ]; then BYTECCRPATH="-Wl,-rpath,"; fi
if [ "x$EXE" = "x" ]; then EXE=""; fi
if [ "x$SUPPORTS_SHARED_LIBRARIES" = "x" ]; then SUPPORTS_SHARED_LIBRARIES="true"; fi
if [ "x$SHAREDCCCOMPOPTS" = "x" ]; then SHAREDCCCOMPOPTS="-fPIC"; fi
if [ "x$MKSHAREDLIBRPATH" = "x" ]; then MKSHAREDLIBRPATH="-Wl,-rpath,"; fi
if [ "x$NATDYNLINKOPTS" = "x" ]; then NATDYNLINKOPTS="-Wl,-E"; fi
# SYSLIB=-l${1}
#ml let syslib x = "-l"^x;;

### How to build a static library
# MKLIB=ar rc ${1} ${2}; ranlib ${1}
#ml let mklib out files opts = Printf.sprintf "ar rc %s %s %s; ranlib %s" out opts files out;;
if [ "x$ARCH" = "x" ]; then ARCH="i386"; fi
if [ "x$MODEL" = "x" ]; then MODEL="default"; fi
if [ "x$SYSTEM" = "x" ]; then SYSTEM="linux_elf"; fi
if [ "x$NATIVECC" = "x" ]; then NATIVECC="gcc"; fi
if [ "x$NATIVECCCOMPOPTS" = "x" ]; then NATIVECCCOMPOPTS="-Wall -D_FILE_OFFSET_BITS=64 -D_REENTRANT"; fi
if [ "x$NATIVECCPROFOPTS" = "x" ]; then NATIVECCPROFOPTS="-Wall -D_FILE_OFFSET_BITS=64 -D_REENTRANT"; fi
if [ "x$NATIVECCLINKOPTS" = "x" ]; then NATIVECCLINKOPTS=""; fi
if [ "x$NATIVECCRPATH" = "x" ]; then NATIVECCRPATH="-Wl,-rpath,"; fi
if [ "x$NATIVECCLIBS" = "x" ]; then NATIVECCLIBS=" -lm  -ldl"; fi
if [ "x$ASM" = "x" ]; then ASM="as"; fi
if [ "x$ASPP" = "x" ]; then ASPP="gcc -c"; fi
if [ "x$ASPPPROFFLAGS" = "x" ]; then ASPPPROFFLAGS="-DPROFILING"; fi
if [ "x$PROFILING" = "x" ]; then PROFILING="prof"; fi
if [ "x$DYNLINKOPTS" = "x" ]; then DYNLINKOPTS=" -ldl"; fi
if [ "x$OTHERLIBRARIES" = "x" ]; then OTHERLIBRARIES="unix str num dynlink bigarray systhreads threads"; fi
if [ "x$DEBUGGER" = "x" ]; then DEBUGGER="ocamldebugger"; fi
if [ "x$CC_PROFILE" = "x" ]; then CC_PROFILE="-pg"; fi
if [ "x$SYSTHREAD_SUPPORT" = "x" ]; then SYSTHREAD_SUPPORT="true"; fi
if [ "x$PARTIALLD" = "x" ]; then PARTIALLD="ld -r"; fi
if [ "x$PACKLD" = "x" ]; then PACKLD="${PARTIALLD} ${NATIVECCLINKOPTS} -o\ "; fi
if [ "x$DLLCCCOMPOPTS" = "x" ]; then DLLCCCOMPOPTS=""; fi
if [ "x$IFLEXDIR" = "x" ]; then IFLEXDIR=""; fi
if [ "x$O" = "x" ]; then O="o"; fi
if [ "x$A" = "x" ]; then A="a"; fi
if [ "x$SO" = "x" ]; then SO="so"; fi
if [ "x$EXT_OBJ" = "x" ]; then EXT_OBJ=".o"; fi
if [ "x$EXT_ASM" = "x" ]; then EXT_ASM=".s"; fi
if [ "x$EXT_LIB" = "x" ]; then EXT_LIB=".a"; fi
if [ "x$EXT_DLL" = "x" ]; then EXT_DLL=".so"; fi
if [ "x$EXTRALIBS" = "x" ]; then EXTRALIBS=""; fi
if [ "x$CCOMPTYPE" = "x" ]; then CCOMPTYPE="cc"; fi
if [ "x$TOOLCHAIN" = "x" ]; then TOOLCHAIN="cc"; fi
if [ "x$NATDYNLINK" = "x" ]; then NATDYNLINK="true"; fi
if [ "x$CMXS" = "x" ]; then CMXS="cmxs"; fi
if [ "x$MKEXE" = "x" ]; then MKEXE="${BYTECC}"; fi
if [ "x$MKDLL" = "x" ]; then MKDLL="gcc -shared"; fi
if [ "x$MKMAINDLL" = "x" ]; then MKMAINDLL="gcc -shared"; fi
WINDOWS=false
