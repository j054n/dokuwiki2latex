#include <stdio.h>
#include "zasobnik.h"

zasobnik newZasobnik() {
    zasobnik ret;
    ret.pocet = 0;
    ret.alokovano = 0;
    ret.data = malloc(ret.alokovano * sizeof(int));
    return ret;
}

int deleteZasobnik(zasobnik * zas) {
    if (zas->alokovano > 0)
        free(zas->data);
    return 1;
}

int zasobnikPush(zasobnik * zas,int cislo) {
    zas->pocet++;
    while (zas->alokovano <= zas->pocet ) {
        if (zas->alokovano < 2)
            zas->alokovano = 1;
        zas->alokovano *=2;
        zas->data = realloc(zas->data,zas->alokovano * sizeof(int));
    }
    zas->data[zas->pocet - 1] = cislo;
    return 1;
}


int zasobnikPull(zasobnik * zas) {
    if (zas->pocet > 0) {
        zas->pocet--;
        return zas->data[zas->pocet];
    } else return 0;
}

int zasobnikReadLast(zasobnik * zas) {
    if (zas->pocet > 0) {
        return zas->data[zas->pocet - 1];
    } else return 0;
}


int zasobnikContains(zasobnik * zas,int cislo) {
    int i;
    char obsahuje = 0;
    for (i= zas->pocet - 1;i >= 0 && obsahuje == 0;i--) {
        if (cislo == zas->data[i]) obsahuje = 1;
    }
    return obsahuje;
}

int zasobnikCount(zasobnik * zas) {
    return zas->pocet;
}

int zasobnikIsEmpty(zasobnik * zas) {
    if (zas->pocet == 0) return 1;
    else return 0;
}
