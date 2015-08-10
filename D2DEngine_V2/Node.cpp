//
//  Node.cpp
//  D2DEngine
//
//  Created by dai on 15/8/2.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#include "Node.h"
#include "Director.h"
#include "Utils.h"
#include "ShaderUtil.h"
#include <assert.h>
#include "CCMath.h"

namespace D2D {
    
    Node::Node(){
        auto director = Director::getInstance();
        _contentSize = Size(director->getWidth(),director->getHeight());
        _colors[0] = _colors[1] = _colors[2] = _colors[3] = 1.0f;
        _transform = Mat4::IDENTITY;
    }
    
    Node::~Node(){
    
    }
    
    // some transform
    void Node::setPosition(float x,float y){
        _x = x;
        _y = y;
        _dirty = true;
    }
    
    void Node::setPositionX(float x){
        _x = x;
        _dirty = true;
    }
    
    void Node::setPositionY(float y){
        _y = y;
        _dirty = true;
    }
    
    void Node::setAnchor(float ax,float ay){
        _anchor.setAnchorX(ax);
        _anchor.setAnchorY(ay);
        _dirty = true;
    }
    
    void Node::setScale(float sx,float sy){
        _scaleX = sx;
        _scaleY = sy;
        _dirty = true;
    }
    
    void Node::setScale(float s){
        _scaleX = _scaleY = s;
        _dirty = true;
    }
    
    void Node::setRotate(float degress){
        _degress = degress;
        _dirty = true;
    }
    
    void Node::setContentSize(float w,float h){
        _contentSize.width = w;
        _contentSize.height = h;
        _dirty = true;
    }
    
    void Node::updateTransform(){
    
        if (_dirty) {
            
            float x = _x;
            float y = _y;
            float scalex = _scaleX;
            float scaley = _scaleY;
            float rad = DegToRad(_degress);
            
            // node的左下角坐标
//            float bl_x = x - _anchor.getAnchorX() * _contentSize.width * scalex;
//            float bl_y = y - _anchor.getAnchorY() * _contentSize.height * scaley;
            float offx = _anchor.getAnchorX() * _contentSize.width * scalex;
            float offy = _anchor.getAnchorY() * _contentSize.height * scaley;
            
            float sinx = sinf(rad);
            float cosx = cosf(rad);
            
            _transform.m[0] = cosx * scalex;
            _transform.m[1] = sinx;
            _transform.m[2] = 0;
            _transform.m[3] = 0;
            
            _transform.m[4] = -sinx;
            _transform.m[5] = cosx * scaley;
            _transform.m[6] = 0;
            _transform.m[7] = 0;
            
            _transform.m[8] = 0;
            _transform.m[9] = 0;
            _transform.m[10] = 1;
            _transform.m[11] = 0;
            
            _transform.m[12] = x;
            _transform.m[13] = y;
            _transform.m[14] = 0;
            _transform.m[15] = 1;
            
            Mat4 addMat;
            Mat4::createTranslation(-offx,-offy,0,&addMat);
            _transform.multiply(addMat);
            
          //  _transform.add();
            
            _dirty = false;
        }
        
        // 计算自己的 modelview
    }
    
    void Node::getModelViewTransform(Mat4& dst){
//        Node* temp = _parent;
//        while (temp) {
//            Mat4::multiply(temp->_transform,_transform,&dst);
//        }
//        
//        
//        if (_parent) {
//            
//        }
    }
    
    
    void Node::testDrawSelf(const Mat4& mat){
        
        //  for test,先绘制自己,再绘制 children
        float w = _contentSize.width;
        float h = _contentSize.height;
        
        // 为了测试需要,渲染一个矩形
        // 0,1,2   0,2,3
//        float vertices[] = {
//            bl_x,bl_y,
//            bl_x + w,bl_y,
//            bl_x + w,bl_y + h,
//            
//            bl_x,bl_y,
//            bl_x + w,bl_y + h,
//            bl_x,bl_y + h
//        };

        float vertices[] = {
            0,0,
            w,0,
            w,h,
            
            0,0,
            w,h,
            0,h
        };
        
        _testShader = ShaderUtil::getInstance()->getShader(ShaderType::Shader_Color);
        glUseProgram(_testShader->getProgramId());
        
//        Mat4 dst;
//        Mat4::multiply(Director::getInstance()->getProjMat4(),mat,&dst);
        
        glUniformMatrix4fv(_testShader->get_U_MatrixId(),1, GL_FALSE,mat.m);
        glUniform4fv(_testShader->get_U_ColorId(),1,_colors);
        
        glVertexAttribPointer(_testShader->get_A_PositionId(),2,GL_FLOAT,GL_FALSE,0,vertices);
        glEnableVertexAttribArray(_testShader->get_A_PositionId());
        
        glDrawArrays(GL_TRIANGLES,0,6);
        CheckGLError();
        glDisableVertexAttribArray(_testShader->get_A_PositionId());
        
    }
    
    
    void Node::visit(){
        
        // 先更新自己
        updateTransform();
        
        // 再更新 children
        for (int i = 0;i < _children.size(); ++i) {
            _children[i]->visit();
        }
    }
    
    void Node::startRender(){
        auto& projMat = Director::getInstance()->getProjMat4();
        Mat4 dst;
        Mat4::multiply(projMat,_transform,&dst);
        testRender(dst);
    }
    
    // for test
    void Node::testRender(const Mat4& mat){
        
        // 1.先排序(TODO)
        
        // 2.进行绘制
        // push model view matrix;
        //   draw local zorder < 0
        //   draw self
        //   draw local zorder > 0
        // pop model view matrix;
        testDrawSelf(mat);
        
        // 绘制 child
        Mat4 tempMat1;
        for (int i = 0; i < _children.size(); ++i) {
            Mat4::multiply(mat,_children[i]->_transform,&tempMat1);
            _children[i]->testRender(tempMat1);
        }
    }
    
    
    
    void Node::addChild(Node* child){
        assert(child != nullptr);
        _children.push_back(child);
    }
    
    void Node::addChild(Node* child,int tag){
        assert(child != nullptr);
        child->setTag(tag);
        _children.push_back(child);
    }
    
    Node* Node::getChildByTag(int tag){
        for (int i = 0; i < _children.size(); ++i) {
            if (_children[i]->getTag() == tag) {
                return _children[i];
            }
        }
        return nullptr;
    }
    
    void Node::removeFromParent(){
        if (_parent != nullptr) {
            
            // 先断开自己
            for (int i = 0; i < _parent->_children.size(); ++i) {
                if (_parent->_children[i] == this) {
                    _parent->_children.erase(_parent->_children.begin() + i);
                    this->onExit();
                    break;
                }
            }
            delete this;
        }
    }
    
    void Node::onExit(){
        for (int i = 0; i < _children.size(); ++i) {
            _children[i]->removeFromParent();
        }
        _children.clear();
    }
    
//    void Node::removeChildren(){
//        for (int i = 0; i < _children.size(); ++i) {
//            _children[i]->removeChildren();
//            _children[i]->onExit();
//            delete _children[i];
//        }
//        _children.clear();
//    }
//    
//    void Node::removeChild(Node* child){
//        for (int i = 0; i < _children.size(); ++i) {
//            if (_children[i] == child) {
//                _children[i]->onExit();
//                _children.erase(_children.begin() + i);
//                delete _children[i];
//                return;
//            }
//        }
//    }
    


}
