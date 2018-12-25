# ofxScope

```cpp
class ofApp : public ofBaseApp {
	ofFbo fbo;
	ofEasyCam cam;
...

public:
	void draw() {
		{
			auto _ = ofxCreateScope(ofxScope::matrix, ofxScope::style);
            // ofPushMatrix();
            // ofPushStyle();
			ofTranslate(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);
			ofScale(2);
			ofSetColor(ofColor::white);
            drawCircles();
            // ofPopStyle();
            // ofPopMatrix();
		}
		
        ofxCreateScope(ofxScope::matrix, ofxScope::style).run([] {
			ofTranslate(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);
			ofScale(2);
			ofSetColor(ofColor::white);
            drawCircles();
        });
        
		{
			auto _ = ofxCreateScope(fbo, cam);
            // fbo.begin();
            // cam.begin();
			drawLines();
            // cam.end();
            // fbo.end();
		}
        
		{
			auto _ = ofxCreateScope(ofxScope::custom([] { ofLogNotice() << "start"; },
                                                     [] { ofLogNotice() << "end"; }));
            // print "start"
            ...
            // print "end"
		}
	}
};
```



### 2018/12/2x ver 0.1.0 release

* remake

### 2016/05/03 ver 0.0.1 release

* initial

## License

MIT License.

## Author

* ISHII 2bit [bufferRenaiss co., ltd.]
* ishii[at]buffer-renaiss.com

## At the last

Please create a new issue if there is a problem.
And please throw a pull request if you have a cool idea!!
