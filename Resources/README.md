
1. 使用 SDL2 创建窗口
   
a. 开发环境搭建
   下载 SDL2.framework,放到 /Library/Frameworks下
   然后 Build Phases ->Link Binary With Libraries 下添加 SDL2.framework 就可以了
   再添加 OpenGL.framework, libz.dylib(libpng需要)
    

b. 加入libpng libfreetype
   build setting 里面设置  Always Search User Paths ->Yes
   User Header Search Paths 设置 libpng libfreetype 的头文件的目录.



//---------------------------------------------------
SDL2 Xcode 直接退出 解决方式:

http://stackoverflow.com/questions/22368202/xcode-5-crashes-when-running-an-app-with-sdl-2

It looks like the problem is that SDL2 isn't properly signed. There's a ticket to fix the issue at https://bugzilla.libsdl.org/show_bug.cgi?id=2058.

When you spawn an application from Xcode 5 with lldb, Xcode loads a plugin that will inspect modules your application loads, presumably to give you insight into your program. Xcode is set up to crash if it reads from a page that is backed by an invalid signature. Because SDL2 isn't properly signed, when Xcode 5 attempts to examine the SDL2 library, it will read from an invalid page, and crash. You get a clue that this is happening by the following line in your crash report above:

Exception Type:  EXC_BAD_ACCESS (Code Signature Invalid)
To see for yourself that SDL isn't properly signed, execute the following command from the folder where SDL2 resides:

$ codesign -vvvv SDL2 
SDL2: invalid signature (code or signature have been modified)
In architecture: x86_64 
If you don't want to build your own version of SDL2 (the solution that's mentioned above), you can update the signature to make it correct:

$ codesign -f -s - SDL2 
SDL2: replacing existing signature
And to verify that the new signature is good:

$ codesign -vvvv SDL2
SDL2: valid on disk
SDL2: satisfies its Designated Requirement