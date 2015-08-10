//
//  ImageLoader.h
//  D2DEngine
//
//  Created by dai on 15/8/3.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#ifndef __D2DEngine__ImageLoader__
#define __D2DEngine__ImageLoader__

#include <stdio.h>


// 加载各种格式的图片
namespace D2D {
    
#define PNG_READ_SUCCESS 0
#define PNG_ERROR_NO_FILE 1
#define PNG_ERROR_NOT_PNG 2
#define PNG_ERROR_CREATE_PNG_STRUCT 3
#define PNG_ERROR_CREATE_INFO_STRUCT 4
#define PNG_ERROR_JMP 5

    
    typedef struct png_data_t {
        unsigned int width;
        unsigned int height;
        char* data;
    }  *png_datap;
    
    int png_read(const char* filename,png_datap data);

    
}

#endif /* defined(__D2DEngine__ImageLoader__) */
