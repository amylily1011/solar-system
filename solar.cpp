/*
 * texture2.cpp
 *
 * - light position is at the sun
 * - the sun emits light (emission material)
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include <iostream>
#include <stdlib.h>

#define ANIMATE 0

int day = 180;
int mode = ANIMATE;

int worldx=500;
int worldy=750;
int worldh=3000;
int floorh=-5;

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

float deltaAngle = 0.0,ratio;
float deltaVAngle = 0.0;
float x=0.0f,y=1.75f,z=5.0f;
float lx=0.0f,ly=0.0f,lz=-1.0f;

//GLuint toTextures[2];       // Two texture objects
int iRenderMode = 3;          // Sphere Mapped is default

GLUquadricObj* disk;

bool   gp;                      // G Pressed? ( New )
GLuint filter;                      // Which Filter To Use
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };   // Storage For Three Types Of Fog
GLuint fogfilter= 0;                    // Which Fog To Use
GLfloat fogColor[4]= {0.2f, 0.2f, 0.2f, 0.4f};      // Fog Color

const int ww = 1000, hh = 670, numTextures = 10;
void keyboard(unsigned char key, int x, int y);
void init();
void display(void);
void reshape(GLsizei w, GLsizei h);
void idle();
void drawAxis();
void LoadGLTextures();
int LoadTextureRAW (const char * filename, int isWrap, int w, int h, int iTextures);
void FreeTexture();
void RenderScene(void);
void fog(float a);

GLuint	texture[numTextures];				// Storage Textures

float angle = 0.0;


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (ww, hh);
	glutCreateWindow ("The Planet");
	init();
    
   // glutDisplayFunc(RenderScene);

    
    glutDisplayFunc(display);
    
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
	//RenderScene();
    glutMainLoop();
	FreeTexture();
    
	return 0;
}

void init()
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 1.0 };
    
    disk = gluNewQuadric();
	gluQuadricDrawStyle(disk, GLU_FILL);
    
	LoadGLTextures();
	
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
    
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 10);
    
	glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_specular);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); 

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LESS);								// The Type Of Depth Test To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
}

void keyboard (unsigned char key, int x, int y)
{
    
    switch (key) {
        case 't': 
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0,5,0, 0,0,0, -1,0,0);	// top view
            glutPostRedisplay(); break;
        case 'f': 
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // front view
            glutPostRedisplay(); break;
        case 'g': fog(0.1f);glutPostRedisplay(); break; 
        case 'p': fog(0.0f);glutPostRedisplay();break;    
        case 's': 
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(5,0,0 , 0,0,0, 0,1,0);	// side view
            glutPostRedisplay(); break;
        case 27:  exit(0); break;
        default:  mode = 1 - mode;   break;
    }
  
  
    
}
void reshape (int w, int h)
{
   /* 
	glViewport (0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
    if (w <= h)
        glFrustum(-1.0, 1.0, -1.0 * (GLfloat) h / (GLfloat) w,
                  1.0 * (GLfloat) h / (GLfloat) w, 1, 10.0);
    else
        glFrustum(-1.0 * (GLfloat) w / (GLfloat) h,
                  1.0 * (GLfloat) w / (GLfloat) h, -1.0, 1.0, 1, 10.0);
 

    glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
 
 */
    glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(80.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  

}
void RenderScene(){
    
    GLfloat brick_ambdiff[] =  {1,1, 1, 1 };
    
    // SKY
    glPushMatrix();
    //  glRotatef(90, 0, 0, 1);
    // glRotatef(90 ,0, 1, 0);
    glEnable(GL_TEXTURE_2D);	
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, brick_ambdiff);
    glMaterialf(GL_FRONT, GL_SHININESS, 0);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
     glVertex3f(worldy*5, worldx*10+5, worldh);
     glVertex3f(-worldy*5, worldx*10+5, worldh);
     glVertex3f(-worldy*5, -worldx*10+5, worldh);
     glVertex3f(worldy*5, -worldx*10+5, worldh);
    glEnd();			
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();	

}


