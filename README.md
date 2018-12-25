# ofxScope

## Example

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
            ...
            // ofPopStyle();
            // ofPopMatrix();
		}
		
        ofxCreateScope(ofxScope::matrix, ofxScope::style).run([] {
            ...
        });
        {
            auto _ = ofxScope::cam(cam);
            // cam.begin();
            ...
            // cam.end();
        }
		{
			auto _ = ofxCreateScope(fbo, cam);
            // fbo.begin();
            // cam.begin();
            ...
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



## Type

* `matrix`
* `style`
* `view`

* `shape`
  * bool: close
* `saveScreenAsPDF`
  * string: path
    * `ofxScope::saveScreenAsPDF('./capture/foo.pdf')`
* `saveScreenAsSVG`
  * string: path
* `depthTest`
  * bool: enable
* `antiAliasing `
  * bool: enable
* `pointSprites`
  * bool: enable
* `usingArbTex`
  * bool: enable
* `smoothing`
  * bool: enable
* `lighting`
  * bool: enable
* `rectMode`
  * ofRectMode: mode
* `blendMode`
  * ofBlendMode: mode
* `fillMode`
  * ofFillFlag: mode
* `lineWidth` 
  * float: width
* `circleResolution`
  * int
* `curveResolution`
  * int
* `sphereResolution`
  * int
* `icoSphereResolution`
  * int
* `boxResolution`
  * ofVec3f
  * int
  * int, int, int
* `coneResolution`
  * ofVec3f
  * int
  * int, int, int
* `cylinderResolution`
  * ofVec3f
  * int
  * int, int, int
* `planeResolution`
  * ofVec2f
  * int
  * int, int
* `begin` 
  * object has `begin()`/`end()`
* `fbo`
  * ofFbo
* `camera`
  * ofCamera
* `shader`
  * ofShader
* `material`
  * ofMaterial
* `bind`
  * object has `bind()`/`unbind()`
* `bindTex`
  * ofTexture
* `custom`
  * `function<void()>`, `function<void()>`

## Update History

### 2018/12/25 ver 0.1.0 release

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