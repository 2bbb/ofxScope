//
//  ofxScope.h
//
//  Created by ISHII 2bit on 2016/05/01.
//
//

#ifndef ofxScope_h
#define ofxScope_h
#endif

#pragma once

#include <tuple>

#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "of3dGraphics.h"
#include "ofLight.h"
#include "ofTexture.h"

#ifdef TARGET_OPENGLES
#   ifndef TARGET_PROGRAMMABLE_GL
#           define OFXSCOPE_POINT_SPRITSE_PARAM GL_POINT_SPRITE_OES
#   endif
#else
#           define OFXSCOPE_POINT_SPRITSE_PARAM GL_PROGRAM_POINT_SIZE
#endif

namespace ofx {
    namespace scope {
        namespace detail {
            template <typename... Ts> using void_t = void;
            
            template <typename type, typename = void>
            struct has_create_scope
            : std::false_type {};
            
            template <typename type>
            struct has_create_scope<type, void_t<decltype(type::create_scope())>>
            : std::true_type {};
            
            template <typename type, typename = void>
            struct has_begin_end
            : std::false_type {};
            
            template <typename type>
            struct has_begin_end<
                type,
                void_t<
                    decltype(std::declval<type>().begin()),
                    decltype(std::declval<type>().end())
                >
            > : std::true_type {};
            
            template <typename type, typename = void>
            struct has_bind_unbind
            : std::false_type {};
            
            template <typename type>
            struct has_bind_unbind<
                type,
                void_t<
                    decltype(std::declval<type>().bind()),
                    decltype(std::declval<type>().unbind())
                >
            > : std::true_type {};
            
            template <typename type, typename = void>
            struct has_getTexture
            : std::false_type {};
            
            template <typename type>
            struct has_getTexture<
                type,
                void_t<decltype(std::declval<type>().getTexture())>
            > : std::true_type {};
        };

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
            
#if 10 <= OF_VERSION_MINOR
            struct matrix_mode {};
            struct poly_mode {};
#endif
            
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
                inline void set(value_type res) const
                { ofSetSphereResolution(res); };
            };
            struct ico_sphere_resolution {
                using value_type = int;
                inline value_type get() const
                { return ofGetIcoSphereResolution(); };
                inline void set(value_type res) const
                { ofSetIcoSphereResolution(res); };
            };
            struct box_resolution {
                using value_type = ofVec3f;
                inline value_type get() const
                { return ofGetBoxResolution(); };
                inline void set(value_type res) const
                { ofSetBoxResolution(res.x, res.y, res.z); };
            };
            struct cone_resolution {
                using value_type = ofVec3f;
                inline value_type get() const
                { return ofGetConeResolution(); };
                inline void set(value_type res) const
                { ofSetConeResolution(res.x, res.y, res.z); };
            };
            struct cylinder_resolution {
                using value_type = ofVec3f;
                inline value_type get() const
                { return ofGetCylinderResolution(); };
                inline void set(value_type res) const
                { ofSetCylinderResolution(res.x, res.y, res.z); };
            };
            struct plane_resolution {
                using value_type = ofVec2f;
                inline value_type get() const
                { return ofGetPlaneResolution(); };
                inline void set(value_type res) const
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
                : base(std::move(rhs))
                , close(rhs.close) {};
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
                template <typename ... types>
                accessor_pattern(types && ... args)
                : accessor_pattern()
                { a.set(value_type(std::forward<types>(args) ...)); };
                accessor_pattern(accessor_pattern &&rhs)
                : base(std::move(rhs))
                , saved(rhs.saved) {};
                ~accessor_pattern()
                { if(!is_moved) a.set(saved); };
                
            private:
                const accessor a;
                value_type saved;
            };
            
            template <GLenum param>
            using gl_bool_parameter_templated = accessor_pattern<accessor::gl_bool<param>>;
            
