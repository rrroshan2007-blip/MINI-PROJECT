#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 25
#define COLS 60
#define MAX_OBJECTS 100

typedef enum {
    LINE = 1,
    RECTANGLE,
    TRIANGLE,
    CIRCLE
} ShapeType;

typedef struct {
    int id;
    ShapeType type;
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int radius;
    int active;
} GraphicObject;

char canvas[ROWS][COLS];
GraphicObject objects[MAX_OBJECTS];
int objectCount = 0;

/* ---------- Canvas ---------- */

void initCanvas() {
    for(int i=0;i<ROWS;i++)
        for(int j=0;j<COLS;j++)
            canvas[i][j] = '_';
}

void displayCanvas() {
    printf("\n   ");
    for(int j=0;j<COLS;j++) printf("%d", j%10);
    printf("\n");

    for(int i=0;i<ROWS;i++) {
        printf("%2d ", i);
        for(int j=0;j<COLS;j++)
            printf("%c", canvas[i][j]);
        printf("\n");
    }
}

void plot(int x,int y) {
    if(x>=0 && x<COLS && y>=0 && y<ROWS)
        canvas[y][x]='*';
}

/* ---------- Drawing Algorithms ---------- */

void drawLine(int x0,int y0,int x1,int y1) {
    int dx = abs(x1-x0);
    int sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0);
    int sy = y0<y1 ? 1 : -1;
    int err = dx + dy;

    while(1) {
        plot(x0,y0);

        if(x0==x1 && y0==y1)
            break;

        int e2 = 2*err;

        if(e2>=dy) {
            err += dy;
            x0 += sx;
        }

        if(e2<=dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void drawRectangle(int x1,int y1,int x2,int y2) {
    drawLine(x1,y1,x2,y1);
    drawLine(x2,y1,x2,y2);
    drawLine(x2,y2,x1,y2);
    drawLine(x1,y2,x1,y1);
}

void drawTriangle(int x1,int y1,int x2,int y2,int x3,int y3) {
    drawLine(x1,y1,x2,y2);
    drawLine(x2,y2,x3,y3);
    drawLine(x3,y3,x1,y1);
}

void drawCircle(int xc,int yc,int r) {
    int x=0,y=r;
    int d=3-2*r;

    while(y>=x) {
        plot(xc+x,yc+y);
        plot(xc-x,yc+y);
        plot(xc+x,yc-y);
        plot(xc-x,yc-y);

        plot(xc+y,yc+x);
        plot(xc-y,yc+x);
        plot(xc+y,yc-x);
        plot(xc-y,yc-x);

        x++;

        if(d>0) {
            y--;
            d += 4*(x-y)+10;
        }
        else {
            d += 4*x+6;
        }
    }
}

/* ---------- Redraw ---------- */

void redrawAll() {
    initCanvas();

    for(int i=0;i<objectCount;i++) {

        if(!objects[i].active)
            continue;

        switch(objects[i].type) {

            case LINE:
                drawLine(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].x2,
                    objects[i].y2
                );
                break;

            case RECTANGLE:
                drawRectangle(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].x2,
                    objects[i].y2
                );
                break;

            case TRIANGLE:
                drawTriangle(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].x2,
                    objects[i].y2,
                    objects[i].x3,
                    objects[i].y3
                );
                break;

            case CIRCLE:
                drawCircle(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].radius
                );
                break;
        }
    }
}

/* ---------- Object List ---------- */

void listObjects() {
    printf("\nActive Objects:\n");

    for(int i=0;i<objectCount;i++) {

        if(objects[i].active)
            printf("ID %d -> Type %d\n",
                   objects[i].id,
                   objects[i].type);
    }
}

/* ---------- Add ---------- */

void addObject() {

    if(objectCount>=MAX_OBJECTS) {
        printf("Maximum objects reached!\n");
        return;
    }

    GraphicObject obj;

    obj.id = objectCount;
    obj.active = 1;

    printf("\n1.Line\n2.Rectangle\n3.Triangle\n4.Circle\n");
    printf("Choice: ");
    scanf("%d",(int*)&obj.type);

    switch(obj.type) {

        case LINE:
            printf("Enter x1 y1 x2 y2: ");
            scanf("%d%d%d%d",
                &obj.x1,&obj.y1,
                &obj.x2,&obj.y2);
            break;

        case RECTANGLE:
            printf("Enter top-left and bottom-right:\n");
            scanf("%d%d%d%d",
                &obj.x1,&obj.y1,
                &obj.x2,&obj.y2);
            break;

        case TRIANGLE:
            printf("Enter x1 y1 x2 y2 x3 y3:\n");
            scanf("%d%d%d%d%d%d",
                &obj.x1,&obj.y1,
                &obj.x2,&obj.y2,
                &obj.x3,&obj.y3);
            break;

        case CIRCLE:
            printf("Enter center x y radius:\n");
            scanf("%d%d%d",
                &obj.x1,
                &obj.y1,
                &obj.radius);
            break;

        default:
            printf("Invalid choice!\n");
            return;
    }

    objects[objectCount++] = obj;
    redrawAll();
}

