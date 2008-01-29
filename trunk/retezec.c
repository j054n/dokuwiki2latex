#include <stdio.h>
#include "retezec.h"

#define OUT_OF_MEMORY "Nedostatek pameti!"

retezec newRetezec() {
    retezec ret;
    ret.delka = 0;
    ret.alokovano = 0;
    ret.typ = 0;
    ret.data = malloc(ret.alokovano * sizeof(char));
    if (ret.data==NULL) {
        fprintf(stderr,OUT_OF_MEMORY);
        exit (1);
    }
    return ret;
}

int deleteRetezec(retezec * ptr) {
    if (ptr->alokovano > 0)
        free(ptr->data);
    return 1;
}

int retezecPutChar(retezec * cil,char zdroj) {
    cil->delka++;
    while (cil->alokovano <= cil->delka ) {
        if (cil->alokovano < 2)
            cil->alokovano = 1;
        cil->alokovano *=2;
        cil->data = realloc(cil->data,cil->alokovano * sizeof(char));
        if (cil->data==NULL) {
            fprintf(stderr,OUT_OF_MEMORY);
            exit (1);
        }
    }
    cil->data[cil->delka - 1] = zdroj;
    return 1;
}

int retezecPutString(retezec * cil,char * zdroj) {
    unsigned int i = 0;
    while (zdroj[i] != 0) {
        retezecPutChar(cil,zdroj[i]);
        i++;
    }
    return 1;
}

int retezecClear(retezec * co,int dealokovat) {
    co->delka = 0;
    if (dealokovat) {
        co->alokovano = 1;
        co->data = realloc(co->data,co->alokovano * sizeof(char));
        if (co->data==NULL) {
            fprintf(stderr,OUT_OF_MEMORY);
            exit (1);
        }

    }
    co->typ = 0;
    return 1;
}

int retezecClean(retezec * co,int dealokovat) {
    co->delka = 0;
    if (dealokovat) {
        co->alokovano = 1;
        co->data = realloc(co->data,co->alokovano * sizeof(char));
        if (co->data==NULL) {
            fprintf(stderr,OUT_OF_MEMORY);
            exit (1);
        }
    }
    return 1;
}

int retezecEndWith(retezec * scim,char * co) {
    unsigned int coLen = 0;
    char rozdilne = 0;
    unsigned int i;
    while (co[coLen] != 0)
        coLen++;
    if (coLen > scim->delka)
        return 0;
    for (i = 0;i < coLen;i++) {
        if (scim->data[scim->delka - 1 - i] != co[coLen - 1 - i])
            rozdilne = 1;
    }
    return rozdilne ? 0 : 1;
}

int retezecIs(retezec * scim,char * co) {
    unsigned int coLen = 0;
    char rozdilne = 0;
    unsigned int i;
    while (co[coLen] != 0)
        coLen++;
    if (coLen != scim->delka) return 0;
    for (i = 0;i < coLen;i++) {
        if (scim->data[i] != co[i])
            rozdilne = 1;
    }
    return rozdilne ? 0 : 1;
}

int retezecSetTyp(retezec * co, char typ) {
    co->typ = typ;
    return 1;
}

int retezecGetTyp(retezec * co) {
    return co->typ;
}

int retezecCopy(retezec * kam, retezec * co) {
    kam->alokovano = co->delka;
    kam->data = realloc(kam->data,kam->alokovano * sizeof(char));
    if (co->data==NULL) {
        fprintf(stderr,OUT_OF_MEMORY);
        exit (1);
    }
    kam->delka = kam->alokovano;
    kam->typ = co->typ;
    //printf("%d %d\n",kam->data,co->data);
    memcpy(kam->data,co->data,kam->alokovano);
    return 1;
}

int retezecPrint(retezec * zdroj, FILE * out) {
    unsigned int i;
    for (i = 0;i < zdroj->delka;i++)
        fprintf(out,"%c",zdroj->data[i]);
    return 1;
}
int retezecGetDelka(retezec * ceho) {
    return ceho->delka;
}

int retezecSplit(retezec * zdroj,retezec * prebytek ,int pocet) {
    int i;
    if (pocet < 0) {
        if (-pocet > retezecGetDelka(zdroj)) pocet = -retezecGetDelka(zdroj);
        pocet = -pocet;
        retezecClear(prebytek,0);
        for (i = 0;i < pocet;i++) {
            retezecPutChar(prebytek,retezecGetChar(zdroj,zdroj->delka - pocet + i));
        }
        zdroj->delka -= pocet;
    } else if (pocet >= 0) {
        if (pocet > retezecGetDelka(zdroj)) pocet = retezecGetDelka(zdroj);
        retezecClear(prebytek,0);
        for (i = 0;i < (retezecGetDelka(zdroj) - pocet);i++) {
            retezecPutChar(prebytek,retezecGetChar(zdroj,pocet + i));
        }
        zdroj->delka = pocet;
    }

    return 1;
}

char retezecGetChar(retezec * zdroj,int poradi) {
    if (poradi < zdroj->delka)
        return zdroj->data[poradi];
    else
        return 0;
}

int retezecReplaceString(retezec * rtzec,char * co,char * cim) {
    int i,j;
    int delkaCo;
    int delkaCim;
    int delta;
    char odpovida;
    for (i = 0;i < retezecGetDelka(rtzec);i++) {
        j = 0;
        odpovida = 1;
        while (co[j] != 0 && odpovida == 1) {
            if (retezecGetChar(rtzec,i+j) != co[j]) odpovida = 0;
            j++;
        }
        if (odpovida == 1) {
            delkaCo = j;
            delkaCim = 0;
            while (cim[delkaCim] != 0) delkaCim++;
            delta = delkaCim - delkaCo;
            if (delta > 0) {
                for (j = 0;j<delta;j++) retezecPutChar(rtzec,'dd');
                for (j = retezecGetDelka(rtzec);j > (i+delkaCim);j--) {
                    rtzec->data[j-1] = rtzec->data[j-delta-1];
                }
                for (j = 0;j<delkaCim;j++) rtzec->data[i+j] = cim[j];
            }
        }
    }
    return 0;
}

int retezecMerge(retezec * cil,retezec * zdroj) {
    int i;
    for (i = 0;i < retezecGetDelka(zdroj);i++) retezecPutChar(cil,retezecGetChar(zdroj,i));
    return 0;
}

