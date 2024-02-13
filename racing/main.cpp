#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
//Game Speed
int FPS = 20;
//Game Track
int start=0;
int gv=0;
int level = 0;
//Track Score
int score = 0;
//Form move track
int roadDivTopMost = 0;
int roadDivTop = 0;
int roadDivMdl = 0;
int roadDivBtm = 0;
//For Card Left / RIGHT
int lrIndex = 0 ;
//Car Coming
int car1 = 0;
int lrIndex1=0;
int car2 = +35;
int lrIndex2=0;
int car3 = +70;
int lrIndex3=0;
//For Display TEXT
const int font1= GLUT_BITMAP_TIMES_ROMAN_24;
const int font2= GLUT_BITMAP_HELVETICA_18 ;
const int font3= GLUT_BITMAP_8_BY_13;
char s[30];
void renderBitmapString(float x, float y, void *font,const char *string){
    const char *c;
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}
void tree(int x, int y){
    int newx = x;
    int newy = y;

    // Tree Left (Bottom)
    glColor3f(0.498, 0.847, 0.847); // Light Cyan
    glBegin(GL_TRIANGLES);
    glVertex2f(newx + 11, newy + 55);
    glVertex2f(newx + 12, newy + 55 - 10);
    glVertex2f(newx + 10, newy + 55 - 10);
    glEnd();

    // Tree Left (Top)
    glColor3f(0.545, 0.000, 0.545); // Dark Magenta
    glBegin(GL_TRIANGLES);
    glVertex2f(newx + 11, newy + 55 + 3);
    glVertex2f(newx + 12 + 3, newy + 55 - 3);
    glVertex2f(newx + 10 - 3, newy + 55 - 3);
    glEnd();
}

void drawRoadDividers(int x, int y) {
    // Draw road dividers
    glColor3f(1.000, 0.647, 0.000); // Orange
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivTop + 80);
    glVertex2f(48, roadDivTop + 100);
    glVertex2f(52, roadDivTop + 100);
    glVertex2f(52, roadDivTop + 80);
    glEnd();

    glColor3f(1.000, 1.000, 0.000); // Yellow
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivMdl + 40);
    glVertex2f(48, roadDivMdl + 60);
    glVertex2f(52, roadDivMdl + 60);
    glVertex2f(52, roadDivMdl + 40);
    glEnd();

    glColor3f(1.000, 1.000, 0.000); // Yellow
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivBtm + 0);
    glVertex2f(48, roadDivBtm + 20);
    glVertex2f(52, roadDivBtm + 20);
    glVertex2f(52, roadDivBtm + 0);
    glEnd();
}


void updateRoadDivisions() {
    roadDivTop--;
    roadDivMdl--;
    roadDivBtm--;
    if (roadDivTop < -100) {
        roadDivTop = 20;
        score++;
    }
    if (roadDivMdl < -60) {
        roadDivMdl = 60;
        score++;
    }
    if (roadDivBtm < -20) {
        roadDivBtm = 100;
        score++;
    }
}

void drawScoreBoard() {
    glBegin(GL_POLYGON);
    glVertex2f(80, 97);
    glVertex2f(100, 97);
    glVertex2f(100, 90);
    glVertex2f(80, 90);
    glEnd();
}

void printScore() {
    char buffer[50];
    sprintf(buffer, "SCORE: %d", score);
    glColor3f(0.000, 1.000, 0.000); // Green
    renderBitmapString(80.5, 95, (void *)font3, buffer);
}

void printSpeed() {
    char buffer[50];
    sprintf(buffer, "SPEED:%dKm/h", FPS);
    glColor3f(0.000, 1.000, 0.000); // Green
    renderBitmapString(80.5, 93, (void *)font3, buffer);
}

void printLevel() {
    if (score % 50 == 0) {
        int last = score / 50;
        if (last != level) {
            level = score / 50;
            FPS += 2;
        }
    }
    char level_buffer[50];
    sprintf(level_buffer, "LEVEL: %d", level);
    glColor3f(0.000, 1.000, 0.000); // Green
    renderBitmapString(80.5, 91, (void *)font3, level_buffer);
}

