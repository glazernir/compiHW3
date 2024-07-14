//
// Created by Nir on 7/8/2024.
//

#include "SymboTable.h"




//=================================================================== up side is types and down side is symboltable

//================ Symbol methods =============================

Symbol::Symbol(std::string name, std::string type, int offset, bool is_symbol_function,
               std::vector<std::string> functions_params) {
    this->Name = name;
    this->Offset = offset;
    this->Type = type;
    this->isFunctionSymbol = is_symbol_function;
    this->functionParams = functions_params;
}


//================ Symbol Table methods =============================

void SymbolTable::insertSymbol(const Symbol &symbol) {
    Symbol* s = new Symbol(symbol);
    this->Table.push_back(s);
    if(s->Offset >= 0){
        this->last_var_offset = s->Offset;
    }
}

bool SymbolTable::isSymbolInTable(const std::string &symbol_name) {

    for(Symbol* s: this->Table){
        if(s){
            if(s->Name == symbol_name){
                return true;
            }
        }
    }
}


//================ tables stack methods =============================












