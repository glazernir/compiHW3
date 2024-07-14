//
// Created by Nir on 7/8/2024.
//

#include "SymboTable.h"
#include "hw3_output.hpp"












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

Symbol_Table_Stack::Symbol_Table_Stack(): symbol_table_stack(),offset_stack()
{
    //the first variance in the first table will open in offset 0
    this->offset_stack.push_back(0);
    this->insert_Symbol_to_stack("print","void", true,{"string"});
    this->insert_Symbol_to_stack("printi","void", true,{"int"});
    this->insert_Symbol_to_stack("print","int", true,{"int"});
}

void Symbol_Table_Stack::insert_Symbol_to_stack(std::string name,std::string type,bool isSymbolFunction,string argType)
{
    Symbol_Table *curr_symbol_table = this->symbol_table_stack.back();
    int new_symbol_offset = (isSymbolFunction) ? 0 : this->offset_stack.back();
    if (!isSymbolFunction) {
        //the next variance will enter to the scope with offset new_symbol_offset+1
        this->offset_stack.push_back(new_symbol_offset + 1);
    }
    curr_symbol_table->insert_Symbol_to_table(Symbol(name, type, new_symbol_offset, isSymbolFunction, argType));
}

bool Symbol_Table_Stack::is_symbol_in_stack(const std::string &symbol_name)
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

void Symbol_Table_Stack::make_table(bool scope_is_loop)
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

void Symbol_Table_Stack::print_scope(const Symbol_Table &symbol_table)
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


void Symbol_Table_Stack::pop_table()
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


bool Symbol_Table_Stack::while_scope_exist()
{
    for (Symbol_Table* symbolTable:this->symbol_table_stack){
        if(symbolTable->is_while_scope){
            return true;
        }
    }
    return false;
}

Symbol *Symbol_Table_Stack::get_symbol_by_name(const std::string &symbol_name) {
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




