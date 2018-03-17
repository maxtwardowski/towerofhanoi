#include "primlib.h"
#include <stdlib.h>
#include <math.h>

#define DISCS 50
#define PEGS 3
#define FPS_RATE 700
#define DISC_HEIGHT 20
#define PEG_HEIGHT_THRESHOLD 99

int gameboard[DISCS][PEGS];
int stack_info[PEGS];

void setDefault_gameboard();
void setDefault_stack();
void cleanScreen();
void drawPegs(int pegs_number);
void drawDiscs();
void drawDiscMoveAnimation(int peg_from, int peg_to, int disctopush);
void moveDisc(int peg_from, int peg_to);
void keyDetect();
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
        keyDetect();
    }

    return 0;
}

void setDefault_gameboard() {
    //Sets default values for gameboard
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
    //Sets default values for stack_info
    for (int i = 0; i < PEGS; i++) {
        if (!i)
            stack_info[i] = DISCS - 1;
        else
            stack_info[i] = -1;
    }
}

void cleanScreen() {
    //Cleans the screen drawing a black background
	filledRect(0, 0, screenWidth(), screenHeight(), BLACK);
}

void drawPegs(int pegs_number) {
    //Draws the given number of pegs
    int x1 = 0,
        x2 = 0,
        x1_base = screenWidth() / (pegs_number + 1),
        y1_base = screenHeight() - DISC_HEIGHT * (PEG_HEIGHT_THRESHOLD - DISCS) / PEG_HEIGHT_THRESHOLD * (DISCS + 1),
        x2_base = screenWidth() / (pegs_number + 1),
        y2_base = screenHeight(),
        peg_width = x1_base / 2 / DISCS;

    for (int i = 0; i < pegs_number; i++) {
        x1 += x1_base;
        x2 += x2_base;
        filledRect(x1 - peg_width, y1_base, x2 + peg_width, y2_base, WHITE);
    }
}

void drawDiscs() {
    //Draws discs on the pegs according to the gameboard and stack_info data
    int x1_base = screenWidth() / (PEGS + 1),
        y1_base = screenHeight(),
        x2_base = x1_base,
        y2_base = screenHeight() - DISC_HEIGHT * (PEG_HEIGHT_THRESHOLD - DISCS) / PEG_HEIGHT_THRESHOLD;

    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

    for (int i = 0; i < PEGS; i++) {
        x1 += x1_base;
        y1 = y1_base;
        x2 += x2_base;
        y2 = y2_base;
        for (int j = 0; j <= stack_info[i]; j++) {
            filledRect(x1 - x1_base / 2 * gameboard[j][i] / DISCS,
                       y1,
                       x2 + x1_base / 2 * gameboard[j][i] / DISCS,
                       y2,
                       RED);
            y1 -= DISC_HEIGHT * (PEG_HEIGHT_THRESHOLD - DISCS) / PEG_HEIGHT_THRESHOLD;
            y2 -= DISC_HEIGHT * (PEG_HEIGHT_THRESHOLD - DISCS) / PEG_HEIGHT_THRESHOLD;
        }
    }
}

void drawDiscMoveAnimation(int peg_from, int peg_to, int disctopush) {
    //Animates the action of moving a disc from one peg to another
    int heightstep = DISC_HEIGHT * (PEG_HEIGHT_THRESHOLD - DISCS) / PEG_HEIGHT_THRESHOLD,
        x1_base = screenWidth() / (PEGS + 1),
        y1_base = screenHeight(),
        x2_base = screenWidth() / (PEGS + 1),
        y2_base = screenHeight() - heightstep;

    int x1_from = x1_base * (peg_from + 1) - x1_base / 2 * disctopush / DISCS,
        y1_from = y1_base - heightstep * (stack_info[peg_from] + 1),
        x2_from = x2_base * (peg_from + 1) + x1_base / 2 * disctopush / DISCS,
        y2_from = y2_base - heightstep * (stack_info[peg_from] + 1),
        x1_to = x1_base * (peg_to + 1) - x1_base / 2 * disctopush / DISCS,
        y1_to = y1_base - heightstep * (stack_info[peg_to] + 1);

    //Simulating X axis animation
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
        SDL_Delay(1000 / FPS_RATE);
    } while (x1_from != x1_to);

    //Simulating Y axis animation
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
        SDL_Delay(1000 / FPS_RATE);
    } while (y1_from != y1_to);

}

void moveDisc(int peg_from, int peg_to) {
    //Moves disc from one peg to another
    //by manipulating the gameboard and stack_info
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

void keyDetect() {
    //Detects keyinput and triggers adequate actions
    int key1 = getkey();
    //Preventing memory errors caused by assigning SDLK_ESCAPE code to the disctopush
    if (key1 == 27)
        exit(1);
    else {
        if (key1 == 48)
            key1 = 9;
        else
            key1 -= 49; //Subtracting 49 to 'convert' the keycode to its actual value (0 acts like 10 though)

        //Not allowing the secong key if impossible action triggered
        //so that no unnecessary read-outs occur
        if (stack_info[key1] != -1 && key1 < PEGS) {
            int key2 = getkey();
            if (key2 == 48)
                key2 = 9;
            else
                key2 -= 49;
            if (key1 < PEGS && key2 < PEGS) //Protection against moving disc to an unexisting peg
                moveDisc(key1, key2);
        }
    }
}

void checkWin() {
    //Checks if the win state occurs
    //If it does, goodbye text is drawn and the program terminates
    if (stack_info[DISCS - 1] == DISCS - 1) {
        textout(screenWidth() / 2 - 120, screenHeight() / 2, "Congratulations, you won!", GREEN);
        updateScreen();
        SDL_Delay(2000);
        exit(1);
    }
}

void testPrint_gameboard() {
    //Use for checking the content of gameboard
    printf("\n====GAMEBOARD TEST====\n");
    for (int i = 0; i < PEGS; i++) {
        for (int j = 0; j < DISCS; j++) {
            printf("%d\n", gameboard[j][i]);
        }
    }
    printf("====GAMEBOARD TEST END====\n");
}

void testPrint_stack() {
    //Use for checking the content of stack_info
    printf("\n====STACKTEST====\n");
    for (int i = 0; i < PEGS; i++)
        printf("%d\n", stack_info[i]);
    printf("====STACKTEST END====\n");
}
