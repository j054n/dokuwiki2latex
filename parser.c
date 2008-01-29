#include <stdio.h>
#include "parser.h"

void lexikalniAnalyza(retezce * kam,FILE * odkud) {
    int c = 0,predchozi;
    char konec =0;
    char tag = 0;
    retezec r;
    retezec zbytek;
    retezec * posledniPrvek;
    r = newRetezec();
    zbytek = newRetezec();
    while (konec == 0) {
        predchozi = c;
        c = getc( odkud );
        /*if (retezceGetPocet(kam) > 0) {
            posledniPrvek = retezceGet(kam, retezceGetPocet(kam) - 1);
            retezecPutChar(posledniPrvek,'A');
        }*/
        if (c == '\n') {
            if (retezecGetDelka(&r) > 0) retezceAdd(kam,&r);
            retezecClear(&r,0);
            retezecPutChar(&r,'\n');
            retezecSetTyp(&r,KONEC_RADKU);
            retezceAdd(kam,&r);
            retezecClear(&r,0);
        }
        if (c != EOF && c != '\n' && predchozi != '\r')
            retezecPutChar(&r,c);
        if (c == EOF) {
            konec = 1;
            retezceAdd(kam,&r);
        }

        if ((retezceGetTyp(kam,-1) == HEADLINE5) && (retezecIs(&r,"="))) {
            retezceAddString(kam,-1,"=");
            retezceSetTyp(kam,-1,HEADLINE4);
            retezecClear(&r,0);
        } else if ((retezceGetTyp(kam,-1) == HEADLINE4) && (retezecIs(&r,"="))) {
            retezceAddString(kam,-1,"=");
            retezceSetTyp(kam,-1,HEADLINE3);
            retezecClear(&r,0);
        } else if ((retezceGetTyp(kam,-1) == HEADLINE3) && (retezecIs(&r,"="))) {
            retezceAddString(kam,-1,"=");
            retezceSetTyp(kam,-1,HEADLINE2);
            retezecClear(&r,0);
        } else if ((retezceGetTyp(kam,-1) == HEADLINE2) && (retezecIs(&r,"="))) {
            retezceAddString(kam,-1,"=");
            retezceSetTyp(kam,-1,HEADLINE1);
            retezecClear(&r,0);
        } else if (retezecEndWith(&r,"^")) {
            retezecSplit(&r,&zbytek,-1);
            retezecSetTyp(&zbytek,TABLEBOLD);
            tag = 1;
        } else if (retezecEndWith(&r,"|")) {
            retezecSplit(&r,&zbytek,-1);
            retezecSetTyp(&zbytek,TABLE);
            tag = 1;
        } else if (retezecEndWith(&r,"*")) {
            retezecSplit(&r,&zbytek,-1);
            retezecSetTyp(&zbytek,STAR);
            tag = 1;
        } else if (retezecEndWith(&r,"-")) {
            retezecSplit(&r,&zbytek,-1);
            retezecSetTyp(&zbytek,MINUS);
            tag = 1;
        } else if (retezecEndWith(&r,"==")) {
            retezecSplit(&r,&zbytek,-2);
            retezecSetTyp(&zbytek,HEADLINE5);
            tag = 1;
        } else if (retezecEndWith(&r,"//")) {
            retezecSplit(&r,&zbytek,-2);
            retezecSetTyp(&zbytek,ITALIK);
            tag = 1;
        } else if (retezecEndWith(&r,"__")) {
            retezecSplit(&r,&zbytek,-2);
            retezecSetTyp(&zbytek,UNDERLINE);
            tag = 1;
        } else if (retezecEndWith(&r,"''")) {
            retezecSplit(&r,&zbytek,-2);
            retezecSetTyp(&zbytek,MONOSPACED);
            tag = 1;
        } else if (retezecEndWith(&r,"{{")) {
            retezecSplit(&r,&zbytek,-2);
            retezecSetTyp(&zbytek,IMAGEBEGIN);
            tag = 1;
        } else if (retezecEndWith(&r,"}}")) {
            retezecSplit(&r,&zbytek,-2);
            retezecSetTyp(&zbytek,IMAGEEND);
            tag = 1;
        } else if (retezecEndWith(&r,"<sub>")) {
            retezecSplit(&r,&zbytek,-5);
            retezecSetTyp(&zbytek,SUBSCRIPTBEGIN);
            tag = 1;
        } else if (retezecEndWith(&r,"</sub>")) {
            retezecSplit(&r,&zbytek,-6);
            retezecSetTyp(&zbytek,SUBSCRIPTEND);
            tag = 1;
        } else if (retezecEndWith(&r,"<sup>")) {
            retezecSplit(&r,&zbytek,-5);
            retezecSetTyp(&zbytek,SUPERSCRIPTBEGIN);
            tag = 1;
        } else if (retezecEndWith(&r,"</sup>")) {
            retezecSplit(&r,&zbytek,-6);
            retezecSetTyp(&zbytek,SUPERSCRIPTEND);
            tag = 1;
        } else if (retezecEndWith(&r,"<del>")) {
            retezecSplit(&r,&zbytek,-5);
            retezecSetTyp(&zbytek,STRIKEBEGIN);
            tag = 1;
        } else if (retezecEndWith(&r,"</del>")) {
            retezecSplit(&r,&zbytek,-6);
            retezecSetTyp(&zbytek,STRIKEEND);
            tag = 1;
        }
        if (tag == 1) {
            if (retezecGetDelka(&r) > 0) retezceAdd(kam,&r);
            retezecClear(&r,0);
            retezceAdd(kam,&zbytek);
            retezecClear(&r,0);
            tag = 0;
        }
    }
    deleteRetezec(&r);
    deleteRetezec(&zbytek);
}

