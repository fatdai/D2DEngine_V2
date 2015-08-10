//
//  Texture2D.cpp
//  D2DEngine
//
//  Created by dai on 15/8/2.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#include "Texture2D.h"
#include "Utils.h"
#include "FileUtil.h"
#include <ctype.h>
#include "ImageLoader.h"
#include <cstdlib>
#include <assert.h>
#include "ShaderUtil.h"

namespace D2D {
    
    TextureCache* TextureCache::_textureCache = nullptr;
    
    TextureCache::~TextureCache(){
        auto it = _textures.begin();
        while (it != _textures.end()) {
            delete it->second;
            ++it;
        }
        _textures.clear();
    }
    
    void TextureCache::init(){
        if (_textureCache == nullptr) {
            _textureCache = new TextureCache;
        }
    }
    
    Texture2D* TextureCache::addImage(const string& relativePath){
        
        // 先检查是否已经存在
        auto it = _textures.begin();
        while (it != _textures.end()) {
            if (it->first == relativePath) {
                return it->second;
            }
        }
        auto texture2d = new Texture2D(relativePath);
        _textures.insert(pair<string,Texture2D*>(relativePath,texture2d));
        return texture2d;
    }
    
    //*******************************************
    //*******************************************
    
    Texture2D::~Texture2D(){
        if (_textureId > 0) {
            glDeleteTextures(1,&_textureId);
            _textureId = 0;
        }
    }
    
    Texture2D::Texture2D(const string& relativePath){
        // 根据后缀判断
        auto postfix = GetPostfix(relativePath);
        transform(postfix.begin(),postfix.end(),postfix.begin(),tolower);
        auto fullpath = FileUtil::getInstacne()->getFullPath(relativePath);
      //  printf("fullpath:%s\n",fullpath.c_str());
        
        if (postfix == "png") {
            readPng(fullpath);
        }else{
            printf("can not support image formatter : %s\n",postfix.c_str());
        }

    }
    
    void Texture2D::readPng(const string& fullpath){
        png_data_t png_data;
        if(!png_read(fullpath.c_str(),&png_data))
        {
            _width = png_data.width;
            _height = png_data.height;
            
            _pixelFormat = PixelFormat::GRBA8888;
            
            glGenTextures(1,&_textureId);
            glBindTexture(GL_TEXTURE_2D,_textureId);
            
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
            
            glTexImage2D(GL_TEXTURE_2D,0,
                         GL_RGBA,
                         png_data.width,png_data.height,0,
                         GL_RGBA,GL_UNSIGNED_BYTE,png_data.data);
            free(png_data.data);  
        }
    }
    
//    void Texture2D::initWithFontData(const void *data, ssize_t dataLen, int pixelsWide, int pixelsHigh, const Size& contentSize){
//        assert(dataLen>0 && pixelsWide>0 && pixelsHigh>0);
//        glGenTextures(1,&_textureId);
//        glBindTexture(GL_TEXTURE_2D,_textureId);
//        
//        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
//        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
//        CheckGLError();
//        
//        glTexImage2D(GL_TEXTURE_2D,0,GL_ALPHA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0,GL_ALPHA,GL_UNSIGNED_BYTE,data);
//    }
    
