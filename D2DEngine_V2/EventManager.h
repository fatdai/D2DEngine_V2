//
//  EventManager.h
//  D2DEngine_V2
//
//  Created by dai on 15/8/16.
//  Copyright (c) 2015年 mac. All rights reserved.
//

#ifndef __D2DEngine_V2__EventManager__
#define __D2DEngine_V2__EventManager__

#include <stdio.h>
#include <SDL2/SDL.h>
#include <queue>
using namespace std;

namespace D2D {
    
#define EventManagerObj EventManager::getInstance()
    
    //-----------------------------------------
    class KeyEvent{
    public:
        
        // keydonw  keyup
        
        
    };
    
    //-----------------------------------------
    
    class TouchEvent{
    public:
        
    };
    
    //-----------------------------------------
    
    class EventManager{
        
        static EventManager* _sEventManager;
        
    public:
        
        static EventManager* getInstance(){
            return _sEventManager;
        }
        
        static void init();
        static void release();
        
        void pollEvent();
        
    private:
        
        SDL_Event _sdlEvent;
        
        // 存放所有的键盘事件
        queue<KeyEvent> _keyEvents;
        
    };
    

}

#endif /* defined(__D2DEngine_V2__EventManager__) */