            using depth_test = gl_bool_parameter_templated<GL_DEPTH_TEST>;
            using anti_aliasing = gl_bool_parameter_templated<GL_MULTISAMPLE>;
            using point_sprites = gl_bool_parameter_templated<OFXSCOPE_POINT_SPRITSE_PARAM>;
            using arb_tex = accessor_pattern<accessor::arb_tex>;
            using smoothing = accessor_pattern<accessor::smoothing>;
            using lighting = accessor_pattern<accessor::lighting>;
            using rect_mode = accessor_pattern<accessor::rect_mode>;
            using blend_mode = accessor_pattern<accessor::blend_mode>;
            using fill_mode = accessor_pattern<accessor::fill_mode>;
            
            namespace detail {
                bool get_gl_bool(GLenum param) {
                    GLboolean v{0};
                    glGetBooleanv(param, &v);
                    return static_cast<bool>(v);
                }
            };
            
#if 10 <= OF_VERSION_MINOR
//            using matrix_mode = accessor_pattern<accessor::matrix_mode>;
//            using poly_mode = accessor_pattern<accessor::polys_mode>;
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

            template <typename type, bool = scope::detail::has_getTexture<type>::value>
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
            struct bind<type, true> : bind<ofTexture> {
                bind(type &v)
                : bind<ofTexture>(v.getTexture()) {};
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
            
            struct gl_begin : base {
                gl_begin(GLenum param)
                : param(param) { glBegin(param); };
                gl_begin(gl_begin &&rhs)
                : base(std::move(rhs))
                , param(param) {};
                ~gl_begin()
                { if(!is_moved) glEnd(); };
            private:
                const GLenum param;
            };
            
            struct gl_set_enabled : base {
                gl_set_enabled(GLenum param, bool enable)
                : param(param)
                , saved(scoped::detail::get_gl_bool(param))
                { set(param); };
                gl_set_enabled(gl_set_enabled &&rhs)
                : base(std::move(rhs))
                , param(rhs.param)
                {};
                ~gl_set_enabled()
                { if(!is_moved) set(saved); };
                
            private:
                inline void set(bool is_enabled) const {
                    if(is_enabled) glEnable(param);
                    else glDisable(param);
                }
                const GLenum param;
                GLboolean saved;
            };
            
            struct gl_enable : gl_set_enabled {
                gl_enable(GLenum param)
                : gl_set_enabled(param, true)
                {};
            };
            
            struct gl_disable : gl_set_enabled {
                gl_disable(GLenum param)
                : gl_set_enabled(param, false)
                {};
            };
            
            // glEnableClientState, glDisableClientState
            
            struct gl_set_client_state_enabled : base {
                gl_set_client_state_enabled(GLenum param, bool enable)
                : param(param)
                , saved(scoped::detail::get_gl_bool(param))
                { set(param); };
                gl_set_client_state_enabled(gl_set_client_state_enabled &&rhs)
                : base(std::move(rhs))
                , param(rhs.param)
                {};
                ~gl_set_client_state_enabled()
                { if(!is_moved) set(saved); };
                
            private:
                inline void set(bool is_enabled) const {
                    if(is_enabled) glEnableClientState(param);
                    else glDisableClientState(param);
                }
                const GLenum param;
                GLboolean saved;
            };

            struct gl_enable_client_state : gl_set_client_state_enabled {
                gl_enable_client_state(GLenum param)
                : gl_set_client_state_enabled(param, true)
                {};
            };
            
            struct gl_disable_client_state : gl_set_client_state_enabled {
                gl_disable_client_state(GLenum param)
                : gl_set_client_state_enabled(param, false)
                {};
            };

            // glNewList, glEndlist
            
            struct custom : base {
                custom(const std::function<void()> &dst)
                : dst(dst)
                {};
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
                const std::function<void()> cns{[]{}};
                const std::function<void()> dst{[]{}};
            };
        }; // end of namespace scoped
        
