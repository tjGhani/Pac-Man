#include <windows.h>
#include <stdio.h>
//#inlucde <stdlib>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#define PI 3.1415926535898
#include <math.h>

int tileMap[32][32];
bool food[32][32];
unsigned int score;
int PacX;
int PacZ;
int GhostXX, GhostXX2;
int GhostXX3, GhostXX4;
int GhostY, GhostY2;
int GhostY3, GhostY4;
float GhostXcoord, GhostXcoord2;
float GhostYcoord, GhostYcoord2;
float GhostXcoord3, GhostXcoord4;
float GhostYcoord3, GhostYcoord4;
float PacXcoord;
float PacYcoord;
bool done; //isse constant hojayega na? idk try kark daikhte there you go stuck in infinite loop lol
int window_valid;
bool hey;
/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
GLint circle_points =100;

void display() {

    checkIncDis(rand()%4);      //random ghost movement
    checkIncDis2(rand()%4);
    checkIncDis3(rand()%4);
    checkIncDis4(rand()%4);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

    //creating maze
    float x = -1.0;
    float y = 1.0;
    int r, c;
    for (r = 0; r<32; y-=0.0625) {
        for (x=-1.0, c = 0; c<32; x+=0.0625) {
            if (tileMap[r][c] == 1) {
                glBegin(GL_QUADS);
                    glColor3f(1.0f, 1.0f, 1.0f);
                    glVertex2f(x, y-0.0625);
                    glVertex2f(x+0.0625, y-0.0625);
                    glVertex2f(x+0.0625, y);
                    glVertex2f(x, y);
                glEnd();
            }
            else {
                if (food[r][c]) {
                    glColor3f(0.0f, 1.0f, 1.0f);
                    glPointSize(0.5f);
                    glBegin(GL_POINTS);
                        glVertex2f(x+0.03125, y-0.03125);
                    glEnd();
                }
            }
            c++;
        }
        r++;
    }

    //displayin score
    /*glColor3f(0.0f, 0.0f, 0.0f);
    char hund = (char)((score/100)+48);
    char ten = (char)(((score%100)/10)+48);
    char one = (char)((score%10)+48);
    printf("%c and %c and %c\n", hund, ten, one);
    char scoreARR[10] = {'S', 'C', 'O', 'R', 'E', ':', ' ', hund, ten, one};
    glRasterPos2f((-1 + (2*0.0625)), (1-(28*0.0625)));
    int i;
    for (i=0; i<10; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoreARR[i]);
    }*/
    glColor3f(0.0f, 0.0f, 0.0f);
    displayScore();

    //displaying pacman and ghosts
    displayPac(PacX, PacZ);
    displayGhost1(GhostXX, GhostY);
    displayGhost2(GhostXX2, GhostY2);
    displayGhost3(GhostXX3, GhostY3);
    displayGhost4(GhostXX4, GhostY4);

    //ending the game conditions
    if (score == 446 || (PacX==GhostXX && PacZ==GhostY) || (PacX==GhostXX2 && PacZ==GhostY2) || (PacX==GhostXX3 && PacZ==GhostY3) || (PacX==GhostXX4 && PacZ==GhostY4)) {
        displayEndScreen();
        Sleep(5000);
        glutHideWindow();
        exit(0);
    }
    glFlush();

}

void displayScore() {
    //glColor3f(0.0f, 0.0f, 0.0f);
    char hund = (char)((score/100)+48);
    char ten = (char)(((score%100)/10)+48);
    char one = (char)((score%10)+48);
    printf("%c and %c and %c\n", hund, ten, one);
    char scoreARR[10] = {'S', 'C', 'O', 'R', 'E', ':', ' ', hund, ten, one};
    glRasterPos2f((-1 + (2*0.0625)), (1-(28*0.0625)));
    int i;
    for (i=0; i<10; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoreARR[i]);
    }
}

void displayEndScreen() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if (score == 446) {
        char scoreARR[10] = {'Y', 'O', 'U', ' ', 'W', 'I', 'N', '!', '!', '!'};
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.20, 0.0);
        int i;
        for (i=0; i<10; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoreARR[i]);
        }
        displayScore();
        glFlush();
    }
    else {
        char scoreARR[10] = {'Y', 'O', 'U', ' ', 'L', 'O', 'S', 'E', '!', '!'};
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.20, 0.0);
        int i;
        for (i=0; i<10; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoreARR[i]);
        }
        displayScore();
        glFlush();
    }
}

