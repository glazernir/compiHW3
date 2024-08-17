//
// Created by Nir on 09/08/2024.
//

#include "Semantic.h"

#include <cmath>

#include "string"
#include "cg.hpp"
#include <sstream>



extern Symbol_table_stack symbolTableStack;


int Semantic::get() {
    return this->reg_index;
}

void Semantic::increment() {
    this->reg_index++;
}

Semantic &Semantic::instacne() {
    static Semantic s;
    return s;
}

std::string Semantic::freshVar() {
    std::string stringIndex = std::to_string(instacne().get());
    instacne().increment();
    std::string res = "%reg_" + stringIndex;
    return res;
}

string Semantic::freshGlobalVar() {
    std::string stringIndex = std::to_string(instacne().get());
    instacne().increment();
    string res = "@.reg_" + stringIndex;
    return res;
}

string readi_implemination() {
    string implemination = "define i32 @readi(i32) { \n"
                            "%ret_val = alloca i32 \n"
                            "%spec_ptr = getelementptr [3 x i8], [3 x i8]* @.int_specifier_scan, i32 0, i32 0\n"
                            "call i32 (i8*, ...) @scanf(i8* %spec_ptr, i32* %ret_val)\n"
                            "%val = load i32, i32* %ret_val\n"
                            "ret i32 %val}\n";
    return implemination;
}

string printi_implemination() {
    string implemination =  "define void @printi(i32) {\n"
                            "%spec_ptr = getelementptr [4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0\n"
                            "call i32 (i8*, ...) @printf(i8* %spec_ptr, i32 %0)\n"
                            "ret void\n"
                            "}\n";
    return implemination;
}

string print_impleminatino() {
    string implemination =  "define void @print(i8*) {\n"
                            "%spec_ptr = getelementptr [4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0\n"
                            "call i32 (i8*, ...) @printf(i8* %spec_ptr, i8* %0)\n"
                            "ret void\n"
                            "}\n";
    return implemination;
}

void varsAllocations() {
    CodeBuffer& Buffer = CodeBuffer::instance();
    for (int i = 0;i < 50;i++) {
        Buffer.emit("%reg_" + to_string(i) + " = alloca i32");
    }
}


void SemanticAction_StartProgaram() {
    CodeBuffer& Buffer = CodeBuffer::instance();
    Buffer.emitGlobal("declare i32 @scanf(i8*, ...)");
    Buffer.emitGlobal("declare i32 @printf(i8*, ...)");
    Buffer.emitGlobal("declare void @exit(i32)");
    Buffer.emitGlobal("@.int_specifier_scan = constant [3 x i8] c\"%d\\00\"");
    Buffer.emitGlobal("@.int_specifier = constant [4 x i8] c\"%d\\0A\\00\"");
    Buffer.emitGlobal("@.str_specifier = constant [4 x i8] c\"%s\\0A\\00\"");
    Buffer.emitGlobal("@.zeroDivisionError = constant [23 x i8] c\"Error division by zero\\00\";");
    Buffer.emitGlobal(printi_implemination());
    Buffer.emitGlobal(print_impleminatino());
    Buffer.emitGlobal(readi_implemination());
    Buffer.emit("“define i32 @main(){“");
    varsAllocations();
}

void SemanticAction_EndProgram() {
    CodeBuffer& Buffer = CodeBuffer::instance();
    Buffer.emit("ret i32 0");
    Buffer.emit("}");
    Buffer.printGlobalBuffer();
    Buffer.printCodeBuffer();
}

void goto_False_Label(Exp_Obj *expression) {
    CodeBuffer& Buffer = CodeBuffer::instance();
    Buffer.emit("br label %" + expression->falseLabel);
    Buffer.emit(expression->falseLabel + ":");
}
void goto_True_Label(Exp_Obj *expression) {
    CodeBuffer& Buffer = CodeBuffer::instance();
    Buffer.emit("br label %" + expression->trueLabel);
    Buffer.emit(expression->trueLabel + ":");
}

void createAndJumpTo() {
    CodeBuffer& Buffer = CodeBuffer::instance();
    string newLabel = Buffer.freshLabel();
    Buffer.emit("br label %" + newLabel);
}


void SemanticAction_DefaultInitialize(Object* object) {

    Symbol* s = symbolTableStack.get_symbol_by_name(object->name);
    CodeBuffer& Buffer = CodeBuffer::instance();
    string toStore = "reg_" + to_string(s->symbol_Offset);
    Buffer.emit("Store i32 0, i32* %" + toStore);
}

