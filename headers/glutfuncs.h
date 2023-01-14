#pragma once

#include <Glut/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "strmng.h"

#define SLDR_RADIUS 0.03
typedef struct {         // button structure
	float pos[2];   	// position
	float dim[2];   	// dimentions
	int is_pressed; 	// pressed or not
	char text[100]; 	// button text
	int id;
} button;

typedef struct {      // slider that changes double value
	float pos[2];
	float length;
	float val;           // position of the slider
	int is_pressed;      // pressed or not
	int id;              // slider ID
	double min;
	double max;
	double *dvar;         // pointer to variable that it changes
	int *ivar;
} slider;

void btn_init(float px, float py, float dx, float dy, char *text, int id, button *btn) { // initialize button
	btn->pos[0] = px;
	btn->pos[1] = py;
	btn->dim[0] = dx;
	btn->dim[1] = dy;
	int i;
	btn->id = id;
	for(i = 0; text[i] != '\0'; i++)
		btn->text[i] = text[i];
	btn->text[i] = '\0';
}

void sldr_init(float px, float py, float len, int id, double min, double max, double *dvar, int *ivar, slider *sldr) {   // initialize integer slider 
	sldr->pos[0] = px;
	sldr->pos[1] = py;
	sldr->length = len;
	sldr->id = id;
	sldr->min = min;
	sldr->max = max;
    sldr->ivar = ivar;
	sldr->dvar = dvar;
	sldr->is_pressed = 0;

}
int btnh(float x, float y, int state, button *btn) {    // button handler: goes in:  m(); where glutMouseFunc(m);
	if(x > btn->pos[0] && x < btn->pos[0] + btn->dim[0] && y > btn->pos[1] && y < btn->pos[1] + btn->dim[1]) {
		btn->is_pressed = (state == GLUT_DOWN) ? 1 : 0;
		return (state == GLUT_DOWN) ? btn->id : -1;
	}
	else
		btn->is_pressed = 0;
	return -1;
}

void sldrh(float x, float y, slider *sldr) {   // slider handler: goes in:  m(); where glutMotionFunc(m);
	float x2 = (sldr->val)*(sldr->length) + sldr->pos[0];
	float y2 = sldr->pos[1];
	float v;
	if(GLUT_LEFT_BUTTON == GLUT_DOWN) {
		if((x-x2)*(x-x2) + (y-y2)*(y-y2) < SLDR_RADIUS*SLDR_RADIUS) {
				sldr->is_pressed = 1;
		}
	}
	v = ((x-sldr->pos[0])/(sldr->length));
	if(sldr->is_pressed) {
		if(v > 1)
			sldr->val = 1;	
		else if(v < 0)
			sldr->val = 0;
		else
			sldr->val = v;

		if(sldr->dvar != NULL)
			*(sldr->dvar) = (sldr->val)*(sldr->max - sldr->min) + sldr->min;
		if(sldr->ivar != NULL)
			*(sldr->ivar) = (sldr->val)*(sldr->max - sldr->min) + sldr->min;
	}
}
void sldrh2(int state, slider *sldr) {    // second slider handler: goes in:  m(); where glutMouseFunc(m);
	if(state != GLUT_DOWN)
		sldr->is_pressed = 0;
}

int displaystring(float x, float y, float length, char s[], int cursor, int bufp){ //x, y: position on screen   length: maximum length of single line   s[]: string   cursor: cursor on or off   bufp: cursor position.
	float nx = x;
	int c = 0;
	char wrd[1000];
	for(int i = 1; simplewordf(s,i,wrd) != 0; i++){
		if(!iswhitespace(wrd[0]) && x+len(wrd)*0.02-nx > length*0.02 && x > nx && length > 0){
			x = nx;
			y-=0.045;  
		}
		for(int a = 0; wrd[a] != '\0'; a++){
			if(wrd[a] == '\n' || (x-nx > (length-1)*0.02 && length > 0 && !iswhitespace(wrd[a]))){
				if(wrd[a] == '\n' && c == bufp && cursor){
					glRasterPos2f(x,y);
					glutBitmapCharacter(GLUT_BITMAP_9_BY_15,'_');
				}
				x = nx;
				y-=0.045;
			}
			if(wrd[a] == '\t') {
				if(wrd[a] == '\t' && c == bufp && cursor){
					glRasterPos2f(x,y);
					glutBitmapCharacter(GLUT_BITMAP_9_BY_15,'_');
				}
				x += 0.08;
			}
			else if(wrd[a] != '\n'){
				glRasterPos2f(x,y);
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(bufp == c && cursor) ? '_' : wrd[a]);
				x+=(x-nx > (length-1)*0.02 && length > 0 && iswhitespace(wrd[a])) ? 0 : 0.02;
			}
			c++;
		}
	}
	if(bufp == c && cursor){
		glRasterPos2f(x,y);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,'_');
	}
	return(y);
}
void circle(float x, float y, float r){
	glBegin(GL_POLYGON);
	for(float A = 0; A < 6.28; A+=0.01)
		glVertex2f(cos(A)*r+x,sin(A)*r+y);
	glEnd();
}
void draw_btn(button *btn) {    // draws button btn
	glColor3f(0.7,0.7,0.7);
	float offset = (btn->is_pressed) ? 0 : -0.005;
	glBegin(GL_QUADS);
	glVertex2f(btn->pos[0],btn->pos[1]);
	glVertex2f(btn->pos[0]+btn->dim[0],btn->pos[1]);
	glVertex2f(btn->pos[0]+btn->dim[0],btn->pos[1]+btn->dim[1]);
	glVertex2f(btn->pos[0],btn->pos[1]+btn->dim[1]);

	glColor3f(0.5,0.5,0.5);
	glVertex2f(btn->pos[0]+offset,btn->pos[1]+offset);
	glVertex2f(btn->pos[0]+btn->dim[0]+offset,btn->pos[1]+offset);
	glVertex2f(btn->pos[0]+btn->dim[0]+offset,btn->pos[1]+btn->dim[1]+offset);
	glVertex2f(btn->pos[0]+offset,btn->pos[1]+btn->dim[1]+offset);
	glEnd();
	glColor3f(0,0,0);
	displaystring(btn->pos[0]+0.005+offset, btn->pos[1]+0.5*(btn->dim[1]) + offset, btn->dim[0]/0.02, btn->text, 0,0);
}
void draw_sldr(slider *sldr) {   //draws slider sldr
	glColor3f(0.7,0.7,0.7);
	glBegin(GL_LINES);
	glVertex2f(sldr->pos[0],sldr->pos[1]);
	glVertex2f(sldr->pos[0]+sldr->length,sldr->pos[1]);
	glEnd();
	circle(sldr->pos[0] + (sldr->length)*(sldr->val), sldr->pos[1], SLDR_RADIUS);
}


void rectangle(float x1, float y1, float x2, float y2) {
	glBegin(GL_POLYGON);
	glVertex2f(x1,y1);
	glVertex2f(x1,y2);
	glVertex2f(x2,y2);
	glVertex2f(x2,y1);
	glEnd();
}