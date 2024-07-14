//
// Created by Nir on 7/8/2024.
//

#ifndef COMPI_HW3_SYMBOTABLE_H
#define COMPI_HW3_SYMBOTABLE_H

#include "vector"
#include "string"



class var_Node{
public:
    std::string val;
    var_Node(const std::string& val);
    virtual ~var_Node() = default;

};

class Program_Node: public var_Node{

    //program -> statements
    Program_Node();
    ~Program_Node() = default;
};

class Exp_Node: public var_Node{


};


class Type_Node: public var_Node{
public:
    std::string type;
    //TYPE -> INT | BOOL | BYTE
    Type_Node(std::string type);
    ~Type_Node() = default;
};

class Call_Node: public var_Node{
public:
    std::string type;
    //𝐶𝑎𝑙𝑙 → 𝐼𝐷 𝐿𝑃𝐴𝑅𝐸𝑁 𝐸𝑥𝑝 𝑅𝑃𝐴𝑅𝐸𝑁
    Call_Node(var_Node* ,Exp_Node* );
    ~Call_Node() = default;
};


class Statement_Node: public var_Node{

    //Statements -> LBRACE STATEMENT RBRACE
    Statement_Node(var_Node* );
    //Statements -> TYPE ID SC
    Statement_Node(Type_Node* ,var_Node* );
    //Statements -> TYPE ID ASSIGN Exp SC
    Statement_Node(Type_Node* ,var_Node* ,Exp_Node* );
    //Statements -> Call SC
    Statement_Node(Call_Node* );



};

class Statements_Node: public var_Node{

    //Statements -> Statement
    Statements_Node(Statements_Node* );
    //Statements -> Statements Statement
    Statements_Node(Statements_Node* ,Statement_Node* );
    ~Statements_Node() = default;
};






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


    //symbol table methods
    Symbol_Table(bool );
    void insert_Symbol_to_table(const Symbol &);
    bool is_symbol_in_table(const std::string &);
    Symbol* get_symbol_from_table(const std::string& );
    ~Symbol_Table() = default;
};


class Symbol_Table_Stack{

    std::vector<Symbol_Table*> symbol_table_stack;
    std::vector<int> offset_stack;

    Symbol_Table_Stack();
    void insert_Symbol_to_stack(std::string , std::string ,bool ,std::string);
    bool is_symbol_in_stack(const std::string &);
    bool while_scope_exist();
    Symbol* get_symbol_by_name(const std::string&);

    void print_scope(const Symbol_Table &);
    void make_table(bool scope_is_loop);
    void pop_table();
    ~Symbol_Table_Stack() = default;
};





#endif //COMPI_HW3_SYMBOTABLE_H
