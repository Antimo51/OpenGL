
/* Recursive subdivision of triangle */
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <math.h>
#endif

/* initial triangle */

GLfloat v[3][2]={{-1.0, -0.58}, {1.0, -0.58}, {0.0, 1.15}};

int n;
float twist;

void triangle( GLfloat *a, GLfloat *b, GLfloat *c, int type)

/* display one triangle  */
{
    GLfloat v[2];
    double d;
    glBegin(GL_POLYGON);
       switch(type)
       {
       case(2):
       case(4):
       case(6):
       case(7):
          glEdgeFlag(GL_FALSE);
       break;
       default:
          glEdgeFlag(GL_TRUE);
       }
       d=sqrt(a[0]*a[0]+a[1]*a[1]);
       v[0] = cos(twist*d)*a[0]-sin(twist*d)*a[1];
       v[1] = sin(twist*d)*a[0]+cos(twist*d)*a[1];
       glVertex2fv(v);
       switch(type)
       {
       case(3):
       case(4):
       case(5):
       case(7):
          glEdgeFlag(GL_FALSE);
       break;
       default:
          glEdgeFlag(GL_TRUE);
       }
       d=sqrt(b[0]*b[0]+b[1]*b[1]);
       v[0] = cos(twist*d)*b[0]-sin(twist*d)*b[1];
       v[1] = sin(twist*d)*b[0]+cos(twist*d)*b[1];
       glVertex2fv(v);
       switch(type)
       {
       case(1):
       case(5):
       case(6):
       case(7):
          glEdgeFlag(GL_FALSE);
       break;
       default:
          glEdgeFlag(GL_TRUE);
       }
       d=sqrt(c[0]*c[0]+c[1]*c[1]);
       v[0] = cos(twist*d)*c[0]-sin(twist*d)*c[1];
       v[1] = sin(twist*d)*c[0]+cos(twist*d)*c[1];
       glVertex2fv(v);
    glEnd();
}

void divide_triangle(GLfloat *a, GLfloat *b, GLfloat *c, int m, int k)
{

/* triangle subdivision using vertex numbers */

    GLfloat v[3][2];
    int j, flag[4];
    if(m>0) 
    {
        for(j=0; j<2; j++) v[0][j]=(a[j]+b[j])/2;
        for(j=0; j<2; j++) v[1][j]=(b[j]+c[j])/2;
        for(j=0; j<2; j++) v[2][j]=(a[j]+c[j])/2;
        switch(k)
        {
        case(0):
           flag[0]=3;
           flag[1]=1;
           flag[2]=2;
        break;
        case(1):
           flag[0]=5;
           flag[1]=1;
           flag[2]=6;
        break;
        case(2):
           flag[0]=4;
           flag[1]=6;
           flag[2]=2;
        break;
        case(3):
           flag[0]=3;
           flag[1]=5;
           flag[2]=4;
        break;
        case(4):
           flag[0]=4;
           flag[1]=7;
           flag[2]=4;
        break;
        case(5):
           flag[0]=5;
           flag[1]=5;
           flag[2]=7;
        break;
        case(6):
           flag[0]=7;
           flag[1]=6;
           flag[2]=6;
        break;
        case(7):
           flag[0]=7;
           flag[1]=7;
           flag[2]=7;
        break;
        }
           flag[3]=7;
        divide_triangle(a, v[0], v[2], m-1, flag[0]);
        divide_triangle(v[0], b, v[1], m-1, flag[1]);
        divide_triangle(v[2], v[1], c, m-1, flag[2]);
        divide_triangle(v[0], v[1], v[2], m-1, flag[3]);
    }
    else(triangle(a,b,c, k)); /* draw triangle at end of recursion */
}


void display(void)
{


    glClear(GL_COLOR_BUFFER_BIT);
	divide_triangle(v[0], v[1], v[2], n, 0);
    glFlush();
}

void myinit()
{

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
    glClearColor (1.0, 1.0, 1.0, 1.0);
	glColor3f(0.0,0.0,0.0);
   glPolygonMode(GL_FRONT, GL_LINE);
}

int
main(int argc, char **argv)
{
    n=2;
    twist = 2.0;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(500, 500);
    glutCreateWindow("Edge Flag Test");
    glutDisplayFunc(display);
    myinit();
    glutMainLoop();
}
