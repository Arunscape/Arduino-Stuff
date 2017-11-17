/*
 Handles cursor movement
 */

#ifndef _JOY_CURSOR_H
#define _JOY_CURSOR_H

void joy_setup();
void processJoystick();
int calcCursorPixelX();
int calcCursorPixelY();
void redrawMapFromMenu(int x, int y);


#endif