void zvyrazneni(retezce * rtzce) {
    int i;
    int typ;
    int ukoncit;
    char hvezda = 0;
    retezec r;
    zasobnik zas;
    zas = newZasobnik();
    for (i = 0;i < retezceGetPocet(rtzce);i++) {
        typ = retezceGetTyp(rtzce,i);
        if ((typ > PAIRBEGIN) &&  (typ < PAIREND)) {
            hvezda = 0;
            // osetreni polotucneho pisma
            if ((typ == STAR) && (retezceGetPocet(rtzce) > (i+1)) && (retezceGetTyp(rtzce,i + 1) ==  STAR)) {
                retezceSetTyp(rtzce,i + 1,NONE);
                retezceSetString(rtzce,i + 1,"");
                retezceSetTyp(rtzce,i,BOLD);
                typ = BOLD;
            }
            // ukoncovaci tagy
            if (zasobnikContains(&zas,typ)) {
                retezceSetString(rtzce,i,"");
                do {
                    ukoncit = zasobnikPull(&zas);
                    if (ukoncit == SUPERSCRIPTEND || ukoncit == SUBSCRIPTEND) retezceAddString(rtzce,i,"}}$");
                    else retezceAddString(rtzce,i,"}");
                } while (ukoncit != typ);
                // zacinajici tagy
            } else {
                switch (typ) {
                case BOLD:
                    retezceSetString(rtzce,i,"\\textbf{");
                    zasobnikPush(&zas,typ);
                    break;
                case ITALIK:
                    retezceSetString(rtzce,i,"\\textit{");
                    zasobnikPush(&zas,typ);
                    break;
                case MONOSPACED:
                    retezceSetString(rtzce,i,"\\texttt{");
                    zasobnikPush(&zas,typ);
                    break;
                case UNDERLINE:
                    retezceSetString(rtzce,i,"\\underline{");
                    zasobnikPush(&zas,typ);
                    break;
                case SUPERSCRIPTBEGIN:
                    retezceSetString(rtzce,i,"$^{\\textrm{");
                    zasobnikPush(&zas,SUPERSCRIPTEND);
                    break;
                case SUBSCRIPTBEGIN:
                    retezceSetString(rtzce,i,"$_{\\textrm{");
                    zasobnikPush(&zas,SUBSCRIPTEND);
                    break;
                    /*                case OVERLINEBEGIN:
                                        retezceSetString(rtzce,i,"\\overline{");
                                        zasobnikPush(&zas,OVERLINEEND);
                                        break*/
                }
            }
        } else if (typ == KONEC_RADKU && (zasobnikIsEmpty(&zas) == 0)) {
            retezceSetTyp(rtzce,i,NONE);
            retezceSetString(rtzce,i," ");
        }
    }
    if (zasobnikIsEmpty(&zas) == 0) {
        r = newRetezec();
        retezecClear(&r,0);
        retezceAdd(rtzce,&r);
        do {
            ukoncit = zasobnikPull(&zas);
            if (ukoncit == SUPERSCRIPTEND || ukoncit == SUBSCRIPTEND) retezceAddString(rtzce,i,"}}$");
            else retezceAddString(rtzce,i,"}");
        } while (zasobnikIsEmpty(&zas) == 0);
        deleteRetezec(&r);
    }
    deleteZasobnik(&zas);
}

