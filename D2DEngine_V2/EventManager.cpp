//
//  EventManager.cpp
//  D2DEngine_V2
//
//  Created by dai on 15/8/16.
//  Copyright (c) 2015年 mac. All rights reserved.
//

#include "EventManager.h"
#include "globals.h"
#include "Application.h"

namespace D2D {
    
    EventManager* EventManager::_sEventManager = nullptr;
    
    void EventManager::init(){
        if (_sEventManager == nullptr) {
            _sEventManager = new EventManager;
        }
    }
    
    void EventManager::release(){
        if (_sEventManager != nullptr) {
            delete _sEventManager;
            _sEventManager = nullptr;
        }
    }
    
    void EventManager::pollEvent(){
        
        while (SDL_PollEvent(&_sdlEvent) != 0) {
            
            // 点击窗口关闭按钮
            if (_sdlEvent.type == SDL_QUIT) {
                log("user close window.");
                App->setQuit(true);
                return;
            }
            
            // 进行捕获键盘事件
            
            
        }
    }
}