        namespace tag {
            namespace detail {
                template <typename ... types>
                using last_of_t = decltype(std::get<sizeof...(types) - 1>(std::declval<std::tuple<types ...>>()));
                
                template <typename ...>
                struct holder {};
                
                template <typename testee, typename holder, typename ... types>
                struct is_constructible_without_last_one_arg_impl;
                
                template <typename testee, typename type, typename ... types>
                struct is_constructible_without_last_one_arg_impl<testee, holder<types ...>, type>
                : std::is_constructible<testee, types ...>
                {};
                
                template <typename testee, typename type, typename type2, typename ... others, typename ... types>
                struct is_constructible_without_last_one_arg_impl<testee, holder<types ...>, type, type2, others ...>
                : is_constructible_without_last_one_arg_impl<testee, holder<types ..., type>, type2, others ...>
                {};
                
                template <typename testee, typename ... types>
                using is_constructible_without_last_one_arg = is_constructible_without_last_one_arg_impl<testee, holder<>, types ...>;
                
                template <std::size_t ... indices>
                struct index_sequence {};
                
                template <std::size_t n, std::size_t ... indices>
                struct index_sequence_helper
                    : index_sequence_helper<n - 1U, n - 1U, indices ...>
                {};
                
                template <std::size_t ... indices>
                struct index_sequence_helper<0U, indices ... >
                { using type = index_sequence<indices ... >; };
                
                template <std::size_t n>
                using make_index_sequence = typename index_sequence_helper<n>::type;
                
                template <typename type, typename ... arguments, std::size_t ... indices>
                inline type create_without_last_arg_impl(std::tuple<arguments ...> && t,
                                                         index_sequence<indices ...> &&)
                {
                    return type(std::get<indices>(t) ...);
                }
                
                template <typename type>
                auto ref(type &v) -> decltype(std::ref(v)) { return std::ref(v); };
                template <typename type>
                auto ref(type &&v) -> type { return std::move(v); };

                template <typename type, typename ... arguments>
                inline type create_without_last_arg(arguments && ... args)
                {
                    return create_without_last_arg_impl<type>(std::make_tuple(ref(std::forward<arguments>(args)) ...),
                                                        make_index_sequence<sizeof...(arguments)  - 1>());
                }
                
                template <typename ... types>
                last_of_t<types ...> get_last(types && ... args) {
                    return std::move(std::get<sizeof...(types) - 1>(std::make_tuple(ref(std::forward<types>(args)) ...)));
                }
            };

            template <typename type, typename creator = void>
            struct base {
                using scoped_type = type;
                static constexpr bool is_tag{true};
                
                scoped_type operator()() const
                { return {}; };
                inline void run(std::function<void()> body) const
                { creator::create_scope().run(body); };
                inline void operator()(std::function<void()> body) const
                { run(body); };
            };

            template <typename type>
            struct base<type, void> {
                using scoped_type = type;
                static constexpr bool is_tag{true};
                
                scoped_type operator()() const
                { return {}; };
                
                void operator()(std::function<void()> body) const
                { type().run(body); };
                
                template <typename ... types>
                inline auto run(types && ... args) const
                    -> typename std::enable_if<
                        tag::detail::is_constructible_without_last_one_arg<type, types ...>::value
                        && std::is_same<
                            decltype(std::declval<tag::detail::last_of_t<types ...>>()()),
                            void
                        >::value
                    >::type
                {
                    tag::detail::create_without_last_arg<type>(std::forward<types>(args) ...)
                    .run(tag::detail::get_last(std::forward<types>(args) ...));
                };
                
                template <typename ... types>
                void operator()(types && ... args) const
                { run(std::forward<types>(args) ...); };
            };

            template <typename type>
            struct no_arg : base<type> {
                using base<type>::operator();
            };
            
            using matrix = no_arg<scoped::matrix>;
            using style = no_arg<scoped::style>;
            using view = no_arg<scoped::view>;
            