void SemanticAction_Store(Object* object, Exp_Obj* expression) {
    Symbol* s = symbolTableStack.get_symbol_by_name(object->name);
    string toStore = "reg_" + to_string(s->symbol_Offset);
    CodeBuffer& Buffer = CodeBuffer::instance();
    string exitLabel = Buffer.freshLabel();
    string res = Semantic::freshVar();
    if (expression->type == "bool") {
        Buffer.emit(expression->trueLabel + ":");
        Buffer.emit("br label %" + exitLabel);
        Buffer.emit(expression->falseLabel + ":");
        Buffer.emit("br label %" + exitLabel);
        Buffer.emit(exitLabel + ":");
        Buffer.emit("%" + res + " = phi i32 [0, %" + expression->falseLabel + "], [1, %" + expression->trueLabel + "]");
        Buffer.emit("store i32 " + res + ", i32* %" + toStore);
    }
    else {
        string expression_reg_value = expression->register_val;
        Buffer.emit("store i32 " + expression_reg_value + ", i32 %" + toStore );
    }
}

void SemanticAction_Return() {
    CodeBuffer& Buffer = CodeBuffer::instance();
    Buffer.emit("ret i32 0");
}

void SemanticAction_Break() {
    CodeBuffer& Buffer = CodeBuffer::instance();
    Buffer.emit("br label %" + symbolTableStack.getCurrerntWhileScope()->endScopeLabel);
}

void SemanticAction_Continue() {
    CodeBuffer& Buffer = CodeBuffer::instance();
    Buffer.emit("br label %" + symbolTableStack.getCurrerntWhileScope()->startScopeLabel);
}

void SemanticAction_While(Exp_Obj* expression) {
    CodeBuffer& Buffer = CodeBuffer::instance();
    symbolTableStack.getCurrerntWhileScope()->endScopeLabel = expression->falseLabel;
    Buffer.emit(expression->trueLabel + ":");
}


void SemanticAction_Call(Call_Obj *call_obj, Object *funcName, Object *funcArg) {
    CodeBuffer& Buffer = CodeBuffer::instance();
    if (funcName->name == "readi") {
        call_obj->register_val = Semantic::freshVar();
        Buffer.emit("%"+ call_obj->register_val + " = call i32 @readi(i32 0)");
        return;
    }
    if (funcName->name == "printi") {
        Buffer.emit("call void @printi(i32 " + funcArg->register_val + ")");
        return;
    }
    if (funcName->name == "print") {
        Buffer.emit("call void (i8*) @print(i8* getelementptr ([" + to_string(funcArg->name.size()-1) +
                            " x i8], [" + to_string(funcArg->name.size()-1) + " x i8]* " + funcArg->register_val + ", i32 0,i 32 0");
    }
}

string toLLVMBinopOperation(string operation) {
    if(operation == "+") {
        return ("add");
    }
    if(operation == "*") {
        return("mul");
    }
    if(operation == "-") {
        return("sub");
    }
    if(operation == "/") {
        return("sdiv");
    }
}

string toLLVMRelopOperation(string operation) {
    if (operation == "==") {
        return("ep");
    }
    if(operation == "!=") {
        return("ne");
    }
    if(operation == "<") {
        return("slt");
    }
    if(operation == ">") {
        return("sgt");
    }
    if(operation == "<=") {
        return("sle");
    }
    if(operation == ">=") {
        return("sge");
    }
}

string truncationByteRes(string resRegister) {
    CodeBuffer& Buffer = CodeBuffer::instance();
    string truncRes = Semantic::freshVar();
    Buffer.emit(truncRes + " = and i32 " + resRegister + ", 255");
    return truncRes;
}

void zeroDivisionHandler() {
    CodeBuffer& Buffer = CodeBuffer::instance();
    Buffer.emit("call void (i8*) @print(i8* getelementptr ([23 x i8], [23 x i8]* @.zeroDivisionError, i32 0, i32 0))");
    Buffer.emit("call void @exit(i32 0)");
}


void SemanticAction_Binop(Exp_Obj* res, Exp_Obj* first, Object* operation, Exp_Obj* second) {
    CodeBuffer& Buffer = CodeBuffer::instance();
    string llvmOperation = toLLVMBinopOperation(operation->name);
    if (llvmOperation == "sdiv") {
        string condition = Semantic::freshVar();
        string errorDivision = Buffer.freshLabel();
        Buffer.emit(condition + " = icmp eq i32" + second->register_val + " ,0");
        Buffer.emit(errorDivision + ":");
        zeroDivisionHandler();
        string next = Buffer.freshLabel();
        Buffer.emit("br label %" + next);
        Buffer.emit(next + ":");
    }
    string resRegister = Semantic::freshVar();
    Buffer.emit(resRegister + " = " + llvmOperation + " i32 " + first->register_val + ", " + second->register_val);
    if(first->type == "BYTE" && second->type == "BYTE") {
        res->register_val = truncationByteRes(resRegister);
        return;
    }
    res->register_val = resRegister;
}


