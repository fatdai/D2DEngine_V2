
1. 使用 SDL2 创建窗口
   


a. 开发环境搭建
   下载 SDL2.framework,放到 /Library/Frameworks下
   然后 Build Phases ->Link Binary With Libraries 下添加 SDL2.framework 就可以了
   再添加 OpenGL.framework, libz.dylib(libpng需要)
    

b. 加入libpng libfreetype
   build setting 里面设置  Always Search User Paths ->Yes
   User Header Search Paths 设置 libpng libfreetype 的头文件的目录.

