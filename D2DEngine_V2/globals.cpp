//
//  globals.cpp
//  D2DEngine_V2
//
//  Created by dai on 15/8/10.
//  Copyright (c) 2015年 mac. All rights reserved.
//

#include "globals.h"
#include <sys/time.h>

namespace D2D {
    
    static void _log(const char *format, va_list args)
    {
        static const int MAX_LOG_LENGTH = 16*1024;
        char buf[MAX_LOG_LENGTH];
        
        vsnprintf(buf, MAX_LOG_LENGTH-3, format, args);
        strcat(buf, "\n");
        
        // Linux, Mac, iOS, etc
        fprintf(stdout, "D2D: %s", buf);
        fflush(stdout);
    }

    
    void log(const char * format, ...){
        va_list args;
        va_start(args, format);
        _log(format, args);
        va_end(args);
    }
    
    
    long Now(){
        
        long lLastTime = 0;
        struct timeval stCurrentTime;
        
        gettimeofday(&stCurrentTime,nullptr);
        lLastTime = stCurrentTime.tv_sec*1000+stCurrentTime.tv_usec*0.001; //millseconds
        return lLastTime;
    }
    
}