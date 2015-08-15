//
//  Sprite.cpp
//  D2DEngine_V2
//
//  Created by dai on 15/8/15.
//  Copyright (c) 2015年 mac. All rights reserved.
//

#include "Sprite.h"
#include "ShaderUtil.h"
#include "Director.h"
#include "Utils.h"
#include "globals.h"
#include <assert.h>

namespace D2D {
    
    Sprite::Sprite(Texture2D* texture):
    _texture(texture)
    {
        assert(texture);
        
        _color[0] = _color[1] = _color[2] = _color[3] = 1.0f;
        _shader = ShaderUtil::getInstance()->getShader(ShaderType::Shader_Sprite);
        setContentSize(_texture->getWidth(),_texture->getHeight());
        
        // 按照 0,1,2,  0,2,3 顺序
        //---- vertices
        _vertices[0] = 0;
        _vertices[1] = 0;
        
        _vertices[2] = _texture->getWidth();
        _vertices[3] = 0;
        
        _vertices[4] = _texture->getWidth();
        _vertices[5] = _texture->getHeight();

        _vertices[6] = 0;
        _vertices[7] = 0;
        
        _vertices[8] = _texture->getWidth();
        _vertices[9] = _texture->getHeight();
        
        _vertices[10] = 0;
        _vertices[11] = _texture->getHeight();

        
        //---- texCoords
        _texCoords[0] = 0;
        _texCoords[1] = 1;
        
        _texCoords[2] = 1;
        _texCoords[3] = 1;
        
        _texCoords[4] = 1;
        _texCoords[5] = 0;
        
        _texCoords[6] = 0;
        _texCoords[7] = 1;
        
        _texCoords[8] = 1;
        _texCoords[9] = 0;
        
        _texCoords[10] = 0;
        _texCoords[11] = 0;
        
    }
    
    void Sprite::setTexture(Texture2D* texture){
        assert(texture);
        _texture = texture;
        
        //---- vertices
        _vertices[0] = 0;
        _vertices[1] = 0;
        
        _vertices[2] = _texture->getWidth();
        _vertices[3] = 0;
        
        _vertices[4] = _texture->getWidth();
        _vertices[5] = _texture->getHeight();
        
        _vertices[6] = 0;
        _vertices[7] = 0;
        
        _vertices[8] = _texture->getWidth();
        _vertices[9] = _texture->getHeight();
        
        _vertices[10] = 0;
        _vertices[11] = _texture->getHeight();
    }
    
    void Sprite::drawSelf(){
        
     //   log("Sprite::drawSelf....................");
        
        glUseProgram(_shader->getProgramId());
        
        Mat4 mvpMatrix;
        Mat4::multiply(Director::getInstance()->getProjMat4(),_modelMatrix,&mvpMatrix);
        
        glUniformMatrix4fv(_shader->get_U_MatrixId(),1, GL_FALSE,mvpMatrix.m);
        glUniform4fv(_shader->get_U_ColorId(),1,_color);
        
        glVertexAttribPointer(_shader->get_A_PositionId(),2,GL_FLOAT,GL_FALSE,0,_vertices);
        glEnableVertexAttribArray(_shader->get_A_PositionId());
        
        glVertexAttribPointer(_shader->get_A_TexCoordsId(),2,GL_FLOAT,GL_FALSE,0,_texCoords);
        glEnableVertexAttribArray(_shader->get_A_TexCoordsId());
        
        glBindTexture(GL_TEXTURE_2D,_texture->getTextureId());
        
        if (_texture->hasAlpha()) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        }else{
            glDisable(GL_BLEND);
        }
        
        glDrawArrays(GL_TRIANGLES,0,6);
        CheckGLError();
        
        glDisableVertexAttribArray(_shader->get_A_PositionId());
        glDisableVertexAttribArray(_shader->get_A_TexCoordsId());
        
        if (_texture->hasAlpha()) {
            glDisable(GL_BLEND);
        }
    }
};