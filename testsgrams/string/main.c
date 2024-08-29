#include "cslib/std/string.h"
#include <stdio.h>


int main()
{
    char *string = cslib_create_string("Hello There!", 128);
    char *prefix = cslib_create_string("Hell", 128);

    printf("Before: `%s` `%s`\n", string, prefix);  
    printf("Your boolean variable is: %s\n",
     cslib_check_prefix(string, prefix) ? "true" : "false");

    cslib_remove_prefix(string, prefix);

    printf("After: `%s` `%s`\n", string, prefix);
    printf("Your boolean variable is: %s\n",
     cslib_check_prefix(string, prefix) ? "true" : "false");


    return 0;
}