void displayPac(int PacXb, int PacZb) {
    int PacXtemp = PacXb;
    int PacZtemp = PacZb;

    //set coordinates according to array indices
    PacXcoord = -1 + (PacXtemp*0.0625);
    PacYcoord = 1 - (PacZtemp*0.0625);

    float originX = PacXcoord+0.03125;
    float originY = PacYcoord-0.03125;

    float radius = 0.03125;
    float radian, angle;

    glBegin(GL_POLYGON);
    //glLineWidth(3);
        glColor3f(1.0f, 1.0f, 0.0f);
        //int i;
        float x, y;

        for(angle=0.0; angle<=360.0; angle+=2.0) {
            radian = angle * (PI/180.0f);
            x = (float)cos(radian)*radius + originX;
            y = (float)sin(radian)*radius + originY;
        /*scale*/
            glVertex2f(x,y);
        }
    glEnd();

    glPointSize(2.0f);
    glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(PacXcoord+0.02083, PacYcoord-0.02083);
        glVertex2f(PacXcoord+0.04166, PacYcoord-0.02083);
    glEnd();

    //draw mouth
    glBegin(GL_LINES);
        glVertex2f(PacXcoord+0.02083, PacYcoord-0.04166);
        glVertex2f(PacXcoord+0.043, PacYcoord-0.04166);
    glEnd();
    glFlush();
}

void displayBlock(int PacXb, int PacZb) {
    int PacXtemp = PacXb;
    int PacZtemp = PacZb;

    //set coordinates according to array indices
    PacXcoord = -1 + (PacXtemp*0.0625);
    PacYcoord = 1 - (PacZtemp*0.0625);

    //draw yellow square
    glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(PacXcoord, PacYcoord-0.0625);
        glVertex2f(PacXcoord+0.0625, PacYcoord-0.0625);
        glVertex2f(PacXcoord+0.0625, PacYcoord);
        glVertex2f(PacXcoord, PacYcoord);
    glEnd();

    //draw eyes
    glPointSize(2.0f);
    glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(PacXcoord+0.02083, PacYcoord-0.02083);
        glVertex2f(PacXcoord+0.04166, PacYcoord-0.02083);
    glEnd();

    //draw mouth
    glBegin(GL_LINES);
        glVertex2f(PacXcoord+0.02083, PacYcoord-0.04166);
        glVertex2f(PacXcoord+0.043, PacYcoord-0.04166);
    glEnd();
    glFlush();
}

void processSpecialKeys(int key, int x, int y) {

    switch(key) {
        case GLUT_KEY_LEFT:
            if (tileMap[PacZ][PacX-1] == 0 && (PacX-1) != -1) {
                PacX -= 1;
                if (PacX == -1)
                    PacX = 31;
                if (food[PacZ][PacX])
                    score++;
                food[PacZ][PacX] = FALSE;
            }
            else if (PacX == 0) {       //allowing pacman to come back from right side
                PacX = 31;
                if (food[PacZ][PacX])
                    score++;
                food[PacZ][PacX] = FALSE;
            }
            break;
        case GLUT_KEY_RIGHT:
            if (tileMap[PacZ][PacX+1] == 0 && (PacX+1)!=32) {
                PacX += 1;
                if (food[PacZ][PacX])
                    score++;
                food[PacZ][PacX] = FALSE;
            }
            else if (PacX == 31) {      //allowing pacman to come back from left side
                PacX = 0;
                if (food[PacZ][PacX])
                    score++;
                food[PacZ][PacX] = FALSE;
            }
            break;
        case GLUT_KEY_UP:
            if (tileMap[PacZ-1][PacX] == 0) {
                PacZ += -1;
                if (food[PacZ][PacX])
                    score++;
                food[PacZ][PacX] = FALSE;
            }
            break;
        case GLUT_KEY_DOWN:
            if (tileMap[PacZ+1][PacX] == 0) {
                PacZ += 1;
                if (food[PacZ][PacX])
                    score++;
                food[PacZ][PacX] = FALSE;
            }
            break;
    }

    glFlush();
}

