#include "stany.h"

Stany::Stany()
{
stan = 0;
}

int Stany::pobierzStan()
{
   return stan;
}

void Stany::zmienStan()
{
    if(stan<2)
        stan++;
    else
        stan=0;
}

void Stany::zeruj()
{
    stan=0;
}

void Stany::zamroz()
{
    stan=4;
}
