/*
 *  ship.cpp
 *  epilogue
 *
 *
 */

#include "ship.h"
#include "shot.h"

ship::ship(float shields, float x, float y, float force, float rad) : phys_obj(x, y, force, rad) {
  collision_cooler = 0.0;
  this->shields = shields;
  coolee = NULL;
}

ship::ship(float x, float y, float force, float rad) : phys_obj(x, y, force, rad) {
  collision_cooler = 0.0;
  shields = 1.0;
  coolee = NULL;
}

void ship::cool() {
  collision_cooler *= 0.95;
}

void ship::collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push) {
  velx += x_push;
  vely += y_push;
  
  for(int i = 0; i < clip(other->force, shields) / 2; i++)
		new smoke(x-velx, y-vely, 0, velx, vely, 20);  //maybe have smoke indicate total shield level again someday?

  if(other == coolee) {
    if(other->force > collision_cooler) {
      shields -= other->force - collision_cooler;
      collision_cooler = other->force;
    }
  } else {
    shields -= other->force;
    if(other->force > collision_cooler) {
      collision_cooler = other->force;
      coolee = other;
    }
  }
}

ship::~ship() {
}
