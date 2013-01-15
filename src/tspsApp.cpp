#include "tspsApp.h"


//--------------------------------------------------------------
tspsApp::tspsApp( int numCameras, string host, int port, string channel ){
    wsHost      = host;
    wsPort      = port;
    wsChannel   = channel;
    
    for (int i=0; i<numCameras; i++){
        delegates.push_back(new CustomDelegate(i));
    }
}

//--------------------------------------------------------------
void tspsApp::setup(){
	ofSetFrameRate(60);
	ofBackground(223, 212, 190);
	
	for (int i=0; i<delegates.size(); i++){
        delegates[i]->setup();
        if ( wsHost != "" && wsPort != 0 ){
            delegates[i]->getPeopleTracker()->setupWebSocketClient(wsHost, wsPort, false, wsChannel);
        }
    }
    
    // which delegate is getting drawn
    currentDelegate = 0;
    if ( delegates.size() > 0 ){
        delegates[currentDelegate]->enableEvents();
    }
    
    // add buttons for switching between cameras
    ofRectangle dimensions = ofRectangle( 105, 10, 50, buttonHeight);
    
    for ( int i=0; i<delegates.size(); i++){
        string name = "C:"+ofToString( i+1 );
        guiTypeButton * btn = new guiTypeButton();
        btn->setup( name, dimensions.width, dimensions.height );
        btn->setPosition( dimensions.x, dimensions.y );
        btn->setBackgroundSelectColor(0,168,156);
        btn->setBackgroundColor(0,84,78);
        ofAddListener(btn->buttonPressed, this, &tspsApp::onButtonPressed );
        buttons.insert( pair<string, guiTypeButton*>( name, btn ) );
        
        if ( i == currentDelegate ){
            btn->setSelected();
        } else {
            btn->setNormal();
        }
        
        dimensions.x += 10 + dimensions.width;
    }
}

//--------------------------------------------------------------
void tspsApp::update(){
	for (int i=0; i<delegates.size(); i++){
        delegates[i]->update();
    }
}

//--------------------------------------------------------------
void tspsApp::draw(){
    delegates[currentDelegate]->draw();
    
    // draw custom buttons
    map<std::string, guiTypeButton*>::iterator it;
    for( it=buttons.begin(); it!=buttons.end(); it++ ){
        it->second->render();
    }
}

//--------------------------------------------------------------
void tspsApp::exit(){
    delegateMutex.lock();
	for (int i=0; i<delegates.size(); i++){
        delegates[i]->disableEvents();
        delete delegates[i];
    }
    delegateMutex.unlock();
    delegates.clear();
}

//--------------------------------------------------------------
void tspsApp::mouseReleased(int x, int y, int button){
    // check hit of buttons
    map<std::string, guiTypeButton*>::iterator it;
    for( it=buttons.begin(); it!=buttons.end(); it++ ){
        it->second->checkHit( x, y, button );
    }
}

//--------------------------------------------------------------
void tspsApp::onButtonPressed( string & button ){
    for (int i=0; i<delegates.size(); i++){
        string name = "C:"+ofToString( i+1 );
        if ( button == name ){
            // does this delegate exist?
            if ( i < delegates.size() ){
                if ( delegates.size() > 0 ){
                    delegates[currentDelegate]->disableEvents();
                }
                currentDelegate = i;
                if ( delegates.size() > 0 ){
                    delegates[currentDelegate]->enableEvents();
                }
            }
            buttons[ name ]->setSelected();
        } else {
            buttons[ name ]->setNormal();
        }
    }
    
}