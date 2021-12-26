//
//  myBeat.cpp
//  BeatFall
//
//  Created by Jiho Rhee on 20/06/2020.
//

#include "myBeat.h"

//----------------------------------------------------------------//
//----------------------       class myObj         ---------------//
//----------------------------------------------------------------//

myObj::myObj(float _x, float _y) {
    x = _x; y = _y;
}


//----------------------------------------------------------------//
//---------------------       class Pedal         ----------------//
//----------------------------------------------------------------//

// Constructor
// Create Pedal at (_x, _y) with degree and pedal id
// (default: (0, 0, 0, Kick))
Pedal::Pedal(float _x, float _y, float _degree, _pedalType pid) : myObj(_x, _y) {
    float rad = _degree * PI / 180;
    rotate_deg = _degree;
    xinit = x - cos(rad) * length/2;
    yinit = y - sin(rad) * length/2;
    xfinish = x + cos(rad) * length/2;
    yfinish = y + sin(rad) * length/2;
    pedalID = pid;
}

// set position at (_x, _y) with degree _degree and pedalID pid
void Pedal::set(float _x, float _y, float _degree) {
    x = _x; y = _y;
    rotate_deg = _degree;
    float rad = _degree * PI / 180;
    xinit = x - cos(rad) * length/2;
    yinit = y - sin(rad) * length/2;
    xfinish = x + cos(rad) * length/2;
    yfinish = y + sin(rad) * length/2;
}
void Pedal::set(float _x, float _y, float _degree, _pedalType pid) {
    x = _x; y = _y;
    rotate_deg = _degree;
    float rad = _degree * PI / 180;
    xinit = x - cos(rad) * length/2;
    yinit = y - sin(rad) * length/2;
    xfinish = x + cos(rad) * length/2;
    yfinish = y + sin(rad) * length/2;
    pedalID = pid;
}

// when BeatBall hits the pedal
// visual collision effect appears
void Pedal::flash() {
    static float unit_offset = 5.0;
    if(hit_flag) {
        flash_offset = MAX_FLASH_OFFSET;
        hit_flag = false;
    }
    flash_offset -= unit_offset;
    if(flash_offset <= 0) {
        flash_offset = 0;
    }
}

// return pedal slope in radian
float Pedal::getRadian() {
    return rotate_deg * PI / 180;
}


//----------------------------------------------------------------//
//------------------        class BeatBall       -----------------//
//----------------------------------------------------------------//

// Constructor
// Create BeatBall at (_x, _y) (default: (0, 0))
BeatBall::BeatBall(float _x, float _y) : myObj(_x, _y){
}

// set position at (_x, _y)
void BeatBall::set(float _x, float _y) { 
    x = _x; y = _y;
}

