/*
 * ICS2311 CAT - QUESTION 1(b) [SOLID VERSION]
 * Program: Triangle + Circumscribed Circle in Red
 * Vertices: A(1,6), B(2,0), C(-4,9)
 * CIRCUMCENTER FORMULA:
 *   D  = 2[x1(y2-y3) + x2(y3-y1) + x3(y1-y2)]
 *   ux = [(x1²+y1²)(y2-y3) + (x2²+y2²)(y3-y1) + (x3²+y3²)(y1-y2)] / D
 *   uy = [(x1²+y1²)(x3-x2) + (x2²+y2²)(x1-x3) + (x3²+y3²)(x2-x1)] / D
 *   R  = distance from circumcenter to any vertex
 */

#include <GL/glut.h>
#include <math.h>

/* Triangle vertices */
#define AX  1.0f
#define AY  6.0f
#define BX  2.0f
#define BY  0.0f
#define CX -4.0f
#define CY  9.0f

float circumX, circumY, circumR;

/*
 * computeCircumcircle()
 * Derives the circumcenter and circumradius mathematically.
 */
void computeCircumcircle() {
    float a1 = AX*AX + AY*AY;
    float b1 = BX*BX + BY*BY;
    float c1 = CX*CX + CY*CY;
    float D  = 2.0f * (AX*(BY-CY) + BX*(CY-AY) + CX*(AY-BY));
    circumX  = (a1*(BY-CY) + b1*(CY-AY) + c1*(AY-BY)) / D;
    circumY  = (a1*(CX-BX) + b1*(AX-CX) + c1*(BX-AX)) / D;
    float dx = AX - circumX, dy = AY - circumY;
    circumR  = sqrt(dx*dx + dy*dy);
}

/*
 * drawSolidCircle()
 * Draws a solid circle outline using GL_LINE_LOOP with parametric equations.
 * Generates 360 points evenly spaced around the circle,
 * then connects them — producing a perfectly smooth solid ring.
 *
 *   x = cx + r * cos(t)
 *   y = cy + r * sin(t)    for t = 0 to 2π
 */
void drawSolidCircle(float cx, float cy, float r) {
    int segments = 360;
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < segments; i++) {
            float angle = 2.0f * M_PI * i / segments;
            glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
        }
    glEnd();
}

/*
 * drawAxes()
 */
void drawAxes() {
    glColor3f(0.3f, 0.3f, 0.3f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
        glVertex2f(-15.0f, 0.0f); glVertex2f(15.0f,  0.0f);
        glVertex2f(  0.0f,-15.0f); glVertex2f( 0.0f, 15.0f);
    glEnd();
    glBegin(GL_LINES);
    for (int i = -14; i <= 14; i++) {
        if (i != 0) {
            glVertex2f((float)i, -0.2f); glVertex2f((float)i,  0.2f);
            glVertex2f(-0.2f, (float)i); glVertex2f( 0.2f, (float)i);
        }
    }
    glEnd();
}

/*
 * display()
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();

    /* Draw the triangle — GL_LINE_LOOP connects vertices and closes the shape */
    glColor3f(0.0f, 0.0f, 0.8f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(AX, AY);   /* A(1,6)  */
        glVertex2f(BX, BY);   /* B(2,0)  */
        glVertex2f(CX, CY);   /* C(-4,9) */
    glEnd();

    /* Draw the circumscribed circle as a solid red ring */
    glColor3f(1.0f, 0.0f, 0.0f);   /* Red */
    glLineWidth(2.0f);
    drawSolidCircle(circumX, circumY, circumR);

    /* Mark triangle vertices */
    glColor3f(0.0f, 0.6f, 0.0f);
    glPointSize(7.0f);
    glBegin(GL_POINTS);
        glVertex2f(AX, AY);
        glVertex2f(BX, BY);
        glVertex2f(CX, CY);
    glEnd();

    /* Mark circumcenter */
    glColor3f(1.0f, 0.5f, 0.0f);
    glPointSize(6.0f);
    glBegin(GL_POINTS);
        glVertex2f(circumX, circumY);
    glEnd();

    glFlush();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-15.0, 15.0, -15.0, 15.0);
}

int main(int argc, char** argv) {
    computeCircumcircle();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Q1b: Triangle + Red Circumscribed Circle [Solid]");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
