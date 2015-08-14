//
//  Scene.cpp
//  D2DEngine_V2
//
//  Created by dai on 15/8/14.
//  Copyright (c) 2015年 mac. All rights reserved.
//

#include "Scene.h"
#include "Director.h"
#include "MatrixStack.h"
#include "CCMath.h"

namespace D2D {
    
    
    Scene::Scene(){
        setAnchor(0, 0);
    }
    
    void Scene::beforeRender(){
        
        // 先对 children 排序
        visit(Mat4::IDENTITY);
        
    }    
}