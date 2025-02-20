#include "zrub/cipher/classic.h"


void zrub_caesar_encipher(char *message, short steps)
{
    /* capital: (A)65-(Z)90 */
    /* lowercase: (a)97-(z)122 */
    size_t ms = strlen(message);
    short c;

    if (steps == 0) return;
    else if (steps >= 26)
    {
        steps %= 26;   
    }
    else if (steps < 0)
    {
        steps %= 26;
        steps += 26;
    }

    for (size_t i = 0; i < ms; i++)
    {
        c = message[i];

        if (65 <= c && c <= 90) {
            c += steps;

            if (c > 90) {
                c -= 26;
            }

            message[i] = c;
        }

        else if (97 <= c && c <= 122) {
            c += steps;

            if (c > 122)
            {
                c -= 26;
            }

            message[i] = c;
        }
    }    
}

void zrub_caesar_decipher(char *message, short steps)
{
    zrub_caesar_encipher(message, 26 - steps);
}
