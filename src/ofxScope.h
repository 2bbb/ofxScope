//
//  ofxScope.h
//
//  Created by ISHII 2bit on 2016/05/01.
//
//

#pragma once

#include <tuple>

#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "ofLight.h"

namespace ofx {
    namespace scope {
        namespace scoped {
            struct base {
                base() {};
                base(const base &rhs) = delete;
                base(base &&rhs) { rhs.is_moved = true; };
                base &operator=(const base &) = delete;
                base &operator=(base &&) = delete;
                static constexpr bool is_scoped_struct{true};
            protected:
                bool is_moved{false};
            };
            
            template <void init(), void finish()>
            struct no_arg : base {
                no_arg() { init(); };
                no_arg(no_arg &&rhs)
                : base(std::move(rhs)) {};
                ~no_arg() { if(!is_moved) finish(); };
            };
            
            using matrix = no_arg<ofPushMatrix, ofPopMatrix>;
            using style = no_arg<ofPushStyle, ofPopStyle>;
            using view = no_arg<ofPushView, ofPopView>;
            
            struct shape : base {
                shape(bool close = false)
                : close(close)
                { ofBeginShape(); };
                shape(shape &&rhs)
                : base(std::move(rhs)) {};
                ~shape() { if(!is_moved) ofEndShape(close); };
            private:
                bool close;
            };
            
            struct save_screen_as_pdf : base {
                save_screen_as_pdf()
                { ofBeginSaveScreenAsPDF(ofToString(ofGetFrameNum()) + ".pdf"); }
                save_screen_as_pdf(const std::string &path)
                { ofBeginSaveScreenAsPDF(path); }
                save_screen_as_pdf(save_screen_as_pdf &&rhs)
                : base(std::move(rhs)) {};
                ~save_screen_as_pdf()
                { if(!is_moved) ofEndSaveScreenAsPDF(); };
            };
            
            struct save_screen_as_svg : base {
                save_screen_as_svg()
                { ofBeginSaveScreenAsSVG(ofToString(ofGetFrameNum()) + ".svg"); }
                save_screen_as_svg(const std::string &path)
                { ofBeginSaveScreenAsSVG(path); }
                save_screen_as_svg(save_screen_as_svg &&rhs)
                : base(std::move(rhs)) {};
                ~save_screen_as_svg()
                { if(!is_moved) ofEndSaveScreenAsSVG(); };
            };
            
            template <GLenum param>
            struct gl_bool_parameter : base {
                gl_bool_parameter()
                : saved(getParam()) {};
                gl_bool_parameter(bool enable)
                : gl_bool_parameter() { setParam(enable); };
                gl_bool_parameter(gl_bool_parameter &&rhs)
                : base(std::move(rhs))
                , saved(rhs.saved) {};
                ~gl_bool_parameter() { setParam(saved); };
            private:
                static bool getParam() {
                    GLboolean v{0};
                    glGetBooleanv(GL_MULTISAMPLE, &v);
                    return static_cast<bool>(v);
                }
                static bool setParam(bool enable) {
                    if(enable) glEnable(param);
                    else glDisable(param);
                }
                bool saved;
            };
            
            using depth_test = gl_bool_parameter<GL_MULTISAMPLE>;
            using anti_aliasing = gl_bool_parameter<GL_MULTISAMPLE>;
#ifdef TARGET_OPENGLES
#   ifndef TARGET_PROGRAMMABLE_GL
            using point_sprites = gl_bool_parameter<GL_POINT_SPRITE_OES>;
#   endif
#else
            using point_sprites = gl_bool_parameter<GL_PROGRAM_POINT_SIZE>;
#endif

            struct smoothing : base {
                smoothing()
                : saved(getParam()) {};
                smoothing(bool enable)
                : smoothing() { setParam(enable); };
                smoothing(smoothing &&rhs)
                : base(std::move(rhs))
                , saved(rhs.saved) {};
                ~smoothing() { setParam(saved); };
            private:
                static bool getParam()
                { return ofGetStyle().smoothing; };
                static bool setParam(bool enable) {
                    if(enable) ofEnableSmoothing();
                    else ofDisableSmoothing();
                }
                bool saved;
            };

