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
//    class Mat3{
//        
//    public:
//    
//       float m[9];
//        
//        Mat3(){
//            init();
//        }
//        
//        
//    private:
//        
//        void init(){
//            
//            m[0] = 1;
//            m[4] = 1;
//            m[8] = 1;
//            
//            m[1] = m[2] = m[3] = m[5] = m[6] = m[7] = 0;
//        }
//    };
    
    
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
        
        // for test
    //    void testDrawSelf(const Mat4& mat);
    //    void startRender();
    //    void testRender(const Mat4& mat);
        
        
        Shader* _testShader = nullptr;
        
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
        
        void setColor(float r,float g,float b){
            _colors[0] = r;
            _colors[1] = g;
            _colors[2] = b;
            _colors[3] = 1;
        }
        
        // -------- scene graph
        Node* getChildByTag(int tag);
        void addChild(Node* child);
        void addChild(Node* child,int tag);
        void removeFromParent();

        void onExit();
        
        void setDebugName(const string& name){
            _debug_name = name;
        }
        
        void setVisible(bool visible){_visible = visible;}
        
    public:
        
        virtual void render();
        virtual void drawSelf();
        virtual void visit(const Mat4& mat);
        virtual void updateTransform();
        
    protected:
        
        AnchorPoint _anchor;

        // position
        float _x = 0, _y = 0;
        
        // scale
        float _scaleX = 1.0f,_scaleY = 1.0f;
        
        // rotate
        float _degress = 0;
        
        // 默认为0,z越大越显示在前面
        // 如果z相同则按照添加的顺序显示
        int _z = 0;
        
        /**
         *  children
         */
        vector<Node*> _children;
        
        Node* _parent = nullptr;
        
        // 默认为按照设计的尺寸
        Size _contentSize;
        
        Mat4 _localTransform;
        Mat4 _modelMatrix;
        
        bool _dirty = false;
        
        int _tag = -1;
        
        float _colors[4];
        
        int _zorder = 0;
        
        string _debug_name = "default";
        
        bool _visible = true;
    };
};

#endif /* defined(__D2DEngine__Node__) */
