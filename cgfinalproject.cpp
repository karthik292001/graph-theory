//#include "stdafx.h"
//#include "stdafx.h"
#include<stdlib.h>
#include<iostream>
#include<GL/glut.h>
#include<math.h>
#include<cstring>
#include<bits/stdc++.h>
using namespace std;
#define V 5
int parent[V];
void find_spanning_tree();
void find_spanning_tree_kruskal();
void bitmap_output(int x, int y, char *string, void *font);
//Global variables
int pointer=0,point=0,line=0,lpointer=0,upointer=-1,undo_cost_stack_pointer=-1;
int costmatrix[100][100],undo_cost_stack[100],lineundostack[100][100];
int undone=0,nf=0,f=0,found=0,nodenum=1,load=0;
int t[100][2],totalcost;
bool paused=false;
bool start=false,enterpressed=false,resdisplay=false,show_algorithmp=false,show_algorithmk=false,back=false,show_help=false,show_flowchart=false;
char undostack[100];
char num[2];
char text[15]="Draw mode:NONE";
char nodemsg[15]="";
float sx=-1,sy=-1;
char choice='K';
int ch=0;
float oldx[100][100],linepoints[2][2],linex[100][4];
float fade=0.1;
GLfloat width=1000,height=720;
//This functions writes the integer into char array
void int_str(int rad,char r[])
{
	sprintf(r,"%d",rad);
}
//whenever undo function is called the cost is pushed into the stack
void push(int n){
	undo_cost_stack[++undo_cost_stack_pointer]=n;
}
//when redo function is called the cost is popped out
int pop(){
	if(undo_cost_stack_pointer>=0){
		return undo_cost_stack[undo_cost_stack_pointer--];
	}
	return 999;
}
//delay function
void delay()
{
	int i=290000000;
	while(i--);
}
//this function displays message when node created
void msg_output()
{
  glColor3f(1.0,0.0,0.0);
  bitmap_output(30,20,"Node created",GLUT_BITMAP_HELVETICA_18);
  delay();
  delay();
  glFlush();//glutSwapBuffers();
}
void maximum_node_msg_output(){
  glColor3f(1.0,0.0,0.0);
  bitmap_output(30,20,"Maximum number of nodes reached",GLUT_BITMAP_HELVETICA_18);
  glFlush();//glutSwapBuffers();
  delay();
  delay();
  delay();
  delay();
}
//this function creates nodes using location stored in oldx[][] array
void drawpoint(){
    float angle=4.0f*3.1416f/360;
	int radius=25;
	for(int i=0;i<pointer;i++){
	glBegin(GL_POINTS);
	glColor3f(0,0,1);
	double angle1=0.0;
	glBegin(GL_POLYGON);
	glVertex2d(oldx[i][0]-radius*cos(0.0),radius*sin(0.0)+oldx[i][1]);
	for(int j=0;j<360;j++){
    glVertex2d(oldx[i][0]-radius*cos(angle1),radius*sin(angle1)+oldx[i][1]);
	angle1+=angle;
    }
    glEnd();
	}
	for(int i=0;i<pointer;i++){
	glColor3f(1,0,0);
	int_str(nodenum,num);
	nodenum++;
	bitmap_output(oldx[i][0]-5,oldx[i][1]-6,num,GLUT_BITMAP_TIMES_ROMAN_24);
	}
    /*	if(nodenum>=1){
		msg_output();
	}*/
}
//This function prints text in graphics window
void bitmap_output(int x, int y, char *string, void *font)
{
  int len, i;
  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(font, string[i]);
  }
}
/*void delay()
{
	int i=290000000;
	while(i--);
}
void msg_output()
{
  glColor3f(1.0,0.0,0.0);
  bitmap_output(30,20,"Node created",GLUT_BITMAP_HELVETICA_18);
  glFlush();//glutSwapBuffers();
  delay();
  delay();
 }*/
