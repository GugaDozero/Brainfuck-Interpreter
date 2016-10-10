#include "utils.h"

void itoa( signed num, char *buff )
{
    int i = 0;
    int j = 0;

    do
    {
        unsigned rem = num % 10;
        buff[i++] = "0123456789"[rem];

    }
    while(num /= 10);

    // Xor Swap Algorithm is faster ;D

    while(i != j)
    {
        if( (i - j) == 1) break;

        --i;
        buff[i] ^= buff[j];
        buff[j] ^= buff[i];
        buff[i] ^= buff[j];
        ++j;
    }
}
