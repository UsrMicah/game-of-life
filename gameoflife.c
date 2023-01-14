#define GL_SILENCE_DEPRECATION
#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include "glutfuncs.h"
#include "filemng.h"
#define LEN 200
#define wrap(V,D) (V+LEN+D)%LEN
double sc = 2.0/(LEN);
int map[LEN][LEN];
int grid = 1;
int savemap[LEN][LEN];
int run = 0;
int warp = 1;
int cursor[2] = {1,1};
int mouse[2] = {0,0};
char input[150] = "\0";
char insert[150] = "shapes/shooters/gosper_gun.lf";
int inbuf = 0;
int input_type = 0;
void update() {
    int temp[LEN][LEN];
    int nbrs;
    for(int x = 0; x < LEN; x++) {
        for(int y = 0; y < LEN; y++) {
            nbrs = map[wrap(x,1)][y] + map[wrap(x,1)][wrap(y,1)] + map[x][wrap(y,1)] + map[wrap(x,-1)][wrap(y,1)] + map[wrap(x,-1)][y] + map[wrap(x,-1)][wrap(y,-1)] + map[x][wrap(y,-1)] + map[wrap(x,1)][wrap(y,-1)];
            if((nbrs == 3 || nbrs == 2) && map[x][y])
                temp[x][y] = 1;
            else if((nbrs == 3 ) && !map[x][y])
                temp[x][y] = 1;
            else
                temp[x][y] = 0;
        }
    }
    for(int x = 0; x < LEN; x++) {
        for(int y = 0; y < LEN; y++)
            map[x][y] = temp[x][y];
    }
}
void renderScene ()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int x = 0; x < LEN; x++) {
        for(int y = 0; y < LEN; y++) {
            if(map[x][y] > 0) {
                glColor3f(1,1,0);
                glBegin(GL_POLYGON);
                glVertex2f(x*sc-1,y*sc-1);
                glVertex2f((x+1)*sc-1,y*sc-1);
                glVertex2f((x+1)*sc-1,(y+1)*sc-1);
                glVertex2f(x*sc-1,(y+1)*sc-1);
                glEnd();
            }
        }
    }
    // draw grid
    glColor3f(0.2,0.2,0.2);
    if(grid) {
        glBegin(GL_LINES);
        for(int i = 0; i < LEN; i++) {
            glVertex2f(i*sc-1,1);
            glVertex2f(i*sc-1,-1);

            glVertex2f(1,i*sc-1);
            glVertex2f(-1,i*sc-1);
        }
    }
    glEnd();
    //draw cursor
    glColor3f(1,0,0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(cursor[0]*sc-1,cursor[1]*sc-1);
    glVertex2f((cursor[0]+1)*sc-1,cursor[1]*sc-1);
    glVertex2f((cursor[0]+1)*sc-1,(cursor[1]+1)*sc-1);
    glVertex2f(cursor[0]*sc-1,(cursor[1]+1)*sc-1);
    glVertex2f(cursor[0]*sc-1,cursor[1]*sc-1);
    glEnd();

    glColor3f(1,1,1);
    if(input_type > 0)
        displaystring(-0.9,0.9, 100, input, input_type > 0 ? 1 : 0, inbuf);
    if(run) {
        for(int w = 0; w < warp; w++)
            update();
    }
    glutSwapBuffers();
}
void mouseclick(int button, int state, int x, int y) {
    x =  (((float) x)/800.0 * LEN);
    y =  ((float)(800 - y))/800.0 * LEN;
    if(state == GLUT_DOWN) {
        // map[x][y] = map[x][y] ? 0 : 1;
        cursor[0] = x;
        cursor[1] = y;
    }
}
void keys(unsigned char key, int x, int y) {
    x =  (((float) x)/800.0 * LEN);
    y = -(((float) y)/800.0 * LEN);
    if(input_type == 0) {
        switch(key) {
            case ' ':
                run = run ? 0 : 1;
                break;
            case '.':
                update();
                break;
            case ']':
                warp += (warp < 5) ? 1 : 0;
                break;
            case '[':
                warp -= (warp > 1) ? 1 : 0;
                break;
            case '\t':
                map[cursor[0]][cursor[1]] = map[cursor[0]][cursor[1]] ? 0 : 1;
                break;
            case 's':
                input_type = 2;
                break;
            case 'd':
                write_file("shapes/SaveFile.lf",LEN,map,0);
                break;
            case 'D':
                read_file(0,0,"shapes/SaveFile.lf",LEN,map,1,1);
                run = 0;

                break;
            case 'r':
                for(int x = 0; x < LEN; x++) {
                    for(int y = 0; y < LEN; y++)
                        map[x][y] = 0;
                }
                break;
            case 'g':
                grid = grid ? 0 : 1;
                break;
            case '1':
                read_file(cursor[0],cursor[1],insert,LEN,map,1,1);
                break;
            case '2':
                read_file(cursor[0],cursor[1],insert,LEN,map,-1,1);
                break;
            case '3':
                read_file(cursor[0],cursor[1],insert,LEN,map,1,-1);
                break;
            case '4':
                read_file(cursor[0],cursor[1],insert,LEN,map,-1,-1);
                break;
            case 'm':
                for(int x = 0; x < LEN; x++) {
                    for(int y = 0; y < LEN; y++)
                        map[x][y] = random()%2;
                }
                run = 0;
                break;
            case '`':
                input_type = 1;
                break;

        }
    }
    else {
        if(key == 127) {
            if(inbuf > 0)
                deletechar(input,--inbuf);
        }
        else if(key != 13 && inbuf < 99) {
            insertchar(input,inbuf-1,key);
            inbuf++;
        }
        else if(key == 13 && input_type == 1) {
            char *directories[] = {"shapes/oscillators/","shapes/shooters/","shapes/still_lifes/","shapes/space_ships/",NULL};
            char temp[150];
            FILE *fp;
            if(len(input) > 0) {
                if(!strend(input,".lf"))
                    append(input,".lf");
                setchar(temp,input,150);
                for(int i = 0; directories[i] != NULL && (fp = fopen(temp,"r")) == NULL; i++) {
                    setchar(temp,input,150);
                    prepend(temp,directories[i]);
                    fclose(fp);
                }
                if((fp = fopen(temp,"r")) != NULL) {
                    setchar(input,temp,150);
                    setchar(insert,input,150);
                }
                else
                    input[0] = '\0';
                printf("%s, %s\n",input,temp);
                fclose(fp);
            }
            input_type = 0;
            inbuf = len(input);
        }
        else if(key == 13 && input_type == 2) {
            if(len(input) > 0) {
                if(!strend(input,".lf"))
                    append(input,".lf");
                if(isword(input,"SaveFile.lf")) {
                    prepend(input,"shapes/");
                    write_file(input,LEN,map,0);
                }
                else {
                    prepend(input,"shapes/");
                    write_file(input,LEN,map,1);
                }
            }
            input_type = 0;
        }

    }
}
void specialkeydetect(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            cursor[1] = wrap(cursor[1],1);
            break;
        case GLUT_KEY_DOWN:
            cursor[1] = wrap(cursor[1],-1);
            break;
        case GLUT_KEY_RIGHT:
            if(input_type) {
                if(input[inbuf] != '\0')
                    inbuf++;
            }
            else
                cursor[0] = wrap(cursor[0],1);
            break;
        case GLUT_KEY_LEFT:
            if(input_type > 0)
                inbuf--;
            else
                cursor[0] = wrap(cursor[0],-1);
            break;
    }
}
int main(int argc, char **argv) {
    for(int x = 0; x < LEN; x++) {
        for(int y = 0; y < LEN; y++) 
            map[x][y] = 0;
    }
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Game Of Life");
	// glColor3f(1.0f,1.0f,0.0f); 
	glutMouseFunc(mouseclick);
	glutKeyboardFunc(keys);
    glutKeyboardFunc(keys);
    glutSpecialFunc(specialkeydetect);
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutMainLoop();
}