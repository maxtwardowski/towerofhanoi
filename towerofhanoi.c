#include "primlib.h"
#include <stdlib.h>
#include <math.h>

#define FPS_RATE 60
#define DISCS 3
#define PEGS 3

int gameboard[DISCS][PEGS];
int stack_info[PEGS];

void setDefault_gameboard();
void setDefault_stack();
void cleanScreen();
void drawPegs(int pegs_number);
void drawDiscs();
void moveDisc(int peg_from, int peg_to);
void keyDetect(int key1);
void checkWin();
void testPrint_gameboard();
void testPrint_stack();

int main(int argc, char* argv[]) {

    setDefault_gameboard();
    setDefault_stack();

    if(initGraph()) {
        exit(3);
    }

    while(1) {
        cleanScreen();
        drawPegs(PEGS);
        drawDiscs();
        updateScreen();
        checkWin();

        testPrint_gameboard();
        testPrint_stack();
        //printf("%d\n", getkey());

        int keyinput1 = getkey();
        if (keyinput1)
            keyDetect(keyinput1);

        if (isKeyDown(SDLK_ESCAPE))
            exit(1);
    }

    return 0;
}

void setDefault_gameboard() {
    for (int i = 0; i < PEGS; i++) {
        for (int j = 0; j < DISCS; j++) {
            if (i == 0)
                gameboard[j][i] = DISCS - j;
            else
                gameboard[j][i] = 0;
        }
    }
}

void setDefault_stack() {
    for (int i = 0; i < PEGS; i++) {
        if (!i)
            stack_info[i] = DISCS - 1;
        else
            stack_info[i] = -1;
    }
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

    for (int i = 0; i < pegs_number; i++) {
        x1 += x1_base;
        x2 += x2_base;
        filledRect(x1 - peg_width, y1_base, x2 + peg_width, y2_base, WHITE);
    }
}


//remove duplicate variables
void drawDiscs() {
    int disc_width = 70, disc_height = 20;

    int x1_base = screenWidth() / (PEGS + 1),
        y1_base = screenHeight(),
        x2_base = screenWidth() / (PEGS + 1),
        y2_base = screenHeight() - disc_height;

    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

    for (int i = 0; i < PEGS; i++) {
        x1 += x1_base;
        y1 = y1_base;
        x2 += x2_base;
        y2 = y2_base;
        for (int j = 0; j <= stack_info[i]; j++) {
            filledRect(x1 - disc_width, y1, x2 + disc_width, y2, RED);
            y1 -= disc_height;
            y2 -= disc_height;
        }
    }
}

void moveDisc(int peg_from, int peg_to) {
    if (stack_info[peg_from] != -1) {
        int disctopush = gameboard[stack_info[peg_from]][peg_from];
        if (stack_info[peg_to] == -1 || gameboard[stack_info[peg_to]][peg_to] > disctopush) {
            gameboard[stack_info[peg_from]][peg_from] = 0; //Indicating empty stack slot
            stack_info[peg_from]--; //Decreasing the origin stack index
            stack_info[peg_to]++; //Increasing the destination stack index
            gameboard[stack_info[peg_to]][peg_to] = disctopush;
        }
    }
}

void keyDetect(int key1) {
    int action1, action2;
    int key2 = getkey();

    if (key1 == 48)
        key1 = 9;
    else if (key2 == 48)
        key2 = 9;
    else {
        key1 -= 49;
        key2 -= 49;
    }

    moveDisc(key1, key2);
}

void checkWin() {
    if (stack_info[DISCS - 1] == DISCS - 1)
        exit(1);
}

void testPrint_gameboard() {
    printf("\n====GAMEBOARD TEST====\n");
    for (int i = 0; i < PEGS; i++) {
        for (int j = 0; j < DISCS; j++) {
            printf("%d\n", gameboard[j][i]);
        }
    }
    printf("====GAMEBOARD TEST END====\n");
}

void testPrint_stack() {
    printf("\n====STACKTEST====\n");
    for (int i = 0; i < PEGS; i++)
        printf("%d\n", stack_info[i]);
    printf("====STACKTEST END====\n");
}
