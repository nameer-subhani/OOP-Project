#include <iostream>
#include <raylib.h>
#include <cmath>
using namespace std;
#ifndef CAR_HPP
#define CAR_HPP

class Car{
    protected:
        string name;
        double speed;
        double upspeed;
        double jumpTime;
        double Xacceleration;
        int BaseMaxSpeed;
        int Yacceleration;
        double x;
        double y;
        int mass;
        double tireTraction;
        int maxSpeed;
        int maxAcceleration;
        static int gunLevel;
        static int tireLevel;
        int jumpspeed;
        bool Grounded; // 1 if car is on the ground, 0 if in the air
    public:
        Car(string n, int mS, int mA, double tT, int m){
            name = n;
            maxSpeed = mS;
            maxAcceleration = mA;
            tireTraction = tT;
            mass = m;
            BaseMaxSpeed = mS;
            jumpspeed = 5;
            jumpTime = 0;
            speed = 0;
            x = 0;
            y = 0;
            upspeed = 0;
            Xacceleration = 0;
            Yacceleration = 10;
            Grounded = 1; // cars start on the ground 
        }
        static void SetGunLevel(int gL){
            gunLevel = gL;
        }
        static void SetTireLevel(int tL){
            tireLevel = tL;
        }
        double GetX(){
            return x;
        }
        double GetY(){
            return y;
        }
        void draw(){
            DrawRectangle((int)x, (int)y, 60, 20, RED);           
            DrawRectangle((int)x+10, (int)y-15, 40, 15, DARKGRAY);
            DrawCircle((int)x+15, (int)y+20, 6, WHITE);           
            DrawCircle((int)x+45, (int)y+20, 6, WHITE);
        }
        double getSpeed(){
            return speed;
        }
        double getXAcceleration(){
            return Xacceleration;
        }
        double getJumpTime() {
            return jumpTime;
        }
        int getYAcceleration(){
            return Yacceleration;
        }
        void setAcceleration(double a){
            Xacceleration = a;
        }
        void setPosition(double newX, double newY){
            x = newX;
            y = newY;
        }
        void jump(){
            if (Grounded){
                upspeed = jumpspeed;  // only jump if on ground
                Grounded = false;
            }
        }
        void applyTerrainEffect(double modifier){
            maxSpeed = BaseMaxSpeed * modifier;
            if(speed > maxSpeed){
                speed = maxSpeed;
            }
            if(speed < -maxSpeed){
                speed = -maxSpeed;
            }

            // if(speed < 1.0 && speed > 0){
            //     speed = 1.0;   // so that when this function is called at each frame, speed doesn't keep getting lower and lower and then eventually just stop.
            // }
            // if(speed > -1.0 && speed < 0){
            //     speed = -1.0;  // same as above but for negative speeds
            // }
        }
        void updateX(float dt){
        // Apply friction when not accelerating
        if(Xacceleration == 0){
            float friction = 4.0f; // tweak this value
            if(speed > 0){
                speed -= friction * dt;
                if(speed < 0){
                    speed = 0; // clamp to 0
                }
            }
            else if(speed < 0){
                speed += friction * dt;
                if(speed > 0){
                    speed = 0; // clamp to 0
                }
            }
        }
        else{
            speed += Xacceleration * tireTraction * dt;
        }
        cout << speed << endl;
        // Clamp to max speed
        if(speed > maxSpeed){
            speed = maxSpeed;
        }
        if(speed < -maxSpeed){
            speed = -maxSpeed;
        }
        // Update position
        x += speed * dt * 300;
    }
    void updateY(float dt) {
        if (!Grounded){
            upspeed -= Yacceleration * dt;  // gravity pulls upspeed down
            y -= upspeed * dt * 100;        // move vertically
            if (y >= 300){     // hit the ground
                y = 300;
                upspeed = 0;
                Grounded = true;
            }
        }
    }
    int getMaxSpeed(){
        return maxSpeed;
    }
};

#endif

class SportsCar : public Car{
    public:
        SportsCar() : Car("SportsCar", 80, 15, 0.6, 10) {}
};
class Truck : public Car{
    public:
        Truck() : Car("Truck", 30, 8, 1.2, 40) {}
};
class Buggy : public Car{
    public:
        Buggy() : Car("Buggy", 50, 10, 0.75, 5) {}
};

int Car::gunLevel = 1;
int Car::tireLevel = 1;
