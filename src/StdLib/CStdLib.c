#include <ocaml_runtime/mlvalues.h>
#include <ocaml_runtime/major_gc.h>
#include <ocaml_runtime/memory.h>
#include <ocaml_runtime/prims.h>
#include <stdio.h>
#include <setjmp.h>

#define Lookup(obj, lab) Field (Field (obj, 0), Int_val(lab))

/* GC interface */

#define Setup_for_gc \
  {}
#define Restore_after_gc \
  {}
#define Setup_for_c_call \
  {}
#define Restore_after_c_call \
  {}

// ================ STDLIB Declaration ================== //

value Env = 0;
void setEnv(value E) {
    ////printf("IN SETENVVV\n\n");
    Env = E;
}
value getEnv() {
    //printf("IN GETENVVV, ENV = %p\n\n", (void*)Env);
    return Env;
}

value isClosureHeader(value Header) {
    return Tag_hd(Header) == Closure_tag;
}

void debug(value Arg) {
    printf("DEBUG : %ld\n", (long) Arg);
}

// Closure handling is a little complex. 
// We need to store :
// - The code pointer
// - The closure captured elements
// - The yet supplied number of args
// - The yet supplied args if any
// In any case we are gonna need 1 + NVars + 1 + NbArgs more fields 
// in the closure to store all this extra information
// The layout of the block will thus be :
//
// -----------------------------
// |CodePtr|Vars|Args|NbRemArgs|
// -----------------------------
//
// It is compatible with the regular ocaml runtime's closure layout 

value makeClosure(value NVars, value FPtr, value NbArgs) {
    //printf("IN MAKE CLOSURE, NVars = %ld, FPtr = %p, NbArgs = %ld\n",NVars, (void*)FPtr, NbArgs);
    value Closure;
    int BlockSize = 3 + NVars + NbArgs;
    Alloc_small(Closure, BlockSize, Closure_tag);
    // Set the code pointer
    Code_val(Closure) = (code_t)FPtr;
    // Set the NbRemArgs to the total nb of args
    Field(Closure, BlockSize - 1) = NbArgs;
    Field(Closure, BlockSize - 2) = NbArgs;
    //printf("CLOSURE ADDR: %p\n", (void*)Closure);
    //printf("CLOSURE FN ADDR: %p\n", (void*)FPtr);
    return Closure;
}

value shiftClosure(value Shift) {
    return Env + (Shift*sizeof(value));
}

// Usage
// ClosureSetNext(MainClosure, NestedClosureOffset,
//                NestedClosureSize, NestedFunctionPtr,
//                NestedClosureNbArgs)
value closureSetNestedClos(value Closure, value NClOffset, value NClSize,
                          value FPtr,    value NbArgs) {
    Field(Closure, NClOffset - 1) = Make_header(NClSize, Infix_tag, Caml_white);
    value NestedClos = Closure+NClOffset*sizeof(value);
    Code_val(NestedClos) = (code_t)FPtr;
    // Set NbRemArgs and NbTotalArgs
    Field(NestedClos, NClSize - 1) = NbArgs;
    Field(NestedClos, NClSize - 2) = NbArgs;
    return NestedClos;
}

void closureSetVar(value Closure, value VarIdx, value Value) {
    //printf("INTO CLOSURESETVAR\n\n");
    Field(Closure, VarIdx + 1) = Value;
}

value apply(value Closure, value NbArgs, value* Args) {

    int ArgsSize = NbArgs;
    value CClosure = Closure;

    // While we still have arg to apply 
    while (NbArgs > 0) {

        // Get the number of args the current closure needs
        int Size = Wosize_val(CClosure);
        //printf("SIZE = %d\n", Size);
        int NbRemArgs = Field(CClosure, (Size - 2));
        int NbTotalArgs = Field(CClosure, (Size - 1));
        //printf("NB REM ARGS : %d, NB TOTAL ARGS : %d\n", NbRemArgs, NbTotalArgs);

        // Fill the closure with args until it's full 
        // or we don't have any args left
        while (NbRemArgs > 0 && NbArgs > 0) {
            Field(CClosure, (Size - 2 - NbRemArgs)) = 
                (value)Args[ArgsSize - NbArgs];
            NbRemArgs--; NbArgs--;
        }

        // If the closure is not full
        // and we don't have any args left, return the closure
        if (NbRemArgs > 0) {
            Field(CClosure, (Size - 2)) = NbRemArgs;
            return CClosure;
        }

        // If the closure is full, apply it
        Field(CClosure, (Size - 2)) = NbTotalArgs;
        value (*FPtr)(value) = (value(*)(value)) Code_val(CClosure);
        value OldEnv = Env;
        Env = CClosure;
        CClosure = FPtr(CClosure);
        Env = OldEnv;

        // If NbArgs = 0, 
        // we get out of the loop and return the application result
    }

    return CClosure;
}

