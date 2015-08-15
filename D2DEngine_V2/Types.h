//
//  Types.h
//  D2DEngine
//
//  Created by dai on 15/8/8.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#ifndef __D2DEngine__Types__
#define __D2DEngine__Types__

#include <stdio.h>
#include "CCGeometry.h"

namespace D2D {
    
    //-------------------------------------
    class Tex2F{
    public:
        float u = 0;
        float v = 0;
    };
    
    //-------------------------------------
    class Color4F{
    public:
        float r = 1.0f;
        float g = 1.0f;
        float b = 1.0f;
        float a = 1.0f;
    };
    
    //-------------------------------------
    class V2F_T2F{
    public:
        Vec2 vertices;
        Tex2F texCoords;
    };
    
    //-------------------------------------
    class V2F_T2F_Quad{
    public:
        V2F_T2F bl;
        V2F_T2F br;
        V2F_T2F tr;
        V2F_T2F tl;
    };
    
    
    //-------------------------------------
    class V2F_C4F_T2F{
    public:
        Vec2 vertices;
        Color4F colors;
        Tex2F texCoords;
    };
    
    //-------------------------------------
    class V2F_C4F_T2F_Quad{
    public:
        V2F_C4F_T2F bl;
        V2F_C4F_T2F br;
        V2F_C4F_T2F tr;
        V2F_C4F_T2F tl;
    };
}

#endif /* defined(__D2DEngine__Types__) */
