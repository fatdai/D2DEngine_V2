//
//  Application.cpp
//  D2DEngine_V2
//
//  Created by dai on 15/8/16.
//  Copyright (c) 2015年 mac. All rights reserved.
//

#include "Application.h"
#include <iostream>
#include <unistd.h>
#include "Director.h"
#include "globals.h"
#include "FileUtil.h"
#include "Texture2D.h"
#include "Sprite.h"

namespace D2D {
    
    Application* Application::_sApplication = nullptr;
    
    Application::Application(const string& resRootPath,const string& title,int width,int height):
    _window(nullptr),
    _title(title),
    _width(width),
    _timeInterval(1000.0f/60.0f),
    _height(height){
        
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            log("SDL could not initialize! SDL Error: %s",SDL_GetError());
            abort();
        }
        
        // use opengl 2.1
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
        
        _window = SDL_CreateWindow(_title.c_str(),SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,_width,_height, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
        if (_window == nullptr) {
            log("Window could not be created! SDL Error: %s",SDL_GetError());
        }
        
        // create opengl context
        auto glContext = SDL_GL_CreateContext(_window);
        if (glContext == nullptr) {
            log("OpenGL context could not be created! SDL Error: %s",SDL_GetError());
        }
        
        // use Vsync
        if( SDL_GL_SetSwapInterval( 1 ) < 0 ){
            log( "Warning: Unable to set VSync! SDL Error: %s", SDL_GetError() );
        }
        
        _sApplication = this;
        
        Director::init(resRootPath,width,height);
        
#ifdef _DEBUG_D2DENGINE_
        log("[Application::Application] called....");
#endif
    }
    
    
    Application::~Application(){
        if (_window != nullptr) {
            SDL_DestroyWindow(_window);
            _window = nullptr;
            
            SDL_Quit();
        }
        
#ifdef _DEBUG_D2DENGINE_
        log("[Application::~Application] called....");
#endif
    }
    
    int Application::run(){
        
        if (!appFinishLaunching()) {
            log("appFinishLaunching maybe wrong.");
            return 1;
        }
        
        long lastTime = 0L;
        long curTime = 0L;
        
        auto director = Director::getInstance();
        
        bool quit = false;
        
        //Event handler
        SDL_Event e;
        
        //Enable text input
        SDL_StartTextInput();
        
        while (!quit) {
            //--------------- capture input ----------------
            while (SDL_PollEvent(&e) != 0) {
                
                lastTime = Now();
                
                //User requests quit
                if (e.type == SDL_QUIT) {
                    log("exit game engine....");
                    quit = true;
                }
                
                // handler other events
                if (e.type == SDL_KEYDOWN) {
                    //                    if (e.key.keysym.sym == SDLK_LEFT) {
                    //                        node1->setPositionX(node1->getPositionX() - 5);
                    //                    }else if (e.key.keysym.sym == SDLK_RIGHT){
                    //                        node1->setPositionX(node1->getPositionX() + 5);
                    //                    }
                }
            }
            
            //---------------------------------
            director->loop();
            
            SDL_GL_SwapWindow(_window);
            
            curTime = Now();
            if (curTime - lastTime < _timeInterval) {
                usleep(static_cast<useconds_t>((_timeInterval - curTime + lastTime)*1000));
            }
        }
        
        //Disable text input
        SDL_StopTextInput();
        
        DirectorObj->release();
        
        return 0;
    }
    
    bool Application::appFinishLaunching(){
        
        //--------------base setting----------------
        FileUtil::getInstacne()->addDirs("images/");
    
        auto scene = new Scene;
        
        auto texCache = TextureCache::getInstance();
        auto bgTex = texCache->addImage("home_bg.png");
        auto klockTex1 = texCache->addImage("play.png");
        auto titleTex = texCache->addImage("title.jpg");
        
        auto bg = new Sprite(bgTex);
        bg->setScale(1,0.5f);
        bg->setPosition(320, 240);
        scene->addChild(bg);
        
        auto title = new Sprite(titleTex);
        title->setPosition(320,400);
        title->setColor(1,1,1,0.1f);
        scene->addChild(title);
        
        auto klock = new Sprite(klockTex1);
        klock->setPosition(320, 240);
        klock->setColor(1, 1, 0, 1);
        scene->addChild(klock);
        
        DirectorObj->replaceScene(scene);
        
#ifdef _DEBUG_D2DENGINE_
        log("[Application::appFinishLaunching] called....");
#endif
        
        return true;
    };
    
    void Application::enterBackground(){
#ifdef _DEBUG_D2DENGINE_
        log("[Application::enterBackground] called....");
#endif
    }
    
    void Application::enterForeground(){
#ifdef _DEBUG_D2DENGINE_
        log("[Application::enterForeground] called....");
#endif
    }
}