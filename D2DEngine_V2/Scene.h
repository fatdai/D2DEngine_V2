//
//  Scene.h
//  D2DEngine_V2
//
//  Created by dai on 15/8/14.
//  Copyright (c) 2015年 mac. All rights reserved.
//

#ifndef __D2DEngine_V2__Scene__
#define __D2DEngine_V2__Scene__

#include <stdio.h>
#include "Node.h"

namespace D2D {
    
    class Scene : public Node{
        
    public:
    
        Scene();
        
         virtual void beforeRender();
    };
}


#endif /* defined(__D2DEngine_V2__Scene__) */