void vycty(retezce * rtzce) {
    int i,j,k,delka;
    char pouzeMezery;
    zasobnik zas,typ;
    retezec r;
    int uroven = 0;
    int urovenDelta = 0;
    zas = newZasobnik();
    typ = newZasobnik();
    for (i = 0;i < retezceGetPocet(rtzce);i++) {
        urovenDelta = 0;
        if (retezceGetTyp(rtzce,i) == KONEC_RADKU) {
            pouzeMezery = 0;
            if (retezceGetDelka(rtzce,i + 1) >= 2) pouzeMezery = 1;
            for (j = 0;(pouzeMezery == 1) && (j < retezceGetDelka(rtzce,i + 1));j++) if (retezceGetChar(rtzce,i + 1,j) != ' ') pouzeMezery = 0;
            if ((pouzeMezery == 1) && ((retezceGetTyp(rtzce,i+2) == STAR ) || (retezceGetTyp(rtzce,i+2) == MINUS))) {
                delka = retezceGetDelka(rtzce,i + 1)/2;
                urovenDelta = uroven;
                if (zasobnikIsEmpty(&zas)) {
                    uroven = 1;
                    zasobnikPush(&zas, delka);
                    zasobnikPush(&typ, retezceGetTyp(rtzce,i+2));
                    retezceSetString(rtzce,i+1,"");
                    for (k = 1;k < uroven;k++) retezceAddString(rtzce,i+1," ");
                    if (retezceGetTyp(rtzce,i+2) == MINUS ) retezceAddString(rtzce,i+1,"\\begin{enumerate}\n");
                    if (retezceGetTyp(rtzce,i+2) == STAR ) retezceAddString(rtzce,i+1,"\\begin{itemize}\n");
                    retezceSetString(rtzce,i+2,"");
                    for (k = 1;k < uroven;k++) retezceAddString(rtzce,i+2," ");
                    retezceAddString(rtzce,i+2,"\\item");
                } else if ((zasobnikIsEmpty(&zas) == 0 ) && (zasobnikReadLast(&zas) < delka)) {
                    uroven++;
                    zasobnikPush(&zas,delka);
                    zasobnikPush(&typ, retezceGetTyp(rtzce,i+2));
                    retezceSetString(rtzce,i+1,"");
                    for (k = 1;k < uroven;k++) retezceAddString(rtzce,i+1," ");
                    if (retezceGetTyp(rtzce,i+2) == MINUS ) retezceAddString(rtzce,i+1,"\\begin{enumerate}\n");
                    else if (retezceGetTyp(rtzce,i+2) == STAR ) retezceAddString(rtzce,i+1,"\\begin{itemize}\n");
                    retezceSetString(rtzce,i+2,"");
                    for (k = 1;k < uroven;k++) retezceAddString(rtzce,i+2," ");
                    retezceAddString(rtzce,i+2,"\\item");
                } else if ((zasobnikIsEmpty(&zas) == 0 ) && (zasobnikReadLast(&zas) > delka)) {
                    retezceSetString(rtzce,i+1,"");
                    while (zasobnikReadLast(&zas) > delka) {
                        zasobnikPull(&zas);
                        if (zasobnikPull(&typ) == MINUS) retezceAddString(rtzce,i+1,"\\end{enumerate}\n");
                        else retezceAddString(rtzce,i+1,"\\end{itemize}\n");
                        retezceSetString(rtzce,i+2,"");
                        for (k = 1;k < uroven;k++) retezceAddString(rtzce,i+2," ");
                        uroven--;
                        retezceAddString(rtzce,i+2,"\\item");
                    }
                } else {
                    retezceSetString(rtzce,i+1,"");
                    for (k = 1;k < uroven;k++) retezceAddString(rtzce,i+1," ");
                    retezceSetString(rtzce,i+2,"\\item");
                }
                urovenDelta = uroven - urovenDelta;
                //printf("vycet, uroven delta: %d, uroven: %d\n",urovenDelta,uroven);
            }
        }

        if (retezceGetTyp(rtzce,i) == KONEC_RADKU) {
            pouzeMezery = 0;
            if (retezceGetDelka(rtzce,i + 1) >= 2) pouzeMezery = 1;
            for (j = 0;(pouzeMezery == 1) && (j < retezceGetDelka(rtzce,i + 1));j++) if (retezceGetChar(rtzce,i + 1,j) != ' ') pouzeMezery = 0;
            if ((pouzeMezery == 0) && ((retezceGetTyp(rtzce,i+2) != STAR ) && (retezceGetTyp(rtzce,i+2) != MINUS))) {
                urovenDelta = 0;
                while (zasobnikIsEmpty(&zas) == 0) {
                    zasobnikPull(&zas);
                    //retezceSetString(rtzce,i-1,"==zde==\n");
                    for (k = 1;k < uroven;k++) retezceAddString(rtzce,i," ");
                    if (zasobnikPull(&typ) == MINUS) retezceAddString(rtzce,i,"\\end{enumerate}\n");
                    else retezceAddString(rtzce,i,"\\end{itemize}\n");
                    urovenDelta--;
                    uroven--;
                }
            }
        }
        //if (urovenDelta != 0) printf("vycet, uroven delta: %d, uroven: %d\n",urovenDelta,uroven);
    }

    if (zasobnikIsEmpty(&zas) == 0) {
        r = newRetezec();
        retezecClear(&r,0);
        retezceAdd(rtzce,&r);
        retezceAddString(rtzce,i,"\n");
        while (zasobnikIsEmpty(&zas) == 0) {
            zasobnikPull(&zas);
            for (k = 1;k < uroven;k++) retezceAddString(rtzce,i," ");
            if (zasobnikPull(&typ) == MINUS) retezceAddString(rtzce,i,"\\end{enumerate}");
            else retezceAddString(rtzce,i,"\\end{itemize}");
            if (zasobnikIsEmpty(&zas) == 0) retezceAddString(rtzce,i,"\n");
            uroven--;
        }
        deleteRetezec(&r);
    }
    deleteZasobnik(&zas);
    deleteZasobnik(&typ);
}

