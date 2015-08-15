//
//  ShaderUtil.cpp
//  D2DEngine
//
//  Created by dai on 15/8/2.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#include "ShaderUtil.h"
#include "Utils.h"
#include <assert.h>
#include "Director.h"
#include "FileUtil.h"
#include <fstream>

namespace D2D {
    
    //***********************************************
    // shader 路径
    const static string _TestVertexShader = "vertex.vert";
    const static string _TestFragShader = "frag.frag";
    
    const static string _BaseTexVertexShader = "base_tex.vert";
    const static string _BaseTexFragShader = "base_tex.frag";
    
    const static string _FontVertexShader = "font.vert";
    const static string _FontFragShader = "font.frag";
    
    const static string _ColorVertexShader = "test_color.vert";
    const static string _ColorFragShader = "test_color.frag";
    
    const static string _SpriteVertexShader = "sprite.vert";
    const static string _SpriteFragShader = "sprite.frag";
    
    Shader::~Shader(){
        if (_program > 0) {
            glDeleteProgram(_program);
            _program = 0;
        }
    }
    
    Shader::Shader(const string& vertFilePath,const string& fragFilePath,ShaderType shaderType){
        
        _shaderType = shaderType;
        
        // 先获取字符串
        auto vertSource = GetStringFromFile(vertFilePath);
        auto fragSource = GetStringFromFile(fragFilePath);
        
        printf("vertSource:%s\n",vertSource.c_str());
        printf("fragSource:%s\n",fragSource.c_str());
        
        _program = glCreateProgram();
        CheckGLError();
        GLuint vertShaderId = 0,fragShaderId = 0;
        
        // vert
        vertShaderId = glCreateShader(GL_VERTEX_SHADER);
        const GLchar* vSources[] = {
            vertSource.c_str(),
        };
        glShaderSource(vertShaderId,sizeof(vSources)/sizeof(*vSources),vSources,nullptr);
        glCompileShader(vertShaderId);
        
        GLint status;
        glGetShaderiv(vertShaderId,GL_COMPILE_STATUS,&status);
        if (!status) {
            printf("complile shader : %s   error!\n",vertFilePath.c_str());
            abort();
        }
        
        
        // frag
        fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar* fSources[] = {
            fragSource.c_str(),
        };
        glShaderSource(fragShaderId,sizeof(fSources)/sizeof(*fSources),fSources,nullptr);
        glCompileShader(fragShaderId);
        
        glGetShaderiv(fragShaderId,GL_COMPILE_STATUS,&status);
        if (!status) {
            printf("complile shader : %s   error!\n",fragFilePath.c_str());
            abort();
        }
        
        // attach shader
        glAttachShader(_program,vertShaderId);
        glAttachShader(_program,fragShaderId);
        CheckGLError();
        
        // link
        glLinkProgram(_program);
        
        glGetProgramiv(_program,GL_LINK_STATUS,&status);
        if (status == GL_FALSE) {
            printf("link program error!\n");
            abort();
        }
        CheckGLError();
        glDeleteShader(vertShaderId);
        glDeleteShader(fragShaderId);
        vertShaderId = fragShaderId = 0;
    }
    
    void Shader::bind(){
        
        _a_posId = glGetAttribLocation(_program,"a_pos");
        _u_matrixId = glGetUniformLocation(_program,"u_matrix");
        
        if (_shaderType == ShaderType::Shader_BaseTex) {
            _a_texCoordsId = glGetAttribLocation(_program,"a_texCoords");
        }else if (_shaderType == ShaderType::Shader_Font){
            _a_texCoordsId = glGetAttribLocation(_program,"a_texCoords");
            _u_colorId = glGetUniformLocation(_program,"u_color");
        }else if (_shaderType == ShaderType::Shader_Color){
            _u_colorId = glGetUniformLocation(_program,"u_color");
        }else if (_shaderType == ShaderType::Shader_Sprite){
            _a_texCoordsId = glGetAttribLocation(_program,"a_texCoords");
            _u_colorId = glGetUniformLocation(_program,"u_color");
        }
        
        CheckGLError();
    }
    
    
    //************************************************
    
    ShaderUtil* ShaderUtil::_shaderUtil = nullptr;
    
    
    ShaderUtil::~ShaderUtil(){
        auto it = _shaders.begin();
        while (it != _shaders.end()) {
            delete it->second;
            ++it;
        }
        _shaders.clear();
    }
        
    void ShaderUtil::init(){
        if (_shaderUtil == nullptr) {
            _shaderUtil = new ShaderUtil;
            _shaderUtil->loadDefaultShaders();
        }
    }

    void ShaderUtil::loadDefaultShaders(){
        
        // 加载默认的shader
        assert(_shaders.size() < 1);
        
        auto fileUtil = FileUtil::getInstacne();
        
        // 1. test shader
        auto testShader = new Shader(fileUtil->getDefauleShaderPath(_TestVertexShader),fileUtil->getDefauleShaderPath(_TestFragShader),Shader_Test);
        testShader->bind();
        _shaders.insert(pair<ShaderType,Shader*>(Shader_Test,testShader));
        
        // 2.base_tex shader
        auto baseTexShader = new Shader(fileUtil->getDefauleShaderPath(_BaseTexVertexShader),fileUtil->getDefauleShaderPath(_BaseTexFragShader),Shader_BaseTex);
        baseTexShader->bind();
        _shaders.insert(pair<ShaderType,Shader*>(Shader_BaseTex,baseTexShader));
        
        // 3.font shader
        auto fontShader = new Shader(fileUtil->getDefauleShaderPath(_FontVertexShader),fileUtil->getDefauleShaderPath(_FontFragShader),Shader_Font);
        fontShader->bind();
        _shaders.insert(pair<ShaderType,Shader*>(Shader_Font,fontShader));
        
        // 4. test color shader
        auto colorShader = new Shader(fileUtil->getDefauleShaderPath(_ColorVertexShader),fileUtil->getDefauleShaderPath(_ColorFragShader),Shader_Color);
        colorShader->bind();
        _shaders.insert(pair<ShaderType,Shader*>(Shader_Color,colorShader));
        
        // 5. sprite shader
        auto spriteShader = new Shader(fileUtil->getDefauleShaderPath(_SpriteVertexShader),fileUtil->getDefauleShaderPath(_SpriteFragShader),Shader_Sprite);
        spriteShader->bind();
        _shaders.insert(pair<ShaderType,Shader*>(Shader_Sprite,spriteShader));
    }
    
    
    Shader* ShaderUtil::getShader(ShaderType type){
        auto it = _shaders.begin();
        while (it != _shaders.end()) {
            if (it->first == type) {
                return it->second;
            }
            ++it;
        }
        printf("not found shader type:%d\n",type);
        abort();
        return nullptr;
    }
}


