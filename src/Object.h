#pragma once
#include "matrix.h"
#include <SDL3/SDL.h>
#include <vector>

#define WIDTH 800
#define HEIGHT 600
#define DELAY 30
#define FLOOR 400

#define gradToRad(x) x * PI/180 
#define PI 3.14159265358979323846
enum{AxisX,AxisY,AxisZ};

class polygon
{
    private:
    SDL_Point first;
    SDL_Point second;
    SDL_Point third;
    SDL_Color lineColor;
    SDL_Color drawColor;
    friend class object;
    public:
    void draw(SDL_Renderer *renderer);
    polygon(){};
    ~polygon(){};
};

class object
{
private:
	matrix cords;
	unsigned int** polygons;
	unsigned int polygCount,pointCount;
	bool isVisible(int polygId);
public:
	object(unsigned int pointCnt,const char* fileName);
	void rotate(double angle,unsigned int axis);
	void scale(double scaleX,double scaleY,double scaleZ);
	void move(double moveX, double moveY, double moveZ);
    void getPolygons(std::vector<polygon*> &drawConveyor);
    void getShadePolygons(std::vector<polygon*> &drawConveyor);
	double getMiddleX();
	double getMiddleY();
	double getMiddleZ();
};

