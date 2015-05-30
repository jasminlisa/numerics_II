#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glx.h>

GLuint base;
void buildFont() {
    Display *dpy;
    XFontStruct *fontInfo;
    base = glGenLists(96);
    dpy = XOpenDisplay(NULL);
    fontInfo = XLoadQueryFont(dpy, "-misc-fixed-medium-r-normal--10-100-75-75-c-60-iso8859-1");
    if (fontInfo == NULL) {
	fontInfo = XLoadQueryFont(dpy, "fixed");
	if (fontInfo == NULL) {
	    printf("no X font available?\n");
	}
    }
    glXUseXFont(fontInfo->fid, 32, 96, base);
    XFreeFont(dpy, fontInfo);
    XCloseDisplay(dpy);
}

void deleteFont() {
	glDeleteLists(base, 96);
}

void printGLs(char *text) {
	if (text == NULL){
		return;
	}
	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}

void printGLd(double x, float xCoord, float yCoord) {
	char* str = malloc(sizeof(char)*10);
	glPushMatrix();
	glTranslatef(xCoord,yCoord,0.0f);	
	glRasterPos3f(0.0f, 0.0f, 0.0f);
	snprintf (str, 100, "%f",x);
	printGLs(str);
	glPopMatrix();
	free(str);
}
