#include <stdio.h>
//#include "player.h"
//#include "supemon.h"
//#include "move.h"
//#include "battle.h"
//#include "shop.h"
//#include "save.h"
//#include "utils.h"
//#include "item.h"

int main(void)
{
    char player_pseudo[50];
    int supemon_choice;

    printf("Enter your name: ");
    scanf("%s", player_pseudo);

    printf("Hello %s!\nWelcome in Supémon World!\n", player_pseudo);

    printf("+-----------------------------+\n");
    printf("| Choose your starter Supémon:|\n");
    printf("| 1 - Supmander               |\n");
    printf("| 2 - Supasaur                |\n");
    printf("| 3 - Supirtle                |\n");
    printf("+-----------------------------+\n");
    printf("1, 2 or 3: ");
    scanf("%d", &supemon_choice);

    //switch (supemon_choice)
    //{
    //case 1:
    //    printf("You have chosen Supmander!\n");
    //    break;
    //case 2:
    //    printf("You have chosen Supasaur!\n");
    //    break;
    //case 3:
    //    printf("You have chosen Supirtle!\n");
    //    break;
    //default:
    //    printf("Invalid choice!\n");
    //    break;
    //}

    return 0;
}