/* ---------- Delete ---------- */

void deleteObject() {

    listObjects();

    int id;

    printf("Enter ID to delete: ");
    scanf("%d",&id);

    if(id>=0 && id<objectCount &&
       objects[id].active) {

        objects[id].active = 0;
        redrawAll();

        printf("Deleted.\n");
    }
    else {
        printf("Invalid ID.\n");
    }
}

/* ---------- Modify ---------- */

void modifyObject() {

    listObjects();

    int id;

    printf("Enter ID to modify: ");
    scanf("%d",&id);

    if(id<0 || id>=objectCount ||
       !objects[id].active) {

        printf("Invalid ID.\n");
        return;
    }

    switch(objects[id].type) {

        case LINE:
            printf("New x1 y1 x2 y2: ");
            scanf("%d%d%d%d",
                &objects[id].x1,
                &objects[id].y1,
                &objects[id].x2,
                &objects[id].y2);
            break;

        case RECTANGLE:
            printf("New coordinates: ");
            scanf("%d%d%d%d",
                &objects[id].x1,
                &objects[id].y1,
                &objects[id].x2,
                &objects[id].y2);
            break;

        case TRIANGLE:
            printf("New coordinates:\n");
            scanf("%d%d%d%d%d%d",
                &objects[id].x1,
                &objects[id].y1,
                &objects[id].x2,
                &objects[id].y2,
                &objects[id].x3,
                &objects[id].y3);
            break;

        case CIRCLE:
            printf("New center x y radius: ");
            scanf("%d%d%d",
                &objects[id].x1,
                &objects[id].y1,
                &objects[id].radius);
            break;
    }

    redrawAll();
}

/* ---------- Main ---------- */

