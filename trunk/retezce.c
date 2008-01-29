#include <stdio.h>
#include "retezce.h"

#define OUT_OF_MEMORY "Nedostatek pameti!"

retezce newRetezce() {
    retezce ret;
    ret.pocet = 0;
    ret.alokovano = 0;
    ret.data = malloc(ret.alokovano * sizeof(retezec));
    if (ret.data==NULL) {
        fprintf(stderr,OUT_OF_MEMORY);
        exit (1);
    }
    return ret;
}

int deleteRetezce(retezce * co) {
    int i;
    for (i = 0;i < co->pocet;i++) {
        deleteRetezec(&co->data[i]);
    }
    free(co->data);
    return 1;
}

int retezceGetPocet(retezce * co) {
    return co->pocet;
}

int retezcePrintTag(retezce * co, FILE * out) {
    int i;
    for (i = 0;i < co->pocet;i++) {
        fprintf(out,"\n%d %d. ",retezecGetTyp(&co->data[i]),i);
        retezecPrint(&co->data[i],out);
    }
    return 1;
}

int retezcePrint(retezce * co, FILE * out) {
    int i;
    for (i = 0;i < co->pocet;i++) {
        retezecPrint(&co->data[i],out);
    }
    return 1;
}

int retezceAdd(retezce * kam,retezec * co) {
    kam->pocet++;
    while (kam->alokovano <= kam->pocet ) {
        if (kam->alokovano < 2)
            kam->alokovano = 1;
        kam->alokovano *=2;
        kam->data = realloc(kam->data,kam->alokovano * sizeof(retezec));
        if (kam->data==NULL) {
            fprintf(stderr,OUT_OF_MEMORY);
            exit (1);
        }
    }
    kam->data[kam->pocet - 1] = newRetezec();
    retezecCopy(&kam->data[kam->pocet - 1],co);
    return 1;
}

retezec * retezceGet(retezce * rtz, int poradi) {
    if (poradi >= 0) return &rtz->data[poradi];
    else if ((poradi < 0)&& ((-poradi) <= rtz->pocet)) return &rtz->data[poradi + rtz->pocet];
}

int retezceGetTyp(retezce * rtz,int poradi) {
    if (poradi >= 0 && poradi < rtz->pocet) return retezecGetTyp(&rtz->data[poradi]);
    if ((poradi < 0) && ((-poradi) <= rtz->pocet)) {
        return retezecGetTyp(&rtz->data[rtz->pocet + poradi]);

    } else return -1;
}

int retezceSetTyp(retezce * rtz,int poradi,int typ) {
    if (poradi >= 0 && poradi < rtz->pocet) {
        retezecSetTyp(&rtz->data[poradi],typ);
        return 1;
    }
    if ((poradi < 0) && ((-poradi) <= rtz->pocet)) {
        retezecSetTyp(&rtz->data[rtz->pocet + poradi],typ);
        return 1;
    } else return 0;
}

int retezceAddString(retezce * rtz,int poradi,char * str) {
    if (poradi >= 0 && poradi < rtz->pocet) {
        retezecPutString(&rtz->data[poradi], str);
        return 1;
    } else if ((poradi < 0) && ((-poradi) <= rtz->pocet)) {
        retezecPutString(&rtz->data[rtz->pocet + poradi], str);
        return 1;
    }
    return 0;
}
int retezceCleanRetezec(retezce * rtz,int poradi) {
    if (poradi >= 0 && poradi < rtz->pocet) {
        retezecClean(&rtz->data[poradi],0);
        return 1;
    } else if ((poradi < 0) && ((-poradi) <= rtz->pocet)) {
        retezecClean(&rtz->data[rtz->pocet + poradi],0);
        return 1;
    }
    return 0;
}

int retezceSetString(retezce * rtz,int poradi,char * str) {
    if (poradi >= 0 && poradi < rtz->pocet) {
        retezecClean(&rtz->data[poradi],0);
        retezecPutString(&rtz->data[poradi], str);
        return 1;
    } else if ((poradi < 0) && ((-poradi) <= rtz->pocet)) {
        retezecClean(&rtz->data[rtz->pocet + poradi],0);
        retezecPutString(&rtz->data[rtz->pocet + poradi], str);
        return 1;
    }
    return 0;

}

char retezceGetChar(retezce * rtz,int poradi,int poradiCharu) {
    if (poradi >= 0 && poradi < rtz->pocet) return retezecGetChar(&rtz->data[poradi],poradiCharu);
    if ((poradi < 0) && ((-poradi) <= rtz->pocet)) return retezecGetChar(&rtz->data[rtz->pocet + poradi],poradiCharu);
    else return 0;
}

int retezceGetDelka(retezce * rtz,int poradi) {
    if (poradi >= 0 && poradi < rtz->pocet) return retezecGetDelka(&rtz->data[poradi]);
    if ((poradi < 0) && ((-poradi) <= rtz->pocet)) return retezecGetDelka(&rtz->data[rtz->pocet + poradi]);
    else return -1;
}

int retezceAddNumber(retezce * rtzce,int poradi,int cislo) {
    char tmp[] = " ";
    if ((cislo / 10) > 0) retezceAddNumber(rtzce,poradi,cislo / 10);
    tmp[0] = (cislo % 10) + '0';
    retezceAddString(rtzce,poradi,tmp);
    return 0;
}

int retezceReplaceString(retezce * rtzce,int poradi,char * co,char * cim) {
    if (poradi >= 0 && poradi < rtzce->pocet) return retezecReplaceString(&rtzce->data[poradi],co,cim);
    if ((poradi < 0) && ((-poradi) <= rtzce->pocet)) return retezecReplaceString(&rtzce->data[rtzce->pocet + poradi],co,cim);
    else return 0;
}

int retezceToRetezec(retezec * cil,retezce * zdroj) {
    int i;

    /*retezec r;
    retezec s;
    r = newRetezec();
    s = newRetezec();
    retezecPutString(&r,"abc");
    retezecPutString(&s,"def");
    retezecMerge(&r,&s);
    retezecMerge(&r,&s);
    retezecPrint(&zdroj->data[0],stdout);
*/
    for (i = 0;i < zdroj->pocet;i++) {
        //retezecPrint(&zdroj->data[i],stdout);
retezecMerge(cil,&zdroj->data[i]);
    }
    return 1;

    //retezecMerge(&cil,&zdroj->data[10]);
    /*for (i = 0;i < retezceGetPocet(zdroj);i++) {
        retezecMerge(cil,&zdroj->data[i]);
    }*/
    return 0;
}
