#pragma once
#include "DataVectors.h"

struct ColorA
{
    float R;
    float G;
    float B;
    float A;
};

void DrawLine(Vec2& Start, Vec2& End, ColorA* Color)
{
    glBegin(GL_LINES);
    glColor4f(Color->R, Color->G, Color->B, Color->A);
    glVertex2f(Start.X, Start.Y);
    glVertex2f(End.X, End.Y);
    glEnd();
}

void Draw2DEspBox(Vec2& ScreenCoordsLeftFront, Vec2& ScreenCoordsRightFront, Vec2& ScreenCoordsLeftBehind, Vec2& ScreenCoordsRightBehind,Vec2&ScreenCoordsCenter, float width, Vec2& HeadCoords, ColorA* Color)
{
    glBegin(GL_LINES);
    glColor4f(Color->R, Color->G, Color->B, Color->A);
    glVertex2f(ScreenCoordsCenter.X - (width / 2), ScreenCoordsCenter.Y);
    glVertex2f(ScreenCoordsCenter.X - (width / 2), HeadCoords.Y);
    glEnd();
    //TopFront
    glBegin(GL_LINES);
    glColor4f(Color->R, Color->G, Color->B, Color->A);
    glVertex2f(ScreenCoordsCenter.X - (width / 2), HeadCoords.Y);
    glVertex2f(ScreenCoordsCenter.X + (width / 2), HeadCoords.Y);
    glEnd();
    //RightFront
    glBegin(GL_LINES);
    glColor4f(Color->R, Color->G, Color->B, Color->A);
    glVertex2f(ScreenCoordsCenter.X + (width / 2), HeadCoords.Y);
    glVertex2f(ScreenCoordsCenter.X + (width / 2), ScreenCoordsCenter.Y);
    glEnd();
    //BottomFront
    glBegin(GL_LINES);
    glColor4f(Color->R, Color->G, Color->B, Color->A);
    glVertex2f(ScreenCoordsCenter.X + (width / 2), ScreenCoordsCenter.Y);
    glVertex2f(ScreenCoordsCenter.X - (width / 2), ScreenCoordsCenter.Y);
    glEnd();

















    /*//LeftFront
    glBegin(GL_LINES);
    glColor4f(Color->R, Color->G, Color->B, Color->A);
    glVertex2f(ScreenCoordsLeftFront.X, ScreenCoordsLeftFront.Y);
    glVertex2f(ScreenCoordsLeftFront.X, ScreenCoordsLeftFront.Y);
    glEnd();
    //TopFront
    glBegin(GL_LINES);
    glColor4f(Color->R, Color->G, Color->B, Color->A);
    glVertex2f(ScreenCoordsLeftFront.X, HeadCoords.Y);
    glVertex2f(ScreenCoordsRightFront.X, HeadCoords.Y);
    glEnd();
    //RightFront
    glBegin(GL_LINES);
    glColor4f(Color->R, Color->G, Color->B, Color->A);
    glVertex2f(ScreenCoordsRightFront.X, HeadCoords.Y);
    glVertex2f(ScreenCoordsRightFront.X, ScreenCoordsRightFront.Y);
    glEnd();
    //BottomFront
    glBegin(GL_LINES);
    glColor4f(Color->R, Color->G, Color->B, Color->A);
    glVertex2f(ScreenCoordsRightFront.X, ScreenCoordsRightFront.Y);
    glVertex2f(ScreenCoordsLeftFront.X, ScreenCoordsLeftFront.Y);
    glEnd();

    //LeftBehind
    glBegin(GL_LINES);
    glColor4f(Color->R, Color->G, Color->B, Color->A);
    glVertex2f(ScreenCoordsLeftBehind.X, ScreenCoordsLeftBehind.Y);
    glVertex2f(ScreenCoordsLeftBehind.X, HeadCoords.Y);
    glEnd();
    //TopBehind
    glBegin(GL_LINES);
    glColor4f(Color->R, Color->G, Color->B, Color->A);
    glVertex2f(ScreenCoordsLeftBehind.X, HeadCoords.Y);
    glVertex2f(ScreenCoordsRightBehind.X, HeadCoords.Y);
    glEnd();
    //RightBehind
    glBegin(GL_LINES);
    glColor4f(Color->R, Color->G, Color->B, Color->A);
    glVertex2f(ScreenCoordsRightBehind.X, HeadCoords.Y);
    glVertex2f(ScreenCoordsRightBehind.X, ScreenCoordsRightBehind.Y);
    glEnd();
    //BottomBehind
    glBegin(GL_LINES);
    glColor4f(Color->R, Color->G, Color->B, Color->A);
    glVertex2f(ScreenCoordsRightBehind.X, ScreenCoordsRightBehind.Y);
    glVertex2f(ScreenCoordsLeftBehind.X, ScreenCoordsLeftBehind.Y);
    glEnd();
    */
}