//this function displays introduction window
void introduction_page()
{
glBegin(GL_POLYGON);
glColor3f(1.0,0.0,0.0);
glVertex2d(0,0);
glColor3f(0.732,0.560,0.560);
glVertex2d(0,height);
glColor3f(0.732,0.560,0.560);
glVertex2d(width,height);
glColor3f(0.0,0.0,1.0);
glVertex2d(width,0);
glEnd();
	glLineWidth(2);
	glColor3f(0.5,1.0,1.0);
	bitmap_output(width/3.1, height-height/10, "SDM Institute of Technology,  Ujire.",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(width/4.0, height-height/6, "Department Of Computer Science and Engineering.",GLUT_BITMAP_TIMES_ROMAN_24);
	glColor3f(0.5,1.0,0.0);
	bitmap_output(width/4+80, height-height/4.5, "Computer Graphics Lab with Mini Project",GLUT_BITMAP_HELVETICA_18);
	bitmap_output(width/4+98, height-height/3.7, "MINIMUM COST SPANNING TREE",GLUT_BITMAP_HELVETICA_18);
	glColor3f(0.0,0.0,1.0);
	bitmap_output(width/4+58, height-height/3.7, "<<<<                                                            >>>>",GLUT_BITMAP_HELVETICA_18);
	glColor3f(0.0,0.0,0.0);
	bitmap_output(width/15+10,height-height/2.2,"Made by -",GLUT_BITMAP_HELVETICA_18);
    glColor3f(1.0,1.0,1.0);
	bitmap_output(width/15+10,height-height/1.9,"Hithaish N (4SU19CS034)",GLUT_BITMAP_HELVETICA_18);
	bitmap_output(width/15+10,height-height/1.7,"Jai Ganesh (4SU19CS035)",GLUT_BITMAP_HELVETICA_18);
	bitmap_output(width/15+10,height-height/1.55,"Karthik (4SU19CS036)",GLUT_BITMAP_HELVETICA_18);
	bitmap_output(width/15+10,height-height/1.42,"Karthik M C (4SU19CS037)",GLUT_BITMAP_HELVETICA_18);
	glColor3f(0.0,0.0,0.0);
	bitmap_output(width/15+595,height-height/1.8,"Guided by -",GLUT_BITMAP_HELVETICA_18);
	glColor3f(1.0,1.0,1.0);
	bitmap_output(width/15+595,height-height/1.6,"Mr. Pradeep.Rao.K.B",GLUT_BITMAP_TIMES_ROMAN_24);
	glFlush();
	delay();
	glColor3f(0.0,1.0,0.0);
	bitmap_output(width/5+270, 60, "Press 'ENTER' to continue",GLUT_BITMAP_TIMES_ROMAN_24);
	glColor3f(1.0,0.0,0.0);
	bitmap_output(width/5+400, 60, "                         >>>>>",GLUT_BITMAP_TIMES_ROMAN_24);
	glFlush();
	delay();
}
//this function displays instruction window
void instruction_page()
{
glBegin(GL_POLYGON);
glColor3f(0.38,0.17,1.0);
glVertex2d(0,0);
glColor3f(0.9,0.72,0.9);
glVertex2d(0,height);
glColor3f(0.9,0.72,0.9);
glVertex2d(width,height);
glColor3f(0.38,0.17,1.0);
glVertex2d(width,0);
glEnd();
    glColor3f(1.0,0.0,0.0);
	bitmap_output(100, height-120, "INSTRUCTIONS --",GLUT_BITMAP_TIMES_ROMAN_24);
	glColor3f(0.0,0.0,0.0);
	bitmap_output(320, height-220, "* Press 'S' --> For Simulation. ",GLUT_BITMAP_TIMES_ROMAN_24);
    bitmap_output(320, height-420, "* Press 'H' -->  For HELP.  ",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(320, height-270, "* Press 'F' --> For Flowchart. ",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(320, height-320, "* Press 'W' --> Prim's algorithm. ",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(320, height-370, "* Press 'T' --> Kruskal's algorithm. ",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(320, height-470, "* Press 'ESC' --> To EXIT. ",GLUT_BITMAP_TIMES_ROMAN_24);
	glColor3f(1.0,1.0,0.0);
	bitmap_output(300, height-600, "***************~#~**************",GLUT_BITMAP_TIMES_ROMAN_24);
}
//this function displays draw instructions  windwow
void draw_instructions()
{
	glBegin(GL_POLYGON);
    glColor3f(0.48,0.41,1.0);
    glVertex2d(0,0);
    glColor3f(1.0,0.43,0.68);
	glVertex2d(0,height);
	glColor3f(1.0,0.43,0.68);
	glVertex2d(width,height);
	glColor3f(0.48,0.41,1.0);
	glVertex2d(width,0);
	glEnd();
	glColor3f(1.0,1.0,0.0);
	bitmap_output(10, height-height/15, "Instructions --",GLUT_BITMAP_TIMES_ROMAN_24);
	glColor3f(0.0,1.0,0.0);
	glLineWidth(1.0);
	glBegin(GL_LINE_LOOP); 
		glVertex2f(10, height-height/13);
		glVertex2f(130, height-height/13);
	glEnd();
	glColor3f(0.0,0.0,0.0);
	bitmap_output(80, height-height/8.2, "* Press 'N' --> To Create Nodes",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(80,height-height/5.9, "* Press 'E' --> To draw Edges",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(80, height-height/4.6, "* Press 'P' --> To simulate spanning tree using prim's algorithm",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(80, height-height/3.8, "* Press 'K' --> To simulate spanning tree using Kruskal's algorithm",GLUT_BITMAP_TIMES_ROMAN_24);
	glColor3f(0.10,1.0,1.0);
	bitmap_output(130, height-height/3.0, "Place cursor below and click to create nodes, then press E to draw edges. ",GLUT_BITMAP_TIMES_ROMAN_24);
	glColor3f(0.85,0.85,0.85);
	glBegin(GL_POLYGON);
	glVertex2d(30,60);
	glVertex2d(30,450);
	glVertex2d(970,450);
	glVertex2d(970,60);
    glFlush();
	glEnd();
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINE_LOOP);
	glVertex2d(30,60);
	glVertex2d(30,450);
	glVertex2d(970,450);
	glVertex2d(970,60);
    glFlush();
	glEnd();
	glColor3f(1.0,0.0,0.0);
	bitmap_output(650, height-700, "<<<<Press 'B' to go Back",GLUT_BITMAP_HELVETICA_18);
}
//displays help window
void help_page()
{
	glBegin(GL_POLYGON);
	glColor3f(0.9,0.82,0.9);
    glVertex2d(0,0);
	glColor3f(0.132,0.560,0.560);
	glVertex2d(0,height);
	glColor3f(0.132,0.560,0.560);
	glVertex2d(width,height);
    glColor3f(0.9,0.82,0.9);
	glVertex2d(width,0);
	glEnd();
	glColor3f(1.0,1.0,0.0);
	bitmap_output(70, height-100, "Descriptions --",GLUT_BITMAP_TIMES_ROMAN_24);
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0,0.0,0.0);
	glVertex2d(200,170);
	glVertex2d(200,550);
	glVertex2d(850,550);
	glVertex2d(850,170);
	glFlush();
	glEnd();
	glBegin(GL_LINES);
	glVertex2d(200,500);
	glVertex2d(850,500);
	glFlush();
	glEnd();
	glBegin(GL_LINES);
	glVertex2d(450,550);
	glVertex2d(450,170);
	glFlush();
	glEnd();
    glColor3f(1.0,0.0,0.0);
	bitmap_output(300, height-205, "Keys",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(584, height-205, "Functions",GLUT_BITMAP_TIMES_ROMAN_24);
	glColor3f(0.0,0.0,0.61);
	bitmap_output(300, height-280, "S or s                         - To start Simulation",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(300, height-310, "F or f                         - To see the Flowchart",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(300, height-340, "W or w                      - About prim's algorithm",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(300, height-370, "T or t                         - About kruskal's algorithm",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(300, height-400, "H or h                        - For Help ",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(300, height-430, "B or b                         - To go back to previous page",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(300, height-460, "Z or z                         - To Undo",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(300, height-490, "Y or y                         - To Redo",GLUT_BITMAP_TIMES_ROMAN_24);
	bitmap_output(300, height-520, "ESC                           - To Exit",GLUT_BITMAP_TIMES_ROMAN_24);
	glColor3f(0.0,0.0,0.0);
bitmap_output(width/15+650, height-680, "<<<<Press 'B' to go back",GLUT_BITMAP_HELVETICA_18);
glColor3f(1.0,0.0,1.0);
bitmap_output(420, height-630, "*****~#~*****",GLUT_BITMAP_TIMES_ROMAN_24);
}
//displays prim's algorithm
void algorithmp()
{ 
glBegin(GL_POLYGON);
glColor3f(0.65,0.65,0.65);
glVertex2d(0,0);
glColor3f(0.0,0.138,0.96);
glVertex2d(0,height);
glColor3f(0.65,0.65,0.65);
glVertex2d(width,height);
glColor3f(0.65,0.65,0.65);
glVertex2d(width,0);
glEnd();
glColor3f(0.0,1.0,1.0);
bitmap_output(width/15+25, height-100, "Steps involved in Prims Algorithm......",GLUT_BITMAP_TIMES_ROMAN_24);
glColor3f(0.0,0.0,0.0);
bitmap_output(width/15+25, height-180, "Step 1:Create a mstSet that keeps track of vertices already included in MST. ",GLUT_BITMAP_HELVETICA_18);
bitmap_output(width/15+25, height-220, "Step 2:Initialize all key values as INFINITE. Assign the key value as 0 for the first vertex so",GLUT_BITMAP_HELVETICA_18);
bitmap_output(width/15+75, height-260, "  that it is picked first.",GLUT_BITMAP_HELVETICA_18);
bitmap_output(width/15+25, height-300, "Step 3:While mstSet doesn't include all vertices. ",GLUT_BITMAP_HELVETICA_18);
bitmap_output(width/15+85, height-340, "   a)Pick a vertex u which is not there in mstSet and has a minimum key value. ",GLUT_BITMAP_HELVETICA_18);
bitmap_output(width/15+85, height-380, "   b)Include u to mstSet. ",GLUT_BITMAP_HELVETICA_18);
bitmap_output(width/15+85, height-420, "   c)Update key value of all adjacent vertices of u.  ",GLUT_BITMAP_HELVETICA_18);
glColor3f(0.0,1.0,0.0);
bitmap_output(width/15+300, height-540, "**********~#~**********",GLUT_BITMAP_TIMES_ROMAN_24);
glColor3f(1.0,0.0,0.0);
bitmap_output(width/15+650, height-650, "<<<<Press 'B' to go back",GLUT_BITMAP_HELVETICA_18);
}
//displays kruskal's algorithm 
void algorithmk()
{
glBegin(GL_POLYGON);
glColor3f(0.65,0.65,0.65);
glVertex2d(0,0);
glColor3f(0.0,0.138,0.96);
glVertex2d(0,height);
glColor3f(0.65,0.65,0.65);
glVertex2d(width,height);
glColor3f(0.65,0.65,0.65);
glVertex2d(width,0);
glEnd();
glColor3f(0.0,1.0,1.0);
bitmap_output(width/15+25, height-150, "Steps involved in Kruskal's Algorithm......",GLUT_BITMAP_TIMES_ROMAN_24);
glColor3f(0.0,0.0,0.0);
bitmap_output(width/15+25, height-230, "Step 1: Sort all edges in increasing order of their edge weights. ",GLUT_BITMAP_TIMES_ROMAN_24);
bitmap_output(width/15+25, height-270, "Step 2: Pick the smallest edge.",GLUT_BITMAP_TIMES_ROMAN_24);
bitmap_output(width/15+25, height-310, "Step 3: Check if the new edge creates a cycle or loop in a spanning tree.",GLUT_BITMAP_TIMES_ROMAN_24);
bitmap_output(width/15+25, height-350, "Step 4: If it doesn’t form the cycle, then include that edge in MST.",GLUT_BITMAP_TIMES_ROMAN_24);
bitmap_output(width/15+95, height-390, "Otherwise, discard it.",GLUT_BITMAP_TIMES_ROMAN_24);
bitmap_output(width/15+25, height-430, "Step 5: Repeat from step 2 until it includes |V| - 1 edges in MST.",GLUT_BITMAP_TIMES_ROMAN_24);
glColor3f(0.0,1.0,0.0);
bitmap_output(width/15+300, height-540, "**********~#~**********",GLUT_BITMAP_TIMES_ROMAN_24);
glColor3f(1.0,0.0,0.0);
bitmap_output(width/15+650, height-650, "<<<<Press 'B' to go back",GLUT_BITMAP_HELVETICA_18);
}
//flowchart
void flowchart(){
glLineWidth(2);
glBegin(GL_POLYGON);
glColor3f(0.45,0.85,0.85);
glVertex2d(0,0);
glColor3f(0.0,0.13,0.96);
glVertex2d(0,height);
glColor3f(0.0,0.13,0.96);
glVertex2d(width,height);
glColor3f(0.45,0.85,0.85);
glVertex2d(width,0);
glEnd();
glColor3f(0.0,1.0,1.0);
bitmap_output(80, height-60, "FLOW CHART --",GLUT_BITMAP_TIMES_ROMAN_24);
//introduction polygon
glColor3f(0.9,0.72,0.9);
glBegin(GL_POLYGON);
glVertex2i(395,600);
glVertex2i(600,600);
glVertex2i(600,560);
glVertex2i(395,560);
glEnd();
glColor3f(0.0,0.0,0.0);
bitmap_output(425, 575, "Introduction page",GLUT_BITMAP_HELVETICA_18);
glColor3f(1.0,1.0,1.0);
glBegin(GL_LINES);
glVertex2i(497.5,560);
glVertex2i(497.5,505);
glEnd();
//instruction polygon
glColor3f(0.9,0.72,0.9);
glBegin(GL_POLYGON);
glVertex2i(395,505);
glVertex2i(600,505);
glVertex2i(600,465);
glVertex2i(395,465);
glEnd();
glColor3f(0.0,0.0,0.0);
bitmap_output(430,480, "Instruction page",GLUT_BITMAP_HELVETICA_18);
glColor3f(1.0,1.0,1.0);
glBegin(GL_LINES);
glVertex2f(498,440);
glVertex2f(498,465);
glEnd();
//prim's algorithm polygon
glColor3f(0.9,0.72,0.9);
glBegin(GL_POLYGON);
glVertex2i(90,420);
glVertex2i(210,420);
glVertex2i(210,350);
glVertex2i(90,350);
glEnd();
glColor3f(0.0,0.0,0.0);
bitmap_output(125, 390, "Prim's",GLUT_BITMAP_HELVETICA_18);
bitmap_output(110, 365, "Algorithm",GLUT_BITMAP_HELVETICA_18);
//horizontal line
glColor3f(1.0,1.0,1.0);
glBegin(GL_LINES);
glVertex2f(150,440);
glVertex2f(940,440);
glEnd();
glColor3f(1.0,1.0,1.0);
glBegin(GL_LINES);
glVertex2f(150,440);
glVertex2f(150,420);
glEnd();
//flowchart polygon
glColor3f(0.9,0.72,0.9);
glBegin(GL_POLYGON);
glVertex2i(235,405);
glVertex2i(375,405);
glVertex2i(375,365);
glVertex2i(235,365);
glEnd();
glColor3f(0.0,0.0,0.0);
bitmap_output(260, 380, "Flow Chart",GLUT_BITMAP_HELVETICA_18);
glColor3f(1.0,1.0,1.0);
glBegin(GL_LINES);
glVertex2f(305,440);
glVertex2f(305,405);
glEnd();
//start simulation polygon
glColor3f(0.9,0.72,0.9);
glBegin(GL_POLYGON);
glVertex2i(395,405);
glVertex2i(600,405);
glVertex2i(600,365);
glVertex2i(395,365);
glEnd();
glColor3f(0.0,0.0,0.0);
bitmap_output(430, 380, "Start Simulation",GLUT_BITMAP_HELVETICA_18);
glColor3f(1.0,1.0,1.0);
glBegin(GL_LINES);
glVertex2f(498,440);
glVertex2f(498,405);
glEnd();
//kruskal's algorithm polygon
glColor3f(0.9,0.72,0.9);
glBegin(GL_POLYGON);
glVertex2i(650,420);
glVertex2i(780,420);
glVertex2i(780,350);
glVertex2i(650,350);
glEnd();
glColor3f(0.0,0.0,0.0);
bitmap_output(680, 390, "Kruska's",GLUT_BITMAP_HELVETICA_18);
bitmap_output(675, 365, "Algorithm",GLUT_BITMAP_HELVETICA_18);
glColor3f(1.0,1.0,1.0);
glBegin(GL_LINES);
glVertex2f(715,440);
glVertex2f(715,420);
glEnd();
//help polygon
glColor3f(0.9,0.72,0.9);
glBegin(GL_POLYGON);
glVertex2i(800,405);
glVertex2i(880,405);
glVertex2i(880,365);
glVertex2i(800,365);
glEnd();
glColor3f(0.0,0.0,0.0);
bitmap_output(823, 380, "Help",GLUT_BITMAP_HELVETICA_18);
glColor3f(1.0,1.0,1.0);
glBegin(GL_LINES);
glVertex2f(840,440);
glVertex2f(840,405);
glEnd();
//exit polygon
glColor3f(0.9,0.72,0.9);
glBegin(GL_POLYGON);
glVertex2i(900,405);
glVertex2i(980,405);
glVertex2i(980,365);
glVertex2i(900,365);
glEnd();
glColor3f(0.0,0.0,0.0);
bitmap_output(927, 380, "Exit",GLUT_BITMAP_HELVETICA_18);
glColor3f(1.0,1.0,1.0);
glBegin(GL_LINES);
glVertex2f(940,440);
glVertex2f(940,405);
glEnd();

glColor3f(1.0,1.0,1.0);
glBegin(GL_LINES);
glVertex2f(498,365);
glVertex2f(498,275);
glEnd();
//simulation polygon
glColor3f(0.9,0.72,0.9);
glBegin(GL_POLYGON);
glVertex2i(395,275);
glVertex2i(600,275);
glVertex2i(600,175);
glVertex2i(395,175);
glEnd();
glColor3f(0.0,0.0,0.0);
bitmap_output(453, 250, "Draw Node",GLUT_BITMAP_HELVETICA_18);
bitmap_output(453, 220, "Draw Edge",GLUT_BITMAP_HELVETICA_18);
bitmap_output(453, 190, "Draw Graph",GLUT_BITMAP_HELVETICA_18);
glColor3f(1.0,1.0,1.0);
glBegin(GL_LINES);
glVertex2f(498,175);
glVertex2f(498,145);
glEnd();
//output polygon
glColor3f(0.9,0.72,0.9);
glBegin(GL_POLYGON);
glVertex2i(395,145);
glVertex2i(600,145);
glVertex2i(600,105);
glVertex2i(395,105);
glEnd();
glColor3f(0.0,0.0,0.0);
bitmap_output(470, 120, "Output",GLUT_BITMAP_HELVETICA_18);
glColor3f(1.0,1.0,1.0);
glBegin(GL_LINES);
glVertex2f(498,105);
glVertex2f(498,75);
glEnd();

glColor3f(1.0,1.0,1.0);
glBegin(GL_LINES);
glVertex2f(600,55);
glVertex2f(940,55);
glEnd();

glColor3f(1.0,1.0,1.0);
glBegin(GL_LINES);
glVertex2f(940,55);
glVertex2f(940,365);
glEnd();
//Exit polygon
glColor3f(0.9,0.72,0.9);
glBegin(GL_POLYGON);
glVertex2i(395,75);
glVertex2i(600,75);
glVertex2i(600,35);
glVertex2i(395,35);
glEnd();
glColor3f(0.0,0.0,0.0);
bitmap_output(485,50, "Exit",GLUT_BITMAP_HELVETICA_18);

glColor3f(1.0,0.0,0.0);
bitmap_output(90, height-680, "<<<<Press 'B' to go Back",GLUT_BITMAP_HELVETICA_18);
}
//this function draws edges
void drawline(){
	glColor3f(1.0,0.0,1.0);
	glLineWidth(4.0);
	char* cst = new char[5];
	int temp;
	glBegin(GL_LINES);
	for(int i=0;i<lpointer;i++){
	glVertex2i(linex[i][0],linex[i][1]);
	glVertex2i(linex[i][2],linex[i][3]);
	}
	glEnd();
	glColor3f(0.0,1.0,0.0);
	for(int i=0;i<lpointer;i++){
		temp=costmatrix[lineundostack[i][0]][lineundostack[i][1]];
		sprintf(cst, "%d", temp);
		bitmap_output((linex[i][0]+linex[i][2])/2-15,(linex[i][1]+linex[i][3])/2+15,cst,GLUT_BITMAP_TIMES_ROMAN_24);
	}
	glFlush();//glutSwapBuffers();
}
//this displays a point when mouse clicked for first time in draw edge mode
void drawPointAt(float x,float y){
	glPointSize(15);
	glColor3f(1.0,1.0,0.0);
	glBegin(GL_POINTS);
	glVertex2i(x,y);
	glEnd();

}
//this function displays output after calculating spanning tree
void output(int ch){
glBegin(GL_POLYGON);
glColor3f(0.48,0.41,1.0);
glVertex2d(0,0);
glColor3f(0.0,0.0,0.0);
glVertex2d(0,height);
glColor3f(0.0,0.0,0.0);
glVertex2d(width,height);
glColor3f(0.48,0.41,1.0);
glVertex2d(width,0);
glEnd();
glColor3f(0.85,0.85,0.85);
	glBegin(GL_POLYGON);
	glVertex2d(30,60);
	glVertex2d(30,450);
	glVertex2d(970,450);
	glVertex2d(970,60);
    glFlush();
	glEnd();
    int xaxis = 250,yaxis=550;
    glColor3f(1.0,1.0,0.0);
	if(ch==0){
		bitmap_output(250,650,"Simulation of spanning tree using Kruskal's Algorithm",GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else{
		bitmap_output(250,650,"Simulation of spanning tree using Prim's Algorithm",GLUT_BITMAP_TIMES_ROMAN_24);
	}
	glColor3f(0.0,1.0,0.0);
	bitmap_output(50,600,"OUTPUT --",GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(1.0,0.0,1.0);
	bitmap_output(xaxis,yaxis,"Costs of Minimum spanning tree :",GLUT_BITMAP_HELVETICA_18);
	char* buffer=new char[50];
	char* cst = new char[50];
	char* a=new char[50];
	char* b = new char[50];
	for(int i=0;i<pointer-1;i++){
		sprintf(a, "%d", t[i][0]);
		strcat(a,"->");
		sprintf(b, "%d", t[i][1]);
		strcat(a,b);
		sprintf(cst, "%d", costmatrix[t[i][0]][t[i][1]]);
		strcat(a," = ");
		strcat(a,cst);
		yaxis-=20;
		bitmap_output(xaxis,yaxis,a,GLUT_BITMAP_HELVETICA_18);
	}
	char sumchar[50];
	sprintf(buffer,"%d", totalcost);
	strcat(sumchar,"spanning tree cost=");
	strcat(sumchar,buffer);
	bitmap_output(xaxis,yaxis-20,sumchar,GLUT_BITMAP_HELVETICA_18);
	glColor3f(0.0,0.0,0.0);
    //output graph
    //Node Drawing
    float angle=4.0f*3.1416f/360;
	int radius=25;
	for(int i=0;i<pointer;i++){
	glBegin(GL_POINTS);
	glColor3f(0,1,0);
	double angle1=0.0;
	glBegin(GL_POLYGON);
	glVertex2d(oldx[i][0]-radius*cos(0.0),radius*sin(0.0)+oldx[i][1]);
	for(int j=0;j<360;j++){
    glVertex2d(oldx[i][0]-radius*cos(angle1),radius*sin(angle1)+oldx[i][1]);
	angle1+=angle;
	}
	glEnd();
	}
	nodenum=0;  //added part
	for(int i=0;i<pointer;i++){
	glColor3f(1,0,0);
	int_str(nodenum,num);
	nodenum++;
	bitmap_output(oldx[i][0]-5,oldx[i][1]-6,num,GLUT_BITMAP_TIMES_ROMAN_24);
	}
    //EDGE DRAWING
    delay();
	glLineWidth(3.0);
	for(int i=0;i<pointer-1;i++){
	glColor3f(0.0,0.0,1.0);
	glBegin(GL_LINES);
	glVertex2i(oldx[t[i][0]][0],oldx[t[i][0]][1]);
	glVertex2i(oldx[t[i][1]][0],oldx[t[i][1]][1]);
	delay();
	glEnd();
	glFlush();//glutSwapBuffers();
	}   
}
//This is callback function called by OpenGL 
void display(){
	nodenum=0;
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0,0.0,0.2,0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,width,0,height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(!start){
		if(enterpressed)
		{
		   instruction_page();
		   if(show_algorithmp){
		     algorithmp();		   
		 }
		   else if(show_algorithmk)
		   {
			   algorithmk();
		   }
		   else if(show_help){
			 help_page();
		}
		   else if(show_flowchart){
			   flowchart();
		   }
		}
		else{
		//glLineWidth(25);
		introduction_page();
		}	
	}
	else if(start){
			if(!found){
				draw_instructions();
				glColor3f(0.1,1,0.0);
				//bitmap_output(width-width/2+200,height-height/9,text,GLUT_BITMAP_HELVETICA_18);  //DRAW MODE
	 		}
	       glPointSize(40);
		   drawpoint();
		   if(sx!=-1&&sy!=-1){
	       drawPointAt(sx,sy);
	      }
		  if(!found)
	      drawline();
	if(found){
	output(ch);
	//blinking_lines();
	}
	}
	glFlush();//glutSwapBuffers();
}
//this is reshape callback function called by OpenGL whenever window is resized
void reshape(int w, int h)
{
	width=w;
	height=h;
	glViewport(0, 0,(GLsizei) w,(GLsizei) h);
   glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
	if(w<=h) 
		glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h/ (GLfloat) w, 2.0* (GLfloat) h / (GLfloat) w, -10.0, 10.0);
	else 
		glOrtho(-2.0 * (GLfloat) w/ (GLfloat) h, 2.0* (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
}
//this function takes cost input from user during edge drawing
void input(){
	int posi=-1,posj=-1;
	int cost;
	int center_difference_error = 100;
	for(int i=0;i<pointer;i++){
		if(oldx[i][0]>=(linepoints[0][0]-center_difference_error)&&oldx[i][0]<=(linepoints[0][0]+center_difference_error)){
			if(oldx[i][1]>=(linepoints[0][1]-center_difference_error)&&oldx[i][1]<=(linepoints[0][1]+center_difference_error)){
		       posi=i;
			   break;}
	    }
	}
	for(int i=0;i<pointer;i++){
		if(oldx[i][0]>=(linepoints[1][0]-center_difference_error)&&oldx[i][0]<=(linepoints[1][0]+center_difference_error)){
			if(oldx[i][1]>=(linepoints[1][1]-center_difference_error)&&oldx[i][1]<=(linepoints[1][1]+center_difference_error)){
		       posj=i;
			break;
			}
	    }
	}
	if((posi==posj)||posi==-1||posj==-1){
		glColor3f(1.0,0.0,0.0);
		bitmap_output(20,50,"Invalid node",GLUT_BITMAP_HELVETICA_18);
		glFlush();//glutSwapBuffers();
		delay();
		delay();
	lpointer--;
	}else{
    cout<<"Enter the cost:";
	cin>>cost;
	lineundostack[lpointer][0]=posi;
	lineundostack[lpointer][1]=posj;
	costmatrix[posi][posj]=costmatrix[posj][posi]=cost;
	}
}
//this is mouse callback function called by OpenGL whenever mouse event occurs
void mousefun(int button,int state,int x,int y){
	found=0;
	if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN){
		if(start){
		glLoadIdentity();
		float xp=(float)x;
		float yp=height-(float)y;
		
		if(point){
			if(nodenum>=7){
				maximum_node_msg_output();
			}else{
			//msg_output();
		    oldx[pointer][0]=xp;
		    oldx[pointer++][1]=yp;
		    undostack[++upointer]='p';
		    msg_output();
			}
		}
        if(line){
			if(f==0){
			linex[lpointer][0]=xp;
			linex[lpointer][1]=yp;
			f=1;
			linepoints[0][0]=sx=xp;
			linepoints[0][1]=sy=yp;
			}
			else{
			linex[lpointer][2]=linepoints[1][0]=xp;
			linex[lpointer][3]=linepoints[1][1]=yp;
			input();
			lpointer++;
			f=0;
			sx=sy=-1;
			undostack[++upointer]='l';
			}
		}
		glutPostRedisplay();
	  }
	}
}
//this function will print matrix constructed
void printmatrix(){
	for(int i=0;i<pointer;i++){
		for(int j=0;j<pointer;j++){
		cout<<costmatrix[i][j]<<"  ";
		}
		cout<<endl;
	}
}
//this checks if the graph is connected or not
bool isconnected(){
	int count;
	for(int i=0;i<pointer;i++){
	count=0;
	for(int j=0;j<pointer;j++){
	if(costmatrix[i][j]==999)count++;
	}
	if(count>=pointer-1){
		return false;
	}
	}
	return true;
}
//this undo the drawing in the order the graph was drawn
void undo(){
	if(pointer>0&&undostack[upointer--]=='p'){
			 pointer--;
			 undone++;
	} else if(lpointer>0){
			 lpointer--;
			 push(costmatrix[lineundostack[lpointer][0]][lineundostack[lpointer][1]]);
			 costmatrix[lineundostack[lpointer][0]][lineundostack[lpointer][1]]=999;
			 costmatrix[lineundostack[lpointer][1]][lineundostack[lpointer][0]]=999;
		     undone++;
	}
}
//this redo the drawing in the order the graph was drawn
void redo(){
	if(undone){
	      if(undostack[++upointer]=='p'){
					 pointer++;
					 undone--;
		   }
		 else{
			 int  cost=pop();
		     costmatrix[lineundostack[lpointer][0]][lineundostack[lpointer][1]]=cost;
			 costmatrix[lineundostack[lpointer][1]][lineundostack[lpointer][0]]=cost;
			 lpointer++;
             undone--;
		   }
       }
}
//this will call find_spanning_tree function if graph is connected
void find(char choice){
	if(isconnected()){
		load=0;
		if(choice=='K' || choice=='k' )
		find_spanning_tree_kruskal();
		else if(choice=='P' || choice=='p'){
			find_spanning_tree();
		}
	 }
	else
	 {
		glColor3f(1.0,0.0,0.0);
		bitmap_output(30,20,"Graph is not connected",GLUT_BITMAP_HELVETICA_18);
		glFlush();//glutSwapBuffers();
		delay();
		delay();
	//	cout<<"spanning tree doesn't exist"<<endl;
	 }
}
//this will select edge draw mode 
void selectedge(){
     strcpy(text,"Draw mode: Edge");
	 line=1;
     point=0;
}
//this will select node draw mode 
void selectnode(){
	strcpy(text,"Draw mode: Node");
    point=1;
	line=0;
}
//this is keyboard callback function called by OpenGL whenever keyboard event occurs
void keyboardfun(unsigned char key,int x,int y){
	sx=-1;sy=-1;
	switch(key){
	     case 13  : found=0;
			        enterpressed=true;
			        break;
	     case 's' :
		 case 'S' :found=0;
			       start=true;
			       break;
		 
	     case 'b' :
		 case 'B' :start=false;
			       show_help=false;
			       show_algorithmp=false;
				   show_algorithmk=false;
				   show_flowchart=false;
			       back=true;
				   break;
		 case 'e':
		 case 'E':found=0;
			      selectedge();
			      break;
		 case 'N':
		 case 'n':found=0;
			      selectnode();
			      break;
		 
		 case 'Z':
		 case 'z':found=0;
			      undo();
				  break;
		 
		 case 'Y':
		 case 'y':found=0;
			      redo();
			      break;
		case 'K':
		 case 'k':found=0;
			      find('K');
			      break;
		case 'P':
		 case 'p':found=0;
			      find('P');
			      break;	
		 case 'w':
		 case 'W':
			      show_algorithmp=true;
				  show_help=false;
				  show_flowchart=false;
			      break;
		 case 'h':
		 case 'H':show_help=true;
			      show_algorithmp=false;
			      show_flowchart=false;
			      break;
		 case 'f':
		 case 'F':show_flowchart=true;
			      show_algorithmp=false;
				  show_help=false;
			      break;
		 case 't':
		 case 'T':show_algorithmk=true;
				  show_help=false;
				  show_flowchart=false;
			      break;
		 case ' ':paused=!paused;
			      break;
		 case 27 :exit(0);
	}
		glutPostRedisplay();
}
//this is menu callback function called by OpenGL as menu function
void menu(int id){
	switch(id){
	case 0:find(choice);
		   break;
	case 1:undo();
		   break;
    case 2:redo();
		   break;
    case 3:printmatrix();
		   break;
	case 4:point=1;
		   line=0;
		   strcpy(text,"Draw Mode: Node");
		   break;
    case 5:point=0;
		   line=1;
		   strcpy(text,"Draw Mode: Edge");
		   break;
	}
	glutPostRedisplay();

}
//kruskal's
int findKruskal(int i)
{
    while (parent[i] != i)
        i = parent[i];
    return i;
}
 // Does union of i and j. It returns
// false if i and j are already in same
// set.
void union1(int i, int j)
{
    int a = findKruskal(i);
    int b = findKruskal(j);
    parent[a] = b;
}
//this function calculates spanning tree using kruskal's algorithm
void find_spanning_tree_kruskal(){
	int mincost = 0; // Cost of min MST.
	int k=0;
	ch=0;
    // Initialize sets of disjoint sets.
	bitmap_output(10,560,"Kruskal",GLUT_BITMAP_HELVETICA_18);
    for (int i = 0; i < V; i++)
        parent[i] = i;
	// Include minimum weight edges one by one
    int edge_count = 0;
    while (edge_count < V - 1) {
        int min = INT_MAX, a = -1, b = -1;
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (findKruskal(i) != findKruskal(j) && costmatrix[i][j] < min) {
                    min = costmatrix[i][j];
                    a = i;
                    b = j;
                }
            }
        }
        union1(a, b);
		t[k][0]=a;
		t[k++][1]=parent[a];
		printf("Kruskal");
        printf("Edge %d:(%d, %d) cost:%d \n",
               edge_count++, a, b, min);
        mincost += min;
    }
	found=1;
	totalcost=mincost;
    printf("\n Minimum cost= %d \n", mincost);
}
//this function calculates spanning tree using Prim's algorithm
void find_spanning_tree(){
	int k,u,v;
	int sum,min,source;
	ch=1;
	int* p = new int[100];
	int* d = new int[100];
	int* s = new int[100];
	min=999;
	source=0;
	for(int i=0;i<pointer;i++){
	   for(int j=0;j<pointer;j++){
		   if(costmatrix[i][j]!=0 && costmatrix[i][j]<=min){
			   min=costmatrix[i][j];
			   source=i;
		   }
	}
	}
    for(int i=0;i<pointer;i++){
		d[i]=costmatrix[source][i];
		p[i]=source;
		s[i]=0;
	}
	s[source]=1;
	sum=0;
	k=0;
	for(int i=1;i<pointer;i++){
	min=999;
	u=-1;
	for(int j=0;j<pointer;j++){
		if(s[j]==0){
			if(d[j]<min){
			min=d[j];
			u=j;
			}
		}
	}
	if(u==-1)return;
	t[k][0]=u;
	t[k++][1]=p[u];
	sum=sum+costmatrix[u][p[u]];
	s[u]=1;
	for(v=0;v<pointer;v++){
		if(s[v]==0 && costmatrix[u][v]<d[v]){
		d[v]=costmatrix[u][v];
		p[v]=u;
		}
	}
	}
	if(sum>=999){
	cout<<"spanning tree doesn't exist\n";
	}else{
	totalcost=sum;
	found=1;
	}
}
//Program execution starts from here
int main(int argc,char** argv){
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++)
		if(i==j){
		costmatrix[i][j]=0;
		}
		else costmatrix[i][j]=999;
	}
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowPosition(250,50);
	glutInitWindowSize(width,height);
	glutCreateWindow("Minimum Cost Spanning Tree");
	glutDisplayFunc(display);
	glutMouseFunc(mousefun);
	glutKeyboardFunc(keyboardfun);
	glutReshapeFunc(reshape);
	glutCreateMenu(menu);
	glutAddMenuEntry("Run",0);
	glutAddMenuEntry("Undo",1);
	glutAddMenuEntry("redo",2);
	glutAddMenuEntry("Draw node",4);
	glutAddMenuEntry("Draw Edge",5);
	glutAddMenuEntry("Print matrix",3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
	return 0;
}