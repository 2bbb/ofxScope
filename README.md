# ofxScope

```
class ofApp : public ofBaseApp {
	ofFbo fbo;

...

public:
	void draw() {
		{
			ofxScopedMatrix _;
			ofxScopedStyle __;
			ofTranslate(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);
			ofScale(2);
			ofSetColor(ofColor::white);						drawCircles();
		}
		
		{
			ofxScope::combine<ofxScope::matrix, ofxScope::style> _;
			ofTranslate(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);
			ofScale(3);
			ofSetColor(ofColor::white);
			drawCircles();
		}
		
		{
			ofxScopedFbo _(fbo);
			drawLines();
		}
	}
};
```

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
