//
//  Text.cpp
//  D2DEngine_V2
//
//  Created by dai on 15/8/10.
//  Copyright (c) 2015年 mac. All rights reserved.
//

#include "Text.h"
#include "ShaderUtil.h"
#include "Director.h"
#include "Utils.h"
#include "FileUtil.h"


namespace D2D {
    
    //----------------------------------------------
    // 静态数据
    const static int _sCacheTextureWidth = 512;
    const static int _sCacheTextureHeight = 512;
    
    //----------------------------------------------
    Letter::~Letter(){
        if (_texId == 0) {
            glDeleteTextures(1,&_texId);
            _texId = 0;
        }
    }
    
    void Letter::build(){
        
        // 0,1,2   0,2,3
        // vertices
        _vertices[0] = _glyphVertices[0].x;
        _vertices[1] = _glyphVertices[0].y;
        _vertices[2] = _glyphVertices[1].x;
        _vertices[3] = _glyphVertices[1].y;
        _vertices[4] = _glyphVertices[2].x;
        _vertices[5] = _glyphVertices[2].y;
        
        _vertices[6] = _glyphVertices[0].x;
        _vertices[7] = _glyphVertices[0].y;
        _vertices[8] = _glyphVertices[2].x;
        _vertices[9] = _glyphVertices[2].y;
        _vertices[10] = _glyphVertices[3].x;
        _vertices[11] = _glyphVertices[3].y;
        
        // texCoords
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
    
    void Letter::render(){
        
        glUseProgram(_shader->getProgramId());
        
        glUniformMatrix4fv(_shader->get_U_MatrixId(),1,GL_FALSE,Director::getInstance()->getProjMat4().m);
        glUniform4fv(_shader->get_U_ColorId(),1,_colors);
        
        glVertexAttribPointer(_shader->get_A_PositionId(),2,GL_FLOAT,GL_FALSE,0,_vertices);
        glEnableVertexAttribArray(_shader->get_A_PositionId());
        
        glVertexAttribPointer(_shader->get_A_TexCoordsId(),2,GL_FLOAT,GL_FALSE,0,_texCoords);
        glEnableVertexAttribArray(_shader->get_A_TexCoordsId());
        
        glBindTexture(GL_TEXTURE_2D,_texId);
        glDrawArrays(GL_TRIANGLES,0,6);
        
        glDisableVertexAttribArray(_shader->get_A_PositionId());
        glDisableVertexAttribArray(_shader->get_A_TexCoordsId());
    }
    
    //----------------------------------------------
    Text::~Text(){
        if (_ftFace) {
            FT_Done_Face(_ftFace);
            _ftFace = nullptr;
        }
        
        for (int i = 0;i < _letters.size();++i) {
            delete _letters[i];
        }
        _letters.clear();
    }
    
    Text::Text(const string& fontfile){
        
        // 加载字体等相关操作
        loadFont(fontfile);
        
        _shader = ShaderUtil::getInstance()->getShader(ShaderType::Shader_Font);
        
        // 黄色
        _colors[0] = 1.0f;
        _colors[1] = 1.0f;
        _colors[2] = 1.0f;
        _colors[3] = 1.0f;
    }
    
    // 暂时不考虑 混存 font
    void Text::loadFont(const string& fontfile){
        auto fullpath = FileUtil::getInstacne()->getFullPath(fontfile);
        if (FT_New_Face(Director::_sFreeTypeLibrary,fullpath.c_str(),0,&_ftFace)) {
            printf("FT_New_Face   load font error.\n");
            abort();
        }
        
        // 选择 unicode
        if (FT_Select_Charmap(_ftFace,FT_ENCODING_UNICODE)) {
            printf("FT_Select_Charmap error.\n");
            abort();
        }
        
        // set char size
        if (FT_Set_Char_Size(_ftFace,0,12*64,0,72)) {
            printf("ft_set char_size error.\n");
            abort();
        }
        
        if (FT_Set_Pixel_Sizes(_ftFace,30,30)) {
            printf("FT_Set_Pixel_Sizes error.\n");
            abort();
        }
    }
    
    void Text::buildLetter(Letter& letter,const wchar_t ch,float& startX, float& startY){
        if (FT_Load_Char(_ftFace,ch,FT_LOAD_RENDER)) {
            printf("[buildLetter] FT_Load_Char error.\n");
            abort();
        }
        
        
        FT_GlyphSlot slot = _ftFace->glyph;
        FT_Glyph_Metrics metrics = slot->metrics;
        auto bitmap = slot->bitmap;
        if (bitmap.pixel_mode != FT_PIXEL_MODE_GRAY) {
            printf("unsupport pixel_mode font.\n");
            abort();
        }
        
        auto buffer = new unsigned char[bitmap.width * bitmap.rows * 2];
        memset(buffer,0,bitmap.width * bitmap.rows * 2);
        
        int tempOffset = 0;
        int bitmapOffset = 0;
        for (int y = 0 ; y < bitmap.rows; ++y) {
            tempOffset = y * bitmap.width * 2;
            bitmapOffset = y * bitmap.pitch;
            for (int x = 0; x < bitmap.width ; ++x) {
                unsigned char temp = bitmap.buffer[bitmapOffset + x];
                buffer[tempOffset + 2 * x] = 255;
                //                buffer[tempOffset + 4 * x + 1] = 255;
                //                buffer[tempOffset + 4 * x + 2] = 255;
                buffer[tempOffset + 2 * x + 1] = temp;//(temp > 255?0:temp);
            }
        }
        
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        
        GLuint texId;
        glGenTextures(1,&texId);
        glBindTexture(GL_TEXTURE_2D,texId);
        
        glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE_ALPHA,bitmap.width,bitmap.rows,0,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE,buffer);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        
        delete [] buffer;
        
        // 计算像素的宽
        int pixel_width = (int)(metrics.width >> 6);
        int pixel_height = (int)(metrics.height >> 6);
        
        // 这里采用基线对齐
        float descent = (int)((metrics.height - metrics.horiBearingY) >> 6);
        
        //----- 开始计算顶点坐标,并且给letter的顶点赋值
        letter._glyphVertices[0].x = startX;
        letter._glyphVertices[0].y = startY - descent;
        letter._glyphVertices[1].x = startX + pixel_width;
        letter._glyphVertices[1].y = startY - descent;
        letter._glyphVertices[2].x = startX + pixel_width;
        letter._glyphVertices[2].y = startY + pixel_height - descent;
        letter._glyphVertices[3].x = startX;
        letter._glyphVertices[3].y = startY + pixel_height - descent;
        
        letter._texId = texId;
        
        // 步进
        startX += (metrics.horiAdvance >> 6);
        
        // 判断 advance
        if (_advanceY < (metrics.vertAdvance >> 6)) {
            _advanceY = ( metrics.vertAdvance >> 6 );
        }
    }
    
    void Text::render(){
        
        // 1. 先计算出每一个具体的Letter
        for (int i = 0;i < _letters.size();++i) {
            delete _letters[i];
        }
        _letters.clear();
        
        UTF8ToUTF16(_text,_unicodeStr);
        float startX = _x;
        float startY = _y;
        for (int i = 0;i < _unicodeStr.size(); ++i) {
            
            if (_unicodeStr[i] == '\n') {
                startY -= _advanceY;
                startX = _x;
                continue;
            }
            // 暂时不考虑 \n
            auto letter = new Letter;
            letter->_shader = _shader;
            letter->_colors = _colors;
            
            // 主要是计算几个顶点 和  纹理
            buildLetter(*letter,_unicodeStr[i],startX,startY);
            
            letter->build();
            _letters.push_back(letter);
        }
        
        
        // 需要计算出正确的位置
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        for (int i = 0; i < _letters.size(); ++i) {
            _letters[i]->render();
        }
        glDisable(GL_BLEND);
        CheckGLError();
    }
}