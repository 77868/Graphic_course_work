#include "Object.h"
#include <vector>
#include <cmath>
#include <fstream>
extern float lightSource[];

object::object(unsigned int pointCnt, const char *fileName) : cords(pointCnt, 4)
{
    
    std::ifstream in(fileName);
    if (!in.is_open())
        return;
    in >> pointCount;
    for (size_t i = 0; i < pointCount; i++)
    {
        in >> cords[i][0] >> cords[i][1] >> cords[i][2];
        cords[i][3] = 1.0f;
    }
    in >> polygCount;
    polygons = new unsigned int *[polygCount];
    for (size_t i = 0; i < polygCount; i++)
    {
        polygons[i] = new unsigned int[3];
        in >> polygons[i][0] >> polygons[i][1] >> polygons[i][2];
    }
}

double object::getMiddleX()
{
    double result = 0;
    for (size_t i = 0; i < pointCount; i++)
    {
        result += cords[i][0];
    }
    return result / pointCount;
}
double object::getMiddleY()
{
    double result = 0;
    for (size_t i = 0; i < pointCount; i++)
    {
        result += cords[i][1];
    }
    return result / pointCount;
}
double object::getMiddleZ()
{
    double result = 0;
    for (size_t i = 0; i < pointCount; i++)
    {
        result += cords[i][2];
    }
    return result / pointCount;
}

void object::move(double moveX, double moveY, double moveZ)
{
    matrix moveM{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {moveX, moveY, moveZ, 1}};
    cords = cords * moveM;
}
void object::scale(double scaleX, double scaleY, double scaleZ)
{
    matrix transformMatrix(4, 4);
    matrix moveM{
        {1, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 1, 0},
        {-getMiddleX(), -getMiddleY(), -getMiddleZ(), 1},
    };
    matrix scaleM{
        {scaleX, 0, 0, 0},
        {0, scaleY, 0, 0},
        {0, 0, scaleZ, 0},
        {0, 0, 0, 1}};
    transformMatrix = moveM * scaleM;
    moveM[3][0] *= -1;
    moveM[3][1] *= -1;
    moveM[3][2] *= -1;
    transformMatrix = transformMatrix * moveM;
    cords = cords * transformMatrix;
}
void object::rotate(double angle, unsigned int axis)
{
    matrix transformM(4, 4);
    matrix moveM{
        {1, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 1, 0},
        {-getMiddleX(), -getMiddleY(), -getMiddleZ(), 1},
    };

    switch (axis)
    {
    case 0:
    {
        matrix rotM{
            {1, 0, 0, 0},
            {0, cos(angle), sin(angle), 0},
            {0, -sin(angle), cos(angle), 0},
            {0, 0, 0, 1}};
        transformM = moveM * rotM;
    }
    break;
    case 1:
    {
        matrix rotM{
            {cos(angle), 0, sin(angle), 0},
            {0, 1, 0, 0},
            {-sin(angle), 0, cos(angle), 0},
            {0, 0, 0, 1}};
        transformM = moveM * rotM;
    }
    break;
    case 2:
    {
        matrix rotM{
            {cos(angle), sin(angle), 0, 0},
            {-sin(angle), cos(angle), 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}};
        transformM = moveM * rotM;
    }
    break;
    default:
        return;
    }

    moveM[3][0] *= -1;
    moveM[3][1] *= -1;
    moveM[3][2] *= -1;
    transformM = transformM * moveM;
    cords = cords * transformM;
}

bool object::isVisible(int polygIndex)
{
    double V1X = cords[polygons[polygIndex][0]][0] - cords[polygons[polygIndex][1]][0];
    double V1Y = cords[polygons[polygIndex][0]][1] - cords[polygons[polygIndex][1]][1];
    double V1Z = cords[polygons[polygIndex][0]][2] - cords[polygons[polygIndex][1]][2];

    double V2X = cords[polygons[polygIndex][1]][0] - cords[polygons[polygIndex][2]][0];
    double V2Y = cords[polygons[polygIndex][1]][1] - cords[polygons[polygIndex][2]][1];
    double V2Z = cords[polygons[polygIndex][1]][2] - cords[polygons[polygIndex][2]][2];

    double A = V1Y * V2Z - V2Y * V1Z;
    double B = V1Z * V2X - V2Z * V1X;
    double C = V1X * V2Y - V2X * V1Y;
    double D = -(A * cords[polygons[polygIndex][0]][0] + B * cords[polygons[polygIndex][0]][1] + C * cords[polygons[polygIndex][0]][2]);

    double sign = ((A * getMiddleX() + B * getMiddleY() + C * getMiddleZ() + D) < 0) ? 1 : -1;

    A *= sign;
    B *= sign;
    C *= sign;
    D *= sign;

    if ((A * 0 + B * 0+ C * 0 + D) < 0)
        return 0;
    return 1;
}

