#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "retezec.h"
#include "retezce.h"
#include "zasobnik.h"
#include "typy.h"
#include "parser.h"

#define OUT_OF_MEMORY "Nedostatek pameti!"

int main(int argc, char *argv[]) {
    int pocet_arg=0;
    FILE * vstup;
    FILE * vystup;
    char * vystupNazev;
    char multi = 0;
    char hlavicka = 0;
    int vystupLen;
    retezec ret;
    retezce seznam;

    for ( pocet_arg = 1 ; pocet_arg < (argc -1) ; pocet_arg++ ) {
        vystupLen = strlen(argv[pocet_arg]);
        if (vystupLen == 2) {
            if (strcmp(argv[pocet_arg],"-o") == 0) {
                multi = pocet_arg;
            }
            vystupNazev = malloc((strlen(argv[pocet_arg]) + 5) * sizeof(char));
            if (vystupNazev==NULL) {
                fprintf(stderr,OUT_OF_MEMORY);
                exit (1);
            }
            vystupNazev[0] = 0;
            strcat (vystupNazev,argv[pocet_arg+1]);
        }
    }
    if (argc <= 1) {
        fprintf(stderr,"dokuwiki2latex - convert dokuwiki to LaTeX\nAuthors: Jiri Nemec <nemec.jiri@gmail.com>\n         Pavel Kuttelwascher <Pavel.ku@tiscali.cz>\n\n");
        fprintf(stderr," dokuwiki2latex document1.txt document2.txt -o output.tex\n   vytvori vystup.tex ve kterem bude preformatovany obsah obou vstupnich souboru \n");
    }
    if (multi == 0) {
        for ( pocet_arg = 1 ; pocet_arg < argc ; pocet_arg++ ) { // pro vice souboru na vstupu
            vstup = fopen(argv[pocet_arg],"r");
            if (vstup != NULL) {
                vystupLen = strlen(argv[pocet_arg]);
                if (vystupLen > 4 && argv[pocet_arg][vystupLen - 4] == '.' && argv[pocet_arg][vystupLen - 3] == 't' && argv[pocet_arg][vystupLen - 2] == 'x' && argv[pocet_arg][vystupLen - 1] == 't') argv[pocet_arg][vystupLen - 4] = 0;
                vystupNazev = malloc((strlen(argv[pocet_arg]) + 5) * sizeof(char));
                if (vystupNazev==NULL) {
                    fprintf(stderr,OUT_OF_MEMORY);
                    exit (1);
                }
                vystupNazev[0] = 0;
                strcat (vystupNazev,argv[pocet_arg]);
                strcat (vystupNazev,".tex");
                //printf("%s\n",vystupNazev);
                vystup = fopen(vystupNazev,"w");
                free(vystupNazev);
                if (vystup != NULL) {
                    seznam = newRetezce();
                    fprintf(vystup,"\\documentclass{article}\n\\usepackage[czech]{babel} \n\\usepackage{ucs}\n\\usepackage[utf8x]{inputenc}\n");
                    lexikalniAnalyza(&seznam,vstup);
                    zvyrazneni(&seznam);
                    vycty(&seznam);
                    nadpisy(&seznam);
                    tabulky(&seznam);
                    if (obrazky(&seznam) > 0) fprintf(vystup,"\\usepackage{graphicx}\n");
                    fprintf(vystup,"\\begin{document}\n");
                    ret = newRetezec();
                    retezceToRetezec(&ret,&seznam);
                    deleteRetezce(&seznam);
                    elementy(&ret);
                    retezecPrint(&ret,vystup);
                    deleteRetezec(&ret);
                    fprintf(vystup,"\n\\end{document}\n");
                    fclose(vystup);
                }
                fclose(vstup);
            }
        }
    } else {
        vystup = fopen(vystupNazev,"w");
        free(vystupNazev);
        fprintf(vystup,"\\documentclass{article}\n\\usepackage[czech]{babel} \n\\usepackage{ucs}\n\\usepackage[utf8x]{inputenc}\n\\usepackage{graphicx}\n\\begin{document}\n");
        if (vystup != NULL) {
            for ( pocet_arg = 1 ; pocet_arg < argc ; pocet_arg++ ) { // pro vice souboru na vstupu
                vstup = fopen(argv[pocet_arg],"r");
                if (vstup != NULL && pocet_arg != multi && pocet_arg != (multi + 1)) {
                    //printf("ctu: %s\n",argv[pocet_arg]);
                    seznam = newRetezce();
                    lexikalniAnalyza(&seznam,vstup);
                    zvyrazneni(&seznam);
                    vycty(&seznam);
                    nadpisy(&seznam);
                    tabulky(&seznam);
                    obrazky(&seznam);
                    ret = newRetezec();
                    retezceToRetezec(&ret,&seznam);
                    deleteRetezce(&seznam);
                    elementy(&ret);
                    retezecPrint(&ret,vystup);
                    deleteRetezec(&ret);
                    fclose(vstup);
                }

            }
        }
        fprintf(vystup,"\n\\end{document}\n");
        fclose(vystup);
    }

    //getch();
    return EXIT_SUCCESS;
}
