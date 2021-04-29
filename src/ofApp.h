#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxMaxim.h"
#include "ofMath.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void drawPopArt(ofVideoGrabber webcam);

		// webcam used
		ofVideoGrabber webcam;
		int camWidth, camHeight, width, height, boxNum, boxNumAcross, boxNumDown;
		float radius;

		ofxPanel panel;
		ofxFloatSlider radSlide;
		ofxToggle briTog;


		/* audio stuff */
		void audioOut(ofSoundBuffer& output) override; //output method
		void audioIn(ofSoundBuffer& input) override; //input method
		ofSoundStream soundStream;
		int sampleRate;

		maxiOsc oscillator1;
		maxiOsc oscillator2;
		maxiOsc oscillator3;
		maxiOsc oscillator4;
		maxiOsc oscillator5;

		maxiSample mySample;

		//This is the Maximilian FFT
		maxiFFT myFFT;
		std::vector<float> currentFreqs; //this will hold calculated freqs

		maxiMFCCAnalyser<float> myMFCC;
		float mfccs[13];


		// For drawing

		// GUI
		ofParameter<float> frequency;
		ofParameter<float> amplitude;

		float boxes[1000][1000];

		ofVec2f freqBox[10];
		float f[10];

		maxiOsc osc1, osc2, osc3, osc4, osc5, osc6, osc7, osc8, osc9, osc10;
		
		maxiFFT fft;
		convert converter;
		int fftSize;
		double binFreq;



};
