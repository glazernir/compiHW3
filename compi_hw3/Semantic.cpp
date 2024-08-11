//
// Created by Nir on 09/08/2024.
//

#include "Semantic.h"

Semantic::Semantic() {
    this->label_index = 0;
}

std::string Semantic::freshVar(bool isVarGlobal) {
    std::string stringIndex = std::to_string(this->reg_index);
    this->reg_index++;
    if(isVarGlobal) {
        std::string res = "@reg_" + stringIndex;
        return res;
    }
    std::string res = "%reg_" + stringIndex;
    return res;
}






















