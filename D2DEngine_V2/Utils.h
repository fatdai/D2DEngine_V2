//
//  Utils.h
//  D2DEngine
//
//  Created by dai on 15/8/2.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#ifndef __D2DEngine__Utils__
#define __D2DEngine__Utils__

#include <stdio.h>
#include <string>
#include <cmath>
using namespace std;

// 包含一些全局方法
namespace D2D {
    
   // void CheckGLError();
    
    string GetStringFromFile(const string& path);
    
    string GetPostfix(const string& filename);
    
    bool UTF8ToUTF16(const std::string& utf8, std::u16string& outUtf16);
    
    bool UTF16ToUTF8(const std::u16string& utf16, std::string& outUtf8);
    
    // 角度转换
//    inline float DegToRad(float degress){
//        return degress * M_PI / 180.0f;
//    }
//    
//    inline float RadToDeg(float rad){
//        return rad * 180.0f / M_PI;
//    }
}


#endif /* defined(__D2DEngine__Utils__) */
