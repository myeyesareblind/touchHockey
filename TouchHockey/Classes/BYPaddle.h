//
//  BYPaddle.h
//  TouchHockey
//
//  Created by myeyesareblind on 10/6/12.
//
//

#ifndef __TouchHockey__BYPaddle__
#define __TouchHockey__BYPaddle__

#include <iostream>
#include "BYGameObject.h"

class BYPaddle : public BYGameObject {
protected:
    typedef BYGameObject super;

public:
    virtual ~BYPaddle();
};

#endif /* defined(__TouchHockey__BYPaddle__) */