void nadpisy(retezce * rtzce) {
    int i,j,k;
    char jeNadpis;
    char jeUkoncen;
    char jePrazdny;
    char jeKonecRadku;
    int ukoncovaciTag;
    for (i = -1;i < retezceGetPocet(rtzce);i++) {
        jeNadpis = 1;
        if ((retezceGetTyp(rtzce,i) != KONEC_RADKU) && i != -1) jeNadpis = 0;
        if (jeNadpis && (retezceGetTyp(rtzce,i+1) <= HEADLINEBEGIN || retezceGetTyp(rtzce,i+1) >= HEADLINEEND)) jeNadpis = 0;
        if (jeNadpis) {
            jeUkoncen = 0;
            jeKonecRadku = 0;
            for (j = i+2;j < retezceGetPocet(rtzce) && jeUkoncen == 0 && jeKonecRadku == 0;j++) {
                if (retezceGetTyp(rtzce,j) > HEADLINEBEGIN && retezceGetTyp(rtzce,j) < HEADLINEEND) {
                    ukoncovaciTag = j;
                    if (retezceGetTyp(rtzce,j+1) == KONEC_RADKU) jeUkoncen = 1;
                    else {
                        jePrazdny = 0;
                        if (retezceGetTyp(rtzce,j+1) == TEXT) jePrazdny = 1;
                        for (k=0;k < retezceGetDelka(rtzce,j+1);k++)
                            if (retezceGetChar(rtzce,j+1,k) != ' ' && retezceGetChar(rtzce,j+1,k) != 13)
                                jePrazdny = 0;
                        if (jePrazdny == 1) jeUkoncen = 1;
                    }
                }
                if (retezceGetTyp(rtzce,j) == KONEC_RADKU) jeKonecRadku = 1;
            }
            if (jeUkoncen == 0) jeNadpis = 0;
        }
        if (jeNadpis == 1) {
            switch (retezceGetTyp(rtzce,i+1)) {
            case HEADLINE1:
                retezceSetString(rtzce,i+1,"\\part{");
                break;
            case HEADLINE2:
                retezceSetString(rtzce,i+1,"\\section{");
                break;
            case HEADLINE3:
                retezceSetString(rtzce,i+1,"\\subsection{");
                break;
            case HEADLINE4:
                retezceSetString(rtzce,i+1,"\\subsubsection{");
                break;
            case HEADLINE5:
                retezceSetString(rtzce,i+1,"\\paragraph{");
                break;
            }
            retezceSetString(rtzce,ukoncovaciTag,"}");
            i = ukoncovaciTag;
        }
    }
}

