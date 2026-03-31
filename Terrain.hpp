#include <iostream>
#include <raylib.h>
#include <vector>
using namespace std;
#ifndef TERRAIN_HPP
#define TERRAIN_HPP

class Terrain{
    public:
        virtual double getSpeedModifier() = 0;
        virtual char getSymbol() = 0;
};
class Grass : public Terrain{
    public:
        double getSpeedModifier(){ 
            return 1.0; 
        }
        char getSymbol(){ 
            return '.'; 
        }
};
class Sand : public Terrain{
    public:
        double getSpeedModifier(){ 
            return 0.6; 
        }
        char getSymbol(){ 
            return '~'; 
        }
};
class Swamp : public Terrain{
    public:
        double getSpeedModifier(){
            return 0.4; 
            }
        char getSymbol(){ 
            return 'S'; 
        }
};
class TerrainManager{
    private:
        vector<Terrain*> map;

    public:
        void generateMap(){
            for(int i = 0; i < 30; i++){
                map.push_back(new Grass());
            }
            for(int i = 0; i < 30; i++){
                map.push_back(new Sand());
            }
            for(int i = 0; i < 30; i++){
                map.push_back(new Swamp());
            }
        };
        Terrain* getTerrain(int index){
            return map[index % map.size()];
        };
        void draw(float offset){
            for (int i = 0; i < map.size(); i++){
                DrawText(TextFormat("%c", map[i]->getSymbol()), i * 20 - offset, 400, 20, WHITE);
            }
        };
};


#endif





