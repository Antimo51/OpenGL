// SimplePolygon.c :
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POLYGON);
		glColor3f(1.0,0.0,0.0);	
		glVertex2f(-0.5,-0.5);
		glColor3f(0.0,1.0,0.0);
		glVertex2f(-0.5,0.5);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(0.5,0.5);
		glColor3f(1.0,1.0,0.0);
		glVertex2f(0.5,-0.5);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(1.0,0.0,0.0);	
		glVertex2f(-0.5,-0.5);
		glColor3f(0.0,1.0,0.0);
		glVertex2f(-0.5,0.5);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(0.5,0.5);
		glColor3f(1.0,1.0,0.0);
		glVertex2f(0.5,-0.5);
	glEnd();
	
	glFlush();
	
}

void init()
{
	//background
	glClearColor(0.0,0.0,0.0,0.0);

	//frontground
	glColor3f(1.0,1.0,1.0);

	//polygon mode
//	glPolygonMode(GL_BACK,GL_POINT);
//	glPolygonMode(GL_BACK,GL_LINE);
	glPolygonMode(GL_BACK,GL_FILL);		//default FILL, both front face and back face

	//clip
	glEnable(GL_CULL_FACE);
	//coordinate: left hand
	glFrontFace(GL_CCW);		//default
//	glFrontFace(GL_CW);

	// clip mode
//	glCullFace(GL_BACK);
	glCullFace(GL_FRONT);
//	glCullFace(GL_FRONT_AND_BACK);

	//Shade mode
	glShadeModel(GL_SMOOTH);	//default
//	glShadeModel(GL_FLAT);

}

int main(int argc, char* argv[])
{
	 
    glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Simple");

	glutDisplayFunc(display);
    
	init();

	glutMainLoop();
        
   
	return 0;
}

