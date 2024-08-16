//
// Created by Nir on 7/8/2024.
//

#ifndef COMPI_HW3_SYMBOLTABLE_H
#define COMPI_HW3_SYMBOLTABLE_H

#include <vector>
#include <string>
#include <iostream>
#include "hw3_output.hpp"




class Object{
public:
public:
    std::string name;
    std::string register_val;
    explicit Object(const std::string& str);
    virtual ~Object() = default;

};

#define YYSTYPE Object*

class Prog_Obj: public Object{
public:

    //program -> statements
    explicit Prog_Obj(const std::string &str);
    ~Prog_Obj() override = default;
};


class Type_Obj: public Object{
public:
    std::string type;
    //TYPE -> INT | BOOL | BYTE
    explicit Type_Obj(const std::string &str, std::string type);
    ~Type_Obj() override = default;
};

class Exp_Obj: public Object{
public:
    std::string type;
    bool var=false;
    std::string trueLabel;
    std::string falseLabel;


    explicit Exp_Obj(const std::string &str);

    Exp_Obj(const std::string &str, Object *term, std::string type);

    Exp_Obj(const std::string &str, Object *first_term, Object *second_term, const std::string operation, const std::string type = "");

    Exp_Obj(const std::string &str, bool var, Object *term);

    Exp_Obj(const std::string &str, Object *exp, Object *type);

    Exp_Obj(const std::string &str, Exp_Obj *exp);

    ~Exp_Obj() override = default;

};

class Call_Obj: public Object{
public:
    std::string type;

    Call_Obj(const std::string &str, Object* first_term , Object* second_term);
    ~Call_Obj() override = default;
};


class State_Obj: public Object{
public:

    //Statements -> LBRACE STATEMENT RBRACE
    State_Obj(const std::string &str, Object*term);
    //Statements -> TYPE ID SC
    State_Obj(const std::string &str, Type_Obj*type , Object*term );
    //Statements -> TYPE ID ASSIGN Exp SC
    State_Obj(const std::string &str, Type_Obj*type , Object*term , Exp_Obj*exp );
    //Statements -> Call SC
    State_Obj(const std::string &str, Call_Obj* call);

    State_Obj(const std::string &str, Object *term, Exp_Obj *exp);

    State_Obj(const std::string &str, const std::string name, Exp_Obj *exp);

    ~State_Obj() override = default;



};

class States_Obj: public Object{
public:

    //Statements -> Statement
    States_Obj(const std::string &str, State_Obj* statement);
    //Statements -> Statements Statement
    States_Obj(const std::string &str, States_Obj*statements , State_Obj*statement );
    ~States_Obj() override = default;
};

void VerifyBool(Object* o);


//================================================================= up side is types and down side is symboltable + stack
//of symbol tables==========================

class Symbol{

public:
    std::string symbol_Name;
    std::string symbol_Type;
    int symbol_Offset;
    bool is_symbol_function;
    std::string arg_type;

    Symbol(std::string, std::string ,int ,bool,std::string);
    ~Symbol() = default;
};


class Symbol_Table {
public:
    //symbol table fields
    std::vector<Symbol*> Table;
    bool is_while_scope;
    std::string startScopeLabel;
    std::string endScopeLabel;

    //symbol table methods
    explicit Symbol_Table(bool);
    void insert_Symbol_to_table(const Symbol &);
    bool is_symbol_in_table(const std::string &);
    Symbol* get_symbol_from_table(const std::string& );
    ~Symbol_Table() = default;
};


class Symbol_table_stack{
public:
    std::vector<Symbol_Table*> symbol_table_stack;
    std::vector<int> offset_stack;

    Symbol_table_stack();
    Symbol_Table* getCurrerntWhileScope();
    void insert_Symbol_to_stack(std::string , std::string ,bool ,std::string);
    bool is_symbol_in_stack(const std::string &);
    bool while_scope_exist();
    Symbol* get_symbol_by_name(const std::string&);
    void check_if_main_exists();
    void printNS();
    void print_scope(const Symbol_Table &);
    void make_table(bool scope_is_loop);
    void pop_table();
    ~Symbol_table_stack() = default;
};





#endif //COMPI_HW3_SYMBOLTABLE_H