            struct lighting : base {
                lighting()
                : saved(ofGetLightingEnabled())
                {};
                lighting(lighting &&rhs)
                : base(std::move(rhs))
                , saved(rhs.saved) {};
                lighting(bool enable)
                : lighting() { setEnable(enable); };
                ~lighting() { if(!is_moved) setEnable(saved); }
            private:
                inline void setEnable(bool enable) {
                    if(enable) ofEnableLighting();
                    else ofDisableLighting();
                }
                bool saved;
            };
            
            struct rect_mode : base {
                rect_mode()
                : saved(ofGetRectMode()) {};
                rect_mode(ofRectMode mode)
                : rect_mode() { ofSetRectMode(mode); };
                rect_mode(rect_mode &&rhs)
                : base(std::move(rhs))
                , saved(rhs.saved) {};
                ~rect_mode() { if(!is_moved) ofSetRectMode(saved); }
            private:
                ofRectMode saved;
            };
            
            struct blend_mode : base {
                blend_mode()
                : saved(ofGetStyle().blendingMode) {};
                blend_mode(ofBlendMode mode)
                : blend_mode() { ofEnableBlendMode(mode); };
                blend_mode(blend_mode &&rhs)
                : base(std::move(rhs))
                , saved(rhs.saved) {};
                ~blend_mode() { if(!is_moved) ofEnableBlendMode(saved); }
            private:
                ofBlendMode saved;
            };

            struct fill_mode : base {
                fill_mode()
                : saved(ofGetFill()) {};
                fill_mode(ofFillFlag mode)
                : fill_mode() { setMode(mode); };
                fill_mode(fill_mode &&rhs)
                : base(std::move(rhs))
                , saved(rhs.saved) {};
                ~fill_mode() { if(!is_moved) setMode(saved); }
            private:
                inline void setMode(ofFillFlag mode) {
                    if(mode == OF_OUTLINE) ofNoFill();
                    else ofFill();
                }
                ofFillFlag saved;
            };
            
#if 10 <= OF_VERSION_MINOR
            // matrix_mode
            // poly_mode
#endif
            
            struct line_width : base {
                line_width()
                : saved(ofGetStyle().lineWidth) {}
                line_width(float width)
                : line_width() { ofSetLineWidth(width); }
                line_width(line_width &&rhs)
                : base(std::move(rhs))
                , saved(rhs.saved) {};
                ~line_width()
                { if(!is_moved) ofSetLineWidth(saved); }
            private:
                float saved;
            };
            
            struct circle_resolution : base {
                circle_resolution()
                : saved(ofGetStyle().circleResolution) {};
                circle_resolution(int resolution)
                : circle_resolution() { ofSetCircleResolution(resolution); }
                circle_resolution(circle_resolution &&rhs)
                : base(std::move(rhs))
                , saved(rhs.saved) {};
                ~circle_resolution()
                { if(!is_moved) ofSetCircleResolution(saved); };
            private:
                int saved;
            };
            
            struct curve_resolution : base {
                curve_resolution()
                : saved(ofGetStyle().curveResolution) {};
                curve_resolution(int resolution)
                : curve_resolution() { ofSetCurveResolution(resolution); }
                curve_resolution(curve_resolution &&rhs)
                : base(std::move(rhs))
                , saved(rhs.saved) {};
                ~curve_resolution()
                { if(!is_moved) ofSetCurveResolution(saved); };
            private:
                int saved;
            };

            // sphere
            // ico_sphere
            // box
            // cone
            // plane
            // cylinder

            template <typename type>
            struct begin_end : base {
                begin_end(type &v)
                : v(v) { v.begin(); };
                begin_end(begin_end &&rhs)
                : base(std::move(rhs))
                , v(rhs.v) {};
                ~begin_end()
                { if(!is_moved) v.end(); };
            private:
                type &v;
            };
        }; // end of namespace scoped
        
        namespace tag {
            struct base { static constexpr bool is_tag{true}; };
            template <typename type>
            struct no_arg : base {
                using scoped_type = type;
                scoped_type operator()() const
                { return {}; };
            };
            using matrix = no_arg<scoped::matrix>;
            using style = no_arg<scoped::style>;
            using view = no_arg<scoped::view>;
            
