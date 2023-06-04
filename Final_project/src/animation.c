#include "animation.h"
#include "helper_functions.h"
#include "max7219.h"

Point colliding(Point ball){
    // Return the direction after collision

    Point new_point;
    float x_dir = ball.x_dir;
    float y_dir = ball.y_dir;
    float z_dir = ball.z_dir; 

    float x = ball.x_pos;
    float y = ball.y_pos;
    float z = ball.z_pos;

    // if collide
    // int isCollide = 0;
    if(x<1 || x>5){
        x_dir*=-1;
        // isCollide = 1;
    }
    if(y<1 || y>5){
        y_dir*=-1;
        // isCollide = 1;
    }
    if(z<1 || z>5){
        z_dir*=-1;
        // isCollide = 1;
    }
    
    // if(isCollide == 0){
        x+=x_dir;
        y+=y_dir;
        z+=z_dir;
    // }
    new_point.x_dir = x_dir;
    new_point.y_dir = y_dir;
    new_point.z_dir = z_dir;
    new_point.x_pos = x;
    new_point.y_pos = y;
    new_point.z_pos = z;
    
    return new_point; 
}