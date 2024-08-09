//
// Created by Nir on 09/08/2024.
//

#ifndef SEMANTIC_H
#define SEMANTIC_H
#include <string>



class Semantic {
    int reg_index;
    int label_index;
public:

    Semantic();
    std::string freshVar(bool isVarGlobal);





};



#endif //SEMANTIC_H
