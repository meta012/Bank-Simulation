#ifndef BIGINTEGER_BIGINT_H
#define BIGINTEGER_BIGINT_H



typedef struct {
    char *skaicius;
    int ilgis;
    int zenklas;
} BigInt;

BigInt *sudeti(BigInt *a, BigInt *b);

BigInt *atimti(BigInt *a, BigInt *b);

BigInt *padauginti(BigInt *a, BigInt *b);

// grazina dalybos liekana
BigInt *modulis(BigInt *a, BigInt *b);

// grazina atsakymo sveikaja dali
BigInt *padalinti(BigInt *a, BigInt *b);

// veikia kaip strcpy()
void kopijuotiSkaiciu(BigInt *naujas, BigInt *kopijuojamas);

// naujo skaiciaus sukurimas
BigInt *naujasSkaicius(char *a);

// grazina skaiciu spausdinimui
char *toString(BigInt *a);

/*  int palyginti(a, b) grazina:
 *  1 jei a > b
 *  0 jei a = b
 * -1 jei a < b
 */
int palyginti(BigInt *a, BigInt *b);

int yraNulis(BigInt *a);

void atlaisvinti (BigInt *a);

#endif // BIGINTEGER_BIGINT_H
