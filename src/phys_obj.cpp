/***************************************************************************
 *   Copyright (C) 2005 by Paul Stansifer                                  *
 *   07pns@williams.edu                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "phys_obj.h"
#include <list>

#include "paint.h"


using namespace std;

list<phys_obj*> phys_obj::world;

phys_obj::phys_obj(float x, float y, float force, float rad) {
	this->x = x; this->y = y;
	this->force = force;
	repel_radius = rad;
	dead = false;
	velx = 0; vely = 0;
	world.push_front(this);
	jiggly = false;
}

/**
 *All effects must be after all non-effects, because of the way collision detection works.
 */
phys_obj::phys_obj(float x, float y, float force, float rad, bool back) {
	this->x = x; this->y = y;
	this->force = force;
	repel_radius = rad;
	dead = false;
	velx = 0; vely = 0;
	if(back)
		world.push_back(this);
	else
		world.push_front(this);
	
	jiggly = false;
}

phys_obj::phys_obj() {
	this->x = -1000; this->y = -1000;
	this->force = 0.0;
	repel_radius = 0.0;
	dead = false;
	velx = 0.0; vely = 0.0;
	world.push_front(this);
}

phys_obj::~phys_obj() {
	world.remove(this);
}

void phys_obj::tick_world() {
	list<phys_obj*> death_row;

	//set_red();
	//mvpaint(1,1,'A'+world.size(),NULL);
	
	for(list<phys_obj*>::iterator i = world.begin(); i != world.end(); i++) {
		if(!(*i)->dead && (*i)->internal_tick())
			(*i)->draw();
		else
			death_row.push_back(*i);
	}
	for(list<phys_obj*>::iterator i = death_row.begin(); i != death_row.end(); i++)
		delete (*i);
}

void phys_obj::nudge_all(float dx, float dy) {
	for(list<phys_obj*>::iterator i = world.begin(); i != world.end(); i++) {
		(*i)->x += dx;
		(*i)->y += dy;
	}
}

inline int sign(float f) {
	if(f > 0)
		return 1;
	if(f < 0)
		return -1;
	return 0;
}

// void phys_obj::internal_collide(phys_obj* other, int xoff, int yoff) {
// 	velx -= xoff/repel_radius * 0.5;
// 	vely -= yoff/repel_radius * 0.5;
//
// //why does this activate even where there are only 1-high objects?
// //	if(yoff)
// //		throw "YIKES!";
//
// 	velx += other->velx * (other->force / force);
// 	vely += other->vely * (other->force / force);
// 	collide(other, xoff, yoff);
// }

/* Ray-casting for large things.  Returns (via out-parameters) the
 * nearest point to the object.
 */
void phys_obj::effective_loc(float the_x, float the_y, float & eff_x, float & eff_y) {
  eff_x = x;
  eff_y = y;
}

void phys_obj::get_repulsion(float & xr, float & yr, bool follow_hero) {
	xr = yr = 0;
	for(list<phys_obj*>::iterator i = world.begin(); i != world.end(); i++) {
		phys_obj* other = *i;
    float othx, othy;
    other->effective_loc(x, y, othx, othy); //get the closest point, when relevant
    float xd = x - othx,  yd = y - othy;

       
		if(other == this)
			continue;
    
    //skip if it's more than 7 radii away
    if(8 * 8 < xd*xd + yd*yd) {
      float xpd = x - (othx + other->velx*7),
            ypd = y - (othy + other->vely*7);
      if(7 * 7 < xpd*xpd + ypd*ypd)
         continue;
    }
    //if(xd*xd + yd*yd > 60)
    //  continue;
		
		float vel = sqrtf(other->velx * other->velx + other->vely * other->vely);
		
		float futurex, futurey;
		float scale;
		if(vel == 0 || other->jiggly) { //if it's not moving or moving irregularly, repel from its current location
			futurex = othx;
			futurey = othy;
		} else { //repel from where it will be when it's closest
      //Ooops.  This is wrong:
			//futurex = (xd * other->velx)/vel + other->x;
			//futurey = (yd * other->vely)/vel + other->y;
      closest(othx, othy, other->velx*7, other->vely*7, x, y, futurex, futurey, true);
			//TODO scale down objects coming a longer time from now.
		}
    float fxd = futurex - x,  fyd = futurey - y;
		
		//float xdist = (other->x+(other->velx) - x), ydist = (other->y + (other->vely) - y);
		//float xdist = (futurex - x), ydist = (futurey - y);

		//if(fabs(fxd) > 7 || fabs(fyd) > 7 || (fxd == 0 && fyd == 0))
		//	continue;
    float dist = (sqrtf(fxd*fxd + fyd*fyd));
    if(dist == 0)
      continue; //Not much to do.
    
		float mult = (other->repel_radius + this->repel_radius + vel*2.5 + other->force*1.75) / dist;
    
    mult *= 1.5;

		xr -= mult * fxd / dist;//sign(fxd);
    yr -= mult * fyd / dist;//sign(fyd);
	}
	
	if(follow_hero && fabsf(xr) + fabsf(yr) < 0.15) {
		float xdist = (the_hero->x - x), ydist = (the_hero->y - 15 - y); //aim above the hero
    
		
		xr += clip(xdist*0.4, 1.3); //max speed
		yr += clip(ydist*0.125, 0.5); //vertical attraction is pretty weak, but can reach max speed
	}
	
	/*
	if(x < (min_col() + max_col()) / 2) {
		float xedge = x - min_col();
		xr += 3.0 / (xedge * xedge);
	} else {
		float xedge = x - max_col();
		xr -= 3.0 / (xedge * xedge);
	}
	 */

	if(y < (min_row() + max_row()) / 2) {
		float yedge = y - min_row();
		yr += 3.0 / (yedge * yedge);
	} else {
		float yedge = y - max_row();
		yr -= 3.0 / (yedge * yedge);
	}

	xr = clip(xr, 1.0);
	yr = clip(yr, 0.5);
}
