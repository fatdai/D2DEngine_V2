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
#include "Scene.h"
#include "Sprite.h"
#include <assert.h>

// 存在如下单例
//  ShaderUtil
//  FileUtil
//  TextureCache


namespace D2D {
    
    Director *Director::_sDirector = nullptr;
    FT_Library Director::_sFreeTypeLibrary = nullptr;
    
    void Director::init(const string& resRootPath,
                        int width,
                        int height){
        if (_sDirector == nullptr) {
            _sDirector = new Director;
            _sDirector->initSelf(resRootPath,width,height);
        }
    }
    
    void Director::initSelf(const string& resRootPath,
                  int width,
                  int height){
        _resRootPath = resRootPath;
        _w = width;
        _h = height;
        
        // 初始化引擎的单例类
        FileUtil::init(_resRootPath);
        ShaderUtil::init();
        TextureCache::init();
     
        // 注意:如果是 retina 屏,width 会是 _w 的2倍
        Mat4::createOrthographicOffCenter(0,_w,0,_h,-1024,1024,&_projMat);
        glViewport(0, 0,_w,_h);
    }
    
    Director::Director():
    _w(0),
    _h(0),
    _projMat(Mat4::IDENTITY),
    _currentScene(nullptr){
    
    }
    
    Director::~Director(){
        
        if (_currentScene != nullptr) {
            _currentScene->onExit();
            delete _currentScene;
            _currentScene = nullptr;
        }
        
        if (_sFreeTypeLibrary) {
            FT_Done_FreeType(_sFreeTypeLibrary);
        }
        
        FileUtil::getInstacne()->release();
        ShaderUtil::getInstance()->release();
        TextureCache::getInstance()->release();
    }
    
//    Director::Director(const string& resRootPath){
//        _resRootPath = resRootPath;
//    }
    
//    void Director::init(const string &title,int w, int h) {
//        _director = this;
//        _w = w;
//        _h = h;
//        
//        initSDL(title,w,h);
//        
//        // 初始化相关单例类
//        FileUtil::init(_resRootPath);
//        ShaderUtil::init();
//        TextureCache::init();
//    
////        initEvent();
//        initRender();
//        initGL();
//        
//        if (_sFreeTypeLibrary == nullptr) {
//            if (FT_Init_FreeType(&_sFreeTypeLibrary)) {
//                printf("init freetype2 error.\n");
//                abort();
//            }
//        }
//    }

    
//    void Director::initRender(){
//        ShaderUtil::init();
//    }
    
//    void Director::initGL() {
//        
//        // *********************
//        // 如果是 retina 屏,width 会是 _w 的2倍
//        Mat4::createOrthographicOffCenter(0,_w,0,_h,-1024,1024,&_projMat);
//        glViewport(0, 0,_w,_h);
//    }
    
//    void Director::run() {
//        
//        struct timeval now;
//        if (gettimeofday(&now, nullptr) != 0) {
//            printf("error in gettimeofday.\n");
//            return;
//        }
//        
////        PrimitiveRender pr;
////        Text label("华文仿宋.ttf");
////        label.setPosition(10,400);
////        label.setString("abcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n1234567890-=_+,.?~@#$%^&*()\n我们的队伍像太阳,脚踏着祖国的大地,\n背负着民族的希望,我们是一支不可战胜的力量.\n龍飛鳳舞。");
//        
//        Scene scene;
//        scene.setDebugName("scene....");
//        
//        auto texCache = TextureCache::getInstance();
//        auto bgTex = texCache->addImage("home_bg.png");
//        auto klockTex1 = texCache->addImage("play.png");
//        auto titleTex = texCache->addImage("title.jpg");
//        
//        auto bg = new Sprite(bgTex);
//        bg->setScale(1,0.5f);
//        bg->setPosition(320, 240);
//        scene.addChild(bg);
//        
//        auto title = new Sprite(titleTex);
//        title->setPosition(320,400);
//        title->setColor(1,1,1,0.1f);
//        scene.addChild(title);
//        
//        auto klock = new Sprite(klockTex1);
//        klock->setPosition(320, 240);
//        klock->setColor(1, 1, 0, 1);
//        scene.addChild(klock);
//        
//
//        log("------- ready");
    
//        auto node1 = new Node;
//        node1->setPosition(320,240);
//        node1->setAnchor(0, 0);
//        node1->setContentSize(200, 100);
//        node1->setColor(1,0,0);
////        node1->setScale(2);
////        node1->setVisible(false);
//        node1->setDebugName("node------1");
//        scene.addChild(node1);
//        
//        auto b = new Node;
//        b->setContentSize(50, 50);
//        b->setColor(0, 1, 0);
//        b->setDebugName("node--------2");
//        b->setPosition(100, 0);
//        b->setZOrder(11);
//        b->setAnchor(0, 0);
//        node1->addChild(b);
        
        //-----------------------------------------
        // before loop
        
//        bool quit = false;
//        
//        //Event handler
//        SDL_Event e;
//        
//        //Enable text input
//        SDL_StartTextInput();
//        
//        while (!quit) {
//            
//            //--------------- capture input ----------------
//            while (SDL_PollEvent(&e) != 0) {
//                //User requests quit
//                if (e.type == SDL_QUIT) {
//                    log("escape called....");
//                    quit = true;
//                }
//                
//                // handler other events
//                if (e.type == SDL_KEYDOWN) {
////                    if (e.key.keysym.sym == SDLK_LEFT) {
////                        node1->setPositionX(node1->getPositionX() - 5);
////                    }else if (e.key.keysym.sym == SDLK_RIGHT){
////                        node1->setPositionX(node1->getPositionX() + 5);
////                    }
//                }
//            }
//            
//            //---------------   rendering   ----------------
//            glClear(GL_COLOR_BUFFER_BIT);
//            
//            // for test
//            // draw line
//            //pr.render();
//           // label.render();
//            scene.beforeRender();
//            scene.render();
//            
//            //----------------- end rendering ------------------
//            
//            SDL_GL_SwapWindow(_gWindow);
//        }
//        
//        //Disable text input
//        SDL_StopTextInput();
//        
//        //Free resources and close SDL
//        destorySDL();
//    }
    
    void Director::loop(){
        assert(_currentScene != nullptr);
        glClear(GL_COLOR_BUFFER_BIT);
        _currentScene->beforeRender();
        _currentScene->render();
    }

    void Director::replaceScene(Scene* scene){
        if (_currentScene == nullptr) {
            _currentScene = scene;
        }else{
            _currentScene->onExit();
            delete _currentScene;
            _currentScene = nullptr;
            _currentScene = scene;
        }
    }
    
    void Director::release(){
        if (_sDirector != nullptr) {
            delete _sDirector;
            _sDirector = nullptr;
        }
    }
};