            struct shape : base<scoped::shape> {
                using base<scoped::shape>::operator();
                inline scoped_type operator()(bool with_close = false) const
                { return { with_close }; };
                
                template <bool is_closed>
                struct mode_t : base<scoped_type, mode_t<is_closed>> {
                    static scoped_type create_scope()
                    { return { is_closed }; };
                };
                const mode_t<false> open{};
                const mode_t<true> closed{};
            };
            
            struct save_screen_as_pdf : base<scoped::save_screen_as_pdf> {
                using base<scoped::save_screen_as_pdf>::operator();
                inline scoped_type operator()(const std::string &path) const
                { return { path }; };
            };
            
            struct save_screen_as_svg : base<scoped::save_screen_as_svg> {
                using base<scoped::save_screen_as_svg>::operator();
                inline scoped_type operator()(const std::string &path) const
                { return { path }; };
            };
            
            template <typename type>
            struct bool_parameter : base<type> {
                using base<type>::operator();
                using scoped_type = typename base<type>::scoped_type;
                inline scoped_type operator()(bool enable) const
                { return { enable }; };
                
                template <bool is_enabled>
                struct mode_t : base<scoped_type, mode_t<is_enabled>> {
                    static scoped_type create_scope()
                    { return { is_enabled }; };
                };
                const mode_t<true> enable{};
                const mode_t<false> disable{};
            };
            
            using depth_test    = bool_parameter<scoped::depth_test>;
            using anti_aliasing = bool_parameter<scoped::anti_aliasing>;
            using point_sprites = bool_parameter<scoped::point_sprites>;
            using arb_tex       = bool_parameter<scoped::arb_tex>;
            using smoothing     = bool_parameter<scoped::smoothing>;
            using lighting      = bool_parameter<scoped::lighting>;

            struct rect_mode : base<scoped::rect_mode> {
                using base<scoped::rect_mode>::operator();
                inline scoped_type operator()(ofRectMode mode) const
                { return { mode }; };

                template <ofRectMode mode>
                struct mode_t : base<scoped::rect_mode, mode_t<mode>> {
                    static scoped_type create_scope()
                    { return { mode }; };
                };
                const mode_t<OF_RECTMODE_CORNER> corner{};
                const mode_t<OF_RECTMODE_CENTER> center{};
            };
            
            struct blend_mode : base<scoped::blend_mode> {
                using base<scoped::blend_mode>::operator();
                inline scoped_type operator()(ofBlendMode mode) const
                { return { mode }; };
                
                template <ofBlendMode mode>
                struct mode_t : base<scoped_type, mode_t<mode>> {
                    static scoped_type create_scope()
                    { return { mode }; };
                };
                const mode_t<OF_BLENDMODE_DISABLED> disable{};
                const mode_t<OF_BLENDMODE_ALPHA> alpha{};
                const mode_t<OF_BLENDMODE_ADD> add{};
                const mode_t<OF_BLENDMODE_SUBTRACT> subtract{};
                const mode_t<OF_BLENDMODE_MULTIPLY> multiply{};
                const mode_t<OF_BLENDMODE_SCREEN> screen{};
                
                struct alpha_blending : base<scoped::blend_mode> {
                    using scoped_type = scoped::blend_mode;
                    inline scoped_type operator()() const
                    { return {}; };
                    
                    // why...?
                    template <typename bool_t, typename = typename std::enable_if<std::is_same<bool_t, bool>::value>::type>
                    inline scoped_type operator()(bool_t enable) const
                    { return { enable ? OF_BLENDMODE_ALPHA : OF_BLENDMODE_DISABLED }; };
                    
                    inline void operator()(const std::function<void()> &body) const
                    { return scoped_type().run(body); };
                    inline void operator()(bool enable,
                                           const std::function<void()> &body) const
                    { return scoped_type(enable ? OF_BLENDMODE_ALPHA : OF_BLENDMODE_DISABLED).run(body); };

