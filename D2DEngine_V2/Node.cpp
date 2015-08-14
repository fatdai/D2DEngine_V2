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
#include "MatrixStack.h"
#include "globals.h"

namespace D2D {
    
    Node::Node(){
        auto director = Director::getInstance();
        _contentSize = Size(director->getWidth(),director->getHeight());
        _colors[0] = _colors[1] = _colors[2] = _colors[3] = 1.0f;
        _localTransform = Mat4::IDENTITY;
        _modelMatrix = Mat4::IDENTITY;
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
            float offx = _anchor.getAnchorX() * _contentSize.width;
            float offy = _anchor.getAnchorY() * _contentSize.height;
            
            float sinx = sinf(rad);
            float cosx = cosf(rad);
            
            _localTransform.m[0] = cosx * scalex;
            _localTransform.m[1] = sinx;
            _localTransform.m[2] = 0;
            _localTransform.m[3] = 0;
            
            _localTransform.m[4] = -sinx;
            _localTransform.m[5] = cosx * scaley;
            _localTransform.m[6] = 0;
            _localTransform.m[7] = 0;
            
            _localTransform.m[8] = 0;
            _localTransform.m[9] = 0;
            _localTransform.m[10] = 1;
            _localTransform.m[11] = 0;
            
            _localTransform.m[12] = x;
            _localTransform.m[13] = y;
            _localTransform.m[14] = 0;
            _localTransform.m[15] = 1;
            
            Mat4 addMat;
            Mat4::createTranslation(-offx,-offy,0,&addMat);
            _localTransform.multiply(addMat);
            
          //  _transform.add();
            
            _dirty = false;
        }
    }
    
//    void Node::testDrawSelf(const Mat4& mat){
//        
//        //  for test,先绘制自己,再绘制 children
//        float w = _contentSize.width;
//        float h = _contentSize.height;
//
//        float vertices[] = {
//            0,0,
//            w,0,
//            w,h,
//            
//            0,0,
//            w,h,
//            0,h
//        };
//        
//        _testShader = ShaderUtil::getInstance()->getShader(ShaderType::Shader_Color);
//        glUseProgram(_testShader->getProgramId());
//        
//        glUniformMatrix4fv(_testShader->get_U_MatrixId(),1, GL_FALSE,mat.m);
//        glUniform4fv(_testShader->get_U_ColorId(),1,_colors);
//        
//        glVertexAttribPointer(_testShader->get_A_PositionId(),2,GL_FLOAT,GL_FALSE,0,vertices);
//        glEnableVertexAttribArray(_testShader->get_A_PositionId());
//        
//        glDrawArrays(GL_TRIANGLES,0,6);
//        CheckGLError();
//        glDisableVertexAttribArray(_testShader->get_A_PositionId());
//        
//    }
    
    
    void Node::visit(const Mat4& mat){
        
        // 先对 children 排序
        sort(_children.begin(),_children.end(),[](Node* n1,Node* n2){
            if (n1->getZOrder() < n2->getZOrder()) {
                return true;
            }else{
                return false;
            }
        });
        
        updateTransform();
        Mat4::multiply(mat,_localTransform,&_modelMatrix);
        
        for (int i = 0; i < _children.size(); ++i) {
            _children[i]->visit(_modelMatrix);
        }
    }
    
//    void Node::startRender(){
//        auto& projMat = Director::getInstance()->getProjMat4();
//        Mat4 dst;
//        Mat4::multiply(projMat,_transform,&dst);
//        testRender(dst);
//    }
    
    void Node::render(){
        
        if (_children.size() < 1) {
            drawSelf();
            return;
        }
        
        int i = 0;
        for (i = 0; i < _children.size(); ++i) {
            if (_children[i]->getZOrder() < 0) {
                _children[i]->render();
            }else{
                break;
            }
        }
        
        drawSelf();
        
        for (; i < _children.size(); ++i) {
            _children[i]->render();
        }
        
        
        // 按照顺序绘制下来即可
//        if (_children.size() > 0) {
//            
//            
//        }else{
//            drawSelf();
//        }
    }
    
    void Node::drawSelf(){
        // 如果是 sprite,则 sprite 重写此方法即可
        // 现在为了测试,渲染自己为一个矩形
        
        if (!_visible) {
            return;
        }
        
        //  for test,先绘制自己,再绘制 children
        float w = _contentSize.width;
        float h = _contentSize.height;
        
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
        
        Mat4 mat;
        Mat4::multiply(Director::getInstance()->getProjMat4(),_modelMatrix, &mat);
        
        glUniformMatrix4fv(_testShader->get_U_MatrixId(),1, GL_FALSE,mat.m);
        glUniform4fv(_testShader->get_U_ColorId(),1,_colors);
        
        glVertexAttribPointer(_testShader->get_A_PositionId(),2,GL_FLOAT,GL_FALSE,0,vertices);
        glEnableVertexAttribArray(_testShader->get_A_PositionId());
        
        glDrawArrays(GL_TRIANGLES,0,6);
        CheckGLError();
        glDisableVertexAttribArray(_testShader->get_A_PositionId());
        
     //   log("rendering %s",_debug_name.c_str());
    }
    
//    // for test
//    void Node::testRender(const Mat4& mat){
//        
//        
////        for (int i = 0; i < _children.size(); ++i) {
////            if (_children[i]->_zorder < 0) {
////                
////            }else if (_children[i]->_zorder == 0)
////        }
//        
//        // 2.进行绘制
//        testDrawSelf(mat);
//        
//        // 绘制 child
//        Mat4 tempMat1;
//        for (int i = 0; i < _children.size(); ++i) {
//            Mat4::multiply(mat,_children[i]->_transform,&tempMat1);
//            _children[i]->testRender(tempMat1);
//        }
//    }
    
    
    
    void Node::addChild(Node* child){
        assert(child != nullptr && child->_parent == nullptr);
        _children.push_back(child);
        child->_parent = this;
    }
    
    void Node::addChild(Node* child,int tag){
        assert(child != nullptr && child->_parent == nullptr);
        child->setTag(tag);
        _children.push_back(child);
        child->_parent = this;
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

}
