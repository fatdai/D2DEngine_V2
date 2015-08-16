//
//  Texture2D.h
//  D2DEngine
//
//  Created by dai on 15/8/2.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#ifndef __D2DEngine__Texture2D__
#define __D2DEngine__Texture2D__

#include <stdio.h>
#include <string>
#include <OpenGL/gl.h>
#include <map>
#include "CCGeometry.h"
#include "Types.h"
using namespace std;

// 暂时不支持多线程加载纹理
namespace D2D {
    
    
//    typedef enum {
//        PNG,
//    } IMG_Type;
   
    class Texture2D;
    class TextureCache{
    public:
        ~TextureCache();
        static TextureCache* _sTextureCache;
        static TextureCache* getInstance(){
            return _sTextureCache;
        }
        static void init();
        
        Texture2D* addImage(const string& relativePath);
        
        void release();
        
    private:
        map<string,Texture2D*> _textures;
    };
    
    
    //-------------------------------------------
    class Texture2D{
        
    public:
        
        enum class PixelFormat{
            I8,
            RGB888,
            RGBA8888,
        };
        
        Texture2D(){}
        Texture2D(const string& relativePath);
        ~Texture2D();
        
        GLuint getTextureId(){
            return _textureId;
        }
        int getWidth(){
            return _width;
        }
        int getHeight(){
            return _height;
        }
        
        bool hasAlpha(){return _hasAlpha;}
        
        
        
    public:
        
        static void convertRGB888ToRGBA8888(const unsigned char* data, ssize_t dataLen, unsigned char* outData);
        void initWithData(const void* data,ssize_t dataLen,Texture2D::PixelFormat format,int width,int height);
        
    private:
        
        void readPng(const string& fullpath);
        void readJpeg(const string& fullpath);
    private:
        
        GLuint _textureId = 0;
        int _width = 0 , _height = 0;
        PixelFormat _pixelFormat;
        bool _hasAlpha = true;
    };
    
    //-------------------------------------------
    class Shader;
    class TextureAtlas{
        
    public:
        
        TextureAtlas(Texture2D* texture,int capacity,Shader* shader);
        
        ~TextureAtlas();
        
        // 插入一个 quad
        void insertQuad(V2F_C4F_T2F_Quad* quad,ssize_t index);
        
        // 插入一些
        void insertQuads(V2F_C4F_T2F_Quad* quads,ssize_t index,int count);
        
    private:
        
        void setupVAOAndVBO();
        
    private:
        
        GLushort* _indices = nullptr;
        GLuint _vao = 0;
        GLuint _bufferVBO[2];
        bool _dirty = false;
        
        ssize_t _totalQuads = 0;
        ssize_t _capacity = 0;
        
        Texture2D* _texture = nullptr;
        V2F_C4F_T2F_Quad* _quads = nullptr;
        Shader* _shader = nullptr;
    };
}

#endif /* defined(__D2DEngine__Texture2D__) */
