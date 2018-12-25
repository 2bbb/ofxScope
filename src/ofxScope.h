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
        namespace accessor {
            struct arb_tex {
                using value_type = bool;
                inline value_type get() const
                { return ofGetUsingArbTex(); };
                inline void set(value_type value) const {
                    if(value) ofEnableArbTex();
                    else ofDisableArbTex();
                };
            };
            
            template <GLenum param>
            struct gl_bool {
                using value_type = bool;
                inline value_type get() const {
                    GLboolean v{0};
                    glGetBooleanv(param, &v);
                    return static_cast<bool>(v);
                }
                inline void set(bool enable) const {
                    if(enable) glEnable(param);
                    else glDisable(param);
                }
            };

            struct smoothing {
                using value_type = bool;
                inline value_type get() const
                { return ofGetStyle().smoothing; };
                inline void set(value_type value) const {
                    if(value) ofEnableSmoothing();
                    else ofDisableSmoothing();
                };
            };
            struct lighting {
                using value_type = bool;
                inline value_type get() const
                { return ofGetLightingEnabled(); };
                inline void set(value_type value) const {
                    if(value) ofEnableLighting();
                    else ofDisableLighting();
                };
            };
            struct rect_mode {
                using value_type = ofRectMode;
                inline value_type get() const
                { return ofGetRectMode(); };
                inline void set(value_type value) const
                { ofSetRectMode(value); };
            };
            struct blend_mode {
                using value_type = ofBlendMode;
                inline value_type get() const
                { return ofGetStyle().blendingMode; };
                inline void set(value_type value) const
                { ofEnableBlendMode(value); };
            };
            struct fill_mode {
                using value_type = ofFillFlag;
                inline value_type get() const
                { return ofGetFill(); };
                inline void set(value_type value) const {
                    if(value == OF_OUTLINE) ofNoFill();
                    else ofFill();
                };
            };
            struct line_width {
                using value_type = float;
                inline value_type get() const
                { return ofGetStyle().lineWidth; };
                inline void set(value_type value) const
                { ofSetLineWidth(value); };
            };
            struct circle_resolution {
                using value_type = int;
                inline value_type get() const
                { return ofGetStyle().circleResolution; };
                inline void set(value_type res) const
                { ofSetCircleResolution(res); };
            };
            struct curve_resolution {
                using value_type = int;
                inline value_type get() const
                { return ofGetStyle().curveResolution; };
                inline void set(value_type res)
                { ofSetCurveResolution(res); };
            };
            struct sphere_resolution {
                using value_type = int;
                inline value_type get() const
                { return ofGetSphereResolution(); };
                inline void set(value_type res)
                { ofSetSphereResolution(res); };
            };
            struct ico_sphere_resolution {
                using value_type = int;
                inline value_type get() const
                { return ofGetIcoSphereResolution(); };
                inline void set(value_type res)
                { ofSetIcoSphereResolution(res); };
            };
            struct box_resolution {
                using value_type = ofVec3f;
                inline value_type get() const
                { return ofGetBoxResolution(); };
                inline void set(value_type res)
                { ofSetBoxResolution(res.x, res.y, res.z); };
            };
            struct cone_resolution {
                using value_type = ofVec3f;
                inline value_type get() const
                { return ofGetConeResolution(); };
                inline void set(value_type res)
                { ofSetConeResolution(res.x, res.y, res.z); };
            };
            struct cylinder_resolution {
                using value_type = ofVec3f;
                inline value_type get() const
                { return ofGetCylinderResolution(); };
                inline void set(value_type res)
                { ofSetCylinderResolution(res.x, res.y, res.z); };
            };
            struct plane_resolution {
                using value_type = ofVec2f;
                inline value_type get() const
                { return ofGetPlaneResolution(); };
                inline void set(value_type res)
                { ofSetPlaneResolution(res.x, res.y); };
            };
        } // end of namespace accessor
        
        namespace scoped {
            struct base {
                base() {};
                base(const base &rhs) = delete;
                base(base &&rhs) { rhs.is_moved = true; };
                base(std::function<void()> body)
                : base() { body(); };
                base &operator=(const base &) = delete;
                base &operator=(base &&) = delete;
                static constexpr bool is_scoped_struct{true};
                
                inline void operator()(std::function<void()> body) const
                { body(); };
                inline void run(std::function<void()> body) const
                { body(); };
            protected:
                bool is_moved{false};
            };
            
            template <void init(), void finish()>
            struct no_arg : base {
                no_arg() { init(); };
                no_arg(no_arg &&rhs)
                : base(std::move(rhs)) {};
                no_arg(std::function<void()> body)
                : base(body) {};
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
            
            template <typename accessor>
            struct accessor_pattern : base {
                using value_type = typename accessor::value_type;
                accessor_pattern()
                : a()
                , saved(a.get()) {};
                accessor_pattern(value_type new_resolution)
                : accessor_pattern() { a.set(new_resolution); };
                accessor_pattern(accessor_pattern &&rhs)
                : base(std::move(rhs))
                , saved(rhs.saved) {};
                ~accessor_pattern()
                { if(!is_moved) a.set(saved); };
                
            private:
                accessor a;
                value_type saved;
            };
            
            template <GLenum param>
            using gl_bool_parameter = accessor_pattern<accessor::gl_bool<param>>;
            
            using depth_test = gl_bool_parameter<GL_DEPTH_TEST>;
            using anti_aliasing = gl_bool_parameter<GL_MULTISAMPLE>;
#ifdef TARGET_OPENGLES
#   ifndef TARGET_PROGRAMMABLE_GL
            using point_sprites = gl_bool_parameter<GL_POINT_SPRITE_OES>;
#   endif
#else
            using point_sprites = gl_bool_parameter<GL_PROGRAM_POINT_SIZE>;
#endif
            using arb_tex = accessor_pattern<accessor::arb_tex>;
            using smoothing = accessor_pattern<accessor::smoothing>;
            using lighting = accessor_pattern<accessor::lighting>;
            using rect_mode = accessor_pattern<accessor::rect_mode>;
            using blend_mode = accessor_pattern<accessor::blend_mode>;
            using fill_mode = accessor_pattern<accessor::fill_mode>;
            
#if 10 <= OF_VERSION_MINOR
            // matrix_mode
            // poly_mode
#endif
            
            using line_width = accessor_pattern<accessor::line_width>;
            
            using circle_resolution = accessor_pattern<accessor::circle_resolution>;
            using curve_resolution = accessor_pattern<accessor::curve_resolution>;
            using sphere_resolution = accessor_pattern<accessor::sphere_resolution>;
            using ico_sphere_resolution = accessor_pattern<accessor::ico_sphere_resolution>;
            
            using box_resolution = accessor_pattern<accessor::box_resolution>;
            using cone_resolution = accessor_pattern<accessor::cone_resolution>;
            using cylinder_resolution = accessor_pattern<accessor::cylinder_resolution>;
            
            using plane_resolution = accessor_pattern<accessor::plane_resolution>;

            template <typename type>
            struct bind : base {
                bind(type &v)
                : v(v) { v.bind(); };
                bind(bind &&rhs)
                : base(std::move(rhs))
                , v(rhs.v) {};
                ~bind()
                { if(!is_moved) v.unbind(); };
            private:
                type &v;
            };
            
            template <typename type>
            struct begin : base {
                begin(type &v)
                : v(v) { v.begin(); };
                begin(begin &&rhs)
                : base(std::move(rhs))
                , v(rhs.v) {};
                ~begin()
                { if(!is_moved) v.end(); };
            private:
                type &v;
            };
            
            struct custom : base {
                custom(const std::function<void()> &cns,
                       const std::function<void()> &dst)
                : cns(cns)
                , dst(dst)
                { cns(); };
                custom(custom &&rhs)
                : base(std::move(rhs))
                , cns(std::move(rhs.cns))
                , dst(std::move(rhs.dst))
                {};
                ~custom()
                { if(!is_moved) dst(); };
            private:
                std::function<void()> cns{[]{}};
                std::function<void()> dst{[]{}};
            };
        }; // end of namespace scoped
        
        namespace tag {
            template <typename type>
            struct base {
                using scoped_type = type;
                static constexpr bool is_tag{true};
                
                scoped_type operator()() const
                { return {}; };
                inline void run(std::function<void()> body) const {
                    scoped_type _;
                    body();
                };
                inline void operator()(std::function<void()> body) const
                { run(body); };
            };
            
            template <typename type>
            struct no_arg : base<type> {};
            
            using matrix = no_arg<scoped::matrix>;
            using style = no_arg<scoped::style>;
            using view = no_arg<scoped::view>;
            
            struct shape : base<scoped::shape> {
                inline scoped_type operator()(bool close = false) const
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
            
            struct save_screen_as_pdf : base<scoped::save_screen_as_pdf> {
                inline scoped_type operator()(const std::string &path) const
                { return { path }; };
            };
            
            struct save_screen_as_svg : base<scoped::save_screen_as_svg> {
                inline scoped_type operator()(const std::string &path) const
                { return { path }; };
            };
            
            template <typename type>
            struct bool_parameter : base<type> {
                using scoped_type = typename base<type>::scoped_type;
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
            using anti_aliasing = bool_parameter<scoped::anti_aliasing>;
            using point_sprites = bool_parameter<scoped::point_sprites>;
            using arb_tex       = bool_parameter<scoped::arb_tex>;
            using smoothing     = bool_parameter<scoped::smoothing>;
            using lighting      = bool_parameter<scoped::lighting>;

            struct rect_mode : base<scoped::rect_mode> {
                inline scoped_type operator()(ofRectMode mode) const
                { return { mode }; };
                
                struct corner : base {
                    static scoped_type create_scope()
                    { return { OF_RECTMODE_CORNER }; };
                };
                struct center : base {
                    static scoped_type create_scope()
                    { return { OF_RECTMODE_CENTER }; };
                };
            };
            
            struct blend_mode : base<scoped::blend_mode> {
                inline scoped_type operator()(ofBlendMode mode) const
                { return { mode }; };
                
                struct alpha_blending : base {
                    using scoped_type = scoped::blend_mode;
                    inline scoped_type operator()() const
                    { return {}; };
                    inline scoped_type operator()(bool enable) const
                    { return { enable ? OF_BLENDMODE_ALPHA : OF_BLENDMODE_DISABLED }; };
                    
                    struct enabler : base {
                        static scoped_type create_scope()
                        { return { OF_BLENDMODE_ALPHA }; };
                    };
                    struct disabler : base {
                        static scoped_type create_scope()
                        { return { OF_BLENDMODE_DISABLED }; };
                    };
                };
            };
            
            struct fill_mode : base<scoped::fill_mode> {
                inline scoped_type operator()(ofFillFlag flag) const
                { return { flag }; };
                
                struct no_fill : base {
                    static scoped_type create_scope()
                    { return { OF_OUTLINE }; };
                };
                struct fill : base {
                    static scoped_type create_scope()
                    { return { OF_FILLED }; };
                };
            };
            
            struct line_width : base<scoped::line_width> {
                inline scoped_type operator()(float width) const
                { return { width }; };
            };
            
            namespace detail {
                template <typename type>
                struct int_resolution : base<type> {
                    using scoped_type = typename base<type>::scoped_type;
                    inline scoped_type operator()(int resolution) const
                    { return { resolution }; };
                };
                template <typename type>
                struct vec2f_resolution : base<type> {
                    using scoped_type = typename base<type>::scoped_type;
                    inline scoped_type operator()(int resolution) const
                    { return { ofVec2f(resolution, resolution) }; };
                    inline scoped_type operator()(ofVec2f resolution) const
                    { return { resolution }; };
                };
                template <typename type>
                struct vec3f_resolution : base<type> {
                    using scoped_type = typename base<type>::scoped_type;
                    inline scoped_type operator()(int resolution) const
                    { return { ofVec3f(resolution, resolution, resolution) }; };
                    inline scoped_type operator()(ofVec3f resolution) const
                    { return { resolution }; };
                };
            };
            using circle_resolution = tag::detail::int_resolution<scoped::circle_resolution>;
            using curve_resolution = tag::detail::int_resolution<scoped::curve_resolution>;
            using sphere_resolution = tag::detail::int_resolution<scoped::sphere_resolution>;
            using ico_sphere_resolution = tag::detail::int_resolution<scoped::ico_sphere_resolution>;
            
            using box_resolution = tag::detail::vec3f_resolution<scoped::box_resolution>;
            using cone_resolution = tag::detail::vec3f_resolution<scoped::cone_resolution>;
            using cylinder_resolution = tag::detail::vec3f_resolution<scoped::cylinder_resolution>;
            
            using plane_resolution = tag::detail::vec2f_resolution<scoped::plane_resolution>;
            
            template <typename type>
            struct begin : base<scoped::begin<type>> {
                using scoped_type = typename base<scoped::begin<type>>::scoped_type;
                inline scoped_type operator()(type &v) const
                { return { v }; };
            };
            
            struct beginable {
                template <typename type>
                inline begin<type> operator()(type &v) const
                { return { v }; };
            };
            
            template <typename type>
            struct bind : base<scoped::bind<type>> {
                using scoped_type = typename base<scoped::bind<type>>::scoped_type;
                scoped_type operator()(type &v) const
                { return { v }; };
            };
            
            struct bindable {
                template <typename type>
                inline bind<type> operator()(type &v) const
                { return { v }; };
            };
            
            struct custom : base<scoped::custom> {
                scoped_type operator()(const std::function<void()> &cns,
                                       const std::function<void()> &dst) const
                { return {cns, dst}; };
            };
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
            
            constexpr tag::anti_aliasing antiAliasing;
            constexpr tag::anti_aliasing::enabler enableAntiAliasing;
            constexpr tag::anti_aliasing::disabler disableAntiAliasing;
            
            constexpr tag::point_sprites pointSprites;
            constexpr tag::point_sprites::enabler enablePointSprites;
            constexpr tag::point_sprites::disabler disablePointSprites;
            
            constexpr tag::arb_tex usingArbTex;
            constexpr tag::arb_tex::enabler enableArbTex;
            constexpr tag::arb_tex::disabler disableArbTex;
            
            constexpr tag::smoothing smoothing;
            constexpr tag::smoothing::enabler enableSmoothing;
            constexpr tag::smoothing::disabler disableSmoothing;
            
            constexpr tag::lighting lighting;
            constexpr tag::lighting::enabler enableLighting;
            constexpr tag::lighting::disabler disableLighting;
            
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
            constexpr tag::sphere_resolution sphereResolution;
            constexpr tag::ico_sphere_resolution icoSphereResolution;
            
            constexpr tag::box_resolution boxResolution;
            constexpr tag::cone_resolution coneResolution;
            constexpr tag::cylinder_resolution cylinderResolution;
            
            constexpr tag::plane_resolution planeResolution;
            
            constexpr tag::beginable begin;
            constexpr tag::begin<ofFbo> fbo;
            constexpr tag::begin<ofCamera> camera;
            constexpr tag::begin<ofShader> shader;
            constexpr tag::begin<ofMaterial> material;
            
            constexpr tag::bindable bind;
            constexpr tag::bind<ofTexture> bindTex;
            
            constexpr tag::custom custom;
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

            // rect_mode
            inline scoped::rect_mode create_scope(ofRectMode mode)
            { return { mode }; };

            // blend_mode
            inline scoped::blend_mode create_scope(ofBlendMode mode)
            { return { mode }; };
            
            // fill_mode
            inline scoped::fill_mode create_scope(ofFillFlag flag)
            { return { flag }; };
            
            // begin / end
            template <typename type>
            inline auto create_scope(type &v)
            -> typename std::enable_if<
                decltype(v.begin(), v.end(), std::true_type())::value,
                scoped::begin<type>
            >::type
            { return {v}; };
        };
        
        template <typename ... types>
        struct holder : scoped::base {
            holder(std::tuple<types ...> &&arg)
            : scopes(std::move(arg))
            {};
            holder(types && ... args)
            : scopes(std::move(args) ...)
            {};
            holder(holder &&rhs)
            : scopes(std::move(rhs.scopes))
            {};
            
            template <typename scope_type>
            auto add(scope_type &&scope) &&
            -> holder<types ..., decltype(create_scope(std::forward<scope_type>(scope)))>
            { return { std::tuple_cat(std::move(scopes), std::make_tuple(create_scope(std::forward<scope_type>(scope)))) }; };
            holder<types ..., scoped::custom> add(std::function<void()> cns,
                                                  std::function<void()> dst) &&
            { return { std::tuple_cat(std::move(scopes), std::make_tuple(custom(cns, dst))) }; };
        private:
            std::tuple<types ...> scopes;
        };
    };
};

namespace ofxScope = ofx::scope;
using ofxScopedMatrix = ofxScope::scoped::matrix;

template <typename ... types>
auto ofxCreateScope(types && ...vs)
    -> ofx::scope::holder<decltype(ofx::scope::create_scope(std::forward<types>(vs))) ...>
{ return { std::make_tuple(ofx::scope::create_scope(std::forward<types>(vs)) ...) }; };

