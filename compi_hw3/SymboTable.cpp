//
// Created by Nir on 7/8/2024.
//

#include "SymboTable.h"
#include <utility>
#include "hw3_output.hpp"
extern int yylineno;
extern symbol_table_stack symbol_table_stack;


bool is_matching (string first, string second)
{
    if (first == "bool" && second == "bool"|| first == "int" && second == "int"|| first == "byte" && second == "byte" || first == "int" && second == "byte" || first == "byte" && second == "int")
    {
        return true;
    }
    return false;
}

/// base form of any object in the code
Object::Object(const std::string& str){
    this->name = str;
}

/// for Program-> Statements
Prog_Obj::Prog_Obj(const string &str) : Object(str) {
}


/// for Type-> INT, BYTE, BOOL
Type_Obj::Type_Obj(const string &str, std::string type) : Object(str) {
    this->type = std::move(type);
}



///for Statement -> RETURN SC
Exp_Obj::Exp_Obj(const std::string &str): Object(str) {
    this->type = "void";
    this->name = "";
}

/// for Exp-> NUM, NUM B, STRING, TRUE, FALSE
Exp_Obj::Exp_Obj(const std::string &str, Object *term, std::string type): Object(str){
    if (type == "byte") {
        if (stoi(term->name) > 255) {
            output::errorByteTooLarge(yylineno, term->name);
            exit(0);
        }
    }
    this->type = std::move(type);
    this->name = term->name;
}
///for Exp-> Exp BINOP Exp, Exp RELOP Exp, NOT Exp, Exp AND Exp, Exp OR Exp
Exp_Obj::Exp_Obj(const std::string &str, Object *first_term, Object *second_term, const std::string operation,
                 const std::string type): Object(str) {
    Exp_Obj *exp1 = dynamic_cast<Exp_Obj *>(first_term);
    Exp_Obj *exp2 = dynamic_cast<Exp_Obj *>(second_term);

    if (exp1->var && !symbol_table_stack.is_symbol_in_stack(exp1->name)) {
        output::errorUndef(yylineno, first_term->name);
        exit(0);
    }

    if (exp2->var && !symbol_table_stack.is_symbol_in_stack(exp2->name)) {
        output::errorUndef(yylineno, second_term->name);
        exit(0);
    }

    if (!is_matching(exp1->type, exp2->type)) {
        output::errorMismatch(yylineno);
        exit(0);
    }

    if (type == "bool") {
        if (exp1->type != "bool" || exp2->type != "bool") {
            output::errorMismatch(yylineno);
            exit(0);
        }
        this->type = "bool";

    }
    else{
        if (exp1->type == "bool" || exp2->type == "bool") {
            output::errorMismatch(yylineno);
            exit(0);
        }
        if (type == "relop")
        {
            this->type = "bool";
        }
        else
        {
            if (exp1->type == "int" || exp2->type == "int") {
                this->type = "int";
            }
            else {
                this->type = "byte";
            }
        }

    }
}

///for Exp -> ID, CALL
Exp_Obj::Exp_Obj(const std::string &str, bool var, Object *term): Object(str) {
    if (var && !symbol_table_stack.is_symbol_in_stack(term->name)) {
        output::errorUndef(yylineno, term->name);
        exit(0);
    }
    this->var = var;
    this->name = term->name;
    this->type = symbol_table_stack.get_symbol_by_name(term->name)->symbol_Type;

}

///for Exp -> LPAREN Type RPAREN Exp
Exp_Obj::Exp_Obj(const std::string &str, Object *first_term, Object *second_term): Object(str) {
    Type_Obj *_type = dynamic_cast<Type_Obj *>(first_term);
    Exp_Obj *_exp = dynamic_cast<Exp_Obj *>(second_term);

    if (!is_matching(_type->type, _exp->type) || _type->type == "bool") {
        output::errorMismatch(yylineno);
        exit(0);
    }

    this->name = _exp->name;
    this->type = _type->type;
}

/// for Exp -> LPAREN Exp RPAREN
Exp_Obj::Exp_Obj(const std::string &str, Exp_Obj *exp) : Object(str){
    this->name = exp->name;
    this->type = exp->type;
}


