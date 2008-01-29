#ifndef PARSER_H
#define PARSER_H

#include "retezec.h"
#include "retezce.h"
#include "zasobnik.h"
#include "typy.h"
void lexikalniAnalyza(retezce * kam,FILE * odkud);
void zvyrazneni(retezce * rtzce);
void vycty(retezce * rtzce);
void nadpisy(retezce * rtzce);
void tabulky(retezce * rtzce);
void elementy(retezec * rtz);
int obrazky(retezce * rtzce);

#endif
