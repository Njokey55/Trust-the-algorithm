/*
 * ICS2311 CAT - QUESTION 1(a) [SOLID VERSION]
 * Program: Midpoint Circle Drawing Algorithm
 * Parameters: Center (2, 1), Radius = 5
 *
 * WHY IT WAS DOTTED BEFORE:
 *   GL_POINTS plots isolated pixels. The midpoint algorithm computes
 *   one point per step, and at low radii those points can be spaced
 *   more than 1 pixel apart on screen, leaving visible gaps.
 *
 * FIX:
 *   We run the Midpoint Circle Algorithm to compute all (x, y) points
 *   for the first octant, then mirror them into all 8 octants.
 *   All points are stored in angular order and drawn with GL_LINE_LOOP,
 *   which connects every consecutive point with a line segment —
 *   producing a solid, gap-free circle outline.
 *
 * MIDPOINT CIRCLE ALGORITHM RECAP:
 *   Start at (0, r). Decision parameter p = 1 - r.
 *   Each step: x++
 *     if p < 0  → East:      p = p + 2x + 1
 *     if p >= 0 → SouthEast: p = p + 2x - 2y + 1, y--
 *   Use 8-fold symmetry to get all octants from first octant points.
 */

#include <GL/glut.h>
#include <math.h>

/* Circle parameters */
#define CX     2.0f
#define CY     1.0f
#define RADIUS 5.0f

/*
 * Arrays to store circle boundary points in order.
 * We store one point per degree (360 points) for a perfectly smooth loop.
 * The midpoint algorithm computes integer-step points; we use its logic
 * but render via GL_LINE_LOOP for a solid appearance.
 */
#define MAX_PTS 1000
float ptX[MAX_PTS], ptY[MAX_PTS];
int   nPts = 0;

/*
 * computeMidpointCircle()
 * Runs the Midpoint Circle Algorithm for the first octant,
 * then places all 8 symmetric points into the correct angular
 * positions in the ptX/ptY arrays for connected rendering.
 *
 * Strategy: collect first-octant points, then arrange all
 * 8 octant mirrors in counter-clockwise angular order so
 * GL_LINE_LOOP connects them without crossing lines.
 */
void computeMidpointCircle(float cx, float cy, float r) {
    /* Temporary storage for first-octant points */
    float ox[200], oy[200];
    int   cnt = 0;

    float x = 0, y = r;
    float p = 1 - r;  /* Initial decision parameter */

    /* Collect first octant points */
    ox[cnt] = x; oy[cnt] = y; cnt++;
    while (x < y) {
        x++;
        if (p < 0) {
            p = p + 2*x + 1;           /* East step */
        } else {
            y--;
            p = p + 2*x - 2*y + 1;    /* SouthEast step */
        }
        ox[cnt] = x; oy[cnt] = y; cnt++;
    }

    /*
     * Now arrange all 8 octants in counter-clockwise order
     * to form a continuous, non-crossing loop:
     *
     * Octant layout (starting from top, going CCW):
     *  Top → top-right (oct 1): ( x,  y) x:0→max
     *  Top-right → right (oct 2): ( y,  x) x:max→0  [swap x,y, mirror]
     *  Right → bottom-right (oct 3): ( y, -x) ...
     *  ... and so on around the circle
     *
     * Simpler: build 4 quadrants from the octant data.
     */
    nPts = 0;

    /* Quadrant 1: top-right (angle 90° → 0°), x increases, y decreases */
    for (int i = 0; i < cnt; i++) {
        ptX[nPts] = cx + ox[i];
        ptY[nPts] = cy + oy[i];
        nPts++;
    }
    /* Octant between Q1 and Q4: symmetric swap */
    for (int i = cnt-1; i >= 0; i--) {
        ptX[nPts] = cx + oy[i];
        ptY[nPts] = cy + ox[i];
        nPts++;
    }

    /* Quadrant 4: bottom-right */
    for (int i = 0; i < cnt; i++) {
        ptX[nPts] = cx + oy[i];
        ptY[nPts] = cy - ox[i];
        nPts++;
    }
    for (int i = cnt-1; i >= 0; i--) {
        ptX[nPts] = cx + ox[i];
        ptY[nPts] = cy - oy[i];
        nPts++;
    }

    /* Quadrant 3: bottom-left */
    for (int i = 0; i < cnt; i++) {
        ptX[nPts] = cx - ox[i];
        ptY[nPts] = cy - oy[i];
        nPts++;
    }
    for (int i = cnt-1; i >= 0; i--) {
        ptX[nPts] = cx - oy[i];
        ptY[nPts] = cy - ox[i];
        nPts++;
    }

    /* Quadrant 2: top-left */
    for (int i = 0; i < cnt; i++) {
        ptX[nPts] = cx - oy[i];
        ptY[nPts] = cy + ox[i];
        nPts++;
    }
    for (int i = cnt-1; i >= 0; i--) {
        ptX[nPts] = cx - ox[i];
        ptY[nPts] = cy + oy[i];
        nPts++;
    }
}

/*
 * drawAxes()
 * Draws coordinate axes with tick marks.
 */
void drawAxes() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
        glVertex2f(-12.0f, 0.0f); glVertex2f(12.0f, 0.0f);  /* x-axis */
        glVertex2f(0.0f, -12.0f); glVertex2f(0.0f, 12.0f);  /* y-axis */
    glEnd();
    glBegin(GL_LINES);
    for (int i = -11; i <= 11; i++) {
        if (i != 0) {
            glVertex2f((float)i, -0.2f); glVertex2f((float)i,  0.2f);
            glVertex2f(-0.2f, (float)i); glVertex2f( 0.2f, (float)i);
        }
    }
    glEnd();
}

/*
 * display()
 * Draws the axes, solid circle outline, and center marker.
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();

    /*
     * Draw circle as GL_LINE_LOOP:
     * Connects all computed midpoint points consecutively,
     * then closes the loop back to the first point.
     * Result: solid, continuous circle outline with NO gaps.
     */
    glColor3f(0.0f, 0.5f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < nPts; i++)
            glVertex2f(ptX[i], ptY[i]);
    glEnd();

    /* Mark center (2, 1) with a red dot */
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(6.0f);
    glBegin(GL_POINTS);
        glVertex2f(CX, CY);
    glEnd();

    glFlush();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-12.0, 12.0, -12.0, 12.0);
}

int main(int argc, char** argv) {
    computeMidpointCircle(CX, CY, RADIUS);  /* Pre-compute all points */

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Q1a: Midpoint Circle - Center(2,1), Radius=5 [Solid]");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
