#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //------------------------------------------------------------//
    //-------------------    Background Setup  -------------------//
    //------------------------------------------------------------//
    ofSetWindowTitle(title);
    ofSetFrameRate(60);
    ofSetBackgroundAuto(false);
    myBackgroundColor = ofColor(255, 243, 232);
    ofSetBackgroundColor(myBackgroundColor);
    ofFill();
    
    //------------------------------------------------------------//
    //---------------------   Font Setup    ----------------------//
    //------------------------------------------------------------//
    // setup font
    int fontsize = 15;
    myfont.load((string)FONTS_DIRECTORY+"TheFridayStroke.otf", fontsize);
    fontsize = 12;
    alertFont.load((string)FONTS_DIRECTORY+"VertigoFLF-Bold.ttf", fontsize);
    
    //------------------------------------------------------------//
    //---------------------    Image Setup    --------------------//
    //------------------------------------------------------------//
    // setup image
    TrashCan.load("TrashCanIcon.jpg");
    
    //------------------------------------------------------------//
    //------------------    Side Menubar Setup  ------------------//
    //------------------------------------------------------------//
    // setup menu
    for(int i = 0; i < MENU_NUM; i++) {
        myMenuRect[i].set(ofGetWidth() - MENU_OFFSET - MENU_WIDTH, (i+1)*MENU_HEIGHT + MENU_OFFSET, MENU_WIDTH, MENU_HEIGHT);
    }
    myMenuRect[MENU_NUM-1].setHeight(2*MENU_HEIGHT);    // trash menu
    
    //------------------------------------------------------------//
    //------------------    Status Monitor Setup  ----------------//
    //------------------------------------------------------------//
    // Program Status
    alertRect.set(MENU_OFFSET, ofGetHeight() - MENU_OFFSET - 20, 330, 20);
    statusRect.set(MENU_OFFSET, alertRect.y - MENU_OFFSET - 50, 60, 50);
    commandRect.set(MENU_OFFSET + statusRect.width + MENU_OFFSET, statusRect.y, alertRect.width - statusRect.width - MENU_OFFSET, statusRect.height);
    
    
    //------------------------------------------------------------//
    //------------------    Initiate Program   -------------------//
    //------------------------------------------------------------//
    // load init file
    load("init.abf");
    
    //------------------------------------------------------------//
    //---------------------    Color Setup    --------------------//
    //------------------------------------------------------------//
    // setup colors
    for(int i = 0; i < MENU_NUM; i++) {
        myColor[i] = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
    }
    
    //------------------------------------------------------------//
    //---------------------    Sound Setup    --------------------//
    //------------------------------------------------------------//
    // setup sounds
    for(int i = 0; i < PEDAL_NUM; i++) {
        // randSampleID = 1 ~ 10
        int randSampleID = (int)ofRandom(255)%10 + 1;
        string randIDstr = to_string(randSampleID);
        soundFileName[i] = myMenuName[i]+randIDstr+".wav";
        myPlayer[i].load(SAMPLES_DIRECTORY+soundFileName[i]);
    }
    
    alert = "Welcome to Amazing BeatFall!!!";
    cout << alert << endl;
}



