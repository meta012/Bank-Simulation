#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "func.h"

int didesnisSkaicius(BigInt *a, BigInt *b) {
    int bool = 0;
    if (a->ilgis > b->ilgis) {
        return 1;
    } else if (a->ilgis < b->ilgis)
        return 0;

    int ilgis = a->ilgis;
    for (int i = 0; i < a->ilgis; i++) {
        if (*(a->skaicius + (ilgis - i) - 1) == *(b->skaicius + (ilgis - i) - 1)) {
            bool = 2;
        } else {
            bool = 0;
            break;
        }
    }

    if (a->ilgis == b->ilgis) {
        int ilgis = a->ilgis;
        for (int i = 0; i < a->ilgis; i++) {
            if (*(a->skaicius + (ilgis - i) - 1) > *(b->skaicius + (ilgis - i) - 1)) {
                return 1;
            }
            if (*(a->skaicius + (ilgis - i) - 1) < *(b->skaicius + (ilgis - i) - 1)) {
                return 0;
            }
        }
    }

    return bool;
}

int yraNulis(BigInt *a) {
    return a->ilgis == 1 && a->skaicius[0] == 0;
}

void kopijuotiSkaiciu(BigInt *naujas, BigInt *kopijuojamas) {
    naujas->ilgis = kopijuojamas->ilgis;
    naujas->skaicius = realloc(naujas->skaicius, naujas->ilgis * sizeof(char));
    naujas->zenklas = kopijuojamas->zenklas;

    for (int i = 0; i < naujas->ilgis; i++)
        naujas->skaicius[i] = kopijuojamas->skaicius[i];
}

int palyginti(BigInt *a, BigInt *b) {
    int bool = 0;
    int didesnis = didesnisSkaicius(a, b);

    if (a->zenklas == 0 && b->zenklas == 0) {
        if (didesnis == 1) {
            bool = 1;
        }
        if (didesnis == 0) {
            bool = -1;
        }
    }
    if (a->zenklas == 1 && b->zenklas == 0) {
        bool = -1;
    }
    if (a->zenklas == 0 && b->zenklas == 1) {
        bool = 1;
    }
    if (a->zenklas == 1 && b->zenklas == 1) {
        if (didesnis == 1) {
            bool = -1;
        }
        if (didesnis == 0) {
            bool = 1;
        }
    }

    return bool;
}

BigInt *naujasSkaicius(char *a) {
    int ilgis = (int) strlen(a);
    BigInt *naujasBigInt = (BigInt *) malloc(sizeof(BigInt));

    char *skaicius = (char *) malloc(ilgis * sizeof(char));

    if (a[0] == '-') {
        naujasBigInt->zenklas = 1;
        ilgis--;
    } else {
        naujasBigInt->zenklas = 0;
    }

    //Skaicius 12345 atmintyje laikomas 54321
    for (int i = 0; i < ilgis; i++) {
        if (naujasBigInt->zenklas) {
            skaicius[i] = (char) (a[(ilgis - 1) - i + 1] - '0');
        } else {
            skaicius[i] = (char) (a[(ilgis - 1) - i] - '0');
        }
    }

    naujasBigInt->skaicius = skaicius;
    naujasBigInt->ilgis = ilgis;

    return naujasBigInt;
}

BigInt *atimtiCore(BigInt *a, BigInt *b) {

    //Didziausias skaiciaus ilgis (a ar b)
    int maxIlgis = a->ilgis > b->ilgis ? a->ilgis : b->ilgis;

    //Sukuriam rezultato BigInt
    BigInt *rezultatas = malloc(sizeof(BigInt));

    //Atlaisvinam vietos naujam skaiciui kurio ilgis lygus ilgesniam demeniui + 1 jei overflow
    char *skaicius = (char *) malloc((maxIlgis + 1) * sizeof(char));
    char overflow = 0;

    //Sudedam abieju skaiciu skaitmenis atskirai
    for (int i = 0; i < maxIlgis; i++) {
        char aSkaicius = 0;
        char bSkaicius = 0;
        char skaitmuo = 0;

        //Jei skaitmens neturi, tai skaitmuo lygus 0
        if (i < a->ilgis)
            aSkaicius = a->skaicius[i];

        if (i < b->ilgis)
            bSkaicius = b->skaicius[i];

        //Apskaiciuojam skaitmeni
        skaitmuo = aSkaicius - bSkaicius - overflow;
        overflow = 0;

        //Jei overflow
        if (skaitmuo < 0) {
            skaitmuo += 10;
            overflow = 1;
        }

        skaicius[i] = skaitmuo;
    }

    //Jei paskutinis skaitmuo overflow:
    if (overflow) {
        skaicius[maxIlgis] = 1;
        rezultatas->ilgis = maxIlgis + 1;
    } else {
        rezultatas->ilgis = maxIlgis;
    }

    int i = 0;
    while (skaicius[rezultatas->ilgis - (i + 1)] == 0) {
        i++;
    }

    rezultatas->ilgis = maxIlgis - i;
    rezultatas->skaicius = skaicius;

    return rezultatas;
}