void drawCar(float lrIndex) {
    // Draw main car
    // Front Tire
    glColor3f(0.000, 0.000, 0.000); // Black
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26 - 2, 5);
    glVertex2f(lrIndex + 26 - 2, 7);
    glVertex2f(lrIndex + 30 + 2, 7);
    glVertex2f(lrIndex + 30 + 2, 5);
    glEnd();

    // Back Tire
    glColor3f(0.000, 0.000, 0.000); // Black
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26 - 2, 1);
    glVertex2f(lrIndex + 26 - 2, 3);
    glVertex2f(lrIndex + 30 + 2, 3);
    glVertex2f(lrIndex + 30 + 2, 1);
    glEnd();

    // Car Body
    glColor3f(0.545, 0.000, 0.000); // Dark Red
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26, 1);
    glVertex2f(lrIndex + 26, 8);
    glVertex2f(lrIndex + 28, 10);
    glVertex2f(lrIndex + 30, 8);
    glVertex2f(lrIndex + 30, 1);
    glEnd();
}

void drawOppositeCar(float lrIndex, float carY) {
    // Draw opposite car
    // Front Tire
    glColor3f(0.000, 0.000, 0.000); // Black
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26 - 2, carY + 100 - 4);
    glVertex2f(lrIndex + 26 - 2, carY + 100 - 6);
    glVertex2f(lrIndex + 30 + 2, carY + 100 - 6);
    glVertex2f(lrIndex + 30 + 2, carY + 100 - 4);
    glEnd();

    // Back Tire
    glColor3f(0.000, 0.000, 0.000); // Black
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26 - 2, carY + 100);
    glVertex2f(lrIndex + 26 - 2, carY + 100 - 2);
    glVertex2f(lrIndex + 30 + 2, carY + 100 - 2);
    glVertex2f(lrIndex + 30 + 2, carY + 100);
    glEnd();

    // Car Body
    glColor3f(1.000, 0.000, 0.000); // Red
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26, carY + 100);
    glVertex2f(lrIndex + 26, carY + 100 - 7);
    glVertex2f(lrIndex + 28, carY + 100 - 9);
    glVertex2f(lrIndex + 30, carY + 100 - 7);
    glVertex2f(lrIndex + 30, carY + 100);
    glEnd();
}

void moveOppositeCars() {
    // Move opposite cars
    car1--;
    if (car1 < -100) {
        car1 = 0;
        lrIndex1 = lrIndex;
    }

    car2--;
    if (car2 < -100) {
        car2 = 0;
        lrIndex2 = lrIndex;
    }

    car3--;
    if (car3 < -100) {
        car3 = 0;
        lrIndex3 = lrIndex;
    }
}

void checkCollision() {
    // Check collision
    if ((abs(lrIndex - lrIndex1) < 8) && (car1 + 100 < 10)) {
        start = 0;
        gv = 1;
    }

    if ((abs(lrIndex - lrIndex2) < 8) && (car2 + 100 < 10)) {
        start = 0;
        gv = 1;
    }

    if ((abs(lrIndex - lrIndex3) < 8) && (car3 + 100 < 10)) {
        start = 0;
        gv = 1;
    }
}

