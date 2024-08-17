//
// Created by Nir on 09/08/2024.
//

#ifndef SEMANTIC_H
#define SEMANTIC_H
#include <chrono>
#include <string>
#include "SymbolTable.h"


class Semantic {
    int reg_index;
    Semantic() {
        this->reg_index = 0;
    }

public:
    int get();
    void increment();
    static Semantic& instacne();
    ~Semantic() = default;
    static std::string freshVar();
    static string freshGlobalVar();

};

void SemanticAction_StartProgaram();
void SemanticAction_EndProgram();
void SemanticAction_DefaultInitialize(Object* );
void SemanticAction_Store(Object*, Exp_Obj*);
void SemanticAction_Return();
void SemanticAction_Break();
void SemanticAction_Continue();
void SemanticAction_While(Exp_Obj*);
void SemanticAction_Call(Call_Obj* call,Object* , Object* );
void SemanticAction_Binop(Exp_Obj*,Exp_Obj*,Object*,Exp_Obj*);
void SemanticAction_Relop(Exp_Obj*,Exp_Obj*,Object*,Exp_Obj*);
void SemanticAction_Load(Exp_Obj*, Object*);
void SemanticAction_Variable(Exp_Obj*, Object*,string);
void SemanticAction_Not(Exp_Obj*, Exp_Obj*);
void SemanticAction_And(Exp_Obj*, Exp_Obj*,Exp_Obj*,string);
void SemanticAction_Or(Exp_Obj*, Exp_Obj*, Exp_Obj*,string);
void SemanticAction_Paren(Exp_Obj*,Exp_Obj*);


void zeroDivisionHandler();
string declareStringAsGlobalVar(string);
std::string truncationByteRes(string);
void createAndJumpTo();
void goto_True_Label(Exp_Obj*);
void goto_False_Label(Exp_Obj*);
void varsAllocations();



#endif //SEMANTIC_H