BigInt *sudetiCore(BigInt *a, BigInt *b) {
    //Didziausias skaiciaus ilgis (a ar b)
    int maxIlgis = a->ilgis > b->ilgis ? a->ilgis : b->ilgis;

    //Sukuriam rezultato BigInt
    BigInt *rezultatas = malloc(sizeof(BigInt));

    //Atlaisvinam vietos naujam skaiciui kurio ilgis lygus ilgesniam demeniui + carry
    char *skaicius = (char *) malloc((maxIlgis + 1) * sizeof(char));

    char overflow = 0;

    //Sudedam abieju skaiciu skaitmenis atskirai
    for (int i = 0; i < maxIlgis; i++) {

        char aSkaicius = 0;
        char bSkaicius = 0;

        char skaitmuo = 0;

        //Jei skaitmens neturi, tai skaitmuo lygus 0
        if (i < a->ilgis) {
            aSkaicius = a->skaicius[i];
        }

        if (i < b->ilgis) {
            bSkaicius = b->skaicius[i];
        }

        //Apskaiciuojam skaitmeni
        skaitmuo = aSkaicius + bSkaicius + overflow;

        overflow = 0;

        //Jei overflow
        if (skaitmuo > 9) {
            skaitmuo -= 10;
            overflow = 1;
        }

        skaicius[i] = skaitmuo;
    }

    //Jei paskutinis skaitmuo overflow:
    if (overflow) {
        skaicius[maxIlgis] = 1;
        rezultatas->ilgis = maxIlgis + 1;
    } else {
        rezultatas->ilgis = maxIlgis;
    }

    rezultatas->skaicius = skaicius;

    return rezultatas;
}

BigInt *sudeti(BigInt *a, BigInt *b) {
    //Sukuriam rezultato BigInt
    BigInt *rezultatas = NULL;

    if (didesnisSkaicius(a, b) == 1) {
        if (a->zenklas == 0 && b->zenklas == 0) {
            rezultatas = sudetiCore(a, b);
            rezultatas->zenklas = 0;
        }
        if (a->zenklas && b->zenklas == 0) {
            rezultatas = atimtiCore(a, b);
            rezultatas->zenklas = 1;
        }
        if (a->zenklas == 0 && b->zenklas) {
            rezultatas = atimtiCore(a, b);
            rezultatas->zenklas = 0;
        }
        if (a->zenklas && b->zenklas) {
            rezultatas = sudetiCore(a, b);
            rezultatas->zenklas = 1;
        }
    }
    if (didesnisSkaicius(a, b) == 0) {
        if (a->zenklas == 0 && b->zenklas == 0) {
            rezultatas = sudetiCore(a, b);
            rezultatas->zenklas = 0;
        }
        if (a->zenklas && b->zenklas == 0) {
            rezultatas = atimtiCore(b, a);
            rezultatas->zenklas = 0;
        }
        if (a->zenklas == 0 && b->zenklas) {
            rezultatas = atimtiCore(b, a);
            rezultatas->zenklas = 1;
        }
        if (a->zenklas && b->zenklas) {
            rezultatas = sudetiCore(a, b);
            rezultatas->zenklas = 1;
        }
    }
    if (didesnisSkaicius(a, b) == 2) {
        if (a->zenklas == 0 && b->zenklas == 0) {
            rezultatas = sudetiCore(a, b);
            rezultatas->zenklas = 0;
        }
        if (a->zenklas && b->zenklas == 0) {
            rezultatas = naujasSkaicius("0");
//            char *skaicius = (char *) malloc(sizeof(char));
//            skaicius[0] = 0;
//            rezultatas->skaicius = skaicius;
//            rezultatas->ilgis = 1;
//            rezultatas->zenklas = 0;
        }
        if (a->zenklas == 0 && b->zenklas) {
            rezultatas = naujasSkaicius("0");
//            char *skaicius = (char *) malloc(sizeof(char));
//            skaicius[0] = 0;
//            rezultatas->skaicius = skaicius;
//            rezultatas->ilgis = 1;
//            rezultatas->zenklas = 0;
        }
        if (a->zenklas && b->zenklas) {
            rezultatas = sudetiCore(a, b);
            rezultatas->zenklas = 1;
        }
    }
    if (rezultatas->ilgis == 0) {
        rezultatas->ilgis = 1;
        rezultatas->skaicius[0] = 0;
        rezultatas->zenklas = 0;
    }

    return rezultatas;
}

