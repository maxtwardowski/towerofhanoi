#include "primlib.h"
#include <stdlib.h>
#include <math.h>

#define FPS_RATE 60
#define DISCS 3

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
void drawRods(int x1, int y1, int x2, int y2, int displacement);
void discMove(Rod *a, Rod *b);
void testPrint(Rod *x, Rod *y, Rod *z);

int main(int argc, char* argv[]) {

    if(initGraph()) {
        exit(3);
    }

    int x1, y1, x2, y2, rod_width = 17, rod_height = 170, displacement = 200, blockwidth = 70;

    x1 = screenWidth() / 2 - rod_width;
    y1 = rod_height;
    x2 = x1 + 2 * rod_width;
    y2 = screenHeight();

    //Declaring the Rods
    struct Rod Rod1;
    struct Rod Rod2;
    struct Rod Rod3;

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
        printf("%d, %d, %d, %d\n", disc_x1cords[i], disc_y1cords[i], disc_x2cords[i], disc_y2cords[i]);
    }

    while(1) {

        testPrint(&Rod1, &Rod2, &Rod3);
        cleanScreen();
        drawRods(x1, y1, x2, y2, displacement);

        for (int i = 0; i < DISCS; i++) {
            filledRect(
                disc_x1cords[i],
                disc_y1cords[i],
                disc_x2cords[i],
                disc_y2cords[i],
                RED
            );
        }

        updateScreen(); //Refreshing the screen
        SDL_Delay(1000 / FPS_RATE); //Setting FPS cap

        if (Rod3.position == DISCS - 1) {
            textout(screenWidth() / 2, screenHeight() / 2, "Y", GREEN);
            updateScreen();
            SDL_Delay(1000);
            exit(1);
        }

        if (isKeyDown(SDLK_1))
            discMove(&Rod1, &Rod2);
        else if (isKeyDown(SDLK_2))
            discMove(&Rod1, &Rod3);
        else if (isKeyDown(SDLK_3))
            discMove(&Rod2, &Rod1);
        else if (isKeyDown(SDLK_4))
            discMove(&Rod2, &Rod3);
        else if (isKeyDown(SDLK_5))
            discMove(&Rod3, &Rod1);
        else if (isKeyDown(SDLK_6))
            discMove(&Rod3, &Rod2);
        else if (isKeyDown(SDLK_ESCAPE))
            exit(1);


        //End of the main loop
    }
    return 0;
}

void cleanScreen() {
	filledRect(0, 0, screenWidth(), screenHeight(), BLACK);
}

void drawRods(int x1, int y1, int x2, int y2, int displacement) {
    filledRect(x1, y1, x2, y2, WHITE);
    filledRect(x1 - displacement, y1, x2 - displacement, y2, WHITE);
    filledRect(x1 + displacement, y1, x2 + displacement, y2, WHITE);
}

void discMove(Rod *a, Rod *b) {
    //This function moves the disc by popping it and pushing into another stack (Rod)
    if (a->position != -1) {
        int disctopush = a->stack[a->position];
        if (b->stack[b->position] > disctopush || b->position == -1) {

            disc_x1cords[DISCS - disctopush] = b->base_x1 + 10 * (DISCS - a->stack[a->position]);
            disc_y1cords[DISCS - disctopush] = b->base_y1 - 25 * (b->position + 1);
            disc_x2cords[DISCS - disctopush] = b->base_x2 - 10 * (DISCS - a->stack[a->position]);
            disc_y2cords[DISCS - disctopush] = b->base_y2 - 25 * (b->position + 1);

            a->stack[a->position] = 0;
            a->position = a->position - 1;
            b->position = b->position + 1;
            b->stack[b->position] = disctopush;
        }
    }
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
