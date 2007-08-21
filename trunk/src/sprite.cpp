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

#include "sprite.h"
#include "paint.h"
#include "epilogue.h"

#include "text.h"

#include <fstream>
#include <string>
#include <list>

hash_map<const char*, sprite*, hash<const char*>, eqstr> sprite::sprites;

void draw(int offx, int offy, phys_obj* who);


sprite* sprite::get(const char * name) {
  if(sprites.count(name) == 0) {
    sprites[name] = new sprite(name);
  }
  return sprites[name];
}

sprite sprite::copy(const char * name) {
  return *get(name);
}

sprite::sprite(const sprite & copy) {
  xsize = copy.xsize;
  ysize = copy.ysize;

  ra_pixels = copy.ra_pixels;

  if(copy.characters.size() != 0) {
    characters.resize(xsize);
    colors.resize(xsize);
    for(int x = 0; x < xsize; x++) {
      characters[x].resize(ysize);
      colors[x].resize(ysize);
    }


    for(int x = 0; x < xsize; x++) {
      for(int y = 0; y < ysize; y++) {
        colors[x][y] = copy.colors[x][y];
        characters[x][y] = copy.characters[x][y];
      }
    }
  }
}

//create an asteriod of the specified size
sprite::sprite(int xsize, int ysize) {
	this->xsize = xsize;
	this->ysize = ysize;
	
	characters.resize(xsize);
	colors.resize(xsize);
	for(int x = 0; x < xsize; x++) {
		characters[x].resize(ysize);
		colors[x].resize(ysize);
		for(int y = 0; y < ysize; y++) {
			characters[x][y] = '\t';
			colors[x][y] = 'w';
		}
	}
	
	int x = xsize/2;
	int y = ysize/2;
	
	int dots = misc_rng(xsize*ysize);
	
	for(int i = 0; i < dots; i++) {
		if(characters[x][y] == '\t') {
			switch(misc_rng(3)) {
				case 0: characters[x][y] = '.'; break;
				case 1: characters[x][y] = ';'; break;
				case 2: characters[x][y] = ','; break;
			}
		} else 
			characters[x][y] = ':';
		
		switch(misc_rng(4)) {
			case 0: x++; break;
			case 1: x--; break;
			case 2: y++; break;
			case 3: y--; break;
		}
		
		if(x >= xsize)
			x = xsize-2;
		if(x < 0)
			x = 1;
		if(y >= ysize)
			y = ysize-2;
		if(y < 0)
			y = 1;
		
	}
}

sprite * const sprite::operator=(const sprite& rhs) {
  xsize = rhs.xsize;
  ysize = rhs.ysize;
	
	for(list<pixel>::const_iterator i = rhs.ra_pixels.begin(); i != rhs.ra_pixels.end(); i++)
		ra_pixels.push_back(pixel(i->x, i->y, i->character, i->color));
	
  if(rhs.characters.size() != 0) {
		characters.resize(xsize);
		colors.resize(xsize);
		for(int x = 0; x < xsize; x++) {
			characters[x].resize(ysize);
			colors[x].resize(ysize);
    }


    for(int x = 0; x < xsize; x++) {
      for(int y = 0; y < ysize; y++) {
        colors[x][y] = rhs.colors[x][y];
        characters[x][y] = rhs.characters[x][y];
      }
    }
  }
  return this;
}

void sprite::read_sprite(ifstream & in) {
	//TODO: error handling
  in >> xsize >> ysize;

	characters.resize(xsize);
	colors.resize(xsize);
	for(int x = 0; x < xsize; x++) {
		characters[x].resize(ysize);
		colors[x].resize(ysize);
  }

  for(int y = 0; y < ysize; y++) {
    for(int x = 0; x < xsize; x++) {
      int c;

      do{
        c = in.get();
      } while( c == '\n' || c == '\r');

        switch(c){
/*          case '|':
            c = ACS_VLINE; break;
          case '-':
            c = ACS_HLINE; break;
          case '+':
            c = ACS_PLUS; break;*/
					case ' ':
						c = '\t'; break;
					case 'G':
						c = ' '; break;
        }

			characters[x][y] = c;
    }
  }
	
	for(int y = 0; y < ysize; y++) {
    for(int x = 0; x < xsize; x++) {
			int c;
			
      do{
        c = in.get();
      } while( c == '\n' || c == '\r');
			colors[x][y] = c;
		}
	}
}

void sprite::remove_character(int x, int y) {
	unsigned int l = characters[x].size();
  characters[x][y] = '\t';
  colors[x][y] = 'X';
	if(l != characters[x].size())
		throw 'a';
}

void sprite::edit(int x, int y, int character, int color) {
  characters[x][y] = character;
  colors[x][y] = color;
	
	//char buffer[50];
	//sprintf(buffer, " [%c %d] %d %d ", character, color, x, y);
	//new text(buffer);
}
void sprite::lose_pieces(int source_x, int source_y) {
  //TODO
}

int sprite::char_at(int x, int y) {
	return characters[x][y];
}
int sprite::color_at(int x, int y) {
	return colors[x][y];
}

void sprite::add_character(int x, int y, int character, int color) {
  ra_pixels.push_back(pixel(x, y, character, color));
}

//compatability for floats
void sprite::add_character(float x, float y, int character, int color) {
  ra_pixels.push_back(pixel((int)x, (int)y, character, color));
}

sprite::sprite(const char * filename) {
  string absolute_name("/usr/share/games/epilogue/");
  absolute_name.append(filename);
  ifstream in(absolute_name.c_str());
  if(!in)
		end_everything_error("Cannot open sprite file.\n");
  read_sprite(in);
}

sprite::sprite(ifstream & in) {
	read_sprite(in);
}

sprite::sprite() {
  xsize = 0;
  ysize = 0;
}

void sprite::init_basic_sprites(const char * filename) {
  string absolute_name("/usr/share/games/epilogue/");
  absolute_name.append(filename);
  ifstream in(absolute_name.c_str());
  if(!in) {
    end_everything_error("Cannot open sprite file.\n");
	}
  while(true) {
    string sprite_name;
    in >> sprite_name;
    if(in.eof())
      return;

    sprites[sprite_name.c_str()]->read_sprite(in);
  }
}


void sprite::draw(int offx, int offy, phys_obj* who) {
  if(!ra_pixels.empty()) {
    for(list<pixel>::iterator i = ra_pixels.begin(); i != ra_pixels.end();  i++) {
      move_cursor(offy + i->y, offx + i->x);
      color_code(i->color);
      paint(i->character, who);
    }
    return;
  }

  bool jump = true; //we need to jump to the first location
  for(int y = 0; y < ysize; y++) {
    for(int x = 0; x < xsize; x++) {
      if(characters[x][y] == '\t') {
        jump = true;
      } else {
				int drawx = offx + x, drawy = offy + y;
				
				if(drawx < min_col() || drawx > max_col() || drawy < min_row() || drawy > max_row()) {
					jump = true;
					continue;
				}
				
        if(jump)
          move_cursor(drawy, drawx);
        color_code(colors[x][y]);
				paint(characters[x][y], who);
				jump = false;
      }
    }
    jump = true; // new line -- must move
  }
}

/*sprite::~sprite() {
	if(characters != NULL) {
		for(int x = 0; x < xsize; x++) {
			delete[] characters[x];
			delete[] colors[x];
		}
		delete[] characters;
		delete[] colors;
	}
	//ra_pixels.clear();
}*/
