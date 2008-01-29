#ifndef RETEZEC_H
#define RETEZEC_H

typedef struct {
    char * data;
    char typ;
    unsigned int delka;
    unsigned int alokovano;
} retezec;

retezec newRetezec();
int deleteRetezec(retezec * ptr);

int retezecGetTyp(retezec * co);
int retezecSetTyp(retezec * co, char typ);
int retezecGetDelka(retezec * ceho);

char retezecGetChar(retezec * zdroj,int poradi);


int retezecPutChar(retezec * cil,char zdroj);
int retezecPutString(retezec * cil,char * zdroj);
int retezecClear(retezec * co,int dealokovat);
int retezecClean(retezec * co,int dealokovat);
int retezecPrint(retezec * zdroj, FILE * out);
int retezecEndWith(retezec * scim,char * co);
int retezecIs(retezec * scim,char * co);
int retezecCopy(retezec * kam, retezec * co);
int retezecSplit(retezec * zdroj,retezec * prebytek ,int pocet);
int retezecReplaceString(retezec * rtzec,char * co,char * cim);
int retezecMerge(retezec * cil,retezec * zdroj);
#endif
