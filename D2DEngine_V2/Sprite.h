//
//  Sprite.h
//  D2DEngine_V2
//
//  Created by dai on 15/8/15.
//  Copyright (c) 2015年 mac. All rights reserved.
//

#ifndef __D2DEngine_V2__Sprite__
#define __D2DEngine_V2__Sprite__

#include <stdio.h>
#include "Node.h"
#include "Texture2D.h"
#include "Types.h"

namespace D2D {
    
    class Sprite : public Node{
        
    public:
        
        Sprite(Texture2D* texture);
        
        virtual void drawSelf() override;
        
    public:
        
        void setColor(float r,float g,float b,float a){
            _color[0] = r;
            _color[1] = g;
            _color[2] = b;
            _color[3] = a;
        };
        
        void setTexture(Texture2D* texture);
        
    protected:
        
        float _vertices[12];
        float _texCoords[12];
        
        float _color[4];
        Texture2D* _texture = nullptr;
        
        Shader* _shader = nullptr;
    };
    
}

#endif /* defined(__D2DEngine_V2__Sprite__) */
