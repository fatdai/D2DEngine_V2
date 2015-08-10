//
//  FileUtil.h
//  D2DEngine
//
//  Created by dai on 15/8/2.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#ifndef __D2DEngine__FileUtil__
#define __D2DEngine__FileUtil__

#include <stdio.h>
#include <vector>
#include <string>
#include "Data.h"
using namespace std;

namespace D2D {
    
    class FileUtil{
        
    public:
        
        static FileUtil* _fileUtil;
        
        static FileUtil* getInstacne(){
            return _fileUtil;
        }
        static void init(const string& baseDir);
        
        void addDirs(const string& dir);
        
        string getFullPath(const string& filename);
        
        Data getDataFromFile(const string& filename);
        
        bool isExist(const string& relativeFile);
        
        // 引擎内部文件目录
        // $(PROJECT_DIR)
        string& getInternalDir(){
            return _internalDir;
        }
        
        string getDefauleShaderPath(const string& shaderFile);
        
        
    private:
        
        bool isFileExist(const string& fullpath);
        
    private:
        
        // 保存一些目录,按照顺序查找目标文件
        vector<string> _dirs;
        
        string _baseDir;
        
        char _tempBuf[256];
        
        string _internalDir;
    };
}

#endif /* defined(__D2DEngine__FileUtil__) */
