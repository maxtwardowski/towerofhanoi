#include "primlib.h"
#include <stdlib.h>
#include <math.h>

#define FPS_RATE 60
#define DISCS 3
#define rod_width 17
#define rod_height 170
#define displacement 200
#define blockwidth 70

typedef struct Rod {
    int position;
    int stack[DISCS];
    int base_x1;
    int base_y1;
    int base_x2;
    int base_y2;
} Rod;

int disc_x1cords[DISCS], disc_y1cords[DISCS], disc_x2cords[DISCS], disc_y2cords[DISCS];

void cleanScreen();
void drawRods(int xcord1, int ycord1, int xcord2, int ycord2, int disp);
void discMove(Rod *a, Rod *b);
void drawDiscs();
void checkWin(Rod *goal);
void drawIndicator(int current_position, int xcord);
int keyDetect(int indicator_position, Rod *Rod1, Rod *Rod2, Rod *Rod3);
void testPrint(Rod *x, Rod *y, Rod *z);

int main(int argc, char* argv[]) {

    if(initGraph()) {
        exit(3);
    }

    int x1 = screenWidth() / 2 - rod_width,
        y1 = rod_height,
        x2 = x1 + 2 * rod_width,
        y2 = screenHeight();

    //Declaring the Rods
    struct Rod Rod1, Rod2, Rod3;

    for(int i = 0; i < DISCS; i++) {
        Rod1.stack[i] = DISCS - i;
        Rod2.stack[i] = 0;
        Rod3.stack[i] = 0;
    }

    Rod1.position = DISCS - 1; //all disc on Rod1 by default
    Rod2.position = -1; //empty by default
    Rod3.position = -1;

    Rod1.base_x1 = x1 - displacement - blockwidth;
    Rod1.base_y1 = screenHeight();
    Rod1.base_x2 = x2 - displacement + blockwidth;
    Rod1.base_y2 = screenHeight() - 20;

    Rod2.base_x1 = x1 - blockwidth;
    Rod2.base_y1 = Rod1.base_y1;
    Rod2.base_x2 = x2 + blockwidth;
    Rod2.base_y2 = Rod1.base_y2;

    Rod3.base_x1 = x1 + displacement - blockwidth;
    Rod3.base_y1 = Rod1.base_y1;
    Rod3.base_x2 = x2 + displacement + blockwidth;
    Rod3.base_y2 = Rod1.base_y2;

    for (int i = 0; i < DISCS; i++) {
        disc_x1cords[i] = x1 - displacement - blockwidth + 10 * (Rod1.stack[DISCS - i - 1] - 1);
        disc_y1cords[i] = screenHeight() - 25 * (Rod1.stack[DISCS - i - 1] - 1);
        disc_x2cords[i] = x2 - displacement + blockwidth - 10 * (Rod1.stack[DISCS - i - 1] - 1);
        disc_y2cords[i] = screenHeight() - 20 - 25 * (Rod1.stack[DISCS - i - 1] - 1);
    }

    int indicator_position = 1;

    while(1) {
        cleanScreen();
        drawRods(x1, y1, x2, y2, displacement);
        drawDiscs();
        drawIndicator(indicator_position, x1);
        updateScreen(); //Refreshing the screen
        SDL_Delay(1000 / FPS_RATE); //Setting FPS cap
        checkWin(&Rod3);
        getkey();
        indicator_position = keyDetect(indicator_position, &Rod1, &Rod2, &Rod3);
    }
    return 0;
}

void cleanScreen() {
	filledRect(0, 0, screenWidth(), screenHeight(), BLACK);
}

void drawRods(int xcord1, int ycord1, int xcord2, int ycord2, int disp) {
    filledRect(xcord1, ycord1, xcord2, ycord2, WHITE);
    filledRect(xcord1 - displacement, ycord1, xcord2 - disp, ycord2, WHITE);
    filledRect(xcord1 + displacement, ycord1, xcord2 + disp, ycord2, WHITE);
}

