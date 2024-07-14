//
// Created by Nir on 7/8/2024.
//

#ifndef COMPI_HW3_SYMBOTABLE_H
#define COMPI_HW3_SYMBOTABLE_H

#include "vector"
#include "string"



//================================================================= up side is types and down side is symboltable + stack
//of symbol tables==========================

class Symbol{
public:
    std::string Name;
    std::string Type;
    int Offset;
    bool isFunctionSymbol;
    std::vector<std::string> functionParams;


    Symbol(std::string name, std::string type,int offset,bool is_symbol_function,std::vector<std::string> functions_params);
    ~Symbol() = default;

};


class SymbolTable {
public:
    std::vector<Symbol*> Table;
    int last_var_offset;

    SymbolTable();

    void insertSymbol(const Symbol &symbol);
    bool isSymbolInTable(const std::string &name);


    ~SymbolTable() = default;
};


class SymbolTableStack{

    std::vector<SymbolTable*> symbol_table_stack;
    std::vector<int> offset_stack;

    void insertSymbol(std::string name, std::string type,int offset,bool is_symbol_function,std::vector<std::string> functions_params);

    void make_table();

    SymbolTableStack();

    ~SymbolTableStack() = default;



};






#endif //COMPI_HW3_SYMBOTABLE_H
