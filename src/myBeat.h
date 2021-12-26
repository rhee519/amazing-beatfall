//
//  myBeat.h
//  BeatFall
//
//  Created by Jiho Rhee on 20/06/2020.
//

#ifndef myBeat_h
#define myBeat_h

#include <ctime>
#include <vector>
#include <sstream>
#include "ofVec2f.h"

// max ball # of each Generator
#define MAX_BALL_NUM        50

// MACRO CONSTANTS
#define BOX_SIZE            30.0        // size of generator
#define PATH_WIDTH          5.0         // thickness of path
#define PEDAL_WIDTH         100.0       // length of pedal & path
#define PEDAL_HEIGHT        30.0        // thickness of pedal

#define CLICK_BOUND         PEDAL_HEIGHT / 2        // click check boundary
#define MAX_ROTATE_DEGREE   75.0        // max degree of pedal rotation
#define BALL_RADIUS         5.0         // radius of BeatBall
#define GRAVITY             0.07        // gravity constant
#define SPRING              0.9         // 1.0 : maximum spring constant

#define MAX_FLASH_OFFSET    100         // bounce of pedal


using namespace std;

class myObj {
public:
    float x; float y;
    bool move_flag = false;
    myObj(float _x, float _y);
};

//---------------------------------------------------------//
//----------------      Pedal & Path       ----------------//
//---------------------------------------------------------//
// <class myObj> + <x,y init & finish> + <flash function>  //
//---------------------------------------------------------//
class Pedal : public myObj{
public:
    // pedal id
    // used for index of array
    enum _pedalType {
        Kick = 0,
        Snare = 1,
        Hat = 2,
        Ride = 3,
        Crash = 4
    };
    _pedalType pedalID = Kick;
    
    
    // coord of both tips
    // will construct a line (xinit, yinit) ~ (xfinish, yfinish)
    float xinit;
    float yinit;
    float xfinish;
    float yfinish;
    float length = PEDAL_WIDTH;
    float rotate_deg = 0;       // -75' ~ +75' (degree)

    
    // Constructor
    // Create Pedal at (_x, _y) with degree and pedal id
    // (default: (0, 0, 0, Kick))
    Pedal(float _x = 0, float _y = 0, float _degree = 0, _pedalType pid = Kick);
    
    // set position at (_x, _y) with degree _degree and pedalID pid
    void set(float _x, float _y, float _degree);
    void set(float _x, float _y, float _degree, _pedalType pid);
    
    // Flags
    bool isClickedCenter = false;       // is clicked to move?
    bool isClickedLeft = false;         // is clicked to rotate? (left)
    bool isClickedRight = false;        // is clicked to rotate? (right)
    bool rotate_flag = false;           // is rotated?
    bool hit_flag = false;              // did collided with BeatBall?
    
    // when hit_flag is set, flash_offset will be set to MAX_FLASH_OFFSET
    // for visual collision effect
    float flash_offset = 0;
    
    // return pedal slope in radian
    float getRadian();
    
    // when BeatBall hits the pedal
    // visual collision effect appears
    void flash();
};

//---------------------------------------------------------//
//----------------        BeatBall          ---------------//
//---------------------------------------------------------//
// <class myObj> + velocity, accel + <collide function>    //
//---------------------------------------------------------//
class BeatBall : public myObj {
public:
    // velocity x, y
    float vx = 0;
    float vy = 0;
    // acceleration x, y
    float ax = 0;
    float ay = GRAVITY;
    
    // check if active
    // true: appears in screen
    // false: does not appears in screen
    bool active_flag = false;
    
    
    // Constructor
    // Create BeatBall at (_x, _y) (default: (0, 0))
    BeatBall(float _x = 0, float _y = 0);
    
    // set position at (_x, _y)
    void set(float _x, float _y);
    
    // compute collision between BeatBall & pedal (of path)
    void collide(vector<Pedal>* ptr_obj);
    
    // compute movement of BeatBall
    void move();
    
    // compute y coord
    // of shadow of BeatBall on pedal (or path)
    float getProjectionY(Pedal _pedal);
    
    // compute perpendicular distance
    // between BeatBall & Pedal (or path)
    float getDistance(Pedal _pedal);
};

//---------------------------------------------------------//
//----------------        Generator          --------------//
//---------------------------------------------------------//
// <class myObj> + <vector<BeatBall> + <generate function> //
//---------------------------------------------------------//
class Generator : public myObj {
public:
    // each Generator has own BeatBall vector
    // and operate
    vector<BeatBall> ball;
    
    // check if clicked (to move)
    bool isClicked = false;
    
    // Constructor
    // Create Generator at (_x, _y) (default: (0, 0))
    Generator(float _x, float _y);
    
    // set position at (_x, _y)
    void set(float _x, float _y);
    
    // Generate & Launch idx-th BeatBall
    // at the point of generator
    void genBall(int idx);
};

#endif /* myBeat_h */
