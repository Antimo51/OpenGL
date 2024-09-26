// Simple.cpp :
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
		glVertex2f(-0.5,-0.5);
		glVertex2f(-0.5,0.5);
		glVertex2f(0.5,0.5);
		glVertex2f(0.5,-0.5);
	glEnd();

	// glutSwapBuffers();
	glFlush();
}


int main(int argc, char* argv[])
{
	// glut库初始化 
    glutInit(&argc, argv);
	// 设定窗口显示模式
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
	// 指定窗口大小、位置，创建窗口
    glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 0);
	glutCreateWindow("Simple");

	// 注册事件处理函数：注册回调函数
	glutDisplayFunc(display);
    
	// 循环检查事件队列，执行相应回调函数
	glutMainLoop();

	return 0;
}
