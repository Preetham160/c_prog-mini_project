#include <stdio.h>
#include <stdlib.h>

#define CANVAS_ROWS 25
#define CANVAS_COLS 60
#define MAX_OBJECTS 100

typedef enum {
    SHAPE_LINE = 1,
    SHAPE_RECTANGLE,
    SHAPE_CIRCLE,
    SHAPE_TRIANGLE
} ShapeType;

typedef struct {
    int active;
    int id;
    ShapeType type;
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int radius;
} Shape;

static void clearCanvas(char canvas[CANVAS_ROWS][CANVAS_COLS + 1]) {
    for (int row = 0; row < CANVAS_ROWS; row++) {
        for (int col = 0; col < CANVAS_COLS; col++) {
            canvas[row][col] = '_';
        }
        canvas[row][CANVAS_COLS] = '\0';
    }
}

static void setPixel(char canvas[CANVAS_ROWS][CANVAS_COLS + 1], int x, int y) {
    if (x >= 0 && x < CANVAS_COLS && y >= 0 && y < CANVAS_ROWS) {
        canvas[y][x] = '*';
    }
}

static int absInt(int value) {
    return value < 0 ? -value : value;
}

static void drawLine(char canvas[CANVAS_ROWS][CANVAS_COLS + 1], int x1, int y1, int x2, int y2) {
    int dx = absInt(x2 - x1);
    int dy = absInt(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        setPixel(canvas, x1, y1);
        if (x1 == x2 && y1 == y2) {
            break;
        }
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

static void drawRectangle(char canvas[CANVAS_ROWS][CANVAS_COLS + 1], int x, int y, int width, int height) {
    if (width <= 0 || height <= 0) {
        return;
    }

    drawLine(canvas, x, y, x + width - 1, y);
    drawLine(canvas, x + width - 1, y, x + width - 1, y + height - 1);
    drawLine(canvas, x + width - 1, y + height - 1, x, y + height - 1);
    drawLine(canvas, x, y + height - 1, x, y);
}

static void drawCircle(char canvas[CANVAS_ROWS][CANVAS_COLS + 1], int centerX, int centerY, int radius) {
    if (radius < 0) {
        return;
    }

    int x = 0;
    int y = radius;
    int decision = 1 - radius;

    while (x <= y) {
        setPixel(canvas, centerX + x, centerY + y);
        setPixel(canvas, centerX - x, centerY + y);
        setPixel(canvas, centerX + x, centerY - y);
        setPixel(canvas, centerX - x, centerY - y);
        setPixel(canvas, centerX + y, centerY + x);
        setPixel(canvas, centerX - y, centerY + x);
        setPixel(canvas, centerX + y, centerY - x);
        setPixel(canvas, centerX - y, centerY - x);

        x++;
        if (decision < 0) {
            decision += 2 * x + 1;
        } else {
            y--;
            decision += 2 * (x - y) + 1;
        }
    }
}

static void drawTriangle(char canvas[CANVAS_ROWS][CANVAS_COLS + 1], int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLine(canvas, x1, y1, x2, y2);
    drawLine(canvas, x2, y2, x3, y3);
    drawLine(canvas, x3, y3, x1, y1);
}

static void renderShapes(char canvas[CANVAS_ROWS][CANVAS_COLS + 1], const Shape shapes[MAX_OBJECTS]) {
    clearCanvas(canvas);

    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (!shapes[i].active) {
            continue;
        }

        switch (shapes[i].type) {
            case SHAPE_LINE:
                drawLine(canvas, shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2);
                break;
            case SHAPE_RECTANGLE:
                drawRectangle(canvas, shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2);
                break;
            case SHAPE_CIRCLE:
                drawCircle(canvas, shapes[i].x1, shapes[i].y1, shapes[i].radius);
                break;
            case SHAPE_TRIANGLE:
                drawTriangle(canvas, shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2, shapes[i].x3, shapes[i].y3);
                break;
            default:
                break;
        }
    }
}

static void displayCanvas(const char canvas[CANVAS_ROWS][CANVAS_COLS + 1]) {
    printf("\n");
    for (int row = 0; row < CANVAS_ROWS; row++) {
        puts(canvas[row]);
    }
    printf("\n");
}

static void displayObjects(const Shape shapes[MAX_OBJECTS]) {
    printf("\nActive objects:\n");
    int found = 0;
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (!shapes[i].active) {
            continue;
        }
        found = 1;
        printf("  ID %d: ", shapes[i].id);
        switch (shapes[i].type) {
            case SHAPE_LINE:
                printf("Line (%d,%d) to (%d,%d)\n", shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2);
                break;
            case SHAPE_RECTANGLE:
                printf("Rectangle at (%d,%d) width=%d height=%d\n", shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2);
                break;
            case SHAPE_CIRCLE:
                printf("Circle center=(%d,%d) radius=%d\n", shapes[i].x1, shapes[i].y1, shapes[i].radius);
                break;
            case SHAPE_TRIANGLE:
                printf("Triangle (%d,%d), (%d,%d), (%d,%d)\n", shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2, shapes[i].x3, shapes[i].y3);
                break;
            default:
                break;
        }
    }
    if (!found) {
        puts("  (none)");
    }
}

static int readInt(const char *prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            return value;
        }
        while (getchar() != '\n') {
        }
        puts("Invalid input. Please enter an integer.");
    }
}

static int findEmptySlot(const Shape shapes[MAX_OBJECTS]) {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (!shapes[i].active) {
            return i;
        }
    }
    return -1;
}