void startGame() {
    // Draw road
    glColor3f(0.000, 0.392, 0.502); // Teal
    glBegin(GL_POLYGON);
    glVertex2f(20, 0);
    glVertex2f(20, 100);
    glVertex2f(80, 100);
    glVertex2f(80, 0);
    glEnd();

    // Draw road borders
    glColor3f(0.545, 0.000, 0.000); // Dark Red
    glBegin(GL_POLYGON);
    glVertex2f(20, 0);
    glVertex2f(20, 100);
    glVertex2f(23, 100);
    glVertex2f(23, 0);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(77, 0);
    glVertex2f(77, 100);
    glVertex2f(80, 100);
    glVertex2f(80, 0);
    glEnd();

    // Draw road divisions
    glColor3f(1.000, 0.647, 0.000); // Orange
    drawRoadDividers(48, roadDivTop + 80);
    drawRoadDividers(48, roadDivMdl + 50);
    drawRoadDividers(48, roadDivBtm + 20);

    // Update road division positions
    updateRoadDivisions();

    // Draw score board
    glColor3f(0.000, 0.000, 0.000); // Black
    drawScoreBoard();

    // Print score, speed, and level
    printScore();
    printSpeed();
    printLevel();

    // Draw main car
    drawCar(lrIndex);//1, 8, 24, 30

    // Draw opposite cars
    drawOppositeCar(lrIndex1, car1);//100
    drawOppositeCar(lrIndex2, car2);//100
    drawOppositeCar(lrIndex3, car3);//100

    //move the opposite cars
    moveOppositeCars();
    // Check collision
    checkCollision();
}
//functions that will be used in the first page displayed upto firstDesign()
void drawHill(float x1, float y1, float x2, float y2, float x3, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x1 + x2, y1 - y2);
    glVertex2f(x3, y1 - y2);
    glEnd();
}


void drawMenuPlaceholder(float x, float y) {
    glColor3f(0.098, 0.098, 0.439); // Dark Blue
    glBegin(GL_POLYGON);
    glVertex2f(x, y + 15);
    glVertex2f(x + 50, y + 15);
    glVertex2f(x + 50, y - 5);
    glVertex2f(x, y - 5);
    glEnd();

    glColor3f(0, 0, 0); // Black
    glBegin(GL_POLYGON);
    glVertex2f(x, y + 15);
    glVertex2f(x + 50, y + 15);
    glVertex2f(x + 50, y + 14);
    glVertex2f(x, y + 14);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x + 49, y + 15);
    glVertex2f(x + 50, y + 15);
    glVertex2f(x + 50, y - 5);
    glVertex2f(x + 49, y - 5);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x, y - 4);
    glVertex2f(x + 50, y - 4);
    glVertex2f(x + 50, y - 5);
    glVertex2f(x, y - 5);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x, y + 15);
    glVertex2f(x - 1, y + 15);
    glVertex2f(x - 1, y - 5);
    glVertex2f(x, y - 5);
    glEnd();
}
//functions from drawHill() onwards will be called here
void firstDesign() {
    // Draw road background
    glColor3f(0.000, 0.392, 0.000); // Dark Green
    glBegin(GL_POLYGON);
    glVertex2f(0, 55);
    glVertex2f(100, 55);
    glColor3f(0.604, 0.804, 0.196); // Light Green
    glVertex2f(100, 0);
    glVertex2f(0, 0);
    glEnd();

    // Draw road design in front page
    glColor3f(0.000, 0.392, 0.502); // Teal
    glBegin(GL_TRIANGLES);
    glVertex2f(51, 55);
    glVertex2f(90, 0);
    glVertex2f(10, 0);
    glEnd();

    // Draw road middle
    glColor3f(1, 1, 1); // White
    glBegin(GL_TRIANGLES);
    glVertex2f(51, 55);
    glVertex2f(54, 0);
    glVertex2f(48, 0);
    glEnd();

    // Draw road sky
    glColor3f(0.000, 0.749, 1.000); // Sky Blue
    glBegin(GL_POLYGON);
    glVertex2f(100, 100);
    glVertex2f(0, 100);
    glColor3f(0.686, 0.933, 0.933); // Light Cyan
    glVertex2f(0, 55);
    glVertex2f(100, 55);
    glEnd();

    // Draw hills
    drawHill(20, 65, 7, 10, 0, 0.235, 0.702, 0.443); // Hill 1
    drawHill(35, 67, 10, 12, 10, 0.000, 0.502, 0.000); // Hill 2
    drawHill(70, 67, 20, 12, 10, 0.235, 0.702, 0.443); // Hill 3
    drawHill(87, 65, 100, 10, 60, 0.000, 0.502, 0.000); // Hill 4

    // Draw trees
    tree(5, -15); // Tree 1
    tree(9, 5); // Tree 2
    tree(85, 9); // Tree 3
    tree(75, -5); // Tree 4

    // Draw menu placeholder
    drawMenuPlaceholder(28, 40); // Menu Placeholder

    // Draw text information in first page
    if (gv == 1) {
        glColor3f(1.000, 0.000, 0.000); // Red
        renderBitmapString(35, 65, (void *)font1, "GAME OVER");
        char buffer2[50];
        sprintf(buffer2, "Your Score is : %d", score);
        renderBitmapString(33, 60, (void *)font1, buffer2);
    }
    glColor3f(1.000, 1.000, 0.000); // Yellow
    renderBitmapString(30, 70, (void *)font1, "2D Car Racing Game");
    renderBitmapString(30, 50 + 1.5, (void *)font2, "Help:");
    renderBitmapString(30, 50 + 1.49, (void *)font2, "____");
    glColor3f(0.200, 1.000, 0.900); // Light Green
    renderBitmapString(30, 48, (void *)font2, "Press SPACE to START New Game");
    renderBitmapString(30, 48 - 3, (void *)font2, "Press ESC to Exit");
    glColor3f(1.000, 1.000, 1.000); // White
    renderBitmapString(30, 49 - 6, (void *)font3, "Press UP Arrow Button to increase Speed");
    renderBitmapString(30, 49 - 8, (void *)font3, "Press DOWN Arrow Button to decrease Speed");
    renderBitmapString(30, 49 - 10, (void *)font3, "Press RIGHT Arrow Button to turn Right");
    renderBitmapString(30, 49 - 12, (void *)font3, "Press LEFT Arrow Button to turn Left");
}



