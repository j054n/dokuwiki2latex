#ifndef RETEZCE_H
#define RETEZCE_H

#include "retezec.h"

typedef struct {
    retezec * data;
    unsigned int pocet;
    unsigned int alokovano;
}
retezce;

retezce newRetezce();
int deleteRetezce(retezce * co);

int retezceAdd(retezce * kam,retezec * co);
int retezceGetPocet(retezce * co);
int retezcePrint(retezce * co, FILE * out);
int retezcePrintTag(retezce * co, FILE * out);

retezec * retezceGet(retezce * rtz, int poradi);


int retezceGetTyp(retezce * rtz,int poradi);  // vrati typ retezce
int retezceSetTyp(retezce * rtz,int poradi,int typ); // nastavi typ retezce
int retezceSetString(retezce * rtz,int poradi,char * str); // smaze puvodni obsah a nahradi ho predanym retezcem
int retezceAddString(retezce * rtz,int poradi,char * str);  // pripoji predany retezec na konec
int retezceAddNumber(retezce * rtzce,int poradi,int cislo); // na konec pripoji cislo
int retezceCleanRetezec(retezce * rtz,int poradi); // smaze obsah retezce, typ zachova
char retezceGetChar(retezce * rtz,int poradi,int poradiCharu); // vrati znak
int retezceGetDelka(retezce * rtz,int poradi); // vrati delku retezce
int retezceReplaceString(retezce * rtzce,int poradi,char * co,char * cim);
int retezceToRetezec(retezec * cil,retezce * zdroj);
#endif