// compute collision between BeatBall & pedal (of path)
void BeatBall::collide(vector<Pedal>* ptr_obj) {
    // check collision with prt_obj
    for(int i = 0; i < ptr_obj->size(); i++) {
        Pedal _pedal = (*ptr_obj)[i];
        float distance = getDistance(_pedal);
        float collisionDist = BALL_RADIUS;
        
        // collision occur
        if(distance < collisionDist) {
            // collide with pedal
            // radian angles between each obj & x-axis
            float angle = _pedal.rotate_deg * PI / 180;
            float incidence = atan2(vy, vx);
            float reflection = -(incidence - angle) + angle;
            float velocity = SPRING * sqrt(vx*vx + vy*vy);
            float y0 = getProjectionY(_pedal);
            
            float initDist = sqrt((x - _pedal.xinit) * (x - _pedal.xinit) + (y - _pedal.yinit) * (y - _pedal.yinit));
            float finishDist = sqrt((x - _pedal.xfinish) * (x - _pedal.xfinish) + (y - _pedal.yfinish) * (y - _pedal.yfinish));
            
            // collide at the tip of pedal
            // angle will be changed about +-PI/2
            // left tip
            if(initDist < collisionDist) {
                if(angle >= 0) {
                    //if(x < _pedal.xinit && y < _pedal.yinit) {
                        if(angle - PI/2 < incidence && incidence < angle + PI/2) {
                            angle -= PI/2;
                            reflection = -(incidence - angle) + angle;
                        }
                    //}
                }
                else if(angle < 0) {
                    //if(x < _pedal.xinit && y > _pedal.yinit) {
                        if(angle - PI/2 < incidence && incidence < angle + PI/2) {
                            angle += PI/2;
                            reflection = -(incidence - angle) + angle;
                        }
                    //}
                }
            }
            // right tip
            else if(finishDist < collisionDist) {
                if(angle >= 0) {
                    //if(x > _pedal.xfinish - collisionDist && y > _pedal.yfinish - collisionDist) {
                        if(incidence < angle - PI/2 || angle + PI/2 < incidence) {
                            angle -= PI/2;
                            reflection = -(incidence - angle) + angle;
                        }
                    //}
                }
                else if(angle < 0) {
                    //if(x > _pedal.xfinish - collisionDist && y < _pedal.yfinish + collisionDist) {
                        if(incidence < angle - PI/2 || angle + PI/2 < incidence) {
                            angle += PI/2;
                            reflection = -(incidence - angle) + angle;
                        }
                    //}
                }
            }
            
            // colliion on wide-side
            x += collisionDist * cos(reflection);
            y += collisionDist * sin(reflection);
            ax = 0;
            ay = GRAVITY;
            vx = velocity * cos(reflection);
            vy = velocity * sin(reflection);
            
            (*ptr_obj)[i].hit_flag = true;
        }
    }
}

// compute movement of BeatBall
void BeatBall::move() { 
    vx += ax; vy += ay;
    x += vx; y += vy;
}

// compute y coord
// of shadow of BeatBall on pedal (or path)
float BeatBall::getProjectionY(Pedal _pedal) {
    float y0 = (_pedal.yfinish - _pedal.yinit) / (_pedal.xfinish - _pedal.xinit) * (x - _pedal.xinit) + _pedal.yinit;
    return y0;
}

// compute perpendicular distance
// between BeatBall & Pedal (or path)
float BeatBall::getDistance(Pedal _pedal) { 
    float distance;
    // get the linear equation of _pedal
    // y0 = y-pos of vertical projection of ball to _pedal
    float y0 = getProjectionY(_pedal);
    // angle = radian of _pedal
    float angle = _pedal.rotate_deg * PI / 180;
    
    
    // orthogonal distance from ball to _pedal
    if(_pedal.xinit <= x && x <= _pedal.xfinish) {
        distance = fabsf((y - y0) * cos(angle));
    }
    else {
        float initDist = sqrt((x - _pedal.xinit) * (x - _pedal.xinit) + (y - _pedal.yinit) *(y - _pedal.yinit));
        float finishDist = sqrt((x - _pedal.xfinish) * (x - _pedal.xfinish) + (y - _pedal.yfinish) *(y - _pedal.yfinish));
        distance = min(initDist, finishDist);
    }
    
    
    return distance;
}

//----------------------------------------------------------------//
//------------------      class Generator        -----------------//
//----------------------------------------------------------------//

// Constructor
// Create Generator at (_x, _y) (default: (0, 0))
Generator::Generator(float _x, float _y) : myObj(_x, _y) {
    ball.resize(MAX_BALL_NUM);
}

// set position at (_x, _y)
void Generator::set(float _x, float _y) {
    x = _x; y = _y;
}

// Generate & Launch idx-th BeatBall
// at the point of generator
void Generator::genBall(int idx) {
    ball[idx].set(x, y);
    ball[idx].vx = 0; ball[idx].vy = 0;
    ball[idx].ax = 0; ball[idx].ay = GRAVITY;
    ball[idx].active_flag = true;
}