int main() {

    int choice;

    initCanvas();

    do {

        printf("\n===== 2D GRAPHICS EDITOR =====\n");
        printf("1. Add Object\n");
        printf("2. Delete Object\n");
        printf("3. Modify Object\n");
        printf("4. Display Picture\n");
        printf("5. List Objects\n");
        printf("6. Exit\n");
        printf("Choice: ");

        scanf("%d",&choice);

        switch(choice) {

            case 1:
                addObject();
                break;

            case 2:
                deleteObject();
                break;

            case 3:
                modifyObject();
                break;

            case 4:
                displayCanvas();
                break;

            case 5:
                listObjects();
                break;

            case 6:
                printf("Goodbye!\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while(choice != 6);

    return 0;
}#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 25
#define COLS 60
#define MAX_OBJECTS 100

typedef enum {
    LINE = 1,
    RECTANGLE,
    TRIANGLE,
    CIRCLE
} ShapeType;

typedef struct {
    int id;
    ShapeType type;
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int radius;
    int active;
} GraphicObject;

char canvas[ROWS][COLS];
GraphicObject objects[MAX_OBJECTS];
int objectCount = 0;

/* ---------- Canvas ---------- */

void initCanvas() {
    for(int i=0;i<ROWS;i++)
        for(int j=0;j<COLS;j++)
            canvas[i][j] = '_';
}

void displayCanvas() {
    printf("\n   ");
    for(int j=0;j<COLS;j++) printf("%d", j%10);
    printf("\n");

    for(int i=0;i<ROWS;i++) {
        printf("%2d ", i);
        for(int j=0;j<COLS;j++)
            printf("%c", canvas[i][j]);
        printf("\n");
    }
}

void plot(int x,int y) {
    if(x>=0 && x<COLS && y>=0 && y<ROWS)
        canvas[y][x]='*';
}

/* ---------- Drawing Algorithms ---------- */

void drawLine(int x0,int y0,int x1,int y1) {
    int dx = abs(x1-x0);
    int sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0);
    int sy = y0<y1 ? 1 : -1;
    int err = dx + dy;

    while(1) {
        plot(x0,y0);

        if(x0==x1 && y0==y1)
            break;

        int e2 = 2*err;

        if(e2>=dy) {
            err += dy;
            x0 += sx;
        }

        if(e2<=dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void drawRectangle(int x1,int y1,int x2,int y2) {
    drawLine(x1,y1,x2,y1);
    drawLine(x2,y1,x2,y2);
    drawLine(x2,y2,x1,y2);
    drawLine(x1,y2,x1,y1);
}

void drawTriangle(int x1,int y1,int x2,int y2,int x3,int y3) {
    drawLine(x1,y1,x2,y2);
    drawLine(x2,y2,x3,y3);
    drawLine(x3,y3,x1,y1);
}

void drawCircle(int xc,int yc,int r) {
    int x=0,y=r;
    int d=3-2*r;

    while(y>=x) {
        plot(xc+x,yc+y);
        plot(xc-x,yc+y);
        plot(xc+x,yc-y);
        plot(xc-x,yc-y);

        plot(xc+y,yc+x);
        plot(xc-y,yc+x);
        plot(xc+y,yc-x);
        plot(xc-y,yc-x);

        x++;

        if(d>0) {
            y--;
            d += 4*(x-y)+10;
        }
        else {
            d += 4*x+6;
        }
    }
}

/* ---------- Redraw ---------- */

void redrawAll() {
    initCanvas();

    for(int i=0;i<objectCount;i++) {

        if(!objects[i].active)
            continue;

        switch(objects[i].type) {

            case LINE:
                drawLine(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].x2,
                    objects[i].y2
                );
                break;

            case RECTANGLE:
                drawRectangle(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].x2,
                    objects[i].y2
                );
                break;

            case TRIANGLE:
                drawTriangle(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].x2,
                    objects[i].y2,
                    objects[i].x3,
                    objects[i].y3
                );
                break;

            case CIRCLE:
                drawCircle(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].radius
                );
                break;
        }
    }
}

/* ---------- Object List ---------- */

void listObjects() {
    printf("\nActive Objects:\n");

    for(int i=0;i<objectCount;i++) {

        if(objects[i].active)
            printf("ID %d -> Type %d\n",
                   objects[i].id,
                   objects[i].type);
    }
}

/* ---------- Add ---------- */

void addObject() {

    if(objectCount>=MAX_OBJECTS) {
        printf("Maximum objects reached!\n");
        return;
    }

    GraphicObject obj;

    obj.id = objectCount;
    obj.active = 1;

    printf("\n1.Line\n2.Rectangle\n3.Triangle\n4.Circle\n");
    printf("Choice: ");
    scanf("%d",(int*)&obj.type);

    switch(obj.type) {

        case LINE:
            printf("Enter x1 y1 x2 y2: ");
            scanf("%d%d%d%d",
                &obj.x1,&obj.y1,
                &obj.x2,&obj.y2);
            break;

        case RECTANGLE:
            printf("Enter top-left and bottom-right:\n");
            scanf("%d%d%d%d",
                &obj.x1,&obj.y1,
                &obj.x2,&obj.y2);
            break;

        case TRIANGLE:
            printf("Enter x1 y1 x2 y2 x3 y3:\n");
            scanf("%d%d%d%d%d%d",
                &obj.x1,&obj.y1,
                &obj.x2,&obj.y2,
                &obj.x3,&obj.y3);
            break;

        case CIRCLE:
            printf("Enter center x y radius:\n");
            scanf("%d%d%d",
                &obj.x1,
                &obj.y1,
                &obj.radius);
            break;

        default:
            printf("Invalid choice!\n");
            return;
    }

    objects[objectCount++] = obj;
    redrawAll();
}

/* ---------- Delete ---------- */

void deleteObject() {

    listObjects();

    int id;

    printf("Enter ID to delete: ");
    scanf("%d",&id);

    if(id>=0 && id<objectCount &&
       objects[id].active) {

        objects[id].active = 0;
        redrawAll();

        printf("Deleted.\n");
    }
    else {
        printf("Invalid ID.\n");
    }
}

/* ---------- Modify ---------- */

void modifyObject() {

    listObjects();

    int id;

    printf("Enter ID to modify: ");
    scanf("%d",&id);

    if(id<0 || id>=objectCount ||
       !objects[id].active) {

        printf("Invalid ID.\n");
        return;
    }

    switch(objects[id].type) {

        case LINE:
            printf("New x1 y1 x2 y2: ");
            scanf("%d%d%d%d",
                &objects[id].x1,
                &objects[id].y1,
                &objects[id].x2,
                &objects[id].y2);
            break;

        case RECTANGLE:
            printf("New coordinates: ");
            scanf("%d%d%d%d",
                &objects[id].x1,
                &objects[id].y1,
                &objects[id].x2,
                &objects[id].y2);
            break;

        case TRIANGLE:
            printf("New coordinates:\n");
            scanf("%d%d%d%d%d%d",
                &objects[id].x1,
                &objects[id].y1,
                &objects[id].x2,
                &objects[id].y2,
                &objects[id].x3,
                &objects[id].y3);
            break;

        case CIRCLE:
            printf("New center x y radius: ");
            scanf("%d%d%d",
                &objects[id].x1,
                &objects[id].y1,
                &objects[id].radius);
            break;
    }

    redrawAll();
}

/* ---------- Main ---------- */

int main() {

    int choice;

    initCanvas();

    do {

        printf("\n===== 2D GRAPHICS EDITOR =====\n");
        printf("1. Add Object\n");
        printf("2. Delete Object\n");
        printf("3. Modify Object\n");
        printf("4. Display Picture\n");
        printf("5. List Objects\n");
        printf("6. Exit\n");
        printf("Choice: ");

        scanf("%d",&choice);

        switch(choice) {

            case 1:
                addObject();
                break;

            case 2:
                deleteObject();
                break;

            case 3:
                modifyObject();
                break;

            case 4:
                displayCanvas();
                break;

            case 5:
                listObjects();
                break;

            case 6:
                printf("Goodbye!\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while(choice != 6);

    return 0;
}