            struct shape : base {
                using scoped_type = scoped::shape;
                scoped_type operator()(bool close = false) const
                { return { close }; };
                
                struct open {
                    static scoped_type create_scope()
                    { return {}; };
                };
                struct closed {
                    static scoped_type create_scope()
                    { return { true }; };
                };
            };
            
            struct save_screen_as_pdf : base {
                using scoped_type = scoped::save_screen_as_pdf;
                inline scoped::save_screen_as_pdf operator()() const
                { return {}; };
                inline scoped::save_screen_as_pdf operator()(const std::string &path) const
                { return { path }; };
            };
            
            struct save_screen_as_svg : base {
                using scoped_type = scoped::save_screen_as_svg;
                inline scoped::save_screen_as_svg operator()() const
                { return {}; };
                inline scoped::save_screen_as_svg operator()(const std::string &path) const
                { return { path }; };
            };
            
            template <typename type>
            struct bool_parameter : base {
                using scoped_type = type;
                inline scoped_type operator()() const
                { return {}; };
                inline scoped_type operator()(bool enable) const
                { return { enable }; };
                
                struct enabler {
                    static scoped_type create_scope()
                    {  return { true }; }
                };
                struct disabler {
                    static scoped_type create_scope()
                    {  return { false }; }
                };
            };
            
            using depth_test    = bool_parameter<scoped::depth_test>;
            using lighting      = bool_parameter<scoped::lighting>;
            using anti_aliasing = bool_parameter<scoped::anti_aliasing>;
            using point_sprites = bool_parameter<scoped::point_sprites>;
            using smoothing     = bool_parameter<scoped::smoothing>;

            // smoothing
            
            struct rect_mode : base {
                using scoped_type = scoped::rect_mode;
                inline scoped::rect_mode operator()() const
                { return {}; };
                inline scoped::rect_mode operator()(ofRectMode mode) const
                { return { mode }; };
                
                struct corner : base {
                    static scoped::rect_mode create_scope()
                    { return { OF_RECTMODE_CORNER }; };
                };
                struct center : base {
                    static scoped::rect_mode create_scope()
                    { return { OF_RECTMODE_CENTER }; };
                };
            };
            
            struct blend_mode : base {
                using scoped_type = scoped::rect_mode;
                inline scoped::blend_mode operator()() const
                { return {}; };
                inline scoped::blend_mode operator()(ofBlendMode mode) const
                { return { mode }; };
                
                struct alpha_blending : base {
                    using scoped_type = scoped::blend_mode;
                    inline scoped::blend_mode operator()() const
                    { return {}; };
                    inline scoped::blend_mode operator()(bool enable) const
                    { return { enable ? OF_BLENDMODE_ALPHA : OF_BLENDMODE_DISABLED }; };
                    
                    struct enabler : base {
                        static scoped::blend_mode create_scope()
                        { return { OF_BLENDMODE_ALPHA }; };
                    };
                    struct disabler : base {
                        static scoped::blend_mode create_scope()
                        { return { OF_BLENDMODE_DISABLED }; };
                    };
                };
            };
            
            struct fill_mode : base {
                using scoped_type = scoped::fill_mode;
                inline scoped::fill_mode operator()() const
                { return {}; };
                inline scoped::fill_mode operator()(ofFillFlag flag) const
                { return { flag }; };
                
                struct no_fill : base {
                    static scoped::fill_mode create_scope()
                    { return { OF_OUTLINE }; };
                };
                struct fill : base {
                    static scoped::fill_mode create_scope()
                    { return { OF_FILLED }; };
                };
            };
            
            struct line_width : base {
                inline scoped::line_width operator()() const
                { return {}; };
                inline scoped::line_width operator()(float width) const
                { return { width }; };
            };
            
            struct circle_resolution : base {
                inline scoped::circle_resolution operator()() const
                { return {}; };
                inline scoped::circle_resolution operator()(int resolution) const
                { return { resolution }; };
            };
            
            struct curve_resolution : base {
                inline scoped::curve_resolution operator()() const
                { return {}; };
                inline scoped::curve_resolution operator()(int resolution) const
                { return { resolution }; };
            };
            
