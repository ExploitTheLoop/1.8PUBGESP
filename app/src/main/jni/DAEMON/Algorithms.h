//
// Created by PCAditiaID29 on 2/10/2021.
//

#ifndef RCHEATS_ALGORITHMS_H
#define RCHEATS_ALGORITHMS_H

class Algorithms {
public:
    static D3DMatrix ReadViewMatrix(uintptr_t vAddv){
        return Read<D3DMatrix>(vAddv);
    }
};

#endif //RCHEATS_ALGORITHMS_H