BigInt *atimti(BigInt *a, BigInt *b) {
    //Sukuriam rezultato BigInt
    BigInt *rezultatas = NULL;

    if (didesnisSkaicius(a, b)) {
        if (a->zenklas == 0 && b->zenklas == 0) {
            rezultatas = atimtiCore(a, b);
            rezultatas->zenklas = 0;
        }
        if (a->zenklas && b->zenklas == 0) {
            rezultatas = sudetiCore(a, b);
            rezultatas->zenklas = 1;
        }
        if (a->zenklas == 0 && b->zenklas) {
            rezultatas = sudetiCore(a, b);
            rezultatas->zenklas = 0;
        }
        if (a->zenklas && b->zenklas) {
            rezultatas = atimtiCore(a, b);
            rezultatas->zenklas = 1;
        }
    }
    if (didesnisSkaicius(a, b) == 0) {
        if (a->zenklas == 0 && b->zenklas == 0) {
            rezultatas = atimtiCore(b, a);
            rezultatas->zenklas = 1;
        }
        if (a->zenklas && b->zenklas == 0) {
            rezultatas = sudetiCore(a, b);
            rezultatas->zenklas = 1;
        }
        if (a->zenklas == 0 && b->zenklas) {
            rezultatas = sudetiCore(b, a);
            rezultatas->zenklas = 0;

        }
        if (a->zenklas && b->zenklas) {
            rezultatas = atimtiCore(b, a);
            rezultatas->zenklas = 0;
        }
    }
    if (didesnisSkaicius(a, b) == 2) {
        if (a->zenklas == 0 && b->zenklas == 0) {
            rezultatas = naujasSkaicius("0");
        }
        if (a->zenklas && b->zenklas == 0) {
            rezultatas = sudetiCore(a, b);
            rezultatas->zenklas = 1;
        }
        if (a->zenklas == 0 && b->zenklas) {
            rezultatas = sudetiCore(a, b);
            rezultatas->zenklas = 0;
        }
        if (a->zenklas && b->zenklas) {
            rezultatas = naujasSkaicius("0");
        }
    }
    if (rezultatas->ilgis == 0) {
        rezultatas->ilgis = 1;
        rezultatas->skaicius[0] = 0;
        rezultatas->zenklas = 0;
    }
    return rezultatas;
}