void object::getPolygons(std::vector<polygon *> &drawConveyor)
{

    for (size_t i = 0; i < polygCount; i++)
    {
        if (isVisible(i))
        {
            auto tmp = new polygon;
            tmp->drawColor.r = 0xff;
            tmp->drawColor.g = 0xff;
            tmp->drawColor.b = 0x00;
            tmp->drawColor.a = 0xff;

            tmp->lineColor.r = 0x00;
            tmp->lineColor.g = 0x00;
            tmp->lineColor.b = 0x00;
            tmp->lineColor.a = 0xff;

            tmp->first.x = 300 * cords[polygons[i][0]][0] / cords[polygons[i][0]][2] + WIDTH / 2;
            tmp->first.y = 300 * cords[polygons[i][0]][1] / cords[polygons[i][0]][2] + HEIGHT / 2;

            tmp->second.x = 300 * cords[polygons[i][1]][0] / cords[polygons[i][1]][2] + WIDTH / 2;
            tmp->second.y = 300 * cords[polygons[i][1]][1] / cords[polygons[i][1]][2] + HEIGHT / 2;

            tmp->third.x = 300 * cords[polygons[i][2]][0] / cords[polygons[i][2]][2] + WIDTH / 2;
            tmp->third.y = 300 * cords[polygons[i][2]][1] / cords[polygons[i][2]][2] + HEIGHT / 2;

            drawConveyor.push_back(tmp);
        }
    }
}
void object::getShadePolygons(std::vector<polygon *> &drawConveyor)
{
     for (size_t i = 0; i < polygCount; i++)
    {
            auto tmp = new polygon;
            tmp->drawColor.r = 0x00;
            tmp->drawColor.g = 0x00;
            tmp->drawColor.b = 0x00;
            tmp->drawColor.a = 0xff;

            tmp->lineColor.r = 0x00;
            tmp->lineColor.g = 0x00;
            tmp->lineColor.b = 0x00;
            tmp->lineColor.a = 0xff;

            tmp->first.x = 300 * (cords[polygons[i][0]][0] + lightSource[0]*(FLOOR-cords[polygons[i][0]][1])/lightSource[1])/ (cords[polygons[i][0]][2] + lightSource[2]*(FLOOR-cords[polygons[i][0]][1])/lightSource[1]) + WIDTH / 2;
            tmp->first.y = 300 * FLOOR / cords[polygons[i][0]][2] + HEIGHT / 2;

            tmp->second.x = 300 * (cords[polygons[i][1]][0] + lightSource[0]*(FLOOR-cords[polygons[i][1]][1])/lightSource[1])/ (cords[polygons[i][1]][2] + lightSource[2]*(FLOOR-cords[polygons[i][1]][1])/lightSource[1]) + WIDTH / 2;
            tmp->second.y = 300 * FLOOR / cords[polygons[i][1]][2] + HEIGHT / 2;

            tmp->third.x = 300 * (cords[polygons[i][2]][0] + lightSource[0]*(FLOOR-cords[polygons[i][2]][1])/lightSource[1]) / (cords[polygons[i][2]][2] + lightSource[2]*(FLOOR-cords[polygons[i][2]][1])/lightSource[1]) + WIDTH / 2;
            tmp->third.y = 300 * FLOOR / cords[polygons[i][2]][2] + HEIGHT / 2;

            drawConveyor.push_back(tmp);
    }
}

void polygon::draw(SDL_Renderer *renderer)
{
    SDL_Point points[3] = {first, second, third};

    if (points[0].y > points[1].y)
        std::swap(points[0], points[1]);
    if (points[1].y > points[2].y)
        std::swap(points[1], points[2]);
    if (points[0].y > points[1].y)
        std::swap(points[0], points[1]);

    SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);

    float dx10 = points[1].x - points[0].x;
    float dx20 = points[2].x - points[0].x;
    float dy10 = points[1].y - points[0].y;
    float dy20 = points[2].y - points[0].y;

    for (size_t y = points[0].y < 0 ? 0 : points[0].y; y < points[1].y && y < HEIGHT; y++)
    {
        
        float x10 = (y - points[0].y) * dx10 / dy10 + points[0].x+1;
        float x20 = (y - points[0].y) * dx20 / dy20 + points[0].x+1;
        SDL_RenderDrawLineF(renderer, x10, y, x20, y);
    }

    float dx21 = points[2].x - points[1].x+1;
    float dy21 = points[2].y - points[1].y+1;

    for (size_t y = points[1].y < 0 ? 0 : points[1].y; y < points[2].y && y < HEIGHT; y++)
    {
        
        float x21 = (y - points[1].y) * dx21 / dy21 + points[1].x;
        float x20 = (y - points[0].y) * dx20 / dy20 + points[0].x;
        SDL_RenderDrawLineF(renderer, x21, y, x20, y);
    }

    SDL_SetRenderDrawColor(renderer, lineColor.r, lineColor.g, lineColor.b, lineColor.a);
    SDL_RenderDrawLine(renderer, first.x, first.y, second.x, second.y);
    SDL_RenderDrawLine(renderer, first.x, first.y, third.x, third.y);
    SDL_RenderDrawLine(renderer, third.x, third.y, second.x, second.y);
}
