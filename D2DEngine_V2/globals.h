//
//  globals.h
//  D2DEngine_V2
//
//  Created by dai on 15/8/10.
//  Copyright (c) 2015年 mac. All rights reserved.
//

#ifndef __D2DEngine_V2__globals__
#define __D2DEngine_V2__globals__

#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
using namespace std;


// ------ 检测 opengl 错误
#ifndef CheckGLError
#define CheckGLError() \
do { \
GLenum __error = glGetError(); \
if(__error) { \
D2D::log("OpenGL error 0x%04X in %s %s %d\n", __error, __FILE__, __FUNCTION__, __LINE__); \
} \
} while (false)
#endif

// 角度转弧度
#define DegToRad(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180

// 弧度转角度
#define RadToDeg(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180

namespace D2D {
    
    void log(const char * format, ...);
    
    /**
     *  @return 从1970年1月1日到现在的毫秒数
     */
    long Now();
}

#endif /* defined(__D2DEngine_V2__globals__) */
