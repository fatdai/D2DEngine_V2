//
//  NonCopyable.h
//  D2DEngine
//
//  Created by dai on 15/8/9.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#ifndef D2DEngine_NonCopyable_h
#define D2DEngine_NonCopyable_h

namespace D2D {
    
    //-----------------------------------------------
    class NonCopyable{
        
    public:
        
        NonCopyable(){}
        ~NonCopyable(){}
    
        NonCopyable(const NonCopyable& other) = delete;
        NonCopyable& operator=(const NonCopyable& other) = delete;
        NonCopyable(NonCopyable&& other) = delete;
        NonCopyable& operator=(NonCopyable&& other) = delete;
    };
}

#endif
