/*
 *  line.h
 *  epilogue
 *
 */
#ifndef LINE_H
#define LINE_H

#include "phys_obj.h"

class line : phys_obj {
protected:
  float dx, dy;
  
  
public:
  enum draw_style { jagged, smooth };
  
  line(float x, float y, float dx, float dy, draw_style style);
  
  virtual void draw();
  virtual void effective_loc(float oth_x, float oth_y, float & eff_x, float & eff_y);
  virtual void line::collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push);
};


#endif