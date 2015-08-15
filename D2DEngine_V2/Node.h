//
//  Node.h
//  D2DEngine
//
//  Created by dai on 15/8/2.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#ifndef __D2DEngine__Node__
#define __D2DEngine__Node__

#include <stdio.h>
#include <vector>
#include "NonCopyable.h"
#include "CCGeometry.h"
#include <string>
using namespace std;

namespace D2D {
    
    //-----------------------------------------
    class AnchorPoint{
        
    public:
        
        AnchorPoint(){}
        
        AnchorPoint(float x,float y):
        _ax(x),_ay(y){}
        
        float getAnchorX(){
            return _ax;
        }
        
        float getAnchorY(){
            return _ay;
        }
        
        void setAnchorX(float ax){ _ax = ax;}
        void setAnchorY(float ay){ _ay = ay;}
        
    private:
        
        float _ax = 0.5f;
        float _ay = 0.5f;
        
    };
    
    //-----------------------------------------
    // 基础类
    // z 主要用于排序
    class Shader;
    class Node : public NonCopyable{
        
    public:
        
        Node();
        
        virtual ~Node();
        
    public:
        
        // 是否可以基于组件?
        void setPosition(float x,float y);
        void setPositionX(float x);
        void setPositionY(float y);
        float getPositionX(){return _x;};
        float getPositionY(){return _y;}
        
        void setAnchor(float ax,float ay);
        
        void setScale(float sx,float sy);
        void setScale(float s);
        
        void setRotate(float degress);
        
        void setContentSize(float w,float h);
        
        void setTag(int tag){ _tag = tag;};
        int getTag(){return _tag;};
        int getZOrder(){return _zorder;}
        void setZOrder(int zorder){_zorder = zorder;}
        
    
        // -------- scene graph
        Node* getChildByTag(int tag);
        void addChild(Node* child);
        void addChild(Node* child,int tag);
        
        /**
         *  将自己从 parent上删除
         */
        void removeFromParent();
        
        /**
         *  如果不可见,则不应该绘制,也不需要响应事件
         *
         *  @param visible 是否可见
         */
        void setVisible(bool visible){_visible = visible;}
        
    public:
        
        /**
         *  渲染自己和children
         */
        virtual void render();
        
        /**
         *   真正绘制的地方
         *   绘制自己的方法,主要让 子类去实现自己的绘制
         */
        virtual void drawSelf();
        
        /**
         *  计算最终的 model matrix
         *
         *  @param parentModelMatrix parent的最终 model matrix
         */
        virtual void calculateModelMatrix(const Mat4& parentModelMatrix);
        
        /**
         *  更新 localTransform
         */
        virtual void updateLocalTransform();
        
        /**
         *  所有node从其parent移除时,都会调用此方法
         */
        virtual void onExit();
        
    protected:
        
        /**
         *  锚点,默认为(0.5,0.5)
         */
        AnchorPoint _anchor;
        
        /**
         *  物体锚点在其 parent 上的坐标
         */
        float _x = 0, _y = 0;
        
        /**
         *  缩放
         */
        float _scaleX = 1.0f,_scaleY = 1.0f;
        
        /**
         *  角度
         */
        float _degress = 0;
        
        /**
         *  所有的 sub node
         */
        vector<Node*> _children;
        
        /**
         *  父 node
         */
        Node* _parent = nullptr;
        
        /**
         *  node 所占据的大小,默认为整个屏幕
         */
        Size _contentSize;
        
        /**
         *  相对于 parent 的  matrix
         */
        Mat4 _localTransform;
        
        /**
         *  最终的 model matrix;
         */
        Mat4 _modelMatrix;
        
        /**
         *  用于判断 _localTransform 是否更新
         */
        bool _dirty = false;
        
        /**
         *  用于标记某一个 node,方便获取.默认为 -1
         */
        int _tag = -1;
        
        /**
         *  相当于显示的顺序.用于绘制时排序.
         *  默认为0.
         *  _zorder 越大表示越显示在前面
         *  如果 _zorder相同则按照添加的顺序显示
         */
        int _zorder = 0;
        
        /**
         *  是否可见. 默认为可见.会影响 所有的 children.
         */
        bool _visible = true;
        
    private:
        
        /**
         *  所有用于测试 的变量,全部以 _test 开头
         *  所有用于debug 的变量,全部以 _debug 开头
         */
        
        Shader* _testShader = nullptr;
        
        /**
         *  主要用于debug
         */
        string _debug_name = "default";
        
        float _testColors[4];
        
    public:
        //-----------------------------------------
        // 用于 debug 和 测试的变量和方法
        void setTestColor(float r,float g,float b){
            _testColors[0] = r;
            _testColors[1] = g;
            _testColors[2] = b;
            _testColors[3] = 1;
        }
        
        void setDebugName(const string& name){
            _debug_name = name;
        }

    };
};

#endif /* defined(__D2DEngine__Node__) */