void SemanticAction_Relop(Exp_Obj* res, Exp_Obj* first, Object* operation, Exp_Obj* second) {
    CodeBuffer& Buffer = CodeBuffer::instance();
    string llvmOperation = toLLVMRelopOperation(operation->name);

    res->falseLabel = Buffer.freshLabel();
    res->trueLabel = Buffer.freshLabel();

    string condition = Semantic::freshVar();
    Buffer.emit(condition + " = icmp " + llvmOperation + " i32 " + "%" + first->register_val + ", %" + second->register_val);
    Buffer.emit("br i1 " + condition + " label %" + res->trueLabel + ", label %" + res->falseLabel);
}

void SemanticAction_Load(Exp_Obj* res, Object* toLoad) {
    CodeBuffer& Buffer = CodeBuffer::instance();
    Symbol* s = symbolTableStack.get_symbol_by_name(toLoad->name);
    string toLoad_reg = "reg_" + s->symbol_Offset;
    string resRegister = Semantic::freshVar();
    Buffer.emit("%" + resRegister + " = i32, i32* %" +toLoad_reg);
    res->register_val = resRegister;
    if(s->symbol_Type == "BOOL") {
        res->falseLabel = Buffer.freshLabel();
        res->trueLabel = Buffer.freshLabel();
        string condition = Semantic::freshVar();
        Buffer.emit(condition + " = icmp " + "eq " + " i32 " + resRegister + ", 1");
        Buffer.emit("br i1 " + condition + " label %" + res->trueLabel + ", label %" + res->falseLabel);
    }
}

string declareStringAsGlobalVar(string str1){
    CodeBuffer& Buffer = CodeBuffer::instance();
    string registerRes = Semantic::freshGlobalVar();
    string stringLiteral = str1.erase(str1.size() - 1) + "\\00\"";
    Buffer.emitGlobal(registerRes + " = private unnamed_addr constant [" + to_string(str1.size()-1) + " x i8] c" + stringLiteral);
    return registerRes;
}

void SemanticAction_Variable(Exp_Obj* res, Object* variableObject, string variableType){
    CodeBuffer& Buffer = CodeBuffer::instance();

    if (variableType == "int"){
        string registerRes = Semantic::freshVar();
        Buffer.emit(registerRes + " = add i32 " + variableObject->name + ", 0");
        res->register_val = registerRes;
        return;
    }
    if (variableType == "byte") {
        string registerRes = Semantic::freshVar();
        string extendedRegisterRes = Semantic::freshVar();
        Buffer.emit(registerRes + " = add i32 " + variableObject->name + ", 0");
        Buffer.emit(extendedRegisterRes + " = zext i8 " + registerRes + " to i32");
        res->register_val = extendedRegisterRes;
        return;
    }
    if(variableType == "string") {
        string registerRes = declareStringAsGlobalVar(variableObject->name);
        res->register_val = registerRes;
        return;
    }
    if(variableType == "True") {
        Buffer.emit("br label %" + res->trueLabel);
        return;
    }
    if(variableType == "False") {
        Buffer.emit("br label %" + res->falseLabel);
    }
}


void SemanticAction_Not(Exp_Obj* notExpression, Exp_Obj* expression) {
    CodeBuffer& Buffer = CodeBuffer::instance();
    notExpression->falseLabel = expression->trueLabel;
    notExpression->trueLabel = expression->falseLabel;
}

void SemanticAction_Or(Exp_Obj* res, Exp_Obj* first, Exp_Obj* second, string evalSide) {
    CodeBuffer& Buffer = CodeBuffer::instance();
    if(evalSide == "Left") {
        Buffer.emit(first->falseLabel + ":");
        return;
    }
    if (evalSide == "Right") {
        res->falseLabel = second->falseLabel;
        res->trueLabel = second->trueLabel;
        Buffer.emit(first->trueLabel + ":");
        Buffer.emit("br label % " + res->trueLabel);
    }
}

void SemanticAction_And(Exp_Obj* res, Exp_Obj* first, Exp_Obj* second,string evalSide) {
    CodeBuffer& Buffer = CodeBuffer::instance();
    if(evalSide == "Left") {
        Buffer.emit("br label %" + first->trueLabel);
        Buffer.emit(first->trueLabel + ":");
        return;
    }

    if (evalSide == "Right") {
        res->falseLabel = second->falseLabel;
        res->trueLabel = second->trueLabel;
        Buffer.emit("br label %" + first->falseLabel);
        Buffer.emit(first->falseLabel + ":");
        Buffer.emit("br label % " + res->falseLabel);
    }
}

void SemanticAction_Paren(Exp_Obj* res, Exp_Obj* expression) {
    res->register_val = expression->register_val;
}