                    template <ofBlendMode mode>
                    struct mode_t : base<scoped_type, mode_t<mode>> {
                        static scoped_type create_scope()
                        { return { mode }; };
                        inline void operator()(const std::function<void()> &body) const
                        { return scoped_type(mode).run(body); };
                    };
                    const mode_t<OF_BLENDMODE_ALPHA> enable{};
                    const mode_t<OF_BLENDMODE_DISABLED> disable{};
                };
            };
            
            struct fill_mode : base<scoped::fill_mode> {
                using base<scoped::fill_mode>::operator();
                inline scoped_type operator()(ofFillFlag flag) const
                { return { flag }; };
                
                template <ofFillFlag flag>
                struct mode_t : base<scoped_type, mode_t<flag>> {
                    static scoped_type create_scope()
                    { return { flag }; };
                };
                const mode_t<OF_OUTLINE> noFill{};
                const mode_t<OF_FILLED> fill{};
            };
            
            struct line_width : base<scoped::line_width> {
                using base<scoped::line_width>::operator();
                inline scoped_type operator()(float width) const
                { return { width }; };
            };
            
            namespace detail {
                template <typename type>
                struct int_resolution : base<type> {
                    using base<type>::operator();
                    using scoped_type = typename base<type>::scoped_type;
                    inline scoped_type operator()(int resolution) const
                    { return { resolution }; };
                };
                template <typename type>
                struct vec2f_resolution : base<type> {
                    using base<type>::operator();
                    using scoped_type = typename base<type>::scoped_type;
                    inline scoped_type operator()(int resolution) const
                    { return { ofVec2f(resolution, resolution) }; };
                    inline scoped_type operator()(int resolution_x, int resolution_y) const
                    { return { ofVec2f(resolution_x, resolution_y) }; };
                    inline scoped_type operator()(ofVec2f resolution) const
                    { return { resolution }; };
                };
                template <typename type>
                struct vec3f_resolution : base<type> {
                    using base<type>::operator();
                    using scoped_type = typename base<type>::scoped_type;
                    inline scoped_type operator()(int resolution) const
                    { return { ofVec3f(resolution, resolution, resolution) }; };
                    inline scoped_type operator()(int resolution_x,
                                                  int resolution_y,
                                                  int resolution_z) const
                    { return { ofVec3f(resolution_x, resolution_y, resolution_z) }; };
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
                using base<scoped::begin<type>>::operator();
                using scoped_type = typename base<scoped::begin<type>>::scoped_type;
                inline scoped_type operator()(type &v) const
                { return { v }; };
            };
            
            struct beginable {
                template <typename type>
                inline scoped::begin<type> operator()(type &v) const
                { return { v }; };
                
                template <typename type>
                inline void operator()(type &v, std::function<void()> body) const
                { scoped::begin<type>(v).run(body); };
            };
            
            struct gl_begin : base<scoped::gl_begin> {
                using base<scoped::gl_begin>::operator();
                template <typename gl_enum, typename = typename std::enable_if<std::is_integral<gl_enum>::value>::type>
                inline scoped_type operator()(gl_enum param) const
                { return { static_cast<GLenum>(param) }; };
            };
            
            struct gl_set_enabled : base<scoped::gl_set_enabled> {
                using base<scoped::gl_set_enabled>::operator();
                template <typename gl_enum, typename = typename std::enable_if<std::is_integral<gl_enum>::value>::type>
                inline scoped_type operator()(gl_enum param, bool is_enabled) const
                { return { static_cast<GLenum>(param), is_enabled }; };
            };
            
            struct gl_enable : base<scoped::gl_enable> {
                using base<scoped::gl_enable>::operator();
                template <typename gl_enum, typename = typename std::enable_if<std::is_integral<gl_enum>::value>::type>
                inline scoped_type operator()(gl_enum param) const
                { return { static_cast<GLenum>(param) }; };
            };

