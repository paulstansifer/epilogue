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
#ifndef SPRITE_H
#define SPRITE_H


#include <hash_map.h> //why does hash_map not work?
#include <list>

#include "phys_obj.h"


//from the STL documentation
struct eqstr {
	bool operator()(const char* s1, const char* s2) const {
		return strcmp(s1, s2) == 0;
	}
};

struct pixel {
	int x, y;
	int color;
	int character;

	pixel(int x, int y, int character, int color) {
		this->x = x;
		this->y = y;
		this->color = color;
		this->character = character;
	}
};

class sprite {
	public:
		sprite();

		static void init_basic_sprites(const char * filename);

		static sprite* get(const char * name);

		static sprite copy(const char * name);

		sprite(const sprite & copy);
		sprite(int xsize, int ysize);
		sprite(ifstream & in);

		sprite * const operator=(const sprite& rhs);

		void draw(int offx, int offy, phys_obj* who);
		sprite generateSprite(const char * name);

		// for block sprites
		void remove_character(int x, int y);
		void edit(int x, int y, int character, int color);
		void lose_pieces(int source_x, int source_y);
		
		int char_at(int x, int y);
		int color_at(int x, int y);

		// for random-access sprites
		void add_character(int x, int y, int character, int color);
		void add_character(float x, float y, int character, int color);


		int xsize, ysize;

				vector<vector<int> > characters; //make this protected again (debugging)
	protected:
		sprite(const char * filename);

		void read_sprite(ifstream & in);

		vector<vector<int> > colors;

		list<pixel> ra_pixels;

		static hash_map<const char*, sprite*, hash<const char*>, eqstr> sprites;
		
};

struct file_not_found {
};


#endif