void tabulky(retezce * rtzce) {
    int i,j,k,l,m;
    char jeTabulka = 0;
    char jeZacatekRadku = 0;
    char jedenRadek;
    char tabulkaUkoncena = 1;
    char tabulkaZacala = 0;
    char viceSloupcu = 0;
    int zacatekTabulky;
    int sloupcuVRadku = -1;
    int sloupcuAktualniTabulka = 0;
    int radek;
    int opakovani;
    int opakovaniPredchozi;
    int predchoziTag;

    for (i = 0;i < retezceGetPocet(rtzce);i++) {
        jeTabulka = 0;
        if (retezceGetTyp(rtzce,i) == TABLE || retezceGetTyp(rtzce,i) == TABLEBOLD) jeZacatekRadku = 1;
        if (jeZacatekRadku == 1 && retezceGetTyp(rtzce,i-1) != KONEC_RADKU && i > 0) jeZacatekRadku = 0;
        jedenRadek = 1;
        sloupcuVRadku = 0;
        if (jeZacatekRadku == 1) {
            for (j=i+1;j<retezceGetPocet(rtzce) && jedenRadek == 1;j++) {
                if (retezceGetTyp(rtzce,j) == KONEC_RADKU) jedenRadek = 0;
                if (retezceGetTyp(rtzce,j) == TABLE || retezceGetTyp(rtzce,j) == TABLEBOLD) sloupcuVRadku++;
            }
            if (sloupcuVRadku > 0) {
                if (jeTabulka == 0) zacatekTabulky = i;
                tabulkaUkoncena = 0;
                jeTabulka = 1;
                if (tabulkaZacala == 1 && sloupcuVRadku != sloupcuAktualniTabulka ) {
                    //printf("tabulka skoncila a zacala zas nova \n");
                    retezceAddString(rtzce,i-1,"\n\\end{tabular}");
                    tabulkaUkoncena = 1;
                    tabulkaZacala = 0;
                }
                if (tabulkaZacala == 0) {
                    radek = 0;
                    //printf("zacatek tabulky\n");
                    tabulkaZacala = 1;
                    tabulkaUkoncena = 0;
                    sloupcuAktualniTabulka = sloupcuVRadku;
                    retezceSetString(rtzce,i,"\n\\begin{tabular}{");
                    for (m = 0;m < sloupcuAktualniTabulka;m++) {
                        retezceAddString(rtzce,i,"|c");
                    }
                    retezceAddString(rtzce,i,"|} \n\\hline \n");
                }
                //printf("zacatek radku -%d %d %d-",i,j,sloupcuAktualniTabulka);
                l = 0;
                opakovani = 0;
                for (k = j-1;k>=i;k--) {
                    if (retezceGetTyp(rtzce,k) == TABLE || retezceGetTyp(rtzce,k) == TABLEBOLD) {
                        if (k != i && (radek != 0 || k != i) && opakovani == 0)  retezceSetString(rtzce,k,"&");
                        else if (k == i && radek != 0) retezceSetString(rtzce,k,"");
                        if ( l == 0) retezceSetString(rtzce,k,"\\\\ \\hline");
                        if (viceSloupcu == 1) {
                            retezceAddString(rtzce,k,"\\multicolumn{");
                            retezceAddNumber(rtzce,k,opakovaniPredchozi);
                            retezceAddString(rtzce,k,"}{|c|}{");
                            //printf("%d %d\n",opakovani,predchoziTag);
                            viceSloupcu = 0;
                        }
                        if (retezceGetTyp(rtzce,k) == TABLEBOLD && l > 0) {
                            retezceAddString(rtzce,k,"\\textbf{");
                            m = k+1;
                            while ((retezceGetTyp(rtzce,k) != TABLE && retezceGetTyp(rtzce,k) != TABLEBOLD)) m++;
                            retezceAddString(rtzce,m,"}");
                        }
                        l++;
                        opakovani++;
                        predchoziTag = k;

                    } else {
                        if (opakovani > 1)  {
                            retezceSetString(rtzce,predchoziTag,"}");
                            opakovaniPredchozi = opakovani;
                            viceSloupcu = 1;
                        }
                        opakovani = 0;
                    }
                }
                radek++;
                i = j - 2;
            }
        }
        if (retezceGetTyp(rtzce,i-1) == KONEC_RADKU && i > 0 && sloupcuVRadku <= 0 && tabulkaUkoncena == 0 ) {
            //printf("konec tabulky %d-\n",i);
            retezceAddString(rtzce,i-2,"\n\\end{tabular}\n");
            tabulkaUkoncena = 1;
            tabulkaZacala = 0;
        }
    }
    if (tabulkaUkoncena == 0 ) {
        retezceAddString(rtzce,i-1,"\n\\end{tabular}\n");
        tabulkaUkoncena = 1;
        tabulkaZacala = 0;
    }
}

