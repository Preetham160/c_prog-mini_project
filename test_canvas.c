#include <stdio.h>

#define CANVAS_ROWS 5
#define CANVAS_COLS 20

int main(void) {
    char canvas[CANVAS_ROWS][CANVAS_COLS + 1];
    
    // Clear and initialize
    for (int row = 0; row < CANVAS_ROWS; row++) {
        for (int col = 0; col < CANVAS_COLS; col++) {
            canvas[row][col] = '_';
        }
        canvas[row][CANVAS_COLS] = '\0';
    }
    
    // Draw a test pixel
    canvas[2][5] = '*';
    
    // Display
    for (int row = 0; row < CANVAS_ROWS; row++) {
        puts(canvas[row]);
    }
    
    return 0;
}
