//
//  Application.h
//  D2DEngine_V2
//
//  Created by dai on 15/8/16.
//  Copyright (c) 2015年 mac. All rights reserved.
//

#ifndef __D2DEngine_V2__Application__
#define __D2DEngine_V2__Application__

#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>
using namespace std;

#ifndef _DEBUG_D2DENGINE_
#define _DEBUG_D2DENGINE_
#endif

//------------------------------------------------
namespace D2D {
    
#define App Application::getInstance()
    
    class Application{
        
    public:
        
        Application(const string& resRootPath,const string& title,int width,int height);
        
        ~Application();
        
        static Application* _sApplication;
        
        static Application* getInstance(){
            return _sApplication;
        }
        
        int run();
        
        bool appFinishLaunching();
        
        void enterBackground();
        
        void enterForeground();
        
        void setQuit(bool quit){_quit = quit;}
        
    private:
        
        SDL_Window* _window;
        
        string _title;
        
        float _width,_height;
        
        const float _timeInterval;
        
        bool _quit;
    };
    
}

#endif /* defined(__D2DEngine_V2__Application__) */
