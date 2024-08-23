#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "cslib/std/hashmap.h"
#include "cslib/std/string.h"


int main()
{
    printf("Hello, and welcome to Cookie Maker 2024! (copyright Oded Zrubavel)\n");
    printf("First off, we need to gather ingredients!\n");

    cslib_hashmap_t *map = (cslib_hashmap_t*)malloc(sizeof(cslib_hashmap_t));
    cslib_allocate_hashmap(map, 64);

    char input[64];
    char cmd[64];

    while (true)
    {
        printf(">> ");
        fgets(input, 64, stdin);
        memcpy(cmd, input, strlen(input) - 1); /* Removes the newline */
        memset(cmd + strlen(input) - 1, '\0', 1);

        if ( memcmp(cmd, "quit", 4) == 0 )
        {
            printf("Quitting cookie maker...\n");
            break;
        }

        else if ( memcmp(cmd, "set-ingredient", 14) == 0 )
        {
            printf(">> Ingredient Key: ");
            fgets(input, 64, stdin);
            memcpy(cmd, input, strlen(input) - 1); /* Removes the newline */
            memset(cmd + strlen(input) - 1, '\0', 1);

            char *key = cslib_allocate_string(cmd, 128);

            printf(">> Ingredient Value: ");
            fgets(input, 64, stdin);
            memcpy(cmd, input, strlen(input) - 1); /* Removes the newline */
            memset(cmd + strlen(input) - 1, '\0', 1);
            
            char *value = cslib_allocate_string(cmd, 128);
            cslib_set_hashmap(map, key, value);
        }

        else if ( memcmp(cmd, "get-ingredient", 15) == 0 )
        {
            printf(">> Ingredient Key: ");
            fgets(input, 64, stdin);
            memcpy(cmd, input, strlen(input) - 1); /* Removes the newline */
            memset(cmd + strlen(input) - 1, '\0', 1);

            void *item = cslib_get_hashmap(map, cmd);

            if (item != NULL)
            {
                printf("%s: %s\n", cmd, (char*)item);
            }
            else
            {
                printf("No such ingredient: %s\n", cmd);
            }
        }

        else
        {
            printf("Received: `%s`, not a recognizable option - press help for help\n", 
                cmd);
        }

    }

    cslib_testfree_hashmap(map, true);
    cslib_dumbfree_hashmap(map);

    return 0;
}
