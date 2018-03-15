#include "primlib.h"
#include <stdlib.h>
#include <math.h>

#define FPS_RATE 60
#define DISCS 2
#define PEGS 10

int gameboard[DISCS][PEGS];

void cleanScreen();
void drawPegs(int pegs_number);

int main(int argc, char* argv[]) {

    if(initGraph()) {
        exit(3);
    }

    while(1) {
        cleanScreen();
        drawPegs(PEGS);
        updateScreen();


        if (isKeyDown(SDLK_ESCAPE))
            exit(1);
    }

    return 0;
}

void cleanScreen() {
	filledRect(0, 0, screenWidth(), screenHeight(), BLACK);
}

void drawPegs(int pegs_number) {
    int x1 = 0, x2 = 0, peg_width = 10, peg_height = 170;

    int x1_base = screenWidth() / (pegs_number + 1),
        y1_base = peg_height,
        x2_base = screenWidth() / (pegs_number + 1),
        y2_base = screenHeight();

    filledRect(x1_base, y1_base, x2_base, y2_base, WHITE);

    for (int i = 0; i < pegs_number; i++) {
        x1 += x1_base;
        x2 += x2_base;
        filledRect(x1 - peg_width, y1_base, x2 + peg_width, y2_base, WHITE);
    }
}
