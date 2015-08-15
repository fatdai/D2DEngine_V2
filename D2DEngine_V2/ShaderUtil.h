//
//  ShaderUtil.h
//  D2DEngine
//
//  Created by dai on 15/8/2.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#ifndef __D2DEngine__ShaderUtil__
#define __D2DEngine__ShaderUtil__

#include <stdio.h>
#include <map>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <string>
using namespace std;

namespace D2D {
    
    typedef enum{
        Shader_Test = 0,
        Shader_BaseTex,
        Shader_Font,
        Shader_Color,
        Shader_Sprite,
    }ShaderType;
    
    
    //------------------------------------------------
    class Shader{
        
    public:
        
        Shader(const string& vertFilePath,const string& fragFilePath,ShaderType shaderType);
        virtual ~Shader();
        
    public:
        
        virtual void bind();
        
        GLuint getProgramId(){
            return _program;
        }
        
        GLint get_U_MatrixId(){
            return _u_matrixId;
        }
        
        GLint get_A_PositionId(){
            return _a_posId;
        }
        
        GLint get_A_TexCoordsId(){
            return _a_texCoordsId;
        }
        
        GLint get_U_ColorId(){
            return _u_colorId;
        }
        
        GLint get_A_ColorId(){
            return _a_colorId;
        }
    public:
        
        GLuint _program = 0;
        
        // some handlers
        GLint _a_posId = -1;
        GLint _a_texCoordsId = -1;
        GLint _a_colorId = -1;
        
        GLint _u_matrixId = -1;

        GLint _u_colorId = -1;
        
        ShaderType _shaderType;
    };
    
    //------------------------------------------------
    class ShaderUtil {
        
    private:
        
        static ShaderUtil* _shaderUtil;
        
        void loadDefaultShaders();
        
    public:
        
        static void init();
        static ShaderUtil* getInstance(){
            return _shaderUtil;
        }
        
        Shader* getShader(ShaderType type);
        
        ~ShaderUtil();
        
    private:
        
        map<ShaderType,Shader*> _shaders;
    };
    
    //------------------------------------------------
    
}

#endif /* defined(__D2DEngine__ShaderUtil__) */
