#include "ofxScope.h"
#include "ofMain.h"

class ofApp : public ofBaseApp {
    ofEasyCam cam;
    ofFbo fbo;
public:
    void setup() {
        ofBackground(0);
        cam.setPosition(0, 0, ofGetHeight() * 0.5f);
        cam.lookAt(glm::vec3());
        ofxScope::disableArbTex([this] {
            fbo.allocate(ofGetWidth(), ofGetHeight());
        });
        ofSetColor(255);
    }
    void update() {}
    void draw() {
        ofxScope::matrix([] {
            ofTranslate(ofGetWidth() * 0.5f, ofGetHeight() * 0.8f);
            ofRotateDeg(45);
            ofDrawRectangle(0, 0, 50, 50);
        });
        ofxScope::camera(cam, [] {
            ofDrawBox(0, 0, 0, 80);
        });
        ofxCreateScope(ofxScope::matrix,
                       ofxScope::rectMode.center,
                       fbo,
                       cam).run([] {
            ofClear(1.0f, 0.0f, 0.0f);
            ofDrawRectangle(0, 0, 80, 80);
            float h = ofGetHeight();
            float w = ofGetWidth();
            for(int i = 0; i < 100; ++i) {
                float x = ofMap(i, 0, 100, -0.5f * w, 0.5f * w);
                ofDrawLine(x, -0.5f * h, x, 0.5f * h);
            }
        });
        fbo.draw(10, 10, fbo.getWidth() * 0.2f, fbo.getHeight() * 0.2f);
        ofxCreateScope(ofxScope::style,
                       ofxScope::bindTex(fbo.getTexture())).run([] {
            ofSetColor(255, 0, 0);
            ofDrawIcoSphere(ofGetWidth() * 0.5f, 100, 80);
        });
    }
    void exit() {}
    
    void keyPressed(int key) {}
    void keyReleased(int key) {}
    void mouseMoved(int x, int y ) {}
    void mouseDragged(int x, int y, int button) {}
    void mousePressed(int x, int y, int button) {}
    void mouseReleased(int x, int y, int button) {}
    void mouseEntered(int x, int y) {}
    void mouseExited(int x, int y) {}
    void windowResized(int w, int h) {}
    void dragEvent(ofDragInfo dragInfo) {}
    void gotMessage(ofMessage msg) {}
};

int main() {
    ofGLFWWindowSettings setting;
#if 10 <= OF_VERSION_MINOR
    setting.setSize(1280, 720);
#else
    setting.width = 1280;
    setting.height = 720;
#endif
    auto window = ofCreateWindow(setting);
    auto app = std::make_shared<ofApp>();
    ofRunApp(window, app);
    ofRunMainLoop();
}
