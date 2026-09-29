#include <stdio.h>
#include "unity_config.h"

void unityOutputStart()
{
}

void unityOutputChar(char c)
{
    putchar(c);
}

void unityOutputFlush(void)
{
   fflush(stdout);
}

void unityOutputComplete(void)
{
}