void displayGhost1(int GhostXb, int GhostYb) {
    int GhostXtemp = GhostXb;
    int GhostYtemp = GhostYb;

    //set ghost coordinates according to indices
    GhostXcoord = -1 + (GhostXtemp*0.0625);
    GhostYcoord = 1 - (GhostYtemp*0.0625);

    float originX = GhostXcoord+0.03125;
    float originY = GhostYcoord-0.03125;

    float radius = 0.03125;
    float radian, angle;

    glBegin(GL_POLYGON);
    //glLineWidth(3);
        glColor3f(1.0f, 0.0f, 0.0f);
        //int i;
        float x, y;

        for(angle=0.0; angle<=360.0; angle+=2.0) {
            radian = angle * (PI/180.0f);
            x = (float)cos(radian)*radius + originX;
            y = (float)sin(radian)*radius + originY;
        /*scale*/
            glVertex2f(x,y);
        }
    glEnd();

    /*glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(GhostXcoord, GhostYcoord-0.0625);
        glVertex2f(GhostXcoord+0.0625, GhostYcoord-0.0625);
        glVertex2f(GhostXcoord+0.0625, GhostYcoord);
        glVertex2f(GhostXcoord, GhostYcoord);
    glEnd();*/
    glPointSize(2.0f);
    glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(GhostXcoord+0.02083, GhostYcoord-0.02083);
        glVertex2f(GhostXcoord+0.04166, GhostYcoord-0.02083);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(GhostXcoord+0.02083, GhostYcoord-0.04166);
        glVertex2f(GhostXcoord+0.043, GhostYcoord-0.04166);
    glEnd();
    glFlush();
}

void displayGhost2(int GhostXb, int GhostYb) {
    int GhostXtemp = GhostXb;
    int GhostYtemp = GhostYb;

    GhostXcoord2 = -1 + (GhostXtemp*0.0625);
    GhostYcoord2 = 1 - (GhostYtemp*0.0625);

    float originX = GhostXcoord2+0.03125;
    float originY = GhostYcoord2-0.03125;

    float radius = 0.03125;
    float radian, angle;

    glBegin(GL_POLYGON);
    //glLineWidth(3);
        glColor3f(1.0f, 0.0f, 0.0f);
        //int i;
        float x, y;

        for(angle=0.0; angle<=360.0; angle+=2.0) {
            radian = angle * (PI/180.0f);
            x = (float)cos(radian)*radius + originX;
            y = (float)sin(radian)*radius + originY;
        /*scale*/
            glVertex2f(x,y);
        }
    glEnd();

    /*glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(GhostXcoord2, GhostYcoord2-0.0625);
        glVertex2f(GhostXcoord2+0.0625, GhostYcoord2-0.0625);
        glVertex2f(GhostXcoord2+0.0625, GhostYcoord2);
        glVertex2f(GhostXcoord2, GhostYcoord2);
    glEnd();*/
    //glFlush();
    glPointSize(2.0f);
    glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(GhostXcoord2+0.02083, GhostYcoord2-0.02083);
        glVertex2f(GhostXcoord2+0.04166, GhostYcoord2-0.02083);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(GhostXcoord2+0.02083, GhostYcoord2-0.04166);
        glVertex2f(GhostXcoord2+0.043, GhostYcoord2-0.04166);
    glEnd();
    glFlush();
}

void displayGhost3(int GhostXb, int GhostYb) {
    int GhostXtemp = GhostXb;
    int GhostYtemp = GhostYb;

    //set ghost coordinates according to indices
    GhostXcoord3 = -1 + (GhostXtemp*0.0625);
    GhostYcoord3 = 1 - (GhostYtemp*0.0625);

    float originX = GhostXcoord3+0.03125;
    float originY = GhostYcoord3-0.03125;

    float radius = 0.03125;
    float radian, angle;

    glBegin(GL_POLYGON);
    //glLineWidth(3);
        glColor3f(1.0f, 0.0f, 0.0f);
        //int i;
        float x, y;

        for(angle=0.0; angle<=360.0; angle+=2.0) {
            radian = angle * (PI/180.0f);
            x = (float)cos(radian)*radius + originX;
            y = (float)sin(radian)*radius + originY;
        /*scale*/
            glVertex2f(x,y);
        }
    glEnd();

    /*glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(GhostXcoord, GhostYcoord-0.0625);
        glVertex2f(GhostXcoord+0.0625, GhostYcoord-0.0625);
        glVertex2f(GhostXcoord+0.0625, GhostYcoord);
        glVertex2f(GhostXcoord, GhostYcoord);
    glEnd();*/
    glPointSize(2.0f);
    glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(GhostXcoord3+0.02083, GhostYcoord3-0.02083);
        glVertex2f(GhostXcoord3+0.04166, GhostYcoord3-0.02083);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(GhostXcoord3+0.02083, GhostYcoord3-0.04166);
        glVertex2f(GhostXcoord3+0.043, GhostYcoord3-0.04166);
    glEnd();
    glFlush();
}

