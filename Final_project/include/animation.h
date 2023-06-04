#ifndef __ANIMATION__H__
#define __ANIMATION__H__

typedef struct Point{
    float x_pos;
    float y_pos;
    float z_pos;

    float x_dir;
    float y_dir;
    float z_dir;
} Point;

Point colliding(Point ball);
#endif
