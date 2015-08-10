//
//  globals.cpp
//  D2DEngine_V2
//
//  Created by dai on 15/8/10.
//  Copyright (c) 2015年 mac. All rights reserved.
//

#include "globals.h"

namespace D2D {
    
    //---------------------------------------------------
    // 有些东西就是要是全局变量,
    // 可以更加方便的访问
    SDL_Window* _gWindow = nullptr;
    
    //OpenGL context
    SDL_GLContext _gGLContext = nullptr;
    
    //---------------------------------------------------
    static void _log(const char *format, va_list args)
    {
        static const int MAX_LOG_LENGTH = 16*1024;
        char buf[MAX_LOG_LENGTH];
        
        vsnprintf(buf, MAX_LOG_LENGTH-3, format, args);
        strcat(buf, "\n");
        
        // Linux, Mac, iOS, etc
        fprintf(stdout, "D2D: %s", buf);
        fflush(stdout);
    }

    
    void log(const char * format, ...){
        va_list args;
        va_start(args, format);
        _log(format, args);
        va_end(args);
    }
    
    bool initSDL(const string& title,int w,int h){
        
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            log("SDL could not initialize! SDL Error: %s",SDL_GetError());
            return false;
        }
        
        // use opengl 2.1
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
        
        _gWindow = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
        if (_gWindow == nullptr) {
            log("Window could not be created! SDL Error: %s",SDL_GetError());
            return false;
        }
        
        // create opengl context
        _gGLContext = SDL_GL_CreateContext(_gWindow);
        if (_gGLContext == nullptr) {
            log("OpenGL context could not be created! SDL Error: %s",SDL_GetError());
            return false;
        }
        
        // use Vsync
        if( SDL_GL_SetSwapInterval( 1 ) < 0 )
        {
            log( "Warning: Unable to set VSync! SDL Error: %s", SDL_GetError() );
        }
        
        return true;
    }
    
    void destorySDL(){
        SDL_DestroyWindow(_gWindow);
        _gWindow = nullptr;
        
        SDL_Quit();
    }
    
}