void displayGhost4(int GhostXb, int GhostYb) {
    int GhostXtemp = GhostXb;
    int GhostYtemp = GhostYb;

    //set ghost coordinates according to indices
    GhostXcoord4 = -1 + (GhostXtemp*0.0625);
    GhostYcoord4 = 1 - (GhostYtemp*0.0625);

    float originX = GhostXcoord4+0.03125;
    float originY = GhostYcoord4-0.03125;

    float radius = 0.03125;
    float radian, angle;

    glBegin(GL_POLYGON);
    //glLineWidth(3);
        glColor3f(1.0f, 0.0f, 0.0f);
        //int i;
        float x, y;

        for(angle=0.0; angle<=360.0; angle+=2.0) {
            radian = angle * (PI/180.0f);
            x = (float)cos(radian)*radius + originX;
            y = (float)sin(radian)*radius + originY;
        /*scale*/
            glVertex2f(x,y);
        }
    glEnd();

    /*glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(GhostXcoord, GhostYcoord-0.0625);
        glVertex2f(GhostXcoord+0.0625, GhostYcoord-0.0625);
        glVertex2f(GhostXcoord+0.0625, GhostYcoord);
        glVertex2f(GhostXcoord, GhostYcoord);
    glEnd();*/
    glPointSize(2.0f);
    glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(GhostXcoord4+0.02083, GhostYcoord4-0.02083);
        glVertex2f(GhostXcoord4+0.04166, GhostYcoord4-0.02083);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(GhostXcoord4+0.02083, GhostYcoord4-0.04166);
        glVertex2f(GhostXcoord4+0.043, GhostYcoord4-0.04166);
    glEnd();
    glFlush();
}

void checkIncDis(int sent) {
    switch(sent) {
        case 0:
            if (tileMap[GhostY][GhostXX+1] == 1) {
                checkIncDis((rand()%4));
            }
            else {
                GhostXX++;
                if (GhostXX == 32)
                    GhostXX = 0;
                displayGhost1(GhostXX, GhostY);
            }
            break;
        case 1:
            if (tileMap[GhostY+1][GhostXX] == 1) {
                checkIncDis((rand()%4));
            }
            else {
                GhostY++;
                displayGhost1(GhostXX, GhostY);

            }
            break;
        case 2:
            if (tileMap[GhostY-1][GhostXX] == 1) {
                checkIncDis((rand()%4));
            }
            else {
                GhostY--;
                displayGhost1(GhostXX, GhostY);
            }
            break;
        case 3:
            if (tileMap[GhostY][GhostXX-1] == 1) {
                checkIncDis((rand()%4));
            }
            else {
                GhostXX--;
                if (GhostXX == -1)
                    GhostXX = 31;
                displayGhost1(GhostXX, GhostY);
            }
            break;
    }
    return;
}

void checkIncDis2(int sent) {

    switch(sent) {
        case 0:
            if (tileMap[GhostY2][GhostXX2+1] == 1)
                checkIncDis2((rand()%4));
            else {
                GhostXX2++;
                if (GhostXX2 == 32)
                    GhostXX2 = 0;
                displayGhost2(GhostXX2, GhostY2);
            }
            break;
        case 1:
            if (tileMap[GhostY2+1][GhostXX2] == 1)
                checkIncDis2((rand()%4));
            else {
                GhostY2++;
                displayGhost2(GhostXX2, GhostY2);
            }
            break;
        case 2:
            if (tileMap[GhostY2-1][GhostXX2] == 1)
                checkIncDis2((rand()%4));
            else {
                GhostY2--;
                displayGhost2(GhostXX2, GhostY2);
            }
            break;
        case 3:
            if (tileMap[GhostY2][GhostXX2-1] == 1)
                checkIncDis2((rand()%4));
            else {
                GhostXX2--;
                if (GhostXX2 == -1)
                    GhostXX2 = 31;
                displayGhost2(GhostXX2, GhostY2);
            }
            break;
    }
    return;
}

