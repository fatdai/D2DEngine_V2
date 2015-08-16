//
//  FileUtil.cpp
//  D2DEngine
//
//  Created by dai on 15/8/2.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#include "FileUtil.h"
#include <fstream>
#include <cstdlib>

namespace D2D {
    
    FileUtil* FileUtil::_sFileUtil = nullptr;
    
    
    void FileUtil::init(const string& baseDir){
        if (_sFileUtil == nullptr) {
            
            _sFileUtil = new FileUtil;
            
            auto bias = baseDir.find_last_of('/');
            string newBaseDir;
            if (bias != string::npos && bias == baseDir.length() - 1) {
                newBaseDir = baseDir.substr(0,baseDir.length() - 1);
            }else{
                newBaseDir = baseDir;
            }
            
            _sFileUtil->_baseDir = newBaseDir;
            
            // 计算默认shader位置
            auto ret = newBaseDir.find_last_of('/');
            if (ret != string::npos) {
                _sFileUtil->_internalDir = newBaseDir.substr(0,ret);
            }else{
                printf("can not set  _internalDir.\n");
                abort();
            }
        }
    }
    
    string FileUtil::getDefauleShaderPath(const string& shaderFile){
        sprintf(_tempBuf,"%s/D2DEngine_V2/%s",_internalDir.c_str(),shaderFile.c_str());
        return string(_tempBuf);
    }
    
    void FileUtil::addDirs(const string& dir){
        
        // 检查 dir 后面是否有 '/'
        auto bias = dir.find_last_of('/');
        string newDir;
        if (bias != string::npos && bias == dir.length() - 1) {
            newDir = dir.substr(0,dir.length() - 1);
        }else{
            newDir = dir;
        }
        
        // 避免重复添加
        auto ret = find(_dirs.begin(),_dirs.end(),newDir);
        if (ret == _dirs.end()) {
            _dirs.push_back(newDir);
        }
    }
    
    // 按照 _dirs 里面的顺序进行查找
    // 需要检查文件是否存在
    string FileUtil::getFullPath(const string& filename){
        auto it = _dirs.begin();
        while (it != _dirs.end()) {
            auto& dir = (*it);
            sprintf(_tempBuf,"%s/%s/%s",_baseDir.c_str(),dir.c_str(),filename.c_str());
            if (isFileExist(string(_tempBuf))) {
                return _tempBuf;
            }
            ++it;
        }
        printf("can not found file : %s.\n",filename.c_str());
        return "";
    }
    
    bool FileUtil::isExist(const string& relativeFile){
        auto it = _dirs.begin();
        while (it != _dirs.end()) {
            auto& dir = (*it);
            sprintf(_tempBuf,"%s/%s/%s",_baseDir.c_str(),dir.c_str(),relativeFile.c_str());
            if (isFileExist(string(_tempBuf))) {
                return true;
            }
        }
        return false;
    }
    
    bool FileUtil::isFileExist(const string& fullpath){

        FILE* fp = fopen(fullpath.c_str(),"rb");
        if (fp == nullptr) {
            return false;
        }
        fclose(fp);
        return true;
    }
    
    void FileUtil::release(){
        if (_sFileUtil != nullptr) {
            delete _sFileUtil;
            _sFileUtil = nullptr;
        }
    }
    
    Data FileUtil::getDataFromFile(const string& filename,bool isFullPath){
        if (filename.empty()) {
            return Data::Null;
        }
        
        FILE* fp = nullptr;
        if (!isFullPath) {
            auto fullpath = getFullPath(filename);
            fp = fopen(fullpath.c_str(),"rb");
        }else{
            fp = fopen(filename.c_str(),"rb");
        }
        
        fseek(fp,0,SEEK_END);
        size_t size = ftell(fp);
        fseek(fp,0,SEEK_SET);
        
        unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char) * size);
        size_t readsize = fread(buffer, sizeof(unsigned char), size, fp);
        Data ret;
        ret.fastSet(buffer,readsize);
        return ret;
    }
    
}