void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    if(start==1){
        // glClearColor(0.627, 0.322, 0.176,1);
        glClearColor(0.000, 0.392, 0.000,1);
        startGame();
    }
    else{
        firstDesign();
        //glClearColor(0.184, 0.310, 0.310,1);
    }
    glFlush();
    glutSwapBuffers();
}
void spe_key(int key, int x, int y){
        switch (key) {
        case GLUT_KEY_DOWN:
            if(FPS>(50+(level*2)))
            FPS=FPS-2;
            break;
        case GLUT_KEY_UP:
            FPS=FPS+2;
            break;
        case GLUT_KEY_LEFT:
            if(lrIndex>=0){
                lrIndex = lrIndex - (FPS/10);
                if(lrIndex<0){
                    lrIndex=-1;
                }
            }
            break;
        case GLUT_KEY_RIGHT:
            if(lrIndex<=44){
                lrIndex = lrIndex + (FPS/10);
                if(lrIndex>44){
                    lrIndex = 45;
                }
            }
            break;
        default:
                break;
        }
}
void processKeys(unsigned char key, int x, int y) {
      switch (key)
            {
                case ' ':
            if(start==0){
                start = 1;
                gv = 0;
                FPS = 50;
                roadDivTopMost = 0;
                roadDivTop = 0;
                roadDivMdl = 0;
                roadDivBtm = 0;
                lrIndex = 0 ;
                car1 = 0;
                lrIndex1=0;
                car2 = +35;
                lrIndex2=0;
                car3 = +70;
                lrIndex3=0;
                score=0;
                level=0;
            }
             break;
             case 27:
                 exit(0);
             break;
             default:
                break;
        }
}
void timer(int){
    glutPostRedisplay();
    glutTimerFunc(1000/FPS,timer,0);
}
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(200,20);
    glutCreateWindow("Car Racing Game");
    glutDisplayFunc(display);
    glutSpecialFunc(spe_key);
    glutKeyboardFunc(processKeys);
    glOrtho(0,100,0,100,-1,1);
    glClearColor(0.184, 0.310, 0.310,1);
    glutTimerFunc(1000,timer,0);
    glutMainLoop();
    return 0;
}
