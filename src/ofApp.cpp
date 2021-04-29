#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	width = ofGetWidth();
	height = ofGetHeight();
	/* Video Setup */
	camWidth = 640;  // try to grab at this size.
	camHeight = 480;
	//radius = 10;


	webcam.setDesiredFrameRate(60);
	webcam.setup(camWidth, camHeight);
	ofSetVerticalSync(true);
	ofSetColor(ofColor::white);


	ofSetRectMode(OF_RECTMODE_CENTER);

	//GUI:
	panel.setup();
	panel.add(radSlide.setup("radius", 72, 5, 100));
	panel.add(briTog.setup("brightness / saturation toggle", true, 30, 10));

	radius = radSlide;

	boxNumAcross = ceil(webcam.getWidth() / radius);
	boxNumDown = ceil(webcam.getHeight() / radius);

	boxNum = boxNumAcross * boxNumDown;

	for (int i = 0; i < 10; ++i)
	{
		int x = floor(ofRandom(boxNumAcross));
		int y = floor(ofRandom(boxNumDown));
		freqBox[i] = ofVec2f(x, y);
	}
	drawPopArt(webcam);

	sampleRate = 44100; // Sampling Rate 
	int initialBufferSize = 512;
	ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);

	// fft setup 
	fftSize = 2048;
	int numBins = 512;
	binFreq = sampleRate / fftSize; //The width of each bin in Hertz
	fft.setup(fftSize, numBins, 256);


	// openFrameworks sound
	ofSoundStreamSettings settings;
	settings.setApi(ofSoundDevice::Api::MS_DS); //setting API to Microsoft direct sound

	auto devices = soundStream.getMatchingDevices("default");
	soundStream.printDeviceList();
	if (!devices.empty()) {
		settings.setInDevice(devices[0]);
	}

	//settings.setInListener(this);
	settings.setOutListener(this);
	settings.sampleRate = sampleRate;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 2;
	settings.bufferSize = initialBufferSize;

	soundStream.setup(settings);
}

//--------------------------------------------------------------
void ofApp::update(){
	webcam.update();

	if (radius != radSlide)
	{
		radius = radSlide;
		boxNumAcross = ceil(webcam.getWidth() / radius);
		boxNumDown = ceil(webcam.getHeight() / radius);

		boxNum = boxNumAcross * boxNumDown;

	//	std::cout << boxNum << " = " << boxNumAcross << " * " << boxNumDown << endl;

		for (int i = 0; i < 10; ++i)
		{
			int x = floor(ofRandom(boxNumAcross));
			int y = floor(ofRandom(boxNumDown));
			freqBox[i] = ofVec2f(x, y);
			f[i] = ofMap(boxes[x][y], 0, 255, 110, 660);
		}
	}

	for (int n = 0; n < 10; ++n)
	{
		int x = freqBox[n].x;
		int y = freqBox[n].y;
		f[n] = ofMap(boxes[x][y], 0, 255, 110, 990);
	}


}

//--------------------------------------------------------------
void ofApp::draw(){

	ofPushMatrix();
	//only need to shift width rn since rectmode is center
	ofTranslate(camWidth, 0);
	drawPopArt(webcam);
	//and pop back 
	ofPopMatrix();

	panel.draw();
}

//--------------------------------------------------------------

void ofApp::drawPopArt(ofVideoGrabber webcam) {
	

	ofPushStyle();

	for (int i = 0; i < webcam.getWidth(); i += radius) {
		for (int j = 0; j < webcam.getHeight(); j += radius) {
			//takes the first pixel of the square and gets the colour
			ofColor color = webcam.getPixels().getColor(i, j);
			//set that as our drawing color
			ofSetColor(color);

			float mapL;

			if (briTog)
			{
			float bri = color.getBrightness();
			mapL = ofMap(bri, 0, 255, 6, radius);

			int h = ceil(i / radius);
			int f = ceil(j / radius);

			boxes[h][f] = (float) bri;
			//std::cout << boxes[i][j] << endl;
			}
			else
			{
				float sat = color.getSaturation();
				mapL = ofMap(sat, 0, 255, 6, radius);
				boxes[i][j] = (float) sat;
			}
			//draw a box in each 16px block with a mapped radius/size
			ofDrawRectangle(-i, j, mapL, mapL);
		}
	}

	ofPopStyle();
}

//--------------------------------------------------------------

void ofApp::audioOut(ofSoundBuffer& output)
{
	std::size_t outChannels = output.getNumChannels();

	for (size_t i = 0; i < output.getNumFrames(); ++i)
	{
		//0.25, 0.5, 0.75
		//root = 1, 1.25, 1.5
		//octave = 2, 2.25, 2.5
		double wave = osc1.sinewave(f[1] * 0.25) + osc2.sinewave(f[2] * 0.5) + osc3.sinewave(f[3] * 0.75) +
					osc4.sinewave(f[4]) + osc5.sinewave(f[5] * 1.25) + osc6.sinewave(f[6] * 1.5) +
					osc7.sinewave(f[7] * 2) + osc8.sinewave(f[8] * 2.25) + osc9.sinewave(f[9] * 2.5);

		if (fft.process(wave))
		{
			fft.magsToDB();
		}

		float amp = 0.2;

		//left
		output[i * outChannels] = wave * amp;
		//right
		output[i * outChannels + 1] = wave * amp;
	} 
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer& input) {
	std::size_t nChannels = input.getNumChannels();
	for (size_t i = 0; i < input.getNumFrames(); ++i)
	{
		myFFT.process(input[i * 2]);
		myFFT.magsToDB();
		myMFCC.mfcc(myFFT.magnitudes, mfccs);

		//Hold the values so the draw method can draw them

		/*
		left[i]        = input[i*2]*0.5;
		right[i]    = input[i*2+1]*0.5;

		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
		 */
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 's') {
		soundStream.start();
	}

	if (key == 'e') {
		soundStream.stop();
	}

	if (key == 'p') {
		for (int i = 0; i < 10; ++i)
		{
			int x = freqBox[i].x;
			int y = freqBox[i].y;
			float preVal = boxes[x][y];
			std::cout << preVal << " to " << f[i] << endl;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