//--------------------------------------------------------------
void ofApp::update(){
    // update() is called 60 times every second.
    // that is, update() is called every 1/60 second.
    fps = ofGetFrameRate();
    
    // update Pedal info
    static int idx = 0;
    static float clock = 0;
    float period = 4 * 60.0 / BPM;       // length of a single bar (seconds)
    float unit_time = 1.0 / 60;     // unit_time = 1/60 second
    clock += unit_time;

    if(clock >= period) {
        clock = 0;
        if(start_flag) {
            for(int i = 0; i < generator.size(); i++) {
                generator[i].genBall(idx);
            }
            idx++;
            idx %= MAX_BALL_NUM;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(myBackgroundColor);
    
    //------------------------------------------------------------//
    //--------------    Side menubar & Status Monitor ------------//
    //--------------            Outline         ------------------//
    //------------------------------------------------------------//
    // myMenu(side menubar) Frame
    ofSetColor(ofColor::black);
    ofSetLineWidth(3);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofNoFill();
    for(int i = 0; i < MENU_NUM; i++) {
        ofDrawRectangle(myMenuRect[i]);
    }
    // status monitor frame
    ofSetLineWidth(1.5);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofDrawRectangle(statusRect);
    ofDrawRectangle(alertRect);
    ofDrawRectangle(commandRect);
    ofFill();
    
    // side menubar display
    // (Kick, Snare, ..., Crash)
    ofSetLineWidth(7.5);
    for(int i = 0; i < PEDAL_NUM; i++) {
        ofSetColor(myColor[i]);
        ofDrawLine(myMenuRect[i].x + MENU_OFFSET, myMenuRect[i].y + MENU_HEIGHT/2, ofGetWidth() - 2*MENU_OFFSET, myMenuRect[i].y + MENU_HEIGHT/2);
        ofSetColor(ofColor::black);
        ofDrawCircle(myMenuRect[i].x + MENU_OFFSET, myMenuRect[i].y + MENU_HEIGHT/2, 2);
        ofDrawCircle(ofGetWidth() - 2*MENU_OFFSET, myMenuRect[i].y + MENU_HEIGHT/2, 2);
    }
    // (Path)
    ofSetLineWidth(5);
    ofSetColor(myColor[PEDAL_NUM]);
    ofDrawLine(myMenuRect[PEDAL_NUM].x + MENU_OFFSET, myMenuRect[PEDAL_NUM].y + MENU_OFFSET, ofGetWidth() - 2*MENU_OFFSET, myMenuRect[PEDAL_NUM+1].y - MENU_OFFSET);
    // (Generator)
    ofSetColor(myColor[PEDAL_NUM+1], 128);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofDrawRectangle(myMenuRect[PEDAL_NUM+1].x + MENU_WIDTH/2, myMenuRect[PEDAL_NUM+1].y + 0.4 * MENU_HEIGHT, 20, 20);
    
    // (Delete)
    ofSetColor(ofGetBackgroundColor());
    TrashCan.draw(myMenuRect[MENU_NUM-1].x + MENU_WIDTH/2, myMenuRect[MENU_NUM-1].y + MENU_HEIGHT, 75, 75);
    
    
    
    //------------------------------------------------------------//
    //---------------------    Text            -------------------//
    //------------------------------------------------------------//
    // side menubar text
    ofSetColor(ofColor::black);
    for(int i = 0; i < MENU_NUM - 1; i++) {
        myfont.drawString(myMenuName[i], myMenuRect[i].x + MENU_OFFSET/2, myMenuRect[i+1].y - MENU_OFFSET/2);
    }
    myfont.drawString(myMenuName[MENU_NUM-1], myMenuRect[MENU_NUM-1].x + MENU_OFFSET/2, myMenuRect[MENU_NUM-1].y + 2*MENU_HEIGHT - MENU_OFFSET/2);
    
    // status monitor text
    stringstream ss;
    status = "fps: ";
    ss << std::fixed << std::setprecision(2) << fps;
    status += ss.str() + "\nBPM: " + to_string(BPM);
    alertFont.drawString(status, statusRect.x + 3, statusRect.y + statusRect.height/2 - 3);
    alertFont.drawString(alert, alertRect.x + 3, alertRect.y + alertRect.height - 3);
    alertFont.drawString(command, commandRect.x + 3, commandRect.y + commandRect.height/2 - 3);
    
    
    //------------------------------------------------------------//
    //---------------------       my objects   -------------------//
    //------------------------------------------------------------//
    
    // draw generator box & balls
    ofSetRectMode(OF_RECTMODE_CENTER);
    for(int i = 0; i < generator.size(); i++) {
        ofSetColor(genColor[i], 128);
        ofDrawRectangle(generator[i].x, generator[i].y, BOX_SIZE, BOX_SIZE);
        
        // draw BeatBalls of each generator
        ofSetColor(genColor[i]);
        for(int j = 0; j < MAX_BALL_NUM; j++) {
            // ball is active
            if(generator[i].ball[j].active_flag) {
                generator[i].ball[j].collide(&pedal);
                generator[i].ball[j].collide(&path);
                generator[i].ball[j].move();
                ofDrawCircle(generator[i].ball[j].x, generator[i].ball[j].y, BALL_RADIUS);
                
                // out of screen
                // deactivate
                if(generator[i].ball[j].x < 0 || generator[i].ball[j].x > ofGetWidth() || generator[i].ball[j].y > ofGetHeight()) {
                    generator[i].ball[j].active_flag = false;
                }
            }
        }
    }
    
    // draw Pedals
    ofSetLineWidth(PEDAL_HEIGHT);
    for(int i = 0; i < pedal.size(); i++) {
        if(pedal[i].hit_flag) playSoundSample(pedal[i]);
        pedal[i].flash();
        float offset = pedal[i].flash_offset/8;
        float angle = pedal[i].getRadian();
        float xinitOff = pedal[i].xinit - offset * cos(angle);
        float yinitOff = pedal[i].yinit - offset * sin(angle);
        float xfinishOff = pedal[i].xfinish + offset * cos(angle);
        float yfinishOff = pedal[i].yfinish + offset * sin(angle);
        
        ofSetLineWidth(PEDAL_HEIGHT + offset);
        ofSetColor(getPedalColor(pedal[i]), 255 - pedal[i].flash_offset);
        ofDrawLine(xinitOff, yinitOff, xfinishOff, yfinishOff);
        ofSetColor(ofColor::black);
        ofDrawCircle(xinitOff, yinitOff, 2);
        ofDrawCircle(xfinishOff, yfinishOff, 2);
    }
    
    // draw Paths
    ofSetLineWidth(PATH_WIDTH);
    ofSetColor(myColor[PEDAL_NUM]);
    for(int i = 0; i < path.size(); i++) {
        ofDrawLine(path[i].xinit, path[i].yinit, path[i].xfinish, path[i].yfinish);
    }
}

void ofApp::clearall() {
    pedal.clear();
    path.clear();
    generator.clear();
    genColor.clear();
    alert = "All Object has been cleared.";
    cout << alert << endl;
}



void ofApp::playSoundSample(Pedal _pedal) {
    myPlayer[(int)_pedal.pedalID].setVolume(volume);
    myPlayer[(int)_pedal.pedalID].play();
}

ofColor ofApp::getPedalColor(Pedal _pedal) {
    return myColor[(int)_pedal.pedalID];
}


//---------------------------------------------------------//
//----------    Keyboard Control Function         ---------//
//---------------------------------------------------------//
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    bool alert_flag = false;
    
    //-----     SPACE BAR       -----//
    // play & stop BeatFall
    if(key == ' ') {
        alert_flag = true;
        if(start_flag) {
            start_flag = false;
            alert = "Pause";
        }
        else {
            start_flag = true;
            alert = "Start!";
        }
    }
    
    //-----     C key           -----//
    // clear all object
    if(key == 'c' || key == 'C') {
        alert_flag = true;
        clearall();
    }
    
    //-----     Q key           -----//
    // quit program
    if(key == 'q' || key == 'Q') {
        alert_flag = true;
        for(int i = 0; i < PEDAL_NUM; i++) {
            myPlayer[i].unload();
        }
        alert = "Good Bye!";
        ofSystemAlertDialog("Quit <Amazing BeatFall!>. Bye!");
        ofExit();
    }
    
    //-----     S key           -----//
    // save current BeatFall status as .abf file format
    if(key == 's' || key == 'S') {
        if(edit_flag){
            save();
            edit_flag = false;
        }
    }
    
    //-----     L key           -----//
    // open .abf File
    if(key == 'l' || key == 'L') {
        string fileName = openFile();
        if(fileName.empty()) {
            alert = "File select cancelled.";
        } else {
            clearall();
            if(load(fileName)) {
                alert = "\"" + fileName + "\"" + " has been loaded successfully.";
            }
            else alert = "[ERROR] File load failure!";
        }
    }
    
    //----- direction key       -----//
    // BPM adjustment
    if(key == OF_KEY_LEFT) {
        alert_flag = true;
        if(BPM > 90) {
            BPM -= 2;
            alert = "BPM set to : " + to_string(BPM);
            edit_flag = true;
        }
        else {
            BPM = 90;
            alert = "BPM " + to_string(BPM) + " is the minimum.";
        }
    }
    if(key == OF_KEY_RIGHT) {
        alert_flag = true;
        if(BPM < 180) {
            BPM += 2;
            alert = "BPM set to : " + to_string(BPM);
            edit_flag = true;
        }
        else {
            BPM = 180;
            alert = "BPM " + to_string(BPM) + " is the maximum.";
        }
    }
    // Volume adjustment
    if(key == OF_KEY_UP) {
        alert_flag = true;
        volume += 0.05;
        alert = "Volume set to : " + to_string((int)nearbyint(volume*100));
        edit_flag = true;
        if(volume > 1.0) {
            volume = 1.0;
            alert = "Volume 100 is the maximum.";
        }
    }
    if(key == OF_KEY_DOWN) {
        alert_flag = true;
        volume -= 0.05;
        alert = "Volume set to : " + to_string((int)nearbyint(volume*100));
        edit_flag = true;
        if(volume < 0.0) {
            volume = 0.0;
            alert = "Volume 0 is the minimum.";
        }
    }
    
    cout << alert << endl;
}




//---------------------------------------------------------//
//----------    Mouse Control Function          -----------//
//---------------------------------------------------------//
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    // left click dragged
    if(button == OF_MOUSE_BUTTON_LEFT) {
        // pedal
        for(int i = 0; i < pedal.size(); i++) {
            // move by drag & drop
            if(pedal[i].isClickedCenter) {
                // (x, y) will be changed
                pedal[i].set(x, y, pedal[i].rotate_deg);
                pedal[i].move_flag = true;
                //pedal[i].collide();
                break;
            }
            // rotate by drag
            else if(pedal[i].isClickedLeft) {
                // rotate_deg will be changed
                float angle = atan2(pedal[i].y - y, pedal[i].x - x);
                float deg = ofRadToDeg(angle);
                if(deg >= MAX_ROTATE_DEGREE) deg = MAX_ROTATE_DEGREE;
                else if(deg <= -MAX_ROTATE_DEGREE) deg = -MAX_ROTATE_DEGREE;
                
                pedal[i].set(pedal[i].x, pedal[i].y, deg);
                pedal[i].rotate_flag = true;
                break;
            }
            else if(pedal[i].isClickedRight) {
                // rotate_deg will be changed
                float angle = atan2(y - pedal[i].y, x - pedal[i].x);
                float deg = ofRadToDeg(angle);
                if(deg >= MAX_ROTATE_DEGREE) deg = MAX_ROTATE_DEGREE;
                else if(deg <= -MAX_ROTATE_DEGREE) deg = -MAX_ROTATE_DEGREE;
                
                pedal[i].set(pedal[i].x, pedal[i].y, deg);
                pedal[i].rotate_flag = true;
                break;
            }
        }
        
        // path
        for(int i = 0; i < path.size(); i++) {
            // move by drag & drop
            if(path[i].isClickedCenter) {
                // (x, y) will be changed
                path[i].set(x, y, path[i].rotate_deg);
                path[i].move_flag = true;
                //path[i].collide();
                break;
            }
            // rotate by drag
            else if(path[i].isClickedLeft) {
                // rotate_deg will be changed
                float angle = atan2(path[i].y - y, path[i].x - x);
                float deg = ofRadToDeg(angle);
                if(deg >= MAX_ROTATE_DEGREE) deg = MAX_ROTATE_DEGREE;
                else if(deg <= -MAX_ROTATE_DEGREE) deg = -MAX_ROTATE_DEGREE;
                
                path[i].set(path[i].x, path[i].y, deg);
                path[i].rotate_flag = true;
                break;
            }
            else if(path[i].isClickedRight) {
                // rotate_deg will be changed
                float angle = atan2(y - path[i].y, x - path[i].x);
                float deg = ofRadToDeg(angle);
                if(deg >= MAX_ROTATE_DEGREE) deg = MAX_ROTATE_DEGREE;
                else if(deg <= -MAX_ROTATE_DEGREE) deg = -MAX_ROTATE_DEGREE;
                
                path[i].set(path[i].x, path[i].y, deg);
                path[i].rotate_flag = true;
                break;
            }
        }
        
        // generator
        for(int i = 0; i < generator.size(); i++) {
            if(generator[i].isClicked) {
                generator[i].set(x, y);
                generator[i].move_flag = true;
                break;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    // left click pressed
    if(button == OF_MOUSE_BUTTON_LEFT) {
        //----------------      move or delete      ---------------//
        // Pedal
        for(int i = 0; i < pedal.size(); i++) {
            // click center : move
            if(fabsf(x - pedal[i].x) < CLICK_BOUND && fabsf(y - pedal[i].y) < CLICK_BOUND) {
                pedal[i].isClickedCenter = true;
                return;
            }
            // click side : rotate
            else if(fabsf(x - pedal[i].xinit) < CLICK_BOUND && fabsf(y - pedal[i].yinit) < CLICK_BOUND) {
                pedal[i].isClickedLeft = true;
                return;
            }
            else if(fabsf(x - pedal[i].xfinish) < CLICK_BOUND && fabsf(y - pedal[i].yfinish) < CLICK_BOUND) {
                pedal[i].isClickedRight = true;
                return;
            }
        }
        
        // Path
        for(int i = 0; i < path.size(); i++) {
            // click center : move
            if(fabsf(x - path[i].x) < CLICK_BOUND && fabsf(y - path[i].y) < CLICK_BOUND) {
                path[i].isClickedCenter = true;
                return;
            }
            // click side : rotate
            else if(fabsf(x - path[i].xinit) < CLICK_BOUND && fabsf(y - path[i].yinit) < CLICK_BOUND) {
                path[i].isClickedLeft = true;
                return;
            }
            else if(fabsf(x - path[i].xfinish) < CLICK_BOUND && fabsf(y - path[i].yfinish) < CLICK_BOUND) {
                path[i].isClickedRight = true;
                return;
            }
        }
        
        // Generator
        for(int i = 0; i < generator.size(); i++) {
            // click : move
            if(fabsf(x - generator[i].x) < CLICK_BOUND && fabsf(y - generator[i].y) < CLICK_BOUND) {
                generator[i].isClicked = true;
                return;
            }
        }
        
        //---------------       create new obj      ---------------//
        // new pedal & path & generator
        
        // Is pedal selected?
        for(int i = 0; i < PEDAL_NUM; i++) {
            if(myMenuRect[i].inside(x, y)) {
                // create new pedal
                Pedal tmpPedal(x, y);
                tmpPedal.pedalID = (Pedal::_pedalType)i;
                
                // when created, automatically selected(clicked)
                // to move toward somewhere
                tmpPedal.isClickedCenter = true;
                
                // add to vector
                pedal.push_back(tmpPedal);
                create_flag = true;
            }
        }
        // Is path selected?
        if(myMenuRect[PEDAL_NUM].inside(x, y)) {
            // create new path
            Pedal tmpPath(x, y);
            
            // when created, automatically selected(clicked)
            // to move toward somewhere
            tmpPath.isClickedCenter = true;
            
            // add to vector
            path.push_back(tmpPath);
            create_flag = true;
        } // Is Generator selected?
        else if(myMenuRect[PEDAL_NUM+1].inside(x, y)) {
            // create new generator
            Generator tmpGen(x, y);
            ofColor tmpGenColor(ofRandom(255), ofRandom(255), ofRandom(255));
            
            // when created, automatically selected(clicked)
            // to move toward somewhere
            tmpGen.isClicked = true;
            
            // add to vector
            generator.push_back(tmpGen);
            genColor.push_back(tmpGenColor);
            create_flag = true;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    // which one was selected
    // right before mouse released?
    // 0: Kick, 1: Snare, 2: Hat, 3: Ride, 4: Crash,
    // 5: Path, 6: Generator
    int whichSelected = -1;
    stringstream ss;
    
    // left click released
    if(button == OF_MOUSE_BUTTON_LEFT) {
        // pedal
        for(int i = 0; i < pedal.size(); i++) {
            if(pedal[i].isClickedCenter) {
                pedal[i].isClickedCenter = false;
                if(pedal[i].move_flag) {
                    alert = myMenuName[(int)pedal[i].pedalID] + " pedal moved toward (" + to_string((int)x) + ", " + to_string((int)y) + ").";
                    pedal[i].move_flag = false;
                    edit_flag = true;
                }
                whichSelected = (int)pedal[i].pedalID;
            } else if(pedal[i].isClickedLeft) {
                pedal[i].isClickedLeft = false;
                if(pedal[i].rotate_flag) {
                    alert = myMenuName[(int)pedal[i].pedalID] + " pedal slope has been changed to ";
                    ss << std::fixed << std::setprecision(2) << pedal[i].rotate_deg;
                    alert += ss.str() + ".";
                    pedal[i].rotate_flag = false;
                    edit_flag = true;
                }
            } else if(pedal[i].isClickedRight) {
                pedal[i].isClickedRight = false;
                if(pedal[i].rotate_flag) {
                    alert = myMenuName[(int)pedal[i].pedalID] + " pedal slope has been changed to ";
                    ss << std::fixed << std::setprecision(2) << pedal[i].rotate_deg;
                    alert += ss.str() + ".";
                    pedal[i].rotate_flag = false;
                    edit_flag = true;
                }
            }
            
            // delete
            if(myMenuRect[MENU_NUM-1].inside(pedal[i].x, pedal[i].y)) {
                alert = myMenuName[(int)pedal[i].pedalID] + " pedal has been deleted.";
                pedal.erase(pedal.begin()+i);
                edit_flag = true;
            }
        }
        
        // path
        for(int i = 0; i < path.size(); i++) {
            if(path[i].isClickedCenter) {
                path[i].isClickedCenter = false;
                if(path[i].move_flag) {
                    alert = "Path moved toward (" + to_string((int)x) + ", " + to_string((int)y) + ").";
                    path[i].move_flag = false;
                    edit_flag = true;
                }
                whichSelected = PEDAL_NUM;
            } else if(path[i].isClickedLeft) {
                path[i].isClickedLeft = false;
                if(path[i].rotate_flag) {
                    alert = "Path slope has been changed to ";
                    ss << std::fixed << std::setprecision(2) << path[i].rotate_deg;
                    alert += ss.str() + ".";
                    path[i].rotate_flag = false;
                    edit_flag = true;
                }
            } else if(path[i].isClickedRight) {
                path[i].isClickedRight = false;
                if(path[i].rotate_flag) {
                    alert = "Path slope has been changed to ";
                    ss << std::fixed << std::setprecision(2) << path[i].rotate_deg;
                    alert += ss.str() + ".";
                    path[i].rotate_flag = false;
                    edit_flag = true;
                }
            }
            
            // delete
            if(myMenuRect[MENU_NUM-1].inside(path[i].x, path[i].y)) {
                alert = "Path has been deleted.";
                path.erase(path.begin()+i);
                edit_flag = true;
            }
        }
        
        // generator
        for(int i = 0; i < generator.size(); i++) {
            if(generator[i].isClicked) {
                generator[i].isClicked = false;
                whichSelected = PEDAL_NUM+1;
                if(generator[i].move_flag) {
                    alert = "Generator moved toward (" + to_string((int)x) + ", " + to_string((int)y) + ").";
                    generator[i].move_flag = false;
                    edit_flag = true;
                }
            }
            
            // delete
            if(myMenuRect[MENU_NUM-1].inside(generator[i].x, generator[i].y)) {
                alert = "Generator has been deleted.";
                generator.erase(generator.begin()+i);
                genColor.erase(genColor.begin()+i);
                edit_flag = true;
            }
        }
        
        // select new object & just drop
        // then, new object may not be created
        if(create_flag) {
            // user just dropped new object.
            // then creation wold be cancelled.
            if(myMenuRect[whichSelected].inside(x, y)) {
                // pedal create cancel
                if(0 <= whichSelected && whichSelected < PEDAL_NUM){
                    pedal.pop_back();
                    alert = myMenuName[whichSelected] + " pedal creation cancelled.";
                } // path create cancel
                else if(whichSelected == PEDAL_NUM) {
                    path.pop_back();
                    alert = "Path creation cancelled.";
                } // generator create cancel
                else if(whichSelected == PEDAL_NUM + 1) {
                    generator.pop_back();
                    genColor.pop_back();
                    alert = "Generator creation cancelled.";
                }
            }
            else {
                alert = myMenuName[whichSelected] + " pedal has been created.";
            }
        }
        
        // clear create flag to false
        // ready to select next new object
        create_flag = false;
        cout << alert << endl;
    }
}



//---------------------------------------------------------//
//-----------------       File Save         ---------------//
//---------------------------------------------------------//

// save as .abf file format
bool ofApp::save() {
    ofFileDialogResult sresult = ofSystemSaveDialog(".abf", "Save BeatFall File");
    ofFile newfile(sresult.getPath(), ofFile::WriteOnly);
    ofBuffer buffer;
    infoSaved = setInfoString();
    buffer.set(infoSaved);
    return newfile.writeFromBuffer(buffer);
}

// set infoSaved & return result
std::string ofApp::setInfoString() {
    string myInfo ="";
    string objtype = "";
    std::stringstream ss;
    
    // BPM setup
    myInfo += BPMSetupLabel+"\n" + to_string(BPM)+"\n";
    
    // volume setup
    myInfo += volumeSetupLabel+"\n"+to_string(volume)+"\n";
    // pedal setup
    myInfo += pedalSetupLabel + "\n";
    for(int i = 0; i < PEDAL_NUM; i++) {
        objtype = to_string(i);
        myInfo += objtype + " ";
        myInfo += colorLabel + " ";
        ss.str(std::string());                   // clear string stream
        ss << std::hex << myColor[i].getHex();
        myInfo += "0x" + ss.str() + "\n";
        myInfo += objtype + " ";
        myInfo += soundLabel + " ";
        myInfo += soundFileName[i] + "\n";
    }
    // path setup
    myInfo += pathSetupLabel + "\n";
    objtype = to_string(PEDAL_NUM);
    myInfo += objtype + " ";
    myInfo += colorLabel + " ";
    ss.str(std::string());                   // clear string stream
    ss << std::hex << myColor[PEDAL_NUM].getHex();
    myInfo += "0x" + ss.str() + "\n";
    
    // generator setup
    myInfo += genSetupLabel + "\n";
    for(int i = 0; i < generator.size(); i++) {
        objtype = to_string(PEDAL_NUM+1);
        myInfo += objtype + " ";
        myInfo += colorLabel + " ";
        ss.str(std::string());                   // clear string stream
        ss << std::hex << genColor[i].getHex();
        myInfo += "0x" + ss.str() + "\n";
    }
    
    // pedal location
    myInfo += pedalLocationLabel + "\n";
    for(int i = 0; i < pedal.size(); i++) {
        objtype = to_string(pedal[i].pedalID);
        myInfo += objtype + " ";
        myInfo += locationLabel + " ";
        myInfo += to_string(pedal[i].x) + " " + to_string(pedal[i].y) + " " + to_string(pedal[i].rotate_deg) + "\n";
    }
    
    // path location
    myInfo += pathLocationLabel + "\n";
    objtype = to_string(PEDAL_NUM);
    for(int i = 0; i < path.size(); i++) {
        myInfo += objtype + " ";
        myInfo += locationLabel + " ";
        myInfo += to_string(path[i].x) + " " + to_string(path[i].y) + " " + to_string(path[i].rotate_deg) + "\n";
    }
    
    // generator location
    myInfo += genLocationLabel + "\n";
    objtype = to_string(PEDAL_NUM+1);
    for(int i = 0; i < generator.size(); i++) {
        myInfo += objtype + " ";
        myInfo += locationLabel + " ";
        myInfo += to_string(generator[i].x) + " " + to_string(generator[i].y) + "\n";
    }
    
    
    return myInfo;
}


//---------------------------------------------------------//
//-----------------       File Load         ---------------//
//---------------------------------------------------------//

// file open
// return opened file name
// return empty string if failed
std::string ofApp::openFile() {
    string fileName = "";
    ofFileDialogResult lresult = ofSystemLoadDialog("Select .abf File");
    fileName = lresult.getName();
    
    // if file does not exist
    // then return empty string
    if(!lresult.bSuccess) fileName = "";
    
    return fileName;
}

// load .abf file
bool ofApp::load(std::string fileName) {
    size_t pos;
    
    // if file open success
    if(!fileName.empty()) {
        // Check File Format (.abf)
        pos = fileName.find_last_of(".");
        if(pos != 0 && pos != string::npos && fileName.substr(pos) == ".abf") {
            ofFile loadFile(BEATFALL_DIRECTORY+fileName);
            
            // Check if file exists
            if(!loadFile.exists()) {
                alert = "[ERROR] File does not exists.";
                cout << alert << endl;
                return false;
            } else {
                alert = "File is opened successfully.";
                cout << alert << endl;
            }
            
            ofBuffer buffer(loadFile);
            ofBuffer::Line iter = buffer.getLines().begin(); // iter->BPMSetupLabel
            stringstream ss;
            string mybuffer;
            string soundName;
            int ptype;
            int hexColor;
            float xin, yin, degin;
            
            // BPM setup
            mybuffer = *(++iter);
            BPM = stoi(mybuffer);
            ++iter;
            
            // volume setup
            mybuffer = *(++iter);
            volume = stof(mybuffer);
            ++iter;
            
            // pedal setup
            for(iter++; (*iter) != pathSetupLabel; iter++) {
                mybuffer = (*iter);
                ptype = stoi(mybuffer);
                if(ptype < 0 || PEDAL_NUM <= ptype) {
                    alert = "[ERROR] Wrong file format.";
                    cout << alert << endl;
                    return false;
                }
                pos = mybuffer.find(colorLabel);
                // process color input
                if(pos != string::npos) {
                    pos = mybuffer.find("0x");
                    hexColor = stoi(mybuffer.substr(pos), nullptr, 16);
                    myColor[ptype].setHex(hexColor);
                } // process sound input
                else {
                    pos = mybuffer.find(soundLabel);
                    if(pos == string::npos) {
                        alert = "[ERROR] Wrong file format.";
                        cout << alert << endl;
                        return false;
                    }
                    pos += soundLabel.length() + 1;
                    soundName = mybuffer.substr(pos);
                    soundFileName[ptype] = soundName;
                    myPlayer[ptype].load(SAMPLES_DIRECTORY+soundName);
                }
            }
            
            // path setup
            mybuffer = *(++iter);     // iter->"5 color 0x??????\n"
            ptype = stoi(mybuffer);
            pos = mybuffer.find(colorLabel);
            if(pos == string::npos) {
                alert = "[ERROR] Wrong file format.";
                cout << alert << endl;
                return false;
            }
            pos = mybuffer.find("0x");
            hexColor = stoi(mybuffer.substr(pos), nullptr, 16);
            myColor[ptype].setHex(hexColor);
            ++iter;
            
            // generator setup
            for(++iter; (*iter) != pedalLocationLabel; iter++) {
                mybuffer = (*iter);
                ptype = stoi(mybuffer);
                if(ptype != PEDAL_NUM+1) {
                    alert = "[ERROR] Wrong file format.";
                    cout << alert << endl;
                    return false;
                }
                pos = mybuffer.find(colorLabel);
                // process color input
                if(pos == string::npos) {
                    alert = "[ERROR] Wrong file format.";
                    cout << alert << endl;
                    return false;
                }
                pos = mybuffer.find("0x");
                hexColor = stoi(mybuffer.substr(pos), nullptr, 16);
                ofColor color;
                color.setHex(hexColor);
                genColor.push_back(color);
            }
            
            // pedal location
            for(++iter; (*iter) != pathLocationLabel; iter++) {
                mybuffer = (*iter);
                ptype = stoi(mybuffer);
                if(ptype < 0 || PEDAL_NUM <= ptype) {
                    alert = "[ERROR] Wrong file format.";
                    cout << alert << endl;
                    return false;
                }
                pos = mybuffer.find(locationLabel);
                if(pos == string::npos) {
                    alert = "[ERROR] Wrong file format.";
                    cout << alert << endl;
                    return false;
                }
                pos += locationLabel.length() + 1;
                xin = stof(mybuffer.substr(pos));
                pos = mybuffer.find(' ', pos) + 1;
                yin = stof(mybuffer.substr(pos));
                pos = mybuffer.find(' ', pos) + 1;
                degin = stof(mybuffer.substr(pos));
                Pedal peditem(xin, yin, degin, (Pedal::_pedalType)ptype);
                pedal.push_back(peditem);
            }
            
            // path location
            for(iter++; (*iter) != genLocationLabel; iter++) {
                mybuffer = (*iter);
                ptype = stoi(mybuffer);
                if(ptype != PEDAL_NUM) {
                    alert = "[ERROR] Wrong file format.";
                    cout << alert << endl;
                    return false;
                }
                pos = mybuffer.find(locationLabel);
                if(pos == string::npos) {
                    alert = "[ERROR] Wrong file format.";
                    cout << alert << endl;
                    return false;
                }
                pos += locationLabel.length() + 1;
                xin = stof(mybuffer.substr(pos));
                pos = mybuffer.find(' ', pos) + 1;
                yin = stof(mybuffer.substr(pos));
                pos = mybuffer.find(' ', pos) + 1;
                degin = stof(mybuffer.substr(pos));
                Pedal pathitem(xin, yin, degin);
                path.push_back(pathitem);
            }
            
            // generator location
            for(++iter; iter != buffer.getLines().end(); iter++) {
                mybuffer = (*iter);
                ptype = stoi(mybuffer);
                if(ptype != PEDAL_NUM+1) {
                    alert = "[ERROR] Wrong file format.";
                    cout << alert << endl;
                    return false;
                }
                pos = mybuffer.find(locationLabel);
                if(pos == string::npos) {
                    alert = "[ERROR] Wrong file format.";
                    cout << alert << endl;
                    return false;
                }
                pos += locationLabel.length() + 1;
                xin = stof(mybuffer.substr(pos));
                pos = mybuffer.find(' ', pos);
                yin = stof(mybuffer.substr(pos));
                pos = mybuffer.find(' ', pos);
                degin = stof(mybuffer.substr(pos));
                Generator genitem(xin, yin);
                generator.push_back(genitem);
            }
            
            if(genColor.size() != generator.size()) {
                alert = "[ERROR] Wrong file format.";
                cout << alert << endl;
                return false;
            }
            
            // successfully loaded
            return true;
        }
        
        // file format is not .abf
    }
    
    // fail to file open
    return false;
}




//---------------------------------------------------------//
//----------       not used           ---------------------//
//---------------------------------------------------------//

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
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
