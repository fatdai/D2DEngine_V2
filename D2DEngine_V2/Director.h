//
//  Director.h
//  D2DEngine
//
//  Created by dai on 15/8/2.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#ifndef __D2DEngine__Director__
#define __D2DEngine__Director__


#include <SDL2/SDL.h>
#include <stdio.h>
#include "CCMath.h"
#include <sys/time.h>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
using namespace std;

namespace D2D {
    
    //---------------------------------------------------
    // 有些东西就是要是全局变量,
    // 可以更加方便的访问
    
    
    //---------------------------------------------------
    
    // 单例,进行全局的管理
    class Director {
        
        static Director *_director;
        
    public:
        
        // 管理一些全局的资源,这些资源需要被初始化和清理
        static FT_Library _sFreeTypeLibrary;

    public:
        
        Director(const string& resRootPath);
        
        ~Director();
        
        static Director *getInstance() { return _director; };
        
        void init(const string &title,int w, int h);
        
        Mat4& getProjMat4(){
            return _projMat;
        }
        
        void run();
        
    public:
        
        float getWidth(){return _w;}
        float getHeight(){return _h;}
        
    private:
        
        void initGL();
        
        void initRender();
        
    private:
                
        float _w = 0, _h = 0;
        
        // 用来计算fps
        unsigned int _totalFrames = 0;
        unsigned int _frames = 0;
        
        struct timeval _startTime;
        
        string _resRootPath;
        
        Mat4 _projMat;
    };
};

#endif /* defined(__D2DEngine__Director__) */