    void Texture2D::initWithData(const void* data,ssize_t dataLen,Texture2D::PixelFormat format,int width,int height){
        assert(dataLen > 0 && width > 0 && height > 0);
        
        glGenTextures(1,&_textureId);
        glBindTexture(GL_TEXTURE_2D,_textureId);
        
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        
        if (format == PixelFormat::I8) {
            glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,width,height,0,GL_UNSIGNED_BYTE,GL_LUMINANCE,data);
        }else if (format == PixelFormat::GRBA8888){
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_UNSIGNED_BYTE,GL_RGBA,data);
        }
        CheckGLError();
        
        _width = width;
        _height = height;
        _pixelFormat = format;
    }
    
    //-------------------------------------------
    TextureAtlas::~TextureAtlas(){
        if (_quads) {
            delete [] _quads;
            _quads = nullptr;
        }
        if (_indices) {
            delete [] _indices;
            _indices = nullptr;
        }
        if (_vao > 0) {
            glDeleteVertexArraysAPPLE(1,&_vao);
            _vao = 0;
        }
        
        if (_bufferVBO[0] > 0) {
            glDeleteBuffers(2,&_bufferVBO[0]);
            _bufferVBO[0] = _bufferVBO[1] = 0;
        }
    }
    
    TextureAtlas::TextureAtlas(Texture2D* texture,int capacity,Shader* shader):
    _texture(texture),
    _shader(shader),
    _capacity(capacity){
        assert(capacity > 0);
        _bufferVBO[0] = _bufferVBO[1] = 0;
        _quads = new V2F_C4F_T2F_Quad[_capacity]();
        _indices = new GLushort[6 * _capacity]();
        
        for (int i = 0; i < _capacity; ++i) {
            _indices[i * 6] = i * 4 + 0;
            _indices[i * 6 + 1] = i * 4 + 1;
            _indices[i * 6 + 2] = i * 4 + 2;
            
            _indices[i * 6 + 3] = i * 4 + 0;
            _indices[i * 6 + 4] = i * 4 + 2;
            _indices[i * 6 + 5] = i * 4 + 3;
        }
        
        // 设置 vbo 和 vao
        setupVAOAndVBO();
    }
    
    void TextureAtlas::insertQuad(V2F_C4F_T2F_Quad* quad,ssize_t index){
        
        // index 可能大于 _totalQuads
        
        assert(index < _capacity && index >= 0);
        ++_totalQuads;
        assert(_totalQuads <= _capacity);
        
        auto remaining = (_totalQuads - 1) - index;
        if (remaining > 0) {
            memmove(&_quads[index + 1],&_quads[index],sizeof(_quads[0]) * remaining);
        }
        _quads[index] = *quad;
        _dirty = true;
    }
    
    void TextureAtlas::insertQuads(V2F_C4F_T2F_Quad* quads,ssize_t index,int count){
        assert(index >= 0 && index + count <= _capacity);
        _totalQuads += count;
        assert(_totalQuads <= _capacity);
        auto remaining = (_totalQuads - 1) - index - count;
        if (remaining > 0) {
            memmove(&_quads[index + count],&_quads[index],sizeof(_quads[0]) * remaining);
        }
        
        auto max = index + count;
        int j = 0;
        for (ssize_t i = index; i < max ; ++i) {
            _quads[index] = quads[j];
            ++index;
            ++j;
        }
        
        _dirty = true;
    }
    
    void TextureAtlas::setupVAOAndVBO(){
        
        glGenVertexArraysAPPLE(1,&_vao);
        glBindVertexArrayAPPLE(_vao);
        
        glGenBuffers(2,&_bufferVBO[0]);
        glBindBuffer(GL_ARRAY_BUFFER,_bufferVBO[0]);
        glBufferData(GL_ARRAY_BUFFER,sizeof(V2F_C4F_T2F_Quad) * _capacity,_quads,GL_DYNAMIC_DRAW);
        
#define kStrip sizeof(_quads[0].bl)
        
        // vertices
        glEnableVertexAttribArray(_shader->get_A_PositionId());
        glVertexAttribPointer(_shader->get_A_PositionId(),2,GL_FLOAT,GL_FALSE,kStrip, (GLvoid*)offsetof(V2F_C4F_T2F,vertices));

        // colors
        glEnableVertexAttribArray(_shader->get_A_ColorId());
        glVertexAttribPointer(_shader->get_A_ColorId(),4,GL_FLOAT,GL_FALSE,kStrip,(GLvoid*)offsetof(V2F_C4F_T2F,colors));
        
        // texCoords
        glEnableVertexAttribArray(_shader->get_A_TexCoordsId());
        glVertexAttribPointer(_shader->get_A_TexCoordsId(),2,GL_FLOAT,GL_FALSE,kStrip, (GLvoid*)offsetof(V2F_C4F_T2F,texCoords));
        
        
        // indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_bufferVBO[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(_indices[0]) * _capacity * 6,_indices,GL_STATIC_DRAW);
        
        // unbind vao vbos
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArrayAPPLE(0);
    }
    
}