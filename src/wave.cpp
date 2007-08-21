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
#include "wave.h"

#include "asteroid.h"
#include "paint.h"
#include "goon.h"
#include "capital_ship.h"
#include "epilogue.h"
#include "line.h"

#include "text.h"

#include <string>
#include <fstream>

int capital_ships;
int live_enemies;
bool run_away;
ifstream input_chain("/usr/share/games/epilogue/waves"); //HACK!  TODO make this flexible

int peace_timer = 0;

/*wave::wave(int goons, int asteroid_improbability): phys_obj() {
	this->asteroid_improbability = asteroid_improbability;
	this->goons = goons;
	
	run_away = false;
	
	goon::create_goon_squad(goons/2, min_col() + 10);
	goon::create_goon_squad((goons+1)/2, max_col() - 10);
	new capital_ship("osgard1.sprite", (min_col() + max_col()) / 2, 2, 20);
}*/


void wave::make_waves(char * filename) {
	//string absolute_name("/usr/share/games/epilogue/");
  //absolute_name.append(filename);
	//input_chain = ifstream(absolute_name.c_str());
  if(!input_chain)
		end_everything_error("Cannot open wave file.\n");
	
	new wave();
}

wave::wave() : phys_obj() {
	live_enemies = 0;
		run_away = false;
	
	string control;
	input_chain >> control;
	while(control != "**end**") {
		if(control == "**goon**")
			goon::create_goons(&input_chain);
		else if(control == "**capital**")
			capital_ship::create_capitals(&input_chain);
		else if(control == "**asteroids**") {
			input_chain >> asteroid_improbability;
		} else if(control == "**gameover**") {
			live_enemies = 1;
			end_everything_error("You win.");
		} else
			new text(control.c_str());
			
		input_chain >> control;
	}	
	
}

void wave::collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push) {
	//not possible
}
void wave::draw() {
  //this isn't really a physical object, even
}

bool wave::tick() {
	while(misc_rng(asteroid_improbability) == 0) {
		//new asteroid(rand_col(), 0.0); //phys_obj s are self-managing
		make_random_big_asteroid();
	}
	
	if(live_enemies < 7 && capital_ships == 0 && !run_away) {
		run_away = true;
		peace_timer = 50;
    
	}

		
		
	if(live_enemies == 0) {
		if(peace_timer == 0) {
			protagonist->boost();
			//call for reinforcements!
			new wave();
			return false;
		}
		peace_timer--;
	}	
	
	
	return true;
}