///for Call-> ID LPAREN Exp RPAREN
Call_Obj::Call_Obj(const std::string &str, Object *first_term, Object *second_term): Object(str) {
    Exp_Obj *_exp = dynamic_cast<Exp_Obj *>(second_term);

    if (!symbol_table_stack.get_symbol_by_name(first_term->name)) {
        output::errorUndefFunc(yylineno, first_term->name);
        exit(0);
    }

    if (!symbol_table_stack.get_symbol_by_name(first_term->name)->is_symbol_function) {
        output::errorUndefFunc(yylineno, first_term->name);
        exit(0);
    }

    if (symbol_table_stack.get_symbol_by_name(first_term->name)->arg_type != _exp->type) {
        if (symbol_table_stack.get_symbol_by_name(first_term->name)->arg_type != "int" || _exp->type != "byte") {
            output::errorPrototypeMismatch(yylineno, first_term->name, symbol_table_stack.get_symbol_by_name(first_term->name)->arg_type);
            exit(0);
        }
    }

    this->type = symbol_table_stack.get_symbol_by_name(first_term->name)->symbol_Type;
    this->name = symbol_table_stack.get_symbol_by_name(second_term->name)->symbol_Name;
}


///for Statement -> BREAK SC, CONTINUE SC, RETURN SC
State_Obj::State_Obj(const std::string &str, Object *term): Object(str) {
    if (term->name == "break") {
        if (!symbol_table_stack.while_scope_exist()) {
            output::errorUnexpectedBreak(yylineno);
            exit(0);
        }
    } else if (term->name == "continue") {
        if (!symbol_table_stack.while_scope_exist()) {
            output::errorUnexpectedContinue(yylineno);
            exit(0);
        }
    }
}

///for Statement -> Type ID SC
State_Obj::State_Obj(const std::string &str, Type_Obj *type, Object *term): Object(str) {
    if (symbol_table_stack.is_symbol_in_stack(term->name)) {
        output::errorDef(yylineno, term->name);
        exit(0);
    }
    symbol_table_stack.insert_Symbol_to_stack(term->name, type->type, false, "");
    this->name = type->name;
}

///for Statement -> Type ID ASSIGN Exp SC
State_Obj::State_Obj(const std::string &str, Type_Obj *type, Object *term, Exp_Obj *exp): Object(str) {
    if (symbol_table_stack.is_symbol_in_stack(term->name)) {
        output::errorDef(yylineno, term->name);
        exit(0);
    }
    if (!(type->type == exp->type) || !(type->type =="int" && exp->type == "byte")) {
        output::errorMismatch(yylineno);
        exit(0);
    }
    symbol_table_stack.insert_Symbol_to_stack(term->name, type->type, false, "");
}

///for Statement -> Call SC
State_Obj::State_Obj(const std::string &str, Call_Obj *call): Object(str) {
    if (!symbol_table_stack.is_symbol_in_stack(call->name)) {
        output::errorUndefFunc(yylineno, call->name);
        exit(0);
    }
    if (!symbol_table_stack.get_symbol_by_name(call->name)->is_symbol_function) {
        output::errorUndefFunc(yylineno, call->name);
        exit(0);
    }
}

///for Statement -> ID ASSIGN Exp SC
State_Obj::State_Obj(const std::string &str, Object *term, Exp_Obj *exp): Object(str) {
    if (!symbol_table_stack.is_symbol_in_stack(term->name)) {
        output::errorUndef(yylineno, term->name);
        exit(0);
    }
    if (symbol_table_stack.get_symbol_by_name(term->name)->is_symbol_function || !(symbol_table_stack.get_symbol_by_name(term->name)->symbol_Type == exp->type) || !(symbol_table_stack().get_symbol_by_name(term->name)->symbol_Type == "int" && exp->type == "byte")) {
        output::errorMismatch(yylineno);
        exit(0);
    }
}

///for Statement -> IF LPAREN Exp RPAREN Statement, IF LPAREN Exp RPAREN Statement ELSE Statement, WHILE LPAREN Exp RPAREN Statement
State_Obj::State_Obj(const std::string &str, const std::string name, Exp_Obj *exp): Object(str) {
    if (exp->type != "bool") {
        output::errorMismatch(yylineno);
        exit(0);
    }
}


///for Statements -> Statement
States_Obj::States_Obj(const std::string &str, State_Obj *statement): Object(str) {
}

