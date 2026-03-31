#include <iostream>
#include "Car.hpp"
#include "Terrain.hpp"
#include "raylib.h"
#include <vector>
#include <cmath> 
using namespace std;

int main(){
    InitWindow(1400, 600, "Test Car + Terrain");

    Camera2D cam = { 0 };
    cam.offset = (Vector2){ 400, -100 };
    cam.zoom = 1.0f;

    Buggy car;
    car.setPosition(300, 300);

    Texture2D cartext = LoadTexture("car-removebg-preview.png");

    const int LAYERS = 6;
    const int PACKS = 4;

    Texture2D bg[PACKS][LAYERS];
    float offsets[LAYERS] = {0};

    for(int p = 0; p < PACKS; p++){
        string folder = "assets/" + to_string(p+1) + "/";
        bg[p][0] = LoadTexture((folder + "background.png").c_str());
        for(int i = 1; i < LAYERS; i++){
            bg[p][i] = LoadTexture((folder + to_string(i) + ".png").c_str());
        }
    }

    float speeds[LAYERS] = {0.02f, 0.05f, 0.1f, 0.2f, 0.4f, 0.8f};

    int currentPack = 0;
    float bgTimer = 0.0f;
    float bgSwitchTime = 30.0f;

    TerrainManager terrain;
    terrain.generateMap();

    float terrainOffset = 0;
    float bgOffset = 0;

    while(!WindowShouldClose()){
        float dt = GetFrameTime();
        bgTimer += dt;
        if(bgTimer >= bgSwitchTime){
            bgTimer = 0.0f;
            currentPack = (currentPack + 1) % PACKS;
        }
        if(IsKeyDown(KEY_Q)){ // Quit button
            break;
        }
        if (IsKeyDown(KEY_W)){
            car.jump();
        }
        if(IsKeyDown(KEY_S)){ // A different button for proper brakes
            if(car.getSpeed() > 0){
                car.setAcceleration(-9);
            }    
            else if(car.getSpeed() < 0){
                car.setAcceleration(9);
            }    
        }
        if(IsKeyDown(KEY_D) && IsKeyDown(KEY_A)){ // A whole acceleration block below.
            car.setAcceleration(0);
        }
        else if(IsKeyDown(KEY_D)){
            if(car.getSpeed() <= 0 && car.getSpeed() > -0.1f){
                car.setAcceleration(15); 
            }
            else if(car.getSpeed() > 0){
                car.setAcceleration(15);
            }
            else{
                car.setAcceleration(0);
            }
        }
        else if(IsKeyDown(KEY_A)){
            if(car.getSpeed() >= 0 && car.getSpeed() < 0.1f){
                car.setAcceleration(-2); 
            }
            else if(car.getSpeed() < 0){
                car.setAcceleration(-2); 
            }
            else{
                car.setAcceleration(0);
            }
        }
        else if(!IsKeyDown(KEY_S)){
            car.setAcceleration(0);
        }

        car.updateY(dt);
        car.updateX(dt);

        terrainOffset += car.getSpeed() * dt * 1.5f;

        //For new scrolling background
        for(int i = 0; i < LAYERS; i++){
            offsets[i] -= car.getSpeed() * dt * 200 * speeds[i];
        }

        int index = (int)(terrainOffset / 20);
        Terrain* current = terrain.getTerrain(index);
        static Terrain* lastTerrain = nullptr;
        if(current != lastTerrain){
            car.applyTerrainEffect(current->getSpeedModifier());
            lastTerrain = current;
        }

        float camX = car.GetX();
        float speedRatio = car.getSpeed() / car.getMaxSpeed();
        float leadOffset = speedRatio * 100.0f;
        float desiredX = camX + leadOffset;
        cam.target.x += (desiredX - cam.target.x) * 2.0f * dt;
        // Below lines basically clamp the camera so that it doesn't go too far ahead or behind car otherwise car would disappear off screen.
        if(cam.target.x > car.GetX() + 200){
            cam.target.x = car.GetX() + 200;
        }
        if(cam.target.x < car.GetX() - 200){
            cam.target.x = car.GetX() - 200;
        }

        BeginDrawing();
            //DrawTexture(back, -(int)bgOffset, -100, WHITE);
            for(int i = 0; i < LAYERS; i++){
                Texture2D& tex = bg[currentPack][i];
                if(tex.id == 0){
                    continue; 
                }
                int w = tex.width;
                float off = fmodf(offsets[i], (float)w);
                if(off > 0){
                    off -= w;
                }
                DrawTexture(tex, (int)off, 0, WHITE);
                DrawTexture(tex, (int)off + w, 0, WHITE);
            }

            BeginMode2D(cam);
            DrawTexture(cartext, (int)car.GetX()-30, car.GetY()-15, WHITE);
            EndMode2D();
        EndDrawing();

        if(IsKeyPressed(KEY_ONE)){
            bgTimer = 0.0f;
            currentPack = 0;
        }
        if(IsKeyPressed(KEY_TWO)){
            bgTimer = 0.0f;
            currentPack = 1;
        }
        if(IsKeyPressed(KEY_THREE)){
            bgTimer = 0.0f;
            currentPack = 2;
        }
        if(IsKeyPressed(KEY_FOUR)){
            bgTimer = 0.0f;
            currentPack = 3;
        }
    }
    CloseWindow();
    return 0;
}