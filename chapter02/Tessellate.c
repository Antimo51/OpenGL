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
int flag;

void triangle( GLfloat *a, GLfloat *b, GLfloat *c, int type)
/* display one triangle  */
{
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
       if(flag) glEdgeFlag(GL_TRUE);
       
       glVertex2fv(a);
       
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
       if(flag) glEdgeFlag(GL_TRUE);

       glVertex2fv(b);
       
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
       if(flag) glEdgeFlag(GL_TRUE);
       
       glVertex2fv(c);
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
    } else
    {
    	triangle(a, b, c, k); /* draw triangle at end of recursion */
    }
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
//    n=atoi(argv[1]);
//    flag=atoi(argv[2]);
    
    n = 4;
    flag = 1;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(500, 500);
    glutCreateWindow("Tessellate");
    
    glutDisplayFunc(display);
    
    myinit();
    
    glutMainLoop();
}
