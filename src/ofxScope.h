//
//  ofxScope.h
//
//  Created by ISHII 2bit on 2016/05/01.
//
//

#pragma once

#include "ofMain.h"

namespace ofx {
    namespace Scope {
        struct matrix {
            matrix() { ofPushMatrix(); };
            ~matrix() { ofPopMatrix(); };
        };
        
        struct style {
            style() { ofPushStyle(); };
            ~style() { ofPopStyle(); };
        };
        
        struct rect_mode {
            rect_mode(ofRectMode mode = ofGetRectMode())
            : saved(ofGetRectMode()) { ofSetRectMode(mode); }
            ~rect_mode() { ofSetRectMode(saved); }
        private:
            ofRectMode saved;
        };
        
        struct circle_resolution {
            circle_resolution(int resolution = ofGetStyle().circleResolution)
            : saved(ofGetStyle().circleResolution) { ofSetCircleResolution(resolution); }
            ~circle_resolution() { ofSetCircleResolution(saved); };
        private:
            int saved;
        };
        
        struct z_depth {
            z_depth() { ofEnableDepthTest(); }
            ~z_depth() { ofDisableDepthTest(); }
        };
        
        struct line_width {
            line_width(float width = ofGetStyle().lineWidth)
            : width(ofGetStyle().lineWidth) { ofSetLineWidth(width); }
            ~line_width() { ofSetLineWidth(width); }
        private:
            float width;
        };
        
        template <typename ... states>
        struct combine {
        private:
            std::tuple<states ...> state;
        };
        
        struct fbo {
            fbo() = delete;
            fbo(ofFbo &fbo_)
            : fbo_(fbo_) { fbo_.begin(); }
            ~fbo() { fbo_.end(); }
        private:
            ofFbo &fbo_;
        };
    };
};

namespace ofxScope = ofx::Scope;
using ofxScopedFbo = ofxScope::fbo;
using ofxScopedMatrix = ofxScope::matrix;
using ofxScopedStyle = ofxScope::style;