BigInt *padauginti(BigInt *a, BigInt *b) {
    BigInt *rezultatas = naujasSkaicius("0");
    BigInt *temp;
    BigInt *Vienetas = naujasSkaicius("1"); // const
    BigInt *daugiklis = naujasSkaicius("0");
    kopijuotiSkaiciu(daugiklis, b);

    if (a->zenklas == 0 && b->zenklas == 0) {
        while (yraNulis(daugiklis) != 1) {
            temp = atimti(daugiklis, Vienetas);
            atlaisvinti(daugiklis);
            daugiklis = temp;

            temp = sudeti(rezultatas, a);
            atlaisvinti(rezultatas);
            rezultatas = temp;
        }
    }

    if (a->zenklas && b->zenklas == 0) {
        while (yraNulis(daugiklis) != 1) {
            temp = atimti(daugiklis, Vienetas);
            atlaisvinti(daugiklis);
            daugiklis = temp;
            temp = atimti(rezultatas, a);
            atlaisvinti(rezultatas);
            rezultatas = temp;
        }
        rezultatas->zenklas = 1;
    }

    if (a->zenklas == 0 && b->zenklas) {
        while (yraNulis(daugiklis) != 1) {
            temp = sudeti(daugiklis, Vienetas);
            atlaisvinti(daugiklis);
            daugiklis = temp;
            temp = sudeti(rezultatas, a);
            atlaisvinti(rezultatas);
            rezultatas = temp;
        }
        rezultatas->zenklas = 1;
    }

    if (a->zenklas && b->zenklas) {
        while (yraNulis(daugiklis) != 1) {
            temp = sudeti(daugiklis, Vienetas);
            atlaisvinti(daugiklis);
            daugiklis = temp;
            temp = atimti(rezultatas, a);
            atlaisvinti(rezultatas);
            rezultatas = temp;
        }
        rezultatas->zenklas = 0;
    }

    atlaisvinti(Vienetas);
    return rezultatas;
}

BigInt *padalinti(BigInt *a, BigInt *b) {
    BigInt *rezultatas = naujasSkaicius("0");
    BigInt *temp;
    BigInt *Vienetas = naujasSkaicius("1"); // const
    BigInt *dalinys = malloc(sizeof(BigInt));
    kopijuotiSkaiciu(dalinys, a);

    if (a->zenklas == 0 && b->zenklas == 0) {
        while (didesnisSkaicius(b, dalinys) != 1) { // b <= dalinys
            temp = atimti(dalinys, b);
            atlaisvinti(dalinys);
            dalinys = temp;
            temp = sudeti(rezultatas, Vienetas);
            atlaisvinti(rezultatas);
            rezultatas = temp;
        }
    }

    if (a->zenklas && b->zenklas == 0) {
        while (didesnisSkaicius(b, dalinys) != 1) {
            temp = sudeti(dalinys, b);
            atlaisvinti(dalinys);
            dalinys = temp;
            temp = sudeti(rezultatas, Vienetas);
            atlaisvinti(rezultatas);
            rezultatas = temp;
        }
        if (didesnisSkaicius(a, b) == 0) {
            rezultatas->zenklas = 0;

        } else {
            rezultatas->zenklas = 1;
        }
    }

    if (a->zenklas == 0 && b->zenklas) {
        while (didesnisSkaicius(b, dalinys) != 1) {
            temp = sudeti(dalinys, b);
            atlaisvinti(dalinys);
            dalinys = temp;
            temp = sudeti(rezultatas, Vienetas);
            atlaisvinti(rezultatas);
            rezultatas = temp;
        }
        if (didesnisSkaicius(a, b) == 0) {
            rezultatas->zenklas = 0;

        } else {
            rezultatas->zenklas = 1;
        }
    }
    if (a->zenklas && b->zenklas) {
        while (didesnisSkaicius(b, dalinys) != 1) {
            temp = atimti(dalinys, b);
            atlaisvinti(dalinys);
            dalinys = temp;
            temp = sudeti(rezultatas, Vienetas);
            atlaisvinti(rezultatas);
            rezultatas = temp;
        }
        rezultatas->zenklas = 0;
    }

    atlaisvinti(Vienetas);
    return rezultatas;
}

char *toString(BigInt *a) {
    char *skaicius = a->skaicius;
    int ilgis = a->ilgis;
    char *rez = malloc(ilgis + 1);

    for (int i = 0; i < ilgis; i++)
        rez[i + 1] = (char) (skaicius[(ilgis - i) - 1] + '0');

    rez[ilgis + 1] = '\0';

    if (a->zenklas) {
        rez[0] = '-';
    } else {
        rez += 1;
    }

    return rez;
}

BigInt *modulis(BigInt *a, BigInt *b) {
    BigInt *temp1 = padalinti(a, b);
    BigInt *temp2 = padauginti(temp1, b);
    BigInt *rez = atimti(a, temp2);
    atlaisvinti(temp1);
    atlaisvinti(temp2);
    return rez;
}

void atlaisvinti(BigInt *a) {
    free(a->skaicius);
    free(a);
}