            // sphere
            // ico_sphere
            // box
            // cone
            // plane
            // cylinder
        };
        
        namespace {
            constexpr tag::matrix matrix;
            constexpr tag::style style;
            constexpr tag::view view;
            
            constexpr tag::shape shape;
            constexpr tag::shape::open openShape;
            constexpr tag::shape::closed closedShape;

            constexpr tag::save_screen_as_pdf saveScreenAsPDF;
            constexpr tag::save_screen_as_svg saveScreenAsSVG;
            
            constexpr tag::depth_test depthTest;
            constexpr tag::depth_test::enabler enableDepthTest;
            constexpr tag::depth_test::disabler disableDepthTest;
            
            constexpr tag::lighting lighting;
            constexpr tag::lighting::enabler enableLighting;
            constexpr tag::lighting::disabler disableLighting;
            
            constexpr tag::anti_aliasing antiAliasing;
            constexpr tag::anti_aliasing::enabler enableAntiAliasing;
            constexpr tag::anti_aliasing::disabler disableAntiAliasing;
            
            constexpr tag::point_sprites pointSprites;
            constexpr tag::point_sprites::enabler enablePointSprites;
            constexpr tag::point_sprites::disabler disablePointSprites;
            
            constexpr tag::smoothing smoothing;
            constexpr tag::smoothing::enabler enableSmoothing;
            constexpr tag::smoothing::disabler disableSmoothing;
            
            constexpr tag::rect_mode rectMode;
            constexpr tag::rect_mode::corner rectModeCorner;
            constexpr tag::rect_mode::center rectModeCenter;
            
            constexpr tag::blend_mode blendMode;
            constexpr tag::blend_mode::alpha_blending alphaBlending;
            constexpr tag::blend_mode::alpha_blending::enabler enableAlphaBlending;
            constexpr tag::blend_mode::alpha_blending::disabler disableAlphaBlending;
            
            constexpr tag::fill_mode fillMode;
            constexpr tag::fill_mode::no_fill noFill;
            constexpr tag::fill_mode::fill fill;
#if 10 <= OF_VERSION_MINOR
            // matrix_mode
            // poly_mode
#endif
            constexpr tag::line_width lineWidth;
            constexpr tag::circle_resolution circleResolution;
            constexpr tag::curve_resolution curveResolution;
            // sphere
            // ico_sphere
            // box
            // cone
            // plane
            // cylinder
        };
        
        namespace {
            template <typename type>
            inline auto create_scope(type &&rhs)
                -> typename std::enable_if<type::is_scoped_struct, type>::type
            { return std::move(rhs); };
            
            template <typename tag_type>
            inline auto create_scope(const tag_type &tag)
                -> typename std::enable_if<
                    tag_type::is_tag,
                    typename tag_type::scoped_type
                >::type
            { return {}; };
            
            template <typename tag_type>
            inline auto create_scope(const tag_type &tag)
                -> typename std::enable_if<
                    decltype(tag_type::create_scope(), std::true_type())::value,
                    decltype(tag_type::create_scope())
                >::type
            { return tag_type::create_scope(); };

            // anti_aliasing
            // point_sprites
            // smoothing

            // rect_mode
            inline scoped::rect_mode create_scope(ofRectMode mode)
            { return { mode }; };

            // blend_mode
            inline scoped::blend_mode create_scope(ofBlendMode mode)
            { return { mode }; };
            
            // fill_mode
            inline scoped::fill_mode create_scope(ofFillFlag flag)
            { return { flag }; };
            
            // sphere
            // ico_sphere
            // box
            // cone
            // plane
            // cylinder

            // begin / end
            template <typename type>
            inline auto create_scope(type &v)
            -> typename std::enable_if<
                decltype(v.begin(), v.end(), std::true_type())::value,
                scoped::begin_end<type>
            >::type
            { return {v}; };
        };
    };
};

namespace ofxScope = ofx::scope;

template <typename ... types>
auto ofxCreateScope(types && ...vs)
    -> std::tuple<decltype(ofx::scope::create_scope(std::forward<types>(vs))) ...>
{ return std::make_tuple(ofx::scope::create_scope(std::forward<types>(vs)) ...); };