///for Statements -> Statements Statement
States_Obj::States_Obj(const std::string &str, States_Obj *statements, State_Obj *statement): Object(str) {
}


//=================================================================== up side is types and down side is symboltable

//================ Symbol methods =============================

Symbol::Symbol(std::string name, std::string type, int offset, bool isSymbolFunction, std::string argType) {
    this->symbol_Name = name;
    this->symbol_Offset = offset;
    this->symbol_Type = type;
    this->is_symbol_function = isSymbolFunction;
    this->arg_type = argType;
}


//================ Symbol Table methods =============================

Symbol_Table::Symbol_Table(bool isWhileScope): Table() {
    this->is_while_scope = isWhileScope;
}

void Symbol_Table::insert_Symbol_to_table(const Symbol &symbol_to_insert) {
    Symbol* s = new Symbol(symbol_to_insert);
    this->Table.push_back(s);
}


//================================================ tables stack methods ===========================================

symbol_table_stack::symbol_table_stack(): symbol_table_stack(), offset_stack()
{
    //the first variance in the first table will open in offset 0
    this->offset_stack.push_back(0);
    this->insert_Symbol_to_stack("print","void", true,{"string"});
    this->insert_Symbol_to_stack("printi","void", true,{"int"});
    this->insert_Symbol_to_stack("print","int", true,{"int"});
}

void symbol_table_stack::insert_Symbol_to_stack(std::string name, std::string type, bool isSymbolFunction, string argType = "")
{
    Symbol_Table *curr_symbol_table = this->symbol_table_stack.back();
    int new_symbol_offset = (isSymbolFunction) ? 0 : this->offset_stack.back();
    if (!isSymbolFunction) {
        //the next variance will enter to the scope with offset new_symbol_offset+1
        this->offset_stack.push_back(new_symbol_offset + 1);
    }
    curr_symbol_table->insert_Symbol_to_table(Symbol(name, type, new_symbol_offset, isSymbolFunction, argType));
}

bool symbol_table_stack::is_symbol_in_stack(const std::string &symbol_name)
{
    for (Symbol_Table* symbolTable:this->symbol_table_stack){
        if(symbolTable){
            for(Symbol* s: symbolTable->Table){
                if(s){
                    if(s->symbol_Name == symbol_name){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void symbol_table_stack::make_table(bool scope_is_loop)
{
    Symbol_Table* to_insert = new Symbol_Table(scope_is_loop);
    this->offset_stack.push_back(this->offset_stack.back());
    this->symbol_table_stack.push_back(to_insert);
}

string type_to_send(const string& type)
{
    if(type == "int"){
        return "INT";
    }
    else if(type == "byte"){
        return "BYTE";
    }
    else if (type == "string"){
        return "STRING";
    }
    else if(type == "bool"){
        return "BYTE";
    }
    return "VOID";
}

void symbol_table_stack::print_scope(const Symbol_Table &symbol_table)
{
    for (Symbol* s:symbol_table.Table)
    {
        int s_offset = this->offset_stack.back();
        this->offset_stack.pop_back();
        if(!s->is_symbol_function){
            output::printID(s->symbol_Name,s->symbol_Offset, type_to_send(s->symbol_Type));
        }
        else{
            output::makeFunctionType(type_to_send(s->symbol_Type), type_to_send(s->arg_type));
        }
    }
}


void symbol_table_stack::pop_table()
{
    Symbol_Table* to_delete = this->symbol_table_stack.back();
    this->symbol_table_stack.pop_back();
    output::endScope();
    print_scope(*to_delete);
    if(this->offset_stack.size() > 0){
        this->offset_stack.pop_back();
    }
    delete to_delete;
}


bool symbol_table_stack::while_scope_exist()
{
    for (Symbol_Table* symbolTable:this->symbol_table_stack){
        if(symbolTable->is_while_scope){
            return true;
        }
    }
    return false;
}

Symbol *symbol_table_stack::get_symbol_by_name(const std::string &symbol_name) {
    for (Symbol_Table* symbolTable:this->symbol_table_stack){
        if(symbolTable){
            for(Symbol* s: symbolTable->Table){
                if(s){
                    if(s->symbol_Name == symbol_name){
                        return s;
                    }
                }
            }
        }
    }
    return nullptr;
}