            struct gl_disable : base<scoped::gl_disable> {
                using base<scoped::gl_disable>::operator();
                template <typename gl_enum, typename = typename std::enable_if<std::is_integral<gl_enum>::value>::type>
                inline scoped_type operator()(gl_enum param) const
                { return { static_cast<GLenum>(param) }; };
            };
            
            // glEnableClientState, glDisableClientState
            
            struct gl_set_client_state_enabled : base<scoped::gl_set_client_state_enabled> {
                using base<scoped::gl_set_client_state_enabled>::operator();
                template <typename gl_enum, typename = typename std::enable_if<std::is_integral<gl_enum>::value>::type>
                inline scoped_type operator()(gl_enum param, bool is_enabled) const
                { return { static_cast<GLenum>(param), is_enabled }; };
            };
            
            struct gl_enable_client_state : base<scoped::gl_enable_client_state> {
                using base<scoped::gl_enable_client_state>::operator();
                template <typename gl_enum, typename = typename std::enable_if<std::is_integral<gl_enum>::value>::type>
                inline scoped_type operator()(gl_enum param) const
                { return { static_cast<GLenum>(param) }; };
            };
            
            struct gl_disable_client_state : base<scoped::gl_disable_client_state> {
                using base<scoped::gl_disable_client_state>::operator();
                template <typename gl_enum, typename = typename std::enable_if<std::is_integral<gl_enum>::value>::type>
                inline scoped_type operator()(gl_enum param) const
                { return { static_cast<GLenum>(param) }; };
            };

            // glNewList, glEndlist

            template <typename type>
            struct bind : base<scoped::bind<type>> {
                using base<scoped::bind<type>>::operator();
                using scoped_type = typename base<scoped::bind<type>>::scoped_type;
                scoped_type operator()(type &v) const
                { return { v }; };
            };
            
            struct bindable {
                template <typename type>
                inline scoped::bind<type> operator()(type &v) const
                { return { v }; };
            };
            
            struct custom : base<scoped::custom> {
                scoped_type operator()(const std::function<void()> &dst) const
                { return { dst }; };
                scoped_type operator()(const std::function<void()> &cns,
                                       const std::function<void()> &dst) const
                { return { cns, dst }; };
            };
        };
        
        namespace {
            constexpr tag::matrix matrix{};
            constexpr tag::style style{};
            constexpr tag::view view{};
            
            constexpr tag::shape shape{};
            constexpr tag::shape::mode_t<false> openShape{};
            constexpr tag::shape::mode_t<true> closedShape{};

            constexpr tag::save_screen_as_pdf saveScreenAsPDF{};
            constexpr tag::save_screen_as_svg saveScreenAsSVG{};
            
            constexpr tag::depth_test depthTest{};
            constexpr tag::depth_test::mode_t<true> enableDepthTest{};
            constexpr tag::depth_test::mode_t<false> disableDepthTest{};
            
            constexpr tag::anti_aliasing antiAliasing{};
            constexpr tag::anti_aliasing::mode_t<true> enableAntiAliasing{};
            constexpr tag::anti_aliasing::mode_t<false> disableAntiAliasing{};
            
            constexpr tag::point_sprites pointSprites{};
            constexpr tag::point_sprites::mode_t<true> enablePointSprites{};
            constexpr tag::point_sprites::mode_t<false> disablePointSprites{};
            
            constexpr tag::arb_tex usingArbTex{};
            constexpr tag::arb_tex::mode_t<true> enableArbTex{};
            constexpr tag::arb_tex::mode_t<false> disableArbTex{};
            
            constexpr tag::smoothing smoothing{};
            constexpr tag::smoothing::mode_t<true> enableSmoothing{};
            constexpr tag::smoothing::mode_t<false> disableSmoothing{};
            
            constexpr tag::lighting lighting{};
            constexpr tag::lighting::mode_t<true> enableLighting{};
            constexpr tag::lighting::mode_t<false> disableLighting{};
            