static int findShapeById(Shape shapes[MAX_OBJECTS], int id) {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (shapes[i].active && shapes[i].id == id) {
            return i;
        }
    }
    return -1;
}

static void addShape(Shape shapes[MAX_OBJECTS]) {
    int slot = findEmptySlot(shapes);
    if (slot == -1) {
        puts("Shape limit reached.");
        return;
    }

    int id = readInt("Enter new object ID: ");
    if (findShapeById(shapes, id) != -1) {
        puts("An object with that ID already exists.");
        return;
    }

    printf("Choose shape type:\n");
    printf("  1. Line\n");
    printf("  2. Rectangle\n");
    printf("  3. Circle\n");
    printf("  4. Triangle\n");

    int typeChoice = readInt("Enter choice: ");
    shapes[slot].active = 1;
    shapes[slot].id = id;
    shapes[slot].type = (ShapeType)typeChoice;

    switch (typeChoice) {
        case SHAPE_LINE:
            shapes[slot].x1 = readInt("Enter x1: ");
            shapes[slot].y1 = readInt("Enter y1: ");
            shapes[slot].x2 = readInt("Enter x2: ");
            shapes[slot].y2 = readInt("Enter y2: ");
            break;
        case SHAPE_RECTANGLE:
            shapes[slot].x1 = readInt("Enter top-left x: ");
            shapes[slot].y1 = readInt("Enter top-left y: ");
            shapes[slot].x2 = readInt("Enter width: ");
            shapes[slot].y2 = readInt("Enter height: ");
            break;
        case SHAPE_CIRCLE:
            shapes[slot].x1 = readInt("Enter center x: ");
            shapes[slot].y1 = readInt("Enter center y: ");
            shapes[slot].radius = readInt("Enter radius: ");
            break;
        case SHAPE_TRIANGLE:
            shapes[slot].x1 = readInt("Enter x1: ");
            shapes[slot].y1 = readInt("Enter y1: ");
            shapes[slot].x2 = readInt("Enter x2: ");
            shapes[slot].y2 = readInt("Enter y2: ");
            shapes[slot].x3 = readInt("Enter x3: ");
            shapes[slot].y3 = readInt("Enter y3: ");
            break;
        default:
            puts("Invalid shape type.");
            shapes[slot].active = 0;
            break;
    }
}

static void deleteShape(Shape shapes[MAX_OBJECTS]) {
    int id = readInt("Enter object ID to delete: ");
    int index = findShapeById(shapes, id);
    if (index == -1) {
        puts("Object not found.");
        return;
    }
    shapes[index].active = 0;
    puts("Object deleted.");
}

static void modifyShape(Shape shapes[MAX_OBJECTS]) {
    int id = readInt("Enter object ID to modify: ");
    int index = findShapeById(shapes, id);
    if (index == -1) {
        puts("Object not found.");
        return;
    }

    printf("Current object will be replaced. Choose new shape type:\n");
    printf("  1. Line\n");
    printf("  2. Rectangle\n");
    printf("  3. Circle\n");
    printf("  4. Triangle\n");

    int typeChoice = readInt("Enter choice: ");
    shapes[index].type = (ShapeType)typeChoice;

    switch (typeChoice) {
        case SHAPE_LINE:
            shapes[index].x1 = readInt("Enter x1: ");
            shapes[index].y1 = readInt("Enter y1: ");
            shapes[index].x2 = readInt("Enter x2: ");
            shapes[index].y2 = readInt("Enter y2: ");
            break;
        case SHAPE_RECTANGLE:
            shapes[index].x1 = readInt("Enter top-left x: ");
            shapes[index].y1 = readInt("Enter top-left y: ");
            shapes[index].x2 = readInt("Enter width: ");
            shapes[index].y2 = readInt("Enter height: ");
            break;
        case SHAPE_CIRCLE:
            shapes[index].x1 = readInt("Enter center x: ");
            shapes[index].y1 = readInt("Enter center y: ");
            shapes[index].radius = readInt("Enter radius: ");
            break;
        case SHAPE_TRIANGLE:
            shapes[index].x1 = readInt("Enter x1: ");
            shapes[index].y1 = readInt("Enter y1: ");
            shapes[index].x2 = readInt("Enter x2: ");
            shapes[index].y2 = readInt("Enter y2: ");
            shapes[index].x3 = readInt("Enter x3: ");
            shapes[index].y3 = readInt("Enter y3: ");
            break;
        default:
            puts("Invalid shape type.");
            break;
    }
}

static void showMenu(void) {
    puts("\n2D Graphics Editor");
    puts("1. Add object");
    puts("2. Delete object");
    puts("3. Modify object");
    puts("4. Display picture");
    puts("5. List objects");
    puts("6. Exit");
}

int main(void) {
    Shape shapes[MAX_OBJECTS] = {0};
    char canvas[CANVAS_ROWS][CANVAS_COLS + 1];
    int choice;

    while (1) {
        showMenu();
        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1:
                addShape(shapes);
                break;
            case 2:
                deleteShape(shapes);
                break;
            case 3:
                modifyShape(shapes);
                break;
            case 4: {
                renderShapes(canvas, shapes);
                displayCanvas(canvas);
                break;
            }
            case 5:
                displayObjects(shapes);
                break;
            case 6:
                puts("Exiting editor.");
                return 0;
            default:
                puts("Invalid choice.");
                break;
        }
    }
}
