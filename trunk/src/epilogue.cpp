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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <stdlib.h>
//#include <unistd.h>
#include <list>

#include "paint.h"
#include "asteroid.h"
#include "phys_obj.h"
#include "hero.h"
#include "title.h"
#include "wave.h"

using namespace std;

hero * protagonist;
subtractive_rng misc_rng;


int main(int argc, char** argv) {
	//sprite::init_basic_sprites("bannertexts");
	initialize_painter();
  
  for(int i = 0; i < argc; i++) {
    if(!strcmp("test", argv[i])) {
      while(true) {
        timeout(1000);
        int key = getch();
        cout << key << endl;
      }
    }
  }


	protagonist = new hero();
	new title();

	while(true){
		clean_screen();
		phys_obj::tick_world();
		move(0,0); // if the cursor can't be removed, leave it in the corner.
		//refresh();  		//replaced by the below
		protagonist->input_and_refresh();
		usleep(1000000/35);
	}

	return 0;
}

void end_everything() {
	timeout(-1); //getch() now blocks again
	//getch();
	endwin();
	exit(0);
}


void end_everything_error(const char * error_message) {
	timeout(-1);
	endwin();
	fprintf(stderr, "%s\n", error_message);
	exit(1);
}