void elementy(retezec * rtz) {
    retezecReplaceString(rtz,"=>","${\\Rightarrow}$");
    retezecReplaceString(rtz,"<=","${\\Leftarrow}$");
    retezecReplaceString(rtz,"->","${\\rightarrow}$");
    retezecReplaceString(rtz,"<-","${\\leftarrow}$");
    retezecReplaceString(rtz,"<=>","${\\Leftrightarrow}$");
    retezecReplaceString(rtz,"<->","${\\leftrightarrow}$");
    retezecReplaceString(rtz,"<<","${\\//}$");
    retezecReplaceString(rtz,">>","${\\gg}$");
    retezecReplaceString(rtz,"(c)","\\copyright");
    retezecReplaceString(rtz,"(r)","\\textregistered");
    retezecReplaceString(rtz,"(tm)","\\texttrademark");
}

int obrazky(retezce * rtzce) {
    int i,pocet = 0;
    char jeObrazek = 0;
    for (i = 0 ;i < retezceGetPocet(rtzce) ;i++) {
        jeObrazek = 0;
        if (retezceGetTyp(rtzce,i) == IMAGEBEGIN) jeObrazek = 1;
        if (jeObrazek == 1 && retezceGetTyp(rtzce,i+2) != IMAGEEND) jeObrazek = 0;
        if (jeObrazek == 1) {
            retezceSetString(rtzce, i ,"\\includegraphics{");
            retezceSetString(rtzce, i+2 ,"}");
            pocet++;
        }
    }
    return pocet;
}
