/*
 * ICS2311 CAT - QUESTION 2 [SOLID VERSION]
 * Program: Midpoint Ellipse + Flood Fill (Pastel Pink #F8C8DC)
 * Center (3, 2), rx = 4, ry = 2
 *
 * FIX: The ellipse boundary is now drawn with GL_LINE_LOOP
 *      (connecting all computed midpoint points) instead of GL_POINTS.
 *      This gives a solid, gap-free ellipse outline before flood fill.
 *
 * FLOOD FILL NOTE:
 *   The recursive flood fill reads pixel colors from the framebuffer.
 *   It needs the ellipse boundary to be fully solid (no pixel gaps)
 *   so the fill doesn't leak outside. The GL_LINE_LOOP fix also
 *   ensures the flood fill is properly contained.
 *
 *   Fill color: Pastel Pink #F8C8DC = RGB(248, 200, 220)
 *   Normalized: (0.973, 0.784, 0.863)
 */

#include <GL/glut.h>
#include <math.h>

/* Ellipse parameters */
#define ECX  3.0f
#define ECY  2.0f
#define RX   4.0f
#define RY   2.0f

/* Window dimensions */
#define WIN_W  700
#define WIN_H  700

/* World coordinate range */
#define WORLD_MIN  -10.0f
#define WORLD_MAX   10.0f
#define WORLD_RANGE 20.0f

/* Pastel Pink #F8C8DC */
#define PINK_R (248.0f/255.0f)
#define PINK_G (200.0f/255.0f)
#define PINK_B (220.0f/255.0f)

int worldToScreenX(float wx) {
    return (int)((wx - WORLD_MIN) / WORLD_RANGE * WIN_W);
}
int worldToScreenY(float wy) {
    return (int)((wy - WORLD_MIN) / WORLD_RANGE * WIN_H);
}

/*
 * drawSolidEllipse()
 * Runs the Midpoint Ellipse Algorithm to compute all boundary points,
 * stores them in order, and renders with GL_LINE_LOOP for a solid outline.
 *
 * MIDPOINT ELLIPSE ALGORITHM:
 *   Region 1: start (0, ry), step x until ry²x >= rx²y
 *     p1 init = ry² - rx²ry + rx²/4
 *     p1 < 0 → East:      p1 += 2ry²x + ry²
 *     p1 >= 0 → SouthEast: p1 += 2ry²x - 2rx²y + ry²; y--
 *
 *   Region 2: continue, step y until y = 0
 *     p2 init = ry²(x+0.5)² + rx²(y-1)² - rx²ry²
 *     p2 > 0 → South:     p2 += -2rx²y + rx²
 *     p2 <= 0 → SouthEast: p2 += 2ry²x - 2rx²y + rx²; x++
 *
 * Points stored in all 4 quadrants in CCW order for LINE_LOOP.
 */
