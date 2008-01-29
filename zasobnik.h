#ifndef ZASOBNIK_H
#define ZASOBNIK_H

typedef struct {
    int * data;
    unsigned int pocet;
    unsigned int alokovano;
} zasobnik;

zasobnik newZasobnik();
int deleteZasobnik(zasobnik * zas);

int zasobnikPush(zasobnik * zas,int cislo); // ulozi
int zasobnikPull(zasobnik * zas); // vybere
int zasobnikReadLast(zasobnik * zas); // precte posledni polozku v zasobniku, ale nesmaze ji
int zasobnikContains(zasobnik * zas,int cislo);  // vrati 1 pokud zasobnik obsahuje cislo
int zasobnikCount(zasobnik * zas); // vrati pocet prvku ulozenych v zasobniku
int zasobnikIsEmpty(zasobnik * zas); // vrati 1 pokud je zasobnik prazdny
#endif
