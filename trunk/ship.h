/*
 *  ship.h
 *  epilogue
 *
 */
#ifndef SHIP_H
#define SHIP_H
#include <phys_obj.h>

class ship : public phys_obj {
protected:
    float shields;
    float collision_cooler;
    phys_obj * coolee;
public:
    ship(float shields, float x, float y, float force, float rad);
    ship(float x, float y, float force, float rad);
    void cool();
    virtual void collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push);
    virtual ~ship();
};

#endif