void discMove(Rod *a, Rod *b) {
    //This function moves the disc by popping it and pushing into another stack (Rod)
    if (a->position != -1) {
        int disctopush = a->stack[a->position];
        if (b->stack[b->position] > disctopush || b->position == -1) {
            int x1 = screenWidth() / 2 - rod_width,
            y1 = rod_height,
            x2 = x1 + 2 * rod_width,
            y2 = screenHeight();
            //X axis animation
            do {
                if (b->base_x1 + 10 * (DISCS - a->stack[a->position]) - disc_x1cords[DISCS - disctopush] > 0) {
                    disc_x1cords[DISCS - disctopush]++;
                    disc_x2cords[DISCS - disctopush]++;
                } else {
                    disc_x1cords[DISCS - disctopush]--;
                    disc_x2cords[DISCS - disctopush]--;
                }
                cleanScreen();
                drawRods(x1, y1, x2, y2, displacement);
                drawDiscs();
                updateScreen();
                SDL_Delay(1000 / 900);
            } while (disc_x1cords[DISCS - disctopush] != b->base_x1 + 10 * (DISCS - a->stack[a->position]));

            //Y axis animation
            do {
                if (b->base_y1 - 25 * (b->position + 1) - disc_y1cords[DISCS - disctopush] > 0) {
                    disc_y1cords[DISCS - disctopush]++;
                    disc_y2cords[DISCS - disctopush]++;
                } else {
                    disc_y1cords[DISCS - disctopush]--;
                    disc_y2cords[DISCS - disctopush]--;
                }
                cleanScreen();
                drawRods(x1, y1, x2, y2, displacement);
                drawDiscs();
                updateScreen();
                SDL_Delay(1000 / FPS_RATE);
            } while (disc_y1cords[DISCS - disctopush] != b->base_y1 - 25 * (b->position + 1));

            a->stack[a->position] = 0;
            a->position = a->position - 1;
            b->position = b->position + 1;
            b->stack[b->position] = disctopush;
        }
    }
}

void drawDiscs() {
    for (int i = 0; i < DISCS; i++) {
        filledRect(
            disc_x1cords[i],
            disc_y1cords[i],
            disc_x2cords[i],
            disc_y2cords[i],
            RED
        );
    }
}

void checkWin(Rod *goal) {
    if (goal->position == DISCS - 1) {
        updateScreen();
        SDL_Delay(1000);
        exit(1);
    }
}

void drawIndicator(int current_position, int xcord) {
    switch (current_position) {
        case 1:
            filledCircle(xcord - displacement + rod_width, 50, 20, GREEN);
            break;
        case 2:
            filledCircle(xcord + rod_width, 50, 20, GREEN);
            break;
        case 3:
            filledCircle(xcord + displacement + rod_width, 50, 20, GREEN);
            break;
        default:
            break;
    }
}

int keyDetect(int indicator_position, Rod *Rod1, Rod *Rod2, Rod *Rod3) {
    if (isKeyDown(SDLK_LEFT)) {
        if (indicator_position == 1)
            indicator_position = 3;
        else
            indicator_position--;
    }
    else if (isKeyDown(SDLK_RIGHT)) {
        if (indicator_position == 3)
            indicator_position = 1;
        else
            indicator_position++;
    }
    else if (isKeyDown(SDLK_1)) {
        switch (indicator_position) {
            case 1:
                break;
            case 2:
                discMove(Rod2, Rod1);
                break;
            case 3:
                discMove(Rod3, Rod1);
                break;
            default:
                break;
        }
    }
    else if (isKeyDown(SDLK_2)) {
        switch (indicator_position) {
            case 1:
                discMove(Rod1, Rod2);
                break;
            case 2:
                break;
            case 3:
                discMove(Rod3, Rod2);
                break;
            default:
                break;
        }
    }
    else if (isKeyDown(SDLK_3)) {
        switch (indicator_position) {
            case 1:
                discMove(Rod1, Rod3);
                break;
            case 2:
                discMove(Rod2, Rod3);
                break;
            case 3:
                break;
            default:
                break;
        }
    }
    else if (isKeyDown(SDLK_ESCAPE))
        exit(1);
    return indicator_position;
}

void testPrint(Rod *x, Rod *y, Rod *z) {
    //Function printing all the crucial parametres of the particles
    //Use it for testing/debugging
    printf("===============\n");
    for (int i = 0; i < DISCS; i++) {
        printf("%d, %d, %d, %d\n", disc_x1cords[i], disc_y1cords[i], disc_x2cords[i], disc_y2cords[i]);
    }
    printf("\n");
    printf("Rod 1, position: %d\n", x->position);
    for (int j = 0; j < DISCS; j++) {
        printf("%d\t", x->stack[j]);
    }

    printf("\n");
    printf("Rod 2, position: %d\n", y->position);
    for (int j = 0; j < DISCS; j++) {
        printf("%d\t", y->stack[j]);
    }

    printf("\n");
    printf("Rod 3, position: %d\n", z->position);
    for (int j = 0; j < DISCS; j++) {
        printf("%d\t", z->stack[j]);
    }
    printf("\n");
}
