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
#include "Scene.h"
#include <ft2build.h>
#include FT_FREETYPE_H
using namespace std;

namespace D2D {
    
#define DirectorObj Director::getInstance()
    
    class Director {
        
        static Director *_sDirector;
        
    public:
        
        // 管理一些全局的资源,这些资源需要被初始化和清理
        static FT_Library _sFreeTypeLibrary;

    public:
        
        Director();
        ~Director();
        
        static Director* getInstance() { return _sDirector; };
        static void init(const string& resRootPath,
                         int width,
                         int height);
        
        Mat4& getProjMat4(){
            return _projMat;
        }
        
        void run();
        
        void loop();
        
        void replaceScene(Scene* scene);
        
        void release();
        
    private:
        
        void initSelf(const string& resRootPath,
                      int width,
                      int height);
        
    public:
        
        float getWidth(){return _w;}
        float getHeight(){return _h;}
        
    private:
        
        float _w = 0, _h = 0;

        string _resRootPath;
        
        Mat4 _projMat;
        
        Scene* _currentScene = nullptr;
    };
};

#endif /* defined(__D2DEngine__Director__) */
