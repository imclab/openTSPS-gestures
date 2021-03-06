#include "ofMain.h"
#include "tspsApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( int argc, char * const argv[] ){
    string  host        = "localhost";
    int     port        = 9987;
    string  channel     = "";
    int     numCameras  = 1;
    int     startIndex  = 0;
    
    // these two should be vectors!
    string  settingsFile = "settings.xml";
    string  uri         = "";
    
    vector<string> args(argv + 1, argv + argc);
    
	// from: http://stackoverflow.com/questions/441547/most-efficient-way-to-process-arguments-from-the-command-line-in-c
	// because no getopt on windows :(
    
    // Loop over command-line args
    for (vector<string>::iterator i = args.begin(); i != args.end(); ++i) {
        if (*i == "-h") {
            host = *++i;
        } else if (*i == "-p") {
            port = ofToInt(*++i);
        } else if (*i == "-i") {
            numCameras = ofToInt(*++i);
        } else if (*i == "-c"){
            channel = *++i;
        } else if (*i == "-w"){
            startIndex = ofToInt(*++i);
        } else if (*i == "-s"){
            settingsFile = *++i;
        } else if (*i == "-u"){
            uri = *++i;
        }
    }
    
    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	ofRunApp( new tspsApp(numCameras, startIndex, settingsFile, host, port, channel, uri) );
}