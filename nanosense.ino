
#include "Mic.h"
#include <arduinoFFT.h>
#include "Classifier.h"    // Need to get training data and generate classifier first

// tune these constants to your needs
#define SAMPLES 64
#define SOUND_THRESHOLD 50

int INFERENCE = 0;

using namespace Eloquent::ML::Port;  
SVM clf;
arduinoFFT fft;

double features[SAMPLES];  // double required for FFT 
Mic mic;

void setup() {
    Serial.begin(115200);
    PDM.onReceive(onAudio);// the PDM library is asynchronous, so we need a callback
    mic.begin();
}

void loop() {
    record();
    delay(500); 
}

void onAudio() { //PDM callback to update mic object
    mic.update(); 
}

void record() {  // Read given number of samples from mic
    if (mic.hasData() && mic.data() > SOUND_THRESHOLD) { 
        for (int i = 0; i < SAMPLES; i++) {
            while (!mic.hasData())
                delay(1);
            features[i] = mic.pop();//* GAIN ;
        }
        fft.Windowing(features, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
        if(INFERENCE ==1){ //Need to have trained a classifier for this to work
          Serial.print(clf.predictLabel(features));   
        }
        else
        {
          printFeatures();
        }
        return;
    }
    memset(features, 0, sizeof(features)); //reset features to blank
    return;
}

void printFeatures() {
    int total = 0;
    for (int i = 0; i < SAMPLES; i++ ){
      total += features[i];      
    }
    if(total > 100){
      const uint16_t numFeatures = sizeof(features) / sizeof(double);
      for (int i = 0; i < numFeatures; i++) {
            Serial.print(features[i], 4);
            Serial.print(i == numFeatures - 1 ? '\n' : ',');
      }      
    }
}
