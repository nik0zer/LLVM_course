#include "sim.h"

#define MAX_OBJ_START_SPEED 200
#define FLOAT_POINT 10
#define BORDER 50
#define RADIUS 20

typedef struct
{
    long long int x;
    long long int y;
} vector;

typedef struct
{
    vector pos;
    vector speed;
    int argb;
} Object;

void set_start_positions(Object *object)
{
    object->speed.x = 0;
    object->speed.y = 0;
    object->pos.x = simRand(BORDER * FLOAT_POINT, (SIM_X_SIZE - BORDER) * FLOAT_POINT);
    object->pos.y = simRand(BORDER * FLOAT_POINT, (SIM_Y_SIZE - BORDER) * FLOAT_POINT - 1);
    object->speed.x = simRand(-1 * MAX_OBJ_START_SPEED, MAX_OBJ_START_SPEED);
    object->speed.y = simRand(-1 * MAX_OBJ_START_SPEED, MAX_OBJ_START_SPEED);
    object->argb = simRand(0, __INT32_MAX__);
}

void update_pos(Object *obj)
{
    if(obj == NULL_PTR)
    {
        return;
    }
    obj->pos.x += obj->speed.x;
    obj->pos.y += obj->speed.y;
}

void clear_draw()
{
    for(size_t j = 0; j < SIM_X_SIZE; j++)
    {
        for(size_t k = 0; k < SIM_Y_SIZE; k++)
        {
            simPutPixel(j, k, 0);
        }
    }
}

void draw_objects(Object *object)
{
    clear_draw();
    for(size_t j = 0; j < SIM_X_SIZE; j++)
    {
        for(size_t k = 0; k < SIM_Y_SIZE; k++)
        {
            if(((object->pos.x / FLOAT_POINT - j) * (object->pos.x / FLOAT_POINT - j) +
            (object->pos.y / FLOAT_POINT - k) * (object->pos.y / FLOAT_POINT - k))
            <= RADIUS * RADIUS)
            {
                simPutPixel(j, k, object->argb);
            }
        }
    }
}

void wall_reflection(Object *object)
{
    if(object == NULL_PTR)
    {
        return;
    }
    if(object->pos.x / FLOAT_POINT >= SIM_X_SIZE - BORDER)
    {
        object->speed.x = -object->speed.x;
    }
    if(object->pos.x / FLOAT_POINT <= BORDER)
    {
        object->speed.x = -object->speed.x;
    }
    if(object->pos.y / FLOAT_POINT >= SIM_Y_SIZE - BORDER)
    {
        object->speed.y = -object->speed.y;
    }
    if(object->pos.y / FLOAT_POINT <= BORDER)
    {
        object->speed.y = -object->speed.y;
    }
}

void app()
{
    Object object;
    set_start_positions(&object);
    while (1) {
        wall_reflection(&object);
        update_pos(&object);
        draw_objects(&object);
        if(simExit() == 1)
        {
            return;
        }
        simFlush();
    }
}