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

namespace D2D {
    
    //--------------------------------------------
    
    extern SDL_Window* _gWindow;
    extern SDL_GLContext _gGLContext;
    
    //--------------------------------------------
    void log(const char * format, ...);
    
    bool initSDL(const string& title,int w,int h);
    
    void destorySDL();
}

#endif /* defined(__D2DEngine_V2__globals__) */