void checkIncDis3(int sent) {

    switch(sent) {
        case 0:
            if (tileMap[GhostY3][GhostXX3+1] == 1)
                checkIncDis3((rand()%4));
            else {
                GhostXX3++;
                if (GhostXX3 == 32)
                    GhostXX3 = 0;
                displayGhost3(GhostXX3, GhostY3);
            }
            break;
        case 1:
            if (tileMap[GhostY3+1][GhostXX3] == 1)
                checkIncDis3((rand()%4));
            else {
                GhostY3++;
                displayGhost3(GhostXX3, GhostY3);
            }
            break;
        case 2:
            if (tileMap[GhostY3-1][GhostXX3] == 1)
                checkIncDis3((rand()%4));
            else {
                GhostY3--;
                displayGhost3(GhostXX3, GhostY3);
            }
            break;
        case 3:
            if (tileMap[GhostY3][GhostXX3-1] == 1)
                checkIncDis3((rand()%4));
            else {
                GhostXX3--;
                if (GhostXX3 == -1)
                    GhostXX3 = 31;
                displayGhost3(GhostXX3, GhostY3);
            }
            break;
    }
    return;
}

void checkIncDis4(int sent) {

    switch(sent) {
        case 0:
            if (tileMap[GhostY4][GhostXX4+1] == 1)
                checkIncDis4((rand()%4));
            else {
                GhostXX4++;
                if (GhostXX4 == 32)
                    GhostXX4 = 0;
                displayGhost4(GhostXX4, GhostY4);
            }
            break;
        case 1:
            if (tileMap[GhostY4+1][GhostXX4] == 1)
                checkIncDis4((rand()%4));
            else {
                GhostY4++;
                displayGhost4(GhostXX4, GhostY4);
            }
            break;
        case 2:
            if (tileMap[GhostY4-1][GhostXX4] == 1)
                checkIncDis4((rand()%4));
            else {
                GhostY4--;
                displayGhost4(GhostXX4, GhostY4);
            }
            break;
        case 3:
            if (tileMap[GhostY4][GhostXX4-1] == 1)
                checkIncDis4((rand()%4));
            else {
                GhostXX4--;
                if (GhostXX4 == -1)
                    GhostXX4 = 31;
                displayGhost4(GhostXX4, GhostY4);
            }
            break;
    }
    return;
}

void onIdle() {
    glutPostRedisplay();
    //Sleep(200);
    glutTimerFunc(200, onIdle, 0);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    srand(time(NULL));
    char t;
    hey = TRUE;

    PacX = 16;
    PacZ = 23;

    GhostXX = 1;
    GhostY = 1;

    GhostXX2 = 30;
    GhostY2 = 1;

    GhostXX3 = 1;
    GhostY3 = 26;

    GhostXX4 = 30;
    GhostY4 = 26;

    score = 0;

    FILE *file;
    file = fopen("mapd.txt", "r+");

    if (file == NULL) {
        printf("error in opening file, try again\n");
        return 0;
    }

    t = fgetc(file);
    int i, j;

    for (i = 0; i<32; i++) {
        for (j = 0; j<32; j++) {
            if (t == '0') {
                tileMap[i][j] = 0;
                food[i][j] = TRUE;
            }
            else if (t == '1') {
                tileMap[i][j] = 1;
                food[i][j] = FALSE;
            }
            t = fgetc(file);
        }
        t = fgetc(file);
    }

    /*for (i = 0; i<27; i++) {
        for (j = 0; j<32; j++) {
            printf("%d", tileMap[i][j]);
        }
        printf("\n");
    }*/


    glutInit(&argc, argv);                  // Initialize GLUT

    glutInitWindowSize(640, 640);           // Set the window's initial width & height
    glutCreateWindow("PACMAN");             // Create a window with the given title
    glutInitWindowPosition(1000, 1000);     // Position the window's initial top-left corner
    glutDisplayFunc(display);               // Register display callback handler for window re-paint
    // Enter the infinitely event-processing loop
    //glutDisplayFunc(display);
    glutKeyboardFunc(NULL);
    glutSpecialFunc(processSpecialKeys);
    //glutIdleFunc(onIdle);
    glutTimerFunc(0, onIdle, 0);
    glutMainLoop();

    return 0;
}
