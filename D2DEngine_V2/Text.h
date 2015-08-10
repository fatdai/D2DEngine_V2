//
//  Text.h
//  D2DEngine_V2
//
//  Created by dai on 15/8/10.
//  Copyright (c) 2015年 mac. All rights reserved.
//

#ifndef __D2DEngine_V2__Text__
#define __D2DEngine_V2__Text__

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <OpenGL/gl.h>
#include <vector>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H
using namespace std;


namespace D2D {
    
    class Shader;
    
    //--------------------------------------------------
    //  比较原始的 GlyphMetrics ,主要用来计算 单个 glyph 的大小,用于排版
    class GlyphMetrics{
    public:
        int height;
        int width;
        int advance;
        int bearingX;
        int bearingY;
    };
    
    //---------------------------------------------------
    class GlyphVertex{
    public:
        float x = 0,y = 0;
        float s = 0,t = 0;
    };
    
    //---------------------------------------------------
    // 每个字母自己绘制自己
    class Letter{
        
    public:
        
        Letter(){}
        ~Letter();
        
        void render();
        
        // 根据 glyphVertices ,填充 _vertices,_texCoords
        void build();
        
    public:
        
        //
        GlyphVertex _glyphVertices[4];
        
        // 纹理
        GLuint _texId = 0;
        
        // 渲染需要用到的顶点和纹理坐标
        float _vertices[12];
        float _texCoords[12];
        
        //  不需要自己管理
        float* _colors = nullptr;
        Shader* _shader = nullptr;
    };
    
    
    //---------------------------------------------------
    
    class Text{
        
    public:
        
        Text(const string& fontfile);
        ~Text();
        
        void render();
        
        void setString(const string& text){_text = text;}
        
        void setPosition(float x,float y){_x = x;_y = y;}
        
    private:
        
        void buildLetter(Letter& letter,const wchar_t ch,float& startX, float& startY);
        
        void loadFont(const string& fontfile);
        
    private:
        
        // 从哪开始绘制
        float _x = 0, _y = 0;
        
        // 是否需要限制宽度? 为了简单暂时不进行限制.
        
        // 需要绘制的文字
        string _text;
        u16string _unicodeStr;
        
        vector<Letter*> _letters;
        
        // 所有的文字渲染共用的
        Shader* _shader = nullptr;
        float _colors[4];
        
        FT_Face _ftFace;
        
        // 进行标识
        float _advanceX = 0;
        float _advanceY = 0;
    };
}


#endif /* defined(__D2DEngine_V2__Text__) */