void display(void)
{
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
    
	GLfloat mat_emission[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_noemis[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_noemis_1[] = { 0.0, 1.0,1.0, 1.0};
    
    //GLfloat mat_noemis_2[] = { 0.0, 0.5,0.5, 1.0};
    
	static float rot = 0.0;
	rot += 0.5;
    
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();
    
	GLUquadricObj *pObj = gluNewQuadric();				// Get a new Quadric off the stack
	gluQuadricTexture(pObj, true);						// This turns on texture coordinates for our Quadrics
    
	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix();
    //***draw sun
    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glRotatef(angle*2,0,1,0);		// rotate around itself
    glRotatef(90,1,0,0);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(5.0, 1.0, 1.0);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    gluSphere(pObj, 1.3f, 20, 20);						// sun
    glDisable(GL_TEXTURE_2D);							// Disable Texture Mapping
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(rot/2, 0,1,0);		// rotate around itself			
    glTranslatef(2,0,0);
  
   // fog();
    
    //***draw earth
    glPushMatrix();
    glRotatef(rot, 0,1,0);	// rotate around itself			
    glTranslatef (-2.0, 0.0, 0.0);

    glRotatef(90,1,0,0);	// rotate so that the earth equation is along x-axis
    glRotatef(90,0,0,1);	// rotate so that the earth equation is along x-axis
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1, 1, 1);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_noemis);
    glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    gluSphere(pObj, 0.5, 20, 16);
    glDisable(GL_TEXTURE_2D);							// Disable Texture Mapping
    glPopMatrix();
    
  
    //***black ball
    glPushMatrix();
    glRotatef(rot, 0,1,0);	// rotate around itself			
    glTranslatef (-5.0, 0.0, 0.0);

    glRotatef(90*1.5,1,0,0);	// rotate so that the earth equation is along x-axis
    glRotatef(90,0,0,1);	// rotate so that the earth equation is along x-axis
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1, 1, 1);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_noemis_1);
    glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    gluSphere(pObj, 0.5, 20, 16);
    glDisable(GL_TEXTURE_2D);							// Disable Texture Mapping
    glPopMatrix();
    
    //***draw 1st moon
    glPushMatrix();
    glRotatef(angle, -1,0,1);
    glRotatef(-45,0,1,0);
    glTranslatef(1,0,0);
    glRotatef(angle, 1,0,1);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_noemis);

    glColor3f(1, 1, 1);
    gluSphere(pObj, 0.1, 20, 16);
    glDisable(GL_TEXTURE_2D);							
    glPopMatrix();
    
    
    //draw 2nd moon
    glPushMatrix();
    glRotatef(angle, 1,0,1);
    glRotatef(45,0,1,0);
    glTranslatef(1,0,0);
    
    glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_noemis);

    glColor3f(1, 1, 1);
    gluSphere(pObj, 0.2, 20, 16);
    glDisable(GL_TEXTURE_2D);							// Disable Texture Mapping
    glPopMatrix();
    glPopMatrix();
	glPopMatrix();
    
    // planet 2 : mars
    
    glPushMatrix();
    glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);  // rotate around the sun
    glTranslatef (-3.0, 0.0, 0.0);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_noemis);

    glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1, 1, 1);
    gluSphere(pObj, 0.2, 20, 16);
    glDisable(GL_TEXTURE_2D);
 

    
    
    glRotatef (20, 1.0, 0.0, 0.0);   
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3f (1.0, 0.62, 0.14);				
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    gluDisk(disk, 0.25, 0.35, 20, 4);		// draw mars's ring

    glPopMatrix();
	
    
        
    glPopMatrix();
    
    
    
	glutSwapBuffers();
	gluDeleteQuadric(pObj);								// Free the Quadric
}

void fog( float a){


    glClearColor(a,a,a,a*2);          // We'll Clear To The Color Of The Fog ( Modified )
    

    
    glFogi(GL_FOG_MODE, fogMode[fogfilter]);        // Fog Mode
    glFogfv(GL_FOG_COLOR, fogColor);            // Set Fog Color
    glFogf(GL_FOG_DENSITY, a*0.35f);              // How Dense Will The Fog Be
    glHint(GL_FOG_HINT, GL_NICEST);          // Fog Hint Value
    glFogf(GL_FOG_START, 1.0f);             // Fog Start Depth
    glFogf(GL_FOG_END, 5.0f);               // Fog End Depth
    glEnable(GL_FOG);                   // Enables GL_FOG

   
}
//********************************
// Load files for Texture Mapping
//********************************
void LoadGLTextures()
{
	LoadTextureRAW("image/sunsun.raw", true, 640, 640, 0);
	LoadTextureRAW("image/earthmap.raw", true, 1024, 512, 1);
	LoadTextureRAW("image/neptune.raw", true, 128, 128, 2);
	LoadTextureRAW("image/mercury.raw", true, 128, 256, 3);
    LoadTextureRAW("image/jupiter.raw", true, 128, 128, 4);
    LoadTextureRAW("image/milky.raw", true,1280, 1291, 5);
   // LoadTextureRAW("galaxy.raw", true,900, 600, 5);    


}

void idle()
{
	angle += 0.2;			// spin the earth 0.2 degree around y-axis
    if (mode == ANIMATE) {
		usleep(20000);
		day++;
        //	if (day%720 == 0)
        
        if (day==720)
            day = 0;
	}
    
	glutPostRedisplay();
}

//*******************************************************
// Texture Mapping Function load .RAW file as a texture
//*******************************************************
int LoadTextureRAW (const char * filename, int isWrap, int width, int height, int iTextures)
{
    char * data;
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
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
    
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
