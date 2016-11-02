/*
 * texture3.c
 *
 * - test texture with the brick wall, wood wall, and marble floor
 *
 */

#include <GLUT/glut.h>
#include <stdio.h>	
#include <string.h>
#include <math.h>
//#include <stdio.h>
//#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <stdbool.h>
#include<stdlib.h>
#include <time.h>
#include<unistd.h>


int LoadTextureRAW (const char * filename, int isWrap, int width, int height, int iTextures);
void FreeTexture();
void bitmap_output(int x,int y, void *font, char *s);

const int numTextures = 3;					// total texture images
const int ww = 1000, hh = 670;
float angle = 0.0;
float blend_color[] = {1.0, 1.0, 1.0, 1.0};

GLuint	texture[numTextures];				// Storage Textures
GLUquadricObj *pObj ;//= gluNewQuadric();		// Get a new Quadric off the stack

// list all texture images we want to use in the program here
void LoadGLTextures()
{
	LoadTextureRAW("marble1.raw", true, 128, 128, 0);
	LoadTextureRAW("brick.raw", true, 256, 256, 1);
	LoadTextureRAW("wood.raw", true, 512, 256, 2);
}

void init()
{
    pObj=gluNewQuadric();
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 2.0, 2.0, 2.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    
	LoadGLTextures();
	
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);		
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    
	glEnable(GL_DEPTH_TEST);						
	glDepthFunc(GL_LESS);								// The Type Of Depth Test To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    
	gluQuadricTexture(pObj, true);		// This turns on texture coordinates for our Quadrics
}

void reshape (int w, int h)
{
	glViewport (0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(60.0,(GLfloat)w/(GLfloat)h, 1, 100.0);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

void display(void)
{
	GLfloat brick_ambdiff[] = { 0.6, 0.4, 0.2, 1.0 };
	GLfloat mat_red[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat mat_white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
    
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();
    
	gluLookAt (0.0, 0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
	glRotatef(angle, 0, 1, 0);
	glPushMatrix();
    // back brick wall
    glPushMatrix();
    glRotatef(90, 0, 0, 1);
    glEnable(GL_TEXTURE_2D);	
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, brick_ambdiff);
    glMaterialf(GL_FRONT, GL_SHININESS, 0);
    
    // Note that OpenGL origin is at the lower left, but texture origin is at upper left.
    // Therefore, we have to map the mirror image of the texture to the Quad.
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0.0, 0.0); glVertex3f(5.0, 5.0, 0.0);
    glTexCoord2f(0.0, 2.0); glVertex3f(-5.0, 5.0, 0.0);
    glTexCoord2f(2.0, 2.0); glVertex3f(-5.0, -5.0, 0.0);
    glTexCoord2f(2.0, 0.0); glVertex3f(5.0, -5.0, 0.0);
    glEnd();			
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    // left wood wall
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);	
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(3.0, 0.0); glVertex3f(-5.0, 5.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, 5.0, 5.0);
    glTexCoord2f(0.0, 9.0); glVertex3f(-5.0, -5.0, 5.0);
    glTexCoord2f(3.0, 9.0); glVertex3f(-5.0, -5.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);	
    glPopMatrix();
    
    //  marble floor
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);	
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_white);
    glMaterialf(GL_FRONT, GL_SHININESS, 100);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0, 0.0); glVertex3f(5.0, -5.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(-5.0, -5.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(-5.0, -5.0, 5.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(5.0, -5.0, 5.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);	
    glPopMatrix();
    
    glTranslatef(0, 0, 1);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_red);
    glMaterialf(GL_FRONT, GL_SHININESS, 100);
    glutSolidSphere(1.0, 20, 20);
    
	glPopMatrix();
	glutSwapBuffers();
}

void idle()
{
	angle += 0.2;
	if (angle == 360)
		angle = 0;
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (ww, hh);
	glutCreateWindow("Texture 3");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

void bitmap_output(int x,int y, void *font, char *s) 
{
	int len,i;
    
	glRasterPos2f(x,y);
	len = (int) strlen(s);
	for (i = 0; i < len; i++) 
		glutBitmapCharacter(font,s[i]); 
}

int LoadTextureRAW (const char * filename, int isWrap, int width, int height, int iTextures)
{
    char* data;
    FILE * file;
    
    file = fopen( filename, "rb" );				// open texture data
    if ( file == NULL ) 
		return 0;
	data = (char *) malloc(width * height * 3); // allocate buffer
    fread( data, width * height * 3, 1, file ); // read texture data
    fclose(file);
    
    glGenTextures( 1, &texture[iTextures] );	// allocate a texture name
    
    // select our current texture
    glBindTexture( GL_TEXTURE_2D, texture[iTextures] );
    
    // select modulate to mix texture with the color of the object
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    
    // when texture area is small, bilinear filter the closest mipmap
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    isWrap ? GL_REPEAT : GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    isWrap ? GL_REPEAT : GL_CLAMP );
    
    // build our texture mipmaps
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,
                      GL_RGB, GL_UNSIGNED_BYTE, data );
    
    // free buffer
    free( data );
    
    return iTextures;
}

void FreeTexture()
{
	int i;
	for (i=0; i<numTextures; i++)
		glDeleteTextures( 1, &texture[i] );
}