//
//  Director.cpp
//  D2DEngine
//
//  Created by dai on 15/8/2.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#include "Director.h"
#include <cstdlib>
#include "ShaderUtil.h"
#include "PrimitiveRender.h"
#include "FileUtil.h"
#include "Texture2D.h"
#include "Node.h"
#include "globals.h"
#include "Text.h"

// 存在如下单例
//  ShaderUtil
//  FileUtil
//  TextureCache


namespace D2D {
    
    Director *Director::_director = nullptr;
    FT_Library Director::_sFreeTypeLibrary = nullptr;
    
    Director::~Director(){
        if (_sFreeTypeLibrary) {
            FT_Done_FreeType(_sFreeTypeLibrary);
        }
        
        delete ShaderUtil::getInstance();
        delete TextureCache::getInstance();
    }
    
    Director::Director(const string& resRootPath){
        _resRootPath = resRootPath;
    }
    
    void Director::init(const string &title,int w, int h) {
        _director = this;
        _w = w;
        _h = h;
        
        initSDL(title,w,h);
        
        // 初始化相关单例类
        FileUtil::init(_resRootPath);
        ShaderUtil::init();
        TextureCache::init();
    
//        initEvent();
        initRender();
        initGL();
        
        if (_sFreeTypeLibrary == nullptr) {
            if (FT_Init_FreeType(&_sFreeTypeLibrary)) {
                printf("init freetype2 error.\n");
                abort();
            }
        }
    }

    
    void Director::initRender(){
        ShaderUtil::init();
    }
    
    void Director::initGL() {
        
        // *********************
        // 如果是 retina 屏,width 会是 _w 的2倍
        Mat4::createOrthographicOffCenter(0,_w,0,_h,-1024,1024,&_projMat);
        glViewport(0, 0,_w,_h);
    }
    
    void Director::run() {
        
        struct timeval now;
        if (gettimeofday(&now, nullptr) != 0) {
            printf("error in gettimeofday.\n");
            return;
        }
        
        PrimitiveRender pr;
//        Text label("华文仿宋.ttf");
//        label.setPosition(10,400);
//        label.setString("abcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n1234567890-=_+,.?~@#$%^&*()\n我们的队伍像太阳,脚踏着祖国的大地,\n背负着民族的希望,我们是一支不可战胜的力量.\n龍飛鳳舞。");
        
        Node rootNode;
        rootNode.setAnchor(0, 0);
        
        auto node1 = new Node;
        node1->setPosition(320,240);
        node1->setAnchor(0.5, 0.5);
        node1->setContentSize(200, 100);
        node1->setColor(1,0,0);
        node1->setScale(1);
        node1->setRotate(30);
        rootNode.addChild(node1);
        
        auto b = new Node;
        b->setContentSize(50, 50);
        b->setPosition(100, 50);
        b->setColor(1, 1, 0);
        b->setAnchor(0.5, 0.5);
        node1->addChild(b);
        
        //-----------------------------------------
        // before loop
        
        bool quit = false;
        
        //Event handler
        SDL_Event e;
        
        //Enable text input
        SDL_StartTextInput();
        
        while (!quit) {
            
            //--------------- capture input ----------------
            while (SDL_PollEvent(&e) != 0) {
                //User requests quit
                if (e.type == SDL_QUIT) {
                    log("escape called....");
                    quit = true;
                }
                
                // handler other events
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_LEFT) {
                        rootNode.setPositionX(rootNode.getPositionX() - 5);
                    }else if (e.key.keysym.sym == SDLK_RIGHT){
                        rootNode.setPositionX(rootNode.getPositionX() + 5);
                    }
                }
            }
            
            //---------------   rendering   ----------------
            glClear(GL_COLOR_BUFFER_BIT);
            
            // for test
            // draw line
            pr.render();
           // label.render();
            rootNode.visit();
            rootNode.startRender();
            
            //----------------- end rendering ------------------
            
            SDL_GL_SwapWindow(_gWindow);
        }
        
        //Disable text input
        SDL_StopTextInput();
        
        //Free resources and close SDL
        destorySDL();
    }

};