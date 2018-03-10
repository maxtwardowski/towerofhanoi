#include "primlib.h"
#include <stdlib.h>
#include <math.h>

#define FPS_RATE 60
#define DISCS 3

typedef struct Rod {
    int position;
    int stack[DISCS];
} Rod;

void cleanScreen();
void discMove(Rod *a, Rod *b);

int main(int argc, char* argv[]) {

    if(initGraph()) {
        exit(3);
    }

    //Declaring the rods
    struct Rod Rod1;
    struct Rod Rod2;
    struct Rod Rod3;

    for(int i = 0; i < DISCS; i++) {
        Rod1.stack[i] = i + 1;
        Rod2.stack[i] = 0;
        Rod3.stack[i] = 0;
    }

    Rod1.position = DISCS - 1;
    Rod2.position = -1;
    Rod3.position = -1;


    //Test printouts
    /*printf("\n");
    printf("Rod 1, position: %d\n", Rod1.position);
    for (int j = 0; j < DISCS; j++) {
        printf("%d\t", Rod1.stack[j]);
    }

    printf("\n");
    printf("Rod 2, position: %d\n", Rod2.position);
    for (int j = 0; j < DISCS; j++) {
        printf("%d\t", Rod2.stack[j]);
    }

    printf("\n");
    printf("Rod 3, position: %d\n", Rod3.position);
    for (int j = 0; j < DISCS; j++) {
        printf("%d\t", Rod3.stack[j]);
    }
    printf("\n");*/


    SDL_Event event;

    while(1) {

        printf("\n");
        printf("Rod 1, position: %d\n", Rod1.position);
        for (int j = 0; j < DISCS; j++) {
            printf("%d\t", Rod1.stack[j]);
        }

        printf("\n");
        printf("Rod 2, position: %d\n", Rod2.position);
        for (int j = 0; j < DISCS; j++) {
            printf("%d\t", Rod2.stack[j]);
        }

        printf("\n");
        printf("Rod 3, position: %d\n", Rod3.position);
        for (int j = 0; j < DISCS; j++) {
            printf("%d\t", Rod3.stack[j]);
        }
        printf("\n");

        discMove(&Rod1, &Rod3);
        discMove(&Rod1, &Rod2);
        discMove(&Rod3, &Rod2);
        discMove(&Rod1, &Rod3);
        discMove(&Rod2, &Rod1);
        discMove(&Rod2, &Rod3);
        discMove(&Rod1, &Rod3);

        printf("\n=======");
        printf("Rod 1, position: %d\n", Rod1.position);
        for (int j = 0; j < DISCS; j++) {
            printf("%d\t", Rod1.stack[j]);
        }

        printf("\n");
        printf("Rod 2, position: %d\n", Rod2.position);
        for (int j = 0; j < DISCS; j++) {
            printf("%d\t", Rod2.stack[j]);
        }

        printf("\n");
        printf("Rod 3, position: %d\n", Rod3.position);
        for (int j = 0; j < DISCS; j++) {
            printf("%d\t", Rod3.stack[j]);
        }
        printf("\n");

        cleanScreen();
        updateScreen(); //Refreshing the screen
        SDL_Delay(1000 / FPS_RATE); //Setting FPS cap
        SDL_PollEvent(&event); //Event detection

        /*switch(event.type){
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_1:
                        discMove(&Rod1, &Rod2);
                        break;
                    case SDLK_2:
                        discMove(&Rod1, &Rod3);
                        break;
                    case SDLK_3:
                        discMove(&Rod2, &Rod1);
                        break;
                    case SDLK_4:
                        discMove(&Rod2, &Rod3);
                        break;
                    case SDLK_5:
                        discMove(&Rod3, &Rod1);
                        break;
                    case SDLK_6:
                        discMove(&Rod3, &Rod2);
                        break;
                    case SDLK_ESCAPE:
                       exit(1);
                    default:
                       break;
                }
        }*/

        /*if (isKeyDown(SDLK_1))
            {discMove(&Rod1, &Rod2);}
        else if (isKeyDown(SDLK_2))
            {discMove(&Rod1, &Rod3);}
        else if (isKeyDown(SDLK_3))
            {discMove(&Rod2, &Rod1);}
        else if (isKeyDown(SDLK_4))
            {discMove(&Rod2, &Rod3);}
        else if (isKeyDown(SDLK_5))
            {discMove(&Rod3, &Rod1);}
        else if (isKeyDown(SDLK_6))
            {discMove(&Rod3, &Rod2);}
        else if (isKeyDown(SDLK_ESCAPE))
            {exit(1);}*/

        if (Rod3.position == DISCS - 1) {
            printf("\nWELL DONE YOU WON!\n");
            exit(1);
        }
        //End of the main loop
    }

    return 0;
}

void cleanScreen() {
	filledRect(0, 0, screenWidth(), screenHeight(), BLACK);
}

void discMove(Rod *a, Rod *b) {
    if (a->position != -1) {
        int disctopush = a->stack[a->position];
        a->stack[a->position] = 0;
        a->position = a->position - 1;

        if (b->stack[b->position] < disctopush) {
            b->position = b->position + 1;
            b->stack[b->position] = disctopush;
        }
    }
}
