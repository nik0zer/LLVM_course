#include "sim.h"

#define MAX_OF_OBJECTS 10
#define MAX_OBJ_SIZE 20
#define MIN_OBJ_SIZE 10
#define MAX_OBJ_START_SPEED 3000
#define G 1000
#define FLOAT_POINT 10
#define BORDER 50
#define MAX_BOOST 100
#define MAX_FORCE 1000000
#define WALL_BOOST 200
#define FRICTION 1

typedef struct
{
    long long int x;
    long long int y;
} vector;


typedef struct
{
    int radius;
    vector pos;
    vector speed;
    vector boost;
    int argb;
    int alive;
} Object;

long long int check_for_force_overflow(long long int a)
{
    if(a < 0)
    {
        return __INT32_MAX__;
    }
    return a;
}

long long int mabs(long long int a)
{
    if(a > 0)
    {
        return a;
    }
    else
    {
        return -1 * a;
    }
}

long long int check_for_boost_overflow(long long int a)
{
    if(mabs(a) > MAX_BOOST)
    {
        if(a > 0)
        {
            return MAX_BOOST;
        }
        else
        {
            return -MAX_BOOST;
        }
    }
    return a;
}

void null_boost(Object *obj)
{
    if(obj == NULL_PTR)
    {
        return;
    }
    obj->boost.x = 0;
    obj->boost.y = 0;
}

void set_start_positions(Object objects[])
{
    for(size_t i = 0; i < MAX_OF_OBJECTS; i++)
    {
        objects[i].alive = 1;
        null_boost(&(objects[i]));
        objects[i].speed.x = 0;
        objects[i].speed.y = 0;
        objects[i].radius = simRand(MIN_OBJ_SIZE, MAX_OBJ_SIZE);
        objects[i].pos.x = simRand(BORDER * FLOAT_POINT, (SIM_X_SIZE - BORDER) * FLOAT_POINT);
        objects[i].pos.y = simRand(BORDER * FLOAT_POINT, (SIM_Y_SIZE - BORDER) * FLOAT_POINT - 1);
        objects[i].speed.x = simRand(-1 * MAX_OBJ_START_SPEED, MAX_OBJ_START_SPEED);
        objects[i].speed.y = simRand(-1 * MAX_OBJ_START_SPEED, MAX_OBJ_START_SPEED);
        objects[i].argb = simRand(0, __INT32_MAX__);
    }
}

void calculate_boost(Object *obj_1, Object *obj_2)
{
    if(obj_1 == NULL_PTR || obj_2 == NULL_PTR || ((obj_2->pos.x / FLOAT_POINT - obj_1->pos.x / FLOAT_POINT == 0) &&
    (obj_2->pos.y / FLOAT_POINT - obj_1->pos.y / FLOAT_POINT == 0)))
    {
        return;
    }
    int mass_1 = obj_1->radius * obj_1->radius;
    int mass_2 = obj_2->radius * obj_2->radius;
    int distanse_2 = ((obj_1->pos.x - obj_2->pos.x) * (obj_1->pos.x - obj_2->pos.x) +
    (obj_1->pos.y - obj_2->pos.y) * (obj_1->pos.y - obj_2->pos.y)) / (FLOAT_POINT * FLOAT_POINT);
    long long int force = 0;
    if(distanse_2 < FLOAT_POINT)
    {
        force = MAX_FORCE;
    }
    else
    {
        force = check_for_force_overflow(G * mass_1 * mass_2 / distanse_2);
    }
    long long int distanse_1_2_x = (obj_2->pos.x - obj_1->pos.x) / FLOAT_POINT;
    long long int distanse_1_2_y = (obj_2->pos.y - obj_1->pos.y) / FLOAT_POINT;
    if(distanse_1_2_x == 0 && distanse_1_2_y == 0)
    {
        distanse_1_2_x = 1;
        distanse_1_2_y = 1;
    }
    obj_1->boost.x += check_for_boost_overflow((distanse_1_2_x * force) /
    (mabs(distanse_1_2_x) + mabs(distanse_1_2_y)) / mass_1);
    obj_1->boost.y += check_for_boost_overflow((distanse_1_2_y * force) /
    (mabs(distanse_1_2_x) + mabs(distanse_1_2_y)) / mass_1);
    obj_2->boost.x += check_for_boost_overflow((-1 * distanse_1_2_x * force) /
    (mabs(distanse_1_2_x) + mabs(distanse_1_2_y)) / mass_2);
    obj_2->boost.y += check_for_boost_overflow((-1 * distanse_1_2_y * force) /
    (mabs(distanse_1_2_x) + mabs(distanse_1_2_y)) / mass_2);
}

void update_pos(Object *obj)
{
    if(obj == NULL_PTR)
    {
        return;
    }
    obj->speed.x += obj->boost.x;
    obj->speed.y += obj->boost.y;
    obj->speed.x = obj->speed.x * (100 - FRICTION) / 100;
    obj->speed.y = obj->speed.y * (100 - FRICTION) / 100;
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

void draw_objects(Object objects[])
{
    clear_draw();
    for(size_t i = 0; i < MAX_OF_OBJECTS; i++)
    {
        for(size_t j = 0; j < SIM_X_SIZE; j++)
        {
            for(size_t k = 0; k < SIM_Y_SIZE; k++)
            {
                if(((objects[i].pos.x / FLOAT_POINT - j) * (objects[i].pos.x / FLOAT_POINT - j) +
                (objects[i].pos.y / FLOAT_POINT - k) * (objects[i].pos.y / FLOAT_POINT - k))
                <= objects[i].radius * objects[i].radius && objects[i].alive)
                {
                    simPutPixel(j, k, objects[i].argb);
                }
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
        object->boost.x = -WALL_BOOST;
        object->speed.x = 0;
    }
    if(object->pos.x / FLOAT_POINT <= BORDER)
    {
        object->boost.x = WALL_BOOST;
        object->speed.x = 0;
    }
    if(object->pos.y / FLOAT_POINT >= SIM_Y_SIZE - BORDER)
    {
        object->boost.y = -WALL_BOOST;
        object->speed.y = 0;
    }
    if(object->pos.y / FLOAT_POINT <= BORDER)
    {
        object->boost.y = WALL_BOOST;
        object->speed.y = 0;
    }
}

void app()
{
    Object objects[MAX_OF_OBJECTS];
    set_start_positions(objects);
    while (1) {
        for(size_t i = 0; i < MAX_OBJ_SIZE; i++)
        {
            null_boost(&(objects[i]));
        }
        for(size_t i = 0; i < MAX_OF_OBJECTS; i++)
        {
            for(size_t j = i + 1; j < MAX_OF_OBJECTS; j++)
            {
                calculate_boost(&(objects[i]), &(objects[j]));
            }
        }
        for(size_t i = 0; i < MAX_OF_OBJECTS; i++)
        {
            wall_reflection(&(objects[i]));
        }
        for(size_t i = 0; i < MAX_OBJ_SIZE; i++)
        {
            update_pos(&(objects[i]));
        }
        draw_objects(objects);
        if(simExit() == 1)
        {
            return;
        }
        simFlush();
    }
}