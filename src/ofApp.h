#pragma once

#include "ofMain.h"
#include "ofTrueTypeFont.h"
#include "ofSoundPlayer.h"
#include "ofImage.h"
#include "myBeat.h"

// information of side menubar
#define MENU_WIDTH      100.0
#define MENU_HEIGHT     50.0
#define MENU_OFFSET     10.0
#define PEDAL_NUM       5
#define MENU_NUM        PEDAL_NUM + 3

// default directories
#define FONTS_DIRECTORY     "fonts/"
#define SAMPLES_DIRECTORY   "samples/"
#define BEATFALL_DIRECTORY  "beatfall/"


class ofApp : public ofBaseApp{

	public:
        string title = "Amazing BeatFall!";
        string myMenuName[MENU_NUM] = {
            "Kick", "Snare", "Hat", "Ride", "Crash", "Path", "Generator", "Trash"
        };
        int BPM = 120;
        float fps;
        float volume = 0.5;
    
    
        //---------------------------------------------------------//
        //----------------  vectors for each object     -----------//
        //---------------------------------------------------------//
        // ball array with dynamic memory allocation
        vector<Generator> generator;
        vector<ofColor> genColor;
        vector<Pedal> pedal;
        vector<Pedal> path;
    
        //---------------------------------------------------------//
        //----------------   my array setting      ----------------//
        //----- Index         -------------------------------------//
        //----- 0: kick       -------------------------------------//
        //----- 1: snare      -------------------------------------//
        //----- 2: hat        -------------------------------------//
        //----- 3: ride       -------------------------------------//
        //----- 4: crash      -------------------------------------//
        //----- 5: path       -------------------------------------//
        //----- 6: generator  -------------------------------------//
        //----- 7: trash      -------------------------------------//
        //---------------------------------------------------------//
        // e.g.  myColor[2] == ofColor of Hat pedal         -------//
        //      myPlayer[4] == sound sample of Crash Pedal  -------//
        //---------------------------------------------------------//
    
    
    
        //---------------------------------------------------------//
        //----------------  Color variables         ---------------//
        //---------------------------------------------------------//
        ofColor myBackgroundColor;
        ofColor myColor[MENU_NUM];
        
        
        //---------------------------------------------------------//
        //----------------  Sound variables         ---------------//
        //---------------------------------------------------------//
        ofSoundPlayer myPlayer[PEDAL_NUM];
        string soundFileName[PEDAL_NUM];
    
    
        //---------------------------------------------------------//
        //----------------  Font variables         ----------------//
        //---------------------------------------------------------//
        ofTrueTypeFont myfont;
        ofTrueTypeFont alertFont;
    
        //---------------------------------------------------------//
        //------------     ofRecntangle variables         ---------//
        //---------------------------------------------------------//
        ofRectangle myMenuRect[MENU_NUM];
        ofRectangle statusRect;
        ofRectangle alertRect;
        ofRectangle commandRect;
        
        //---------------------------------------------------------//
        //------------     Image variables             ------------//
        //---------------------------------------------------------//
        ofImage TrashCan;
        
    
        //---------------------------------------------------------//
        //------------     String Format            ---------------//
        //---------------------------------------------------------//
        // status monitor
        string alert = "Welcome to Amazing BeatFall!";
        string status = "";
        string command =
            "[Space] Play/Pause  [Q] Quit  [S] Save  [L] Load\n[UP/DOWN] Volume  [< >] BPM  [C] Clear";
    
        // .abf file format
        string BPMSetupLabel =
            "----------------- BPM --------------------------";
        string volumeSetupLabel =
            "----------------- Volume Setup -----------------";
        string pedalSetupLabel =
            "----------------- Pedal Setup ------------------";
        string pathSetupLabel =
            "---------------- Path Setup --------------------";
        string genSetupLabel =
            "---------------- Generator Setup ---------------";
        string pedalLocationLabel =
            "---------------- Pedal Location ----------------";
        string pathLocationLabel =
            "---------------- Path Location -----------------";
        string genLocationLabel =
            "---------------- Generator Location ------------";
        string colorLabel = "color";
        string soundLabel = "sound";
        string locationLabel = "location";
    
    
        //---------------------------------------------------------//
        //-----------------       Flags             ---------------//
        //---------------------------------------------------------//
        // start_flag
        // true: Beatballs are getting start falling down.
        // false: Beatball are getting stop falling down
        bool start_flag = false;
    
        // create_flag
        // true: user-selected object has been successfully created.
        // false: creation has been cancelled.
        bool create_flag = false;
    
        // edit_flag
        // true: user editted current status.
        // false: nothing is editted since file is opened.
        bool edit_flag = false;
    
    
    
        //---------------------------------------------------------//
        //-----------------       File Save         ---------------//
        //---------------------------------------------------------//
        // infoSaved
        // string text data indicating current .abf file
        string infoSaved;
        
        // set infoSaved & return result
        string setInfoString();
    
        // save as .abf file format
        bool save();
        void processSaveFile(ofFileDialogResult sresult);
    
    
        //---------------------------------------------------------//
        //-----------------       File Load         ---------------//
        //---------------------------------------------------------//
    
        // load .abf file
        bool load(std::string fileName = "");
        
        // file open
        // return opened file name
        // return empty string if failed
        string openFile();          // return opened filed name
    
    
        //---------------------------------------------------------//
        //----------       User-Defined Functions         ---------//
        //---------------------------------------------------------//
		void setup();
		void update();
		void draw();
        void playSoundSample(Pedal _pedal);
        ofColor getPedalColor(Pedal _pedal);
    
        // clear all object (pedal, path, generator)
        // by clear() ing all vector
        void clearall();
    
        //---------------------------------------------------------//
        //----------    Keyboard Control Function         ---------//
        //---------------------------------------------------------//
		void keyReleased(int key);
    
        //---------------------------------------------------------//
        //----------    Mouse Control Function          -----------//
        //---------------------------------------------------------//
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
        
    
    
        //---------------------------------------------------------//
        //----------       not used           ---------------------//
        //---------------------------------------------------------//
        void keyPressed(int key);
        void mouseMoved(int x, int y );
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
};