            constexpr tag::rect_mode rectMode{};
            constexpr tag::rect_mode::mode_t<OF_RECTMODE_CORNER> rectModeCorner{};
            constexpr tag::rect_mode::mode_t<OF_RECTMODE_CENTER> rectModeCenter{};
            
            constexpr tag::blend_mode blendMode{};
            constexpr tag::blend_mode::mode_t<OF_BLENDMODE_ALPHA> blendModeAlpha{};
            constexpr tag::blend_mode::mode_t<OF_BLENDMODE_ADD> blendModeAdd{};
            constexpr tag::blend_mode::mode_t<OF_BLENDMODE_SUBTRACT> blendModeSubtract{};
            constexpr tag::blend_mode::mode_t<OF_BLENDMODE_MULTIPLY> blendModeMultiply{};
            constexpr tag::blend_mode::mode_t<OF_BLENDMODE_SCREEN> blendModeScreen{};
            constexpr tag::blend_mode::alpha_blending alphaBlending{};
            constexpr tag::blend_mode::alpha_blending::mode_t<OF_BLENDMODE_ALPHA> enableAlphaBlending{};
            constexpr tag::blend_mode::alpha_blending::mode_t<OF_BLENDMODE_DISABLED> disableAlphaBlending{};
            
            constexpr tag::fill_mode fillMode{};
            constexpr tag::fill_mode::mode_t<OF_OUTLINE> noFill{};
            constexpr tag::fill_mode::mode_t<OF_FILLED> fill{};
#if 10 <= OF_VERSION_MINOR
            // matrix_mode
            // poly_mode
#endif
            constexpr tag::line_width lineWidth{};
            
            constexpr tag::circle_resolution circleResolution{};
            constexpr tag::curve_resolution curveResolution{};
            constexpr tag::sphere_resolution sphereResolution{};
            constexpr tag::ico_sphere_resolution icoSphereResolution{};
            
            constexpr tag::box_resolution boxResolution{};
            constexpr tag::cone_resolution coneResolution{};
            constexpr tag::cylinder_resolution cylinderResolution{};
            
            constexpr tag::plane_resolution planeResolution{};
            
            constexpr tag::beginable begin{};
            constexpr tag::begin<ofFbo> fbo{};
            constexpr tag::begin<ofCamera> camera{};
            constexpr tag::begin<ofShader> shader{};
            constexpr tag::begin<ofMaterial> material{};
            
            constexpr tag::gl_begin glBegin{};
            
            constexpr tag::gl_set_enabled glSetEnabled{};
            constexpr tag::gl_enable glEnable{};
            constexpr tag::gl_disable glDisable{};
            
            constexpr tag::gl_set_client_state_enabled glSetClientStateEnabled{};
            constexpr tag::gl_enable_client_state glEnableClientState{};
            constexpr tag::gl_disable_client_state glDisableClientState{};

            constexpr tag::bindable bind{};
            constexpr tag::bind<ofTexture> bindTex{};
            
            constexpr tag::custom custom{};
        };
        
        namespace {
            template <typename type>
            inline auto create_scope(type &&rhs)
                -> typename std::enable_if<type::is_scoped_struct, type>::type
            { return std::move(rhs); };
            
            template <typename tag_type>
            inline auto create_scope(const tag_type &tag)
                -> typename std::enable_if<
                    tag_type::is_tag && !scope::detail::has_create_scope<tag_type>::value,
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
                scope::detail::has_begin_end<type>::value,
                scoped::begin<type>
            >::type
            { return {v}; };
            
            // begin / end
            template <typename type>
            inline auto create_scope(type &v)
            -> typename std::enable_if<
                !scope::detail::has_begin_end<type>::value
                && scope::detail::has_bind_unbind<type>::value,
                scoped::bind<type>
            >::type
            { return {v}; };
            
