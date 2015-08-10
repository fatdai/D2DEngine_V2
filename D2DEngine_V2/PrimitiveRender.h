//
//  PrimitiveRender.h
//  D2DEngine
//
//  Created by dai on 15/8/2.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#ifndef __D2DEngine__PrimitiveRender__
#define __D2DEngine__PrimitiveRender__

#include <stdio.h>
#include "ShaderUtil.h"
#include "Director.h"
#include "Texture2D.h"
#include <assert.h>
using namespace D2D;

// 当成测试
class PrimitiveRender{
    
public:
    
    PrimitiveRender(){
        mShader = ShaderUtil::getInstance()->getShader(ShaderType::Shader_BaseTex);
        mTexture2D = TextureCache::getInstance()->addImage("home_bg.png");
        assert(mTexture2D);
    }
    
    void render(){
        
        float triangles[] = {
            160,120,
            480,120,
            480,320,
            
            160,120,
            480,320,
            160,320
        };
        
        float texCoords[] = {
            0,1,
            1,1,
            1,0,
            
            0,1,
            1,0,
            0,0
        };
        
        glUseProgram(mShader->getProgramId());
        
        glBindTexture(GL_TEXTURE_2D,mTexture2D->getTextureId());
        
        // 传递数据
        glUniformMatrix4fv(mShader->get_U_MatrixId(),1,GL_FALSE,Director::getInstance()->getProjMat4().m);
        glVertexAttribPointer(mShader->get_A_PositionId(),2,GL_FLOAT,GL_FALSE,0,triangles);
        glVertexAttribPointer(mShader->get_A_TexCoordsId(),2,GL_FLOAT,GL_FALSE,0,texCoords);
        
        glEnableVertexAttribArray(mShader->get_A_PositionId());
        glEnableVertexAttribArray(mShader->get_A_TexCoordsId());
        
        // draw
        glDrawArrays(GL_TRIANGLES,0,6);
    }
    
private:
    
    Shader* mShader = nullptr;
    Texture2D* mTexture2D = nullptr;
};

#endif /* defined(__D2DEngine__PrimitiveRender__) */
