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
#ifndef HERO_H
#define HERO_H

#include <phys_obj.h>
#include <ship.h>

/**
@author Paul Stansifer
*/
class hero : public ship
{
	public:
    hero();
	
		void boost();
		
		void input_and_refresh();
	protected:
		void draw();
    void collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push);
    bool tick();
    
    void emit(phys_obj* p);

		float max_shields;
		float max_energy, recharge;

		float energy;
		
		float effective_velx; //What our x speed is for the purpose of shooting.
		
		int key;
    
    float collision_cooler; //similar to when Mario flashes after getting hit by a bad thing
};

#endif
