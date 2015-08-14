//
//  MatrixStack.h
//  D2DEngine_V2
//
//  Created by dai on 15/8/14.
//  Copyright (c) 2015年 mac. All rights reserved.
//

#ifndef __D2DEngine_V2__MatrixStack__
#define __D2DEngine_V2__MatrixStack__

#include <stdio.h>
#include <vector>
#include <stack>
#include "CCMath.h"
#include <assert.h>
using namespace std;

namespace D2D {
    class MatrixStack{
    public:
        
        static stack<Mat4> _sMatStack;
        
        static void push(const Mat4& mat){
            _sMatStack.push(mat);
        }
        
        static void pop(){
            assert(_sMatStack.size() > 0);
            _sMatStack.pop();
        }
    };
}

#endif /* defined(__D2DEngine_V2__MatrixStack__) */