void drawSolidEllipse(float cx, float cy, float rx, float ry) {
    /* Temporary storage for first-quadrant points */
    float qx[500], qy[500];
    int   cnt = 0;

    float x = 0, y = ry;
    float p1 = (ry*ry) - (rx*rx*ry) + (0.25f * rx*rx);

    /* Region 1 */
    qx[cnt] = x; qy[cnt] = y; cnt++;
    while ((ry*ry*x) < (rx*rx*y)) {
        x++;
        if (p1 < 0) {
            p1 += 2*ry*ry*x + ry*ry;
        } else {
            y--;
            p1 += 2*ry*ry*x - 2*rx*rx*y + ry*ry;
        }
        qx[cnt] = x; qy[cnt] = y; cnt++;
    }

    /* Region 2 */
    float p2 = (ry*ry)*(x+0.5f)*(x+0.5f) + (rx*rx)*(y-1)*(y-1) - (rx*rx*ry*ry);
    while (y > 0) {
        y--;
        if (p2 > 0) {
            p2 += -2*rx*rx*y + rx*rx;
        } else {
            x++;
            p2 += 2*ry*ry*x - 2*rx*rx*y + rx*rx;
        }
        qx[cnt] = x; qy[cnt] = y; cnt++;
    }

    /*
     * Build full ellipse in CCW order from 4 quadrant mirrors.
     * Draw with GL_LINE_LOOP to connect all points solidly.
     */
    glBegin(GL_LINE_LOOP);
        /* Quadrant 1: top-right, x:0→max (forward) */
        for (int i = 0; i < cnt; i++)
            glVertex2f(cx + qx[i], cy + qy[i]);
        /* Quadrant 4: bottom-right, x:max→0 (reverse) */
        for (int i = cnt-1; i >= 0; i--)
            glVertex2f(cx + qx[i], cy - qy[i]);
        /* Quadrant 3: bottom-left, x:0→max (forward, negated) */
        for (int i = 0; i < cnt; i++)
            glVertex2f(cx - qx[i], cy - qy[i]);
        /* Quadrant 2: top-left, x:max→0 (reverse, negated) */
        for (int i = cnt-1; i >= 0; i--)
            glVertex2f(cx - qx[i], cy + qy[i]);
    glEnd();
}

/*
 * floodFill()
 * Recursive 4-connected flood fill.
 * Reads each pixel; if white (background), paints it pastel pink
 * and recurses on the 4 neighbors.
 */
void floodFill(int sx, int sy) {
    if (sx < 0 || sx >= WIN_W || sy < 0 || sy >= WIN_H) return;

    unsigned char pixel[3];
    glReadPixels(sx, sy, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

    /* Only fill white (background) pixels */
    if (pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255) {
        /* Convert screen pixel back to world coordinates for drawing */
        float wx = (float)sx / WIN_W * WORLD_RANGE + WORLD_MIN;
        float wy = (float)sy / WIN_H * WORLD_RANGE + WORLD_MIN;

        glColor3f(PINK_R, PINK_G, PINK_B);
        glBegin(GL_POINTS);
            glVertex2f(wx, wy);
        glEnd();
        glFlush();

        /* Recurse on 4-connected neighbors */
        floodFill(sx+1, sy);
        floodFill(sx-1, sy);
        floodFill(sx, sy+1);
        floodFill(sx, sy-1);
    }
}

void drawAxes() {
    glColor3f(0.3f, 0.3f, 0.3f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
        glVertex2f(WORLD_MIN, 0.0f); glVertex2f(WORLD_MAX, 0.0f);
        glVertex2f(0.0f, WORLD_MIN); glVertex2f(0.0f, WORLD_MAX);
    glEnd();
    glBegin(GL_LINES);
    for (int i = (int)WORLD_MIN; i <= (int)WORLD_MAX; i++) {
        if (i != 0) {
            glVertex2f((float)i, -0.2f); glVertex2f((float)i, 0.2f);
            glVertex2f(-0.2f, (float)i); glVertex2f(0.2f, (float)i);
        }
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();

    /* Draw solid ellipse boundary */
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(2.0f);
    drawSolidEllipse(ECX, ECY, RX, RY);
    glFlush();  /* Flush so boundary is in framebuffer before flood fill reads it */

    /* Flood fill from center of ellipse */
    glPointSize(1.0f);
    floodFill(worldToScreenX(ECX), worldToScreenY(ECY));

    /* Re-draw boundary on top of fill so it stays crisp */
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(2.0f);
    drawSolidEllipse(ECX, ECY, RX, RY);

    /* Mark center */
    glColor3f(0.0f, 0.0f, 0.8f);
    glPointSize(6.0f);
    glBegin(GL_POINTS);
        glVertex2f(ECX, ECY);
    glEnd();

    glFlush();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(WORLD_MIN, WORLD_MAX, WORLD_MIN, WORLD_MAX);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Q2: Midpoint Ellipse Center(3,2) + Pastel Pink Fill [Solid]");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
