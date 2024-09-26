#include<windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>


int menu=0, submenu1=0, submenu2;

//画一个绿色多边形
void draw(){
    glClearColor(0,0,0,0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(0,1,0);
    glOrtho(-1,1,-1,1,-1,1);
    
    glBegin(GL_POLYGON);
        glVertex2f(-0.5,-0.5);
        glVertex2f(-0.5,0.5);
        glVertex2f(0.5,0.5);
        glVertex2f(0.5,-0.5);
    glEnd();
    
    glutSwapBuffers();
}

//点击某个条目时，输出该条目属于哪个菜单
void getcurrentmenu(){
    int nmenu;
    nmenu = glutGetMenu();
    if(nmenu == menu)
        printf("current is main menu.\n");
    else if(nmenu == submenu1)
        printf("current is sub menu 1.\n");
    else if(nmenu == submenu2)
        printf("current is sub menu 2.\n");
}

//第一个子菜单
void submenufunc1(int data){
    getcurrentmenu();
    switch(data){
        case 1:
            printf("sub menu 1's item 1 is triggered.\n");
            break;
        case 2:
            printf("sub menu 1's item 2 is triggered.\n");
            break;
        case 3:
            printf("sub menu 1's item 3 is triggered.\n");
            break;
    }
}
//第二个子菜单
void submenufunc2(int data){
    getcurrentmenu();
    switch(data){
        case 1:
            printf("sub menu 2's item 1 is triggered.\n");
            break;
        case 2:
            printf("sub menu 2's item 2 is triggered.\n");
            break;
        case 3:
            printf("sub menu 2's item 3 is triggered.\n");
            break;
    }
}

//主菜单
void menufunc(int data){
    getcurrentmenu();
    switch(data){
        case 1:
            printf("menu's item 1 is triggered.\n");
            break;
        case 2:
            printf("menu's item 2 is triggered.\n");
            break;
        case 3:
            printf("menu's item 3 is triggered.\n");
            break;
    }
}

int main(int argc,char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(512,512);
    glutCreateWindow("test");
    glutDisplayFunc(draw);

    //构建子菜单1的内容
    submenu1 = glutCreateMenu(submenufunc1);
    glutAddMenuEntry("1's item 1",1);
    glutAddMenuEntry("1's item 2",2);
    glutAddMenuEntry("1's item 3",3);
    glutAttachMenu(GLUT_LEFT_BUTTON);
    
    //构建子菜单2的内容
    submenu2 = glutCreateMenu(submenufunc2);
    glutAddMenuEntry("2's item 1",1);
    glutAddMenuEntry("2's item 2",2);
    glutAddMenuEntry("2's item 3",3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //构建主菜单的内容
    menu = glutCreateMenu(menufunc);
    glutAddMenuEntry("item 1",1);
    glutAddMenuEntry("item 2",2);
    glutAddMenuEntry("item 3",3);
    
    //将两个菜单变为另一个菜单的子菜单
    glutAddSubMenu("sub1",submenu1);
    glutAddSubMenu("sub2",submenu2);
    //点击鼠标右键时显示菜单
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
