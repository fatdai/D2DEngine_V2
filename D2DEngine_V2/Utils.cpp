//
//  Utils.cpp
//  D2DEngine
//
//  Created by dai on 15/8/2.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#include "Utils.h"
#include <OpenGL/gl.h>
#include <assert.h>
#include "ConvertUTF.h"

namespace D2D {
    
    typedef unsigned int    UTF32;  /* at least 32 bits */
    typedef unsigned short  UTF16;  /* at least 16 bits */
    typedef unsigned char   UTF8;   /* typically 8 bits */
    typedef unsigned char   Boolean; /* 0 or 1 */
    
    void CheckGLError(){
        GLenum error = glGetError();
        if (error) {
            printf("OpenGL error %0xd\n", error);
        }
    }
    
    string GetStringFromFile(const string& path){
        FILE* fp = fopen(path.c_str(),"rt");
        assert(fp);
        fseek(fp,0,SEEK_END);
        long size = ftell(fp);
        fseek(fp,0,SEEK_SET);
        
        unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char) * (size + 1));
        buffer[size] = '\0';
        size_t readSize = fread(buffer,sizeof(unsigned char),size,fp);
        fclose(fp);
        
        if (readSize < size) {
            buffer[readSize] = '\0';
        }
        
        string result((char*)buffer);
        delete buffer;
        return result;
    }
    
    string GetPostfix(const string& filename){
        auto idx = filename.find_last_of('.');
        if (idx == string::npos) {
            printf("file : %s do not have postfix.\n",filename.c_str());
            return "";
        }
        
        auto ret = filename.substr(idx + 1,idx + filename.length());
        return ret;
    }
    
    bool UTF8ToUTF16(const std::string& utf8, std::u16string& outUtf16){
        if (utf8.empty()){
            outUtf16.clear();
            return true;
        }
        
        bool ret = false;
        
        const size_t utf16Bytes = (utf8.length()+1) * sizeof(char16_t);
        char16_t* utf16 = (char16_t*)malloc(utf16Bytes);
        memset(utf16, 0, utf16Bytes);
        
        char* utf16ptr = reinterpret_cast<char*>(utf16);
        const UTF8* error = nullptr;
        
        if (llvm::ConvertUTF8toWide(2, utf8, utf16ptr, error)){
            outUtf16 = utf16;
            ret = true;
        }
        
        free(utf16);
        
        return ret;
    }

    bool UTF16ToUTF8(const std::u16string& utf16, std::string& outUtf8){
        if (utf16.empty()){
            outUtf8.clear();
            return true;
        }
        
        return llvm::convertUTF16ToUTF8String(utf16, outUtf8);
    }

    
}