            // begin / end
            template <typename type>
            inline auto create_scope(type &v)
            -> typename std::enable_if<
                !scope::detail::has_begin_end<type>::value
                && !scope::detail::has_bind_unbind<type>::value
                && !scope::detail::has_getTexture<type>::value,
                scoped::bind<type>
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
using ofxScopedStyle = ofxScope::scoped::style;
using ofxScopedView = ofxScope::scoped::style;
using ofxScopedShape = ofxScope::scoped::style;

using ofxScopedDepthTest = ofxScope::scoped::depth_test;
using ofxScopedAntiAliasing = ofxScope::scoped::anti_aliasing;
using ofxScopedPointSprites = ofxScope::scoped::point_sprites;
using ofxScopedUsingArbTex = ofxScope::scoped::arb_tex;
using ofxScopedSmoothing = ofxScope::scoped::smoothing;
using ofxScopedLighting = ofxScope::scoped::lighting;

using ofxScopedRectMode = ofxScope::scoped::rect_mode;
using ofxScopedBlendMode = ofxScope::scoped::blend_mode;
using ofxScopedFillMode = ofxScope::scoped::fill_mode;

using ofxScopedLineWidth = ofxScope::scoped::line_width;

using ofxScopedCircleResolution = ofxScope::scoped::circle_resolution;
using ofxScopedCurveResolution = ofxScope::scoped::curve_resolution;
using ofxScopedSphereResolution = ofxScope::scoped::sphere_resolution;
using ofxScopedIcoSphereResolution = ofxScope::scoped::ico_sphere_resolution;

using ofxScopedBoxResolution = ofxScope::scoped::box_resolution;
using ofxScopedConeResolution = ofxScope::scoped::cone_resolution;
using ofxScopedCylinderResolution = ofxScope::scoped::cylinder_resolution;

using ofxScopedPlaneResolution = ofxScope::scoped::plane_resolution;

using ofxScopedFbo = ofxScope::scoped::begin<ofFbo>;
using ofxScopedCamera = ofxScope::scoped::begin<ofCamera>;
using ofxScopedShader = ofxScope::scoped::begin<ofShader>;
using ofxScopedMaterial = ofxScope::scoped::begin<ofBaseMaterial>;

using ofxScopedVbo = ofxScope::scoped::bind<ofVbo>;

using ofxScopedBeginFbo = ofxScope::scoped::begin<ofFbo>;
using ofxScopedBeginCamera = ofxScope::scoped::begin<ofCamera>;
using ofxScopedBeginShader = ofxScope::scoped::begin<ofShader>;
using ofxScopedBeginMaterial = ofxScope::scoped::begin<ofBaseMaterial>;

using ofxScopedGLBegin = ofxScope::scoped::gl_begin;

using ofxScopedGLSetEnabled = ofxScope::scoped::gl_set_enabled;
using ofxScopedGLEnable = ofxScope::scoped::gl_enable;
using ofxScopedGLDisable = ofxScope::scoped::gl_disable;

using ofxScopedGLSetClientStateEnabled = ofxScope::scoped::gl_set_client_state_enabled;
using ofxScopedGLEnableClientState = ofxScope::scoped::gl_enable_client_state;
using ofxScopedGLDisableClientState = ofxScope::scoped::gl_disable_client_state;

using ofxScopedBindTexture = ofxScope::scoped::bind<ofTexture>;
using ofxScopedBindFbo = ofxScope::scoped::bind<ofFbo>;
using ofxScopedBindVbo = ofxScope::scoped::bind<ofVbo>;
using ofxScopedBindImage = ofxScope::scoped::bind<ofImage>;
using ofxScopedBindVideo = ofxScope::scoped::bind<ofBaseVideoDraws>;

using ofxScoped = ofxScope::scoped::custom;

template <typename ... types>
auto ofxCreateScope(types && ...vs)
    -> ofx::scope::holder<decltype(ofx::scope::create_scope(std::forward<types>(vs))) ...>
{ return { std::make_tuple(ofx::scope::create_scope(std::forward<types>(vs)) ...) }; };

