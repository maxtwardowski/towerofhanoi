#include "primlib.h"
#include <stdlib.h>
#include <math.h>

#define FPS_RATE 60
#define DISCS 3
#define PEGS 3
#define DISC_WIDTH 70
#define DISC_HEIGHT 20
#define PEG_WIDTH 10
#define PEG_HEIGHT 170

int gameboard[DISCS][PEGS];
int stack_info[PEGS];

void setDefault_gameboard();
void setDefault_stack();
void cleanScreen();
void drawPegs(int pegs_number);
void drawDiscs();
void drawDiscMoveAnimation(int peg_from, int peg_to, int disctopush);
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
    int x1 = 0, x2 = 0;
    int x1_base = screenWidth() / (pegs_number + 1),
        y1_base = PEG_HEIGHT,
        x2_base = screenWidth() / (pegs_number + 1),
        y2_base = screenHeight();

    for (int i = 0; i < pegs_number; i++) {
        x1 += x1_base;
        x2 += x2_base;
        filledRect(x1 - PEG_WIDTH, y1_base, x2 + PEG_WIDTH, y2_base, WHITE);
    }
}

//LATER: remove duplicate variables
void drawDiscs() {
    int x1_base = screenWidth() / (PEGS + 1),
        y1_base = screenHeight(),
        x2_base = screenWidth() / (PEGS + 1),
        y2_base = screenHeight() - DISC_HEIGHT;

    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

    for (int i = 0; i < PEGS; i++) {
        x1 += x1_base;
        y1 = y1_base;
        x2 += x2_base;
        y2 = y2_base;
        for (int j = 0; j <= stack_info[i]; j++) {
            filledRect(x1 - DISC_WIDTH * gameboard[j][i] / DISCS,
                       y1,
                       x2 + DISC_WIDTH * gameboard[j][i] / DISCS,
                       y2,
                       RED);
            y1 -= DISC_HEIGHT;
            y2 -= DISC_HEIGHT;
        }
    }
}

void drawDiscMoveAnimation(int peg_from, int peg_to, int disctopush) {
    int x1_base = screenWidth() / (PEGS + 1),
        y1_base = screenHeight(),
        x2_base = screenWidth() / (PEGS + 1),
        y2_base = screenHeight() - DISC_HEIGHT;

    int x1_from = x1_base * (peg_from + 1) - DISC_WIDTH * disctopush / DISCS,
        y1_from = y1_base - DISC_HEIGHT * (stack_info[peg_from] + 1),
        x2_from = x2_base * (peg_from + 1) + DISC_WIDTH * disctopush / DISCS,
        y2_from = y2_base - DISC_HEIGHT * (stack_info[peg_from] + 1),
        x1_to = x1_base * (peg_to + 1) - DISC_WIDTH * disctopush / DISCS,
        y1_to = y1_base - DISC_HEIGHT * (stack_info[peg_to] + 1),
        x2_to = x1_base * (peg_to + 1) - DISC_WIDTH * disctopush / DISCS,
        y2_to = y2_base - DISC_HEIGHT * (stack_info[peg_to] + 1);

    do {
        cleanScreen();
        drawPegs(PEGS);
        drawDiscs();
        if (x1_from < x1_to) {
            x1_from++, x2_from++;
            filledRect(x1_from,
                       y1_from,
                       x2_from,
                       y2_from,
                       RED);
        }
        else {
            x1_from--, x2_from--;
            filledRect(x1_from, y1_from, x2_from, y2_from, RED);
        }
        updateScreen();
        SDL_Delay(1000 / 600);
    } while (x1_from != x1_to);

    do {
        cleanScreen();
        drawPegs(PEGS);
        drawDiscs();
        if (y1_from < y1_to) {
            y1_from++, y2_from++;
            filledRect(x1_from, y1_from, x2_from, y2_from, RED);
        }
        else {
            y1_from--, y2_from--;
            filledRect(x1_from, y1_from, x2_from, y2_from, RED);
        }
        updateScreen();
        SDL_Delay(1000 / 60);
    } while (y1_from != y1_to);

}

void moveDisc(int peg_from, int peg_to) {
    if (stack_info[peg_from] != -1) {
        int disctopush = gameboard[stack_info[peg_from]][peg_from];
        if (stack_info[peg_to] == -1 || gameboard[stack_info[peg_to]][peg_to] > disctopush) {
            gameboard[stack_info[peg_from]][peg_from] = 0; //Indicating empty stack slot
            stack_info[peg_from]--; //Decreasing the origin stack index
            drawDiscMoveAnimation(peg_from, peg_to, disctopush);
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

    if (key1 < PEGS && key2 < PEGS) //Protection against moving disc to an unexisting peg
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
