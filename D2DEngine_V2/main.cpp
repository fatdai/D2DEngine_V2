//
//  main.cpp
//  D2DEngine_V2
//
//  Created by dai on 15/8/10.
//  Copyright (c) 2015年 mac. All rights reserved.
//

#include <iostream>

#include "Director.h"
#include "FileUtil.h"
#include "Texture2D.h"
#include "Application.h"
using namespace D2D;

int main(int argc, const char * argv[]) {
    
//    auto director = new Director("/Users/dai/Graphics/D2DEngine_V2/Resources");
//    director->init("D2D Engine",640, 480);
//    
//    // 添加资源的文件夹
//    FileUtil::getInstacne()->addDirs("images/");
//    TextureCache::getInstance()->addImage("home_bg.png");
//    director->run();
//    
//    delete director;
    
    string resRootPath = "/Users/dai/Graphics/D2DEngine_V2/Resources";
    Application app(resRootPath,"D2D Engine",640,480);
    return app.run();
}