/*
#define MAPPLY(a) \
    { FPtr = (value(*)(value)) Code_val(CClosure); \
    value OldEnv = Env; \
    Env = CClosure; \
    CClosure = FPtr(CClosure); \
    Env = OldEnv };


value apply1(value Closure, value Arg1) {
    return 0;
}

value apply2(value Closure, value Arg1, value Arg2) {

    value CClosure = Closure;
    int Size = Wosize_val(CClosure);
    int NbRemArgs = Field(CClosure, (Size - 1));
    value (*FPtr)(value);
    value OldEnv;

    Field(CClosure, (Size - 1 - NbRemArgs)) = Arg1;
    NbRemArgs--; 
    if (--NbRemArgs == 0) {
    }
    Field(CClosure, (Size - 1 - NbRemArgs)) = Arg2;
    NbRemArgs--; if (--NbRemArgs == 0) {}

    ret_lbl: return CClosure;
}
*/

value getBlockSize(value Block) { return Wosize_val(Block); }

value getField(value Block, value Idx) { 
    //printf("Getfield : %p\n", (void*)Field(Block, Idx)); 
    return Field(Block, Idx); 
}

void setField(value Field, value Idx, value NewVal) {
    Modify(&Field(Field, Idx), NewVal);
}

value getAtom(value Idx) {
    return Atom(Idx);
}

value makeBlock1(value tag, value Val1) {
    //printf("INTO MAKEBLOCK1\n");
      value block;
      Alloc_small(block, 1, (tag_t)tag);
      Field(block, 0) = Val1;
      return block;
}

value makeBlock2(value tag, value Val1, value Val2) {
    //printf("INTO MAKEBLOCK2\n");
      value block;
      Alloc_small(block, 2, (tag_t)tag);
      Field(block, 0) = Val1;
      Field(block, 1) = Val2;
      return block;
}

value makeBlock3(value tag, value Val1, value Val2, value Val3) {
    //printf("INTO MAKEBLOCK3\n");
    value block;
    Alloc_small(block, 3, (tag_t)tag);
    Field(block, 0) = Val1;
    Field(block, 1) = Val2;
    Field(block, 2) = Val3;
    return block;
}

value makeBlock(value tag, value NbVals) {
    //printf("INTO MAKEBLOCK N, NbVals = %ld", NbVals);
    value block;
    Alloc_small(block, NbVals, (tag_t)tag);
    return block;
}

value makeFloatBlock(value Size) {
      value Block;
      if ((unsigned)Size <= Max_young_wosize / Double_wosize) {
        Alloc_small(Block, Size * Double_wosize, Double_array_tag);
      } else {
        Block = caml_alloc_shr(Size * Double_wosize, Double_array_tag);
      }
      return Block;
}

void storeDoubleField(value Block,  value Val) {
    Store_double_field(Block, 0, Double_val(Val));
}

value getDoubleField(value Block, value Idx) {
    double d = Double_field(Block, Idx);
    value Double;
    Alloc_small(Double, Double_wosize, Double_tag);
    Store_double_val(Double, d);
    return Double;
}


// ============================= C CALLS ============================== //

value primCall(value Prim) {
  Setup_for_c_call;
  value Ret = Primitive(Prim)();
  Restore_after_c_call;
  return Ret;
}

value primCall1(value Prim, value Val1) {
  //printf("In primCall1, Prim number = %ld, Val1 = %p\n", Prim, (void*)Val1);
  Setup_for_c_call;
  value Ret = Primitive(Prim)(Val1);
  Restore_after_c_call;
  //printf("PrimCall Result: %ld\n", Ret);
  return Ret;
}

value primCall2(value Prim, value Val1, value Val2) {
  //printf("In primCall2, Prim number = %ld, Val1 = %p, Val2 = %p\n", Prim, (void*)Val1, (void*)Val2);
  Setup_for_c_call;
  value Ret = Primitive(Prim)(Val1, Val2);
  Restore_after_c_call;
  //printf("PrimCall Result: %ld\n", Ret);
  return Ret;
}

value primCall3(value Prim, value Val1, value Val2, value Val3) {
  //printf("In primCall3, Prim number = %ld, Val1 = %p, Val2 = %p, Val3 = %p\n", Prim, (void*)Val1, (void*)Val2, (void*)Val3);
  Setup_for_c_call;
  value Ret = Primitive(Prim)(Val1, Val2, Val3);
  Restore_after_c_call;
  //printf("PrimCall Result: %ld\n", Ret);
  return Ret;
}

value primCall4(value Prim, value Val1, value Val2, value Val3, value Val4) {
  //printf("In primCall4, Prim number = %ld, Val1 = %p, Val2 = %p, Val3 = %p, Val4 = %p\n", Prim, (void*)Val1, (void*)Val2, (void*)Val3, (void*)Val4);
    Setup_for_c_call;
    value Ret = Primitive(Prim)(Val1, Val2, Val3, Val4);
    Restore_after_c_call;
  //printf("PrimCall Result: %ld\n", Ret);
    return Ret;
}

value primCall5(value Prim, value Val1, value Val2, value Val3, value Val4, value Val5) {
    Setup_for_c_call;
    value Ret = Primitive(Prim)(Val1, Val2, Val3, Val4, Val5);
    Restore_after_c_call;
    return Ret;
}

// ================================= GLOBAL DATA ============================ //

value getGlobal(value Idx) {
    //printf("In get global number %ld\n", Idx);
    value Glob = Field(caml_global_data, Idx);
    //printf("Global = %p\n", (void*)Glob);
    return Glob;
}

void setGlobal(value Idx, value Val) {
    //printf("In set global number %ld\n", Idx);
    //printf("Global = %p\n", (void*)Val);
    Modify(&Field(caml_global_data, Idx), Val);
}


// ============================ EXCEPTION HANDLING ========================= //

typedef struct _JmpBufList {
    struct _JmpBufList* Next;
    jmp_buf JmpBuf;
    value Env;
} JmpBufList;

JmpBufList* NextExceptionContext = NULL;
value CurrentExceptionVal;

char* getNewBuffer() {
    JmpBufList* NewContext = malloc(sizeof(JmpBufList));
    NewContext->Next = NextExceptionContext;
    NewContext->Env = Env;
    NextExceptionContext = NewContext;
    return (char*)NewContext->JmpBuf;
}

value getExceptionValue() {
    return CurrentExceptionVal;
}

value addExceptionContext() {
    printf("IN ADD EXCEPTION CONTEXT\n");
    JmpBufList* NewContext = malloc(sizeof(JmpBufList));
    NewContext->Next = NextExceptionContext;
    NextExceptionContext = NewContext;

    if (setjmp(NewContext->JmpBuf) == 0) {
        printf("RETURNING FROM ADD EXCEPTION CONTEXT NORMALLY\n");
        return 0;
    } else {
        printf("RETURNING FROM A THROW\n");
        JmpBufList* Ctx = NextExceptionContext;
        NextExceptionContext = Ctx->Next;
        free(Ctx);
        printf("WE OUTTA THERE\n");
        return CurrentExceptionVal;
    }
}

void removeExceptionContext() {
    JmpBufList* Context = NextExceptionContext;
    NextExceptionContext = Context->Next;
    free(Context);
}

void throwException(value ExcVal) {
    if (!NextExceptionContext) {
        printf("Uncatched Exception\n");
        exit(0);
    }
    CurrentExceptionVal = ExcVal;
    Env = NextExceptionContext->Env;
    longjmp(NextExceptionContext->JmpBuf, 1);
}

value vectLength(value Vect) {
    mlsize_t Size = Wosize_val(Vect);
    if (Tag_val(Vect) == Double_array_tag) Size = Size / Double_wosize;
    return Val_long(Size);
}

value getVectItem(value Vect, value Idx) {
    return Field(Vect, Long_val(Idx));
}

void setVectItem(value Vect, value Idx, value NewVal) {
      Modify(&Field(Vect, Long_val(Idx)), NewVal);
}

value getStringChar(value String, value CharIdx) {
      return Val_int(Byte_u(String, Long_val(CharIdx)));
}

void setStringChar(value String, value CharIdx, value Char) {
    Byte_u(String, Long_val(CharIdx)) = Int_val(Char);
}

// ============================= OBJECTS ============================== //

value getMethod(value Object, value Label) {
    return Lookup(Object, Label);
}

value getDynMethod(value Object, value Tag) {
      value Methods = Field (Object, 0);
      int Li = 3, 
          Hi = Field(Methods,0), 
          Mi;
      while (Li < Hi) {
        Mi = ((Li+Hi) >> 1) | 1;
        if (Tag < Field(Methods,Mi)) Hi = Mi-2;
        else Li = Mi;
      }
      return Field (Methods, Li-1);
}

value getSwitchOffset(value SwitchArg, value Dispatch) {
    if (Is_block(Dispatch)) {
        int index = Tag_val(Dispatch);
        return (SwitchArg & 0xFFFF) + index;
    } else {
        int index = Long_val(Dispatch);
        return index;
    }
}
