/*
 * Arthur PINGARD --- arthur.pingard@universite-paris-saclay.fr
 * Pierre LEANDRI --- pierre.leandri@universite-paris-saclay.fr
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>  // Pour utiliser le temps en tant que seed SIMPLE pour l'aléatoire.

// Type Booleen
typedef enum { false, true } bool;

// Factorielle
long fact1(int n) {
    if (n == 0)
        return 1;
    else
        return fact1(n - 1) * n;
}

// itou avec un argument out => passage par adresse
// le calcul se fait comme dans la version récursive
// (...(((1*1)*2)*3) ... *n)
void bisfact2(int n, long* r) {
    if (n == 0)
        *r = 1;
    else {
        bisfact2(n - 1, r);
        *r = n * *r;  // ou *r*n
    }  // Notez le double sens de * ...
}

long fact2(int n) {
    long r;
    bisfact2(n, &r);
    return r;
}

// variante qui consiste en fait à initialier r beaucoup plus tôt
// du coup, r est inout
void bisfact3(int n, long* r) {
    if (n == 0) {
    } else {
        bisfact3(n - 1, r);
        *r = n * *r;
    }
}

long fact3(int n) {
    long r = 1;
    bisfact3(n, &r);
    return r;
}

// Ici r est vraiment inout, récursif terminal
// le calcul est différent : (((...((1*n)*(n-1)) ... *3)*2)*1)

void bisfact4(int n, long* r) {
    if (n == 0) {
    } else {
        *r = n * *r;
        bisfact4(n - 1, r);
    }
}

long fact4(int n) {
    long r = 1;
    bisfact4(n, &r);
    return r;
}

// et ce code-ci est *** FAUX **** !!! Comprenez-vous pourquoi ?
void bisfact5(int n, long* r) {
    if (n == 0)
        *r = 1;
    else {
        *r = n * *r;
        bisfact5(n - 1, r);
    }
}

long fact5(int n) {
    long r;
    bisfact5(n, &r);
    return r;
}

// itou en stoquant tout dans un tableau... (très bof en complexité espace)
// c'est juste pour manipuler un peu les tableaux
long fact6(int n) {
    long* T = (long*)malloc((n + 1) * sizeof(long));
    T[0] = 1;
    for (int i = 1; i <= n; i++) T[i] = i * T[i - 1];
    long r = T[n];
    free(T);
    return r;
}

#define VersionsFact 6
// nombre de versions de factorielle

long fact(int n, int v)  // numéro de version
{
    switch (v) {
        case 1:
            return fact1(n);
            break;
        case 2:
            return fact2(n);
            break;
        case 3:
            return fact3(n);
            break;
        case 4:
            return fact4(n);
            break;
        case 5:
            return fact5(n);
            break;
        case 6:
            return fact6(n);
            break;
        default:
            return 0;
    }
}

/*************************************************/
/* QUESTION 1 --- Calcul de e                    */
/*************************************************/
// e = 2,7182818284 5904523536 0287471352 6624977572 4709369995
//       9574966967 6277240766 3035354759 4571382178 5251664274

float Efloat(void) {
    float sum = 1;
    float fact = 1;

    for (int i = 1; i <= 100; i++) {
        fact = fact * i;
        sum = sum + (1. / fact);
    }
    return sum;
}

double Edouble(void) {
    double sum = 1;
    double fact = 1;
    for (int i = 1; i <= 100; i++) {
        fact = fact * i;
        sum = sum + (1. / fact);
    }
    return sum;
}

long double Elongdouble(void) {
    long double sum = 1;
    long double fact = 1;
    for (int i = 1; i <= 100; i++) {
        fact = fact * i;
        sum = sum + (1. / fact);
    }
    return sum;
}

/*************************************************/
/* QUESTION 2 --- Suite Y                        */
/*************************************************/

void afficheYfloat(int n) {
    float currentY = Efloat() - 1.0f;
    for (int i = 0; i != n; i++) {
        printf(" y%d = %.20f\n", i, currentY);
        currentY = (i + 1) * currentY - 1.0f;
    }
}

void afficheYdouble(int n) {
    double currentY = Edouble() - 1.0;
    for (int i = 0; i != n; i++) {
        printf(" y%d = %.30lf\n", i, currentY);
        currentY = (i + 1) * currentY - 1.0;
    }
}

void afficheYlongdouble(int n) {
    long double currentY = Elongdouble() - 1.0L;
    for (int i = 0; i != n; i++) {
        printf(" y%d = %.40LF\n", i, currentY);
        currentY = (i + 1) * currentY - 1.0L;
    }
}

/*************************************************/
/* QUESTION 3 --- Suite de Syracuse              */
/*************************************************/

#define CSyr 2025

// Version 1
int SyracuseI(int n) {
    int Syr = CSyr;
    for (int i = n; i > 0; --) {
        if (Syr % 2 == 0)
            Syr = Syr / 2;
        else
            Syr = 3 * Syr + 1;
    }
    return Syr;
}

// Version 2
int Syracuse_SF_aux(int Syr, int iter) {
    if (iter == 0)
        return Syr;
    else if (Syr % 2 == 0)
        return Syracuse_SF_aux(Syr / 2, iter - 1);
    else
        return Syracuse_SF_aux(3 * Syr + 1, iter - 1);
}

int SyracuseSF(int n) { return Syracuse_SF_aux(CSyr, n); }

// Version 3

int Syracuse_SP_aux(int* Syr, int iter) {
    if (iter == 0)
        return *Syr;

    else if (*Syr % 2 == 0) {
        *Syr = *Syr / 2;
        return Syracuse_SP_aux(&*Syr, iter - 1);
    }

    else {
        *Syr = (3 * (*Syr)) + 1;
        return Syracuse_SP_aux(&*Syr, iter - 1);
    }
}

int SyracuseSP(int n) {
    int Syr = CSyr;
    Syracuse_SP_aux(&Syr, n);
    return Syr;
}

/*************************************************/

int SyracuseR(int n) {
    if (n == 0)
        return CSyr;
    else {
        int Syr = SyracuseR(n - 1);
        if (Syr % 2 == 0)
            return Syr / 2;
        else
            return 3 * Syr + 1;
    }
}

/*************************************************/

#define VersionsSyracuse 4
// nombre de versions codees

/*************************************************/

int Syracuse(int n, int i) {
    switch (i) {
        case 1:
            return SyracuseI(n);  // break ;
        case 2:
            return SyracuseSF(n);  // break ;
        case 3:
            return SyracuseSP(n);  // break ;
        case 4:
            return SyracuseR(n);  // break ;
        default:
            return 0;
    }
}

/*************************************************/
/* QUESTION 4 --- Permutations tableaux          */
/*************************************************/

int Int_Lire(void)  // Lecture simpliste d'un entier positif ou nul
                    // ignore les char avant le premier chiffre
                    // s'arrête en absorbant le char après le dernier chiffre
                    // exemple "truc bidule 345x" donne 345
{
    char c;
    do c = getchar();
    while (c < '0' || c > '9');
    int n = (int)(c - '0');
    do {
        c = getchar();
        if (c < '0' || c > '9') break;
        n = n * 10 + (int)(c - '0');
    } while (true);
    return n;
}

/*************************************************/

int* P_Lire(int n) {
    printf("Input Permutation 0..%d : ", n - 1);
    int* T = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) T[i] = Int_Lire();
    return T;
}

/*************************************************/

int* P_Zero(int n)  // Permutation bidon, qui n'est pas une permutation (sauf
                    // n=0 ou 1 ...)
{
    int* T = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) T[i] = 0;
    return T;
}

/*************************************************/

void P_Affiche(int* P, int n) {
    printf("[");
    for (int i = 0; i < n; i++) printf(" %d", P[i]);
    printf(" ]\n");
}

/*************************************************/

int* P_identite(int n) {
    // malloc(n=0) renverra soit NULL soit un pointeur.
    // L'utilisateur devra donc free obligatoirement.
    // Même histoire pour ce qui suivra.
    int* T = (int*)malloc(n * sizeof(int));
    // OSEF du cas où l'allocation échoue.
    // En effet, si l'allocation échoue, c'est déjà signe de problème sur la
    // machine.

    for (int i = 0; i < n; i++) T[i] = i;
    return T;
}

/*************************************************/

int* P_Inverse(int* P, int n) {
    int* T = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) T[P[i]] = i;
    return T;
}

/*************************************************/

void P_Compose(int* P, int* Q, int* R, int n)  // écrit PoQ dans R
{
    for (int i = 0; i < n; i++) R[i] = P[Q[i]];
}

/*************************************************/

bool P_Verifie(int* P, int n) {
    // Préparation de la vérification avec le déjà vu.
    bool* deja_vu = (bool*)malloc(n * sizeof(bool));
    for (int i = 0; i < n; i++) {
        deja_vu[i] = false;
    }

    // Vérification linéaire des éléments.
    for (int i = 0; i < n; i++) {
        if (P[i] < 0 ||
            P[i] >=
                n) {  // On traite le cas où on est hors intervalle [0, n-1].
            free(deja_vu);
            return false;
        }

        if (deja_vu[P[i]]) {  // Un déjà vu s'est produit.
            free(deja_vu);
            return false;
        }

        deja_vu[P[i]] = true;
    }

    free(deja_vu);
    return true;
}

/**********************/

int* P_power1(int* P, int n, int k)  // Itératif, complexité environ k
{
    int* res = P_identite(n);
    int* tmp = malloc(n * sizeof(int));

    for (int i = 0; i < k; i++) {
        P_Compose(res, P, tmp, n);
        for (int j = 0; j < n; j++) res[j] = tmp[j];
    }

    free(tmp);
    return res;
}

/**********************/

int* P_power2(int* P, int n, int k)  // Récursif, complexité environ k
{
    if (k == 0) return P_identite(n);

    // Calcul récursif : P^k = P^(k-1) * P d'après la consigne.
    int* res = P_power2(P, n, k - 1); // Calcul de P^(k-1)
    int* tmp = malloc(n * sizeof(int));
    P_Compose(res, P, tmp, n);

    free(res);
    return tmp;
}

/**********************/
// pourquoi le cas spécial k==1 ? et pourquoi n'est-elle pas que `return P` ? OK.
// un peu étrange de calculer `P_k2 = P_power3(P, n, k / 2)` premièrement et « corriger » après si k est impair: A VOIR.
int* P_power3(int* P, int n, int k)  // Récursif, complexité environ log2(k)
{
    if (k == 0) return P_identite(n);
    if (k == 1) return P;

    int* half = P_power3(P, n, k / 2); // On divise par 2 pour l'effet log2.
    int* half_squared = malloc(n * sizeof(int));
    P_Compose(half, half, half_squared, n);
    free(half);

    if (k % 2 == 0) {
        return half_squared;
    } else {
        int* result = malloc(n * sizeof(int));
        P_Compose(P, half_squared, result, n);
        free(half_squared);
        return result;
    }
}

/**********************/

int* P_power4(int* P, int n, int k)  // Itératif, complexité environ log2(k)
{
    int* result = P_identite(n);
    int* power = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) power[i] = P[i];

    int* tmp = malloc(n * sizeof(int));

    while (k > 0) {
        if (k % 2 == 1) {
            P_Compose(power, result, tmp, n);
            // Remplacer result par tmp directement
            for (int j = 0; j < n; j++) result[j] = tmp[j];
        }

        k /= 2;

        if (k > 0) {
            P_Compose(power, power, tmp, n);
            // Remplacer power par tmp directement
            for (int j = 0; j < n; j++) power[j] = tmp[j];
        }
    }

    free(power);
    free(tmp);
    return result;
}

/*************************************************/

const int VersionsPuissance = 4;
// nombre de versions de Puissance, soit 4 selon énoncé

/*************************************************/

int* P_power(int* P, int n, int k, int v)  // version v de 1 à VersionsPuissance
{
    switch (v) {
        case 1:
            return P_power1(P, n, k);
            break;
        case 2:
            return P_power2(P, n, k);
            break;
        case 3:
            return P_power3(P, n, k);
            break;
        case 4:
            return P_power4(P, n, k);
            break;
        default:
            return NULL;
    }
}

/*************************************************/

int* P_random(int n) {
    int* T = P_identite(n);
    // Cet algorithme porte le nom de mélange de Fisher-Yates.
    // https://fr.wikipedia.org/wiki/Mélange_de_Fisher-Yates
    // Pour i allant de n − 1 à 1 (inclus) faire :
    //  j ← entier aléatoire entre 0 et i inclus
    //  échanger a[j] et a[i]
    for (int i = n - 1; i > 0; i--) {
        // Générer un indice aléatoire entre 0 et i
        int j = rand() % (i + 1);

        // Échanger T[i] et T[j]
        int temp = T[i];
        T[i] = T[j];
        T[j] = temp;
    }

    return T;
}

/*************************************************/

void recursive(int depth) {
    if (depth == 0) {
        return;
    }
    recursive(depth - 1);
}

/*************************************************/

// MAIN - EXECUTION
int main(void) {
    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));

    // Mettre "if true" vs "if false" selon que vous voulez les tests ou pas

    /************************  taille des nombres  *************************/
    if (false) {
        printf("tailles des nombres (peuvent varier selon le compilo) :\n");

        printf("short : %d octets\n", (int)sizeof(short));
        printf("int : %d octets\n", (int)sizeof(int));
        printf("long : %d octets\n", (int)sizeof(long));
        printf("long long : %d octets\n", (int)sizeof(long long));
        printf("float : %d octets\n", (int)sizeof(float));
        printf("double : %d octets\n", (int)sizeof(double));
        printf("long double : %d octets\n", (int)sizeof(long double));
        printf("\n");

        printf("limite des flottants (peuvent varier selon le compilo) :\n");

        float z1;
        double z2;
        long double z3;

        int cpt;

        z1 = 2.0;
        cpt = 0;
        while (z1 > 1) {
            z1 = (z1 - 1) / 10 + 1;
            cpt++;
        }
        printf("1+1/10^c devient 1 a partir de c=%d pour les float\n", cpt);

        z2 = 2.0;
        cpt = 0;
        while (z2 > 1) {
            z2 = (z2 - 1) / 10 + 1;
            cpt++;
        }
        printf("1+1/10^c devient 1 a partir de c=%d pour les double\n", cpt);

        z3 = 2.0;
        cpt = 0;
        while (z3 > 1) {
            z3 = (z3 - 1) / 10 + 1;
            cpt++;
        }
        printf("1+1/10^c devient 1 a partir de c=%d pour les long double\n",
               cpt);

        z1 = 1.0;
        cpt = 0;
        while (z1 > 0) {
            z1 = z1 / 10;
            cpt++;
        }
        printf("1/10^c devient 0 a partir de c=%d pour les float\n", cpt);

        z2 = 1.0;
        cpt = 0;
        while (z2 > 0) {
            z2 = z2 / 10;
            cpt++;
        }
        printf("1/10^c devient 0 a partir de c=%d pour les double\n", cpt);

        z3 = 1.0;
        cpt = 0;
        while (z3 > 0) {
            z3 = z3 / 10;
            cpt++;
        }
        printf("1/10^c devient 0 a partir de c=%d pour les long double\n", cpt);

        printf("\n");
    }

    /************************  factorielle  *************************/
    if (false) {
        printf("factorielles de 0, 1, 2, 3, 4, 5, 10, 30, 100, %d codes : \n",
               VersionsFact);
        for (short v = 1; v <= VersionsFact; v++)
            printf("version %d : %ld %ld %ld %ld %ld %ld %ld %ld %ld \n", v,
                   fact(0, v), fact(1, v), fact(2, v), fact(3, v), fact(4, v),
                   fact(5, v), fact(10, v), fact(30, v), fact(100, v));
        printf("\n");
    }

    // QUESTION 1
    // e = 2,7182818284 5904523536 0287471352 6624977572 4709369995
    //       9574966967 6277240766 3035354759 4571382178 5251664274
    if (false) {
        printf(" e1 = %.20f  \n", Efloat());
        printf(" e2 = %.30lf \n", Edouble());
        printf(" e3 = %.40LF \n", Elongdouble());
        printf("\n");
    }

    // QUESTION 2
    // On constate que la suite tend vers 0 au début mais à partir de n=16
    // on voit une remontée pour afficheYdouble et afficheYlongdouble.
    // Pour afficheYfloat, la remontée s'effectue à partir de n=9. On conclut
    // que la précision joue un rôle important dans le calcul de la suite (Y_n)
    // qui tend mathématiquement bien vers 0 par le théorème du sandwich.
    if (false) {
        printf("Valeurs de Y, selon float, double, longdouble :\n");
        afficheYfloat(30);
        afficheYdouble(30);
        afficheYlongdouble(30);
        printf("\n");
    }

    // QUESTION 3
    if (false) {  // Test de toutes les versions
        for (int v = 1; v <= VersionsSyracuse; v++)  // numéro de version
        {
            printf("\n Syr %d \n", v);
            for (int j = 0; j <= 3; j++)
                printf("Syr%d(%d) = %d \n", v, j, Syracuse(j, v));

            long n = 1;
            for (int j = 1; j <= 3; j++) {
                n = n * 10;
                printf("Syr%d(10^%d) = %d \n", v, j, Syracuse(n, v));
            }

            printf("\n");
        }
    }

    // QUESTION 4
    if (true) {
        printf("dim des permutations ? : \n");
        int dim = Int_Lire();

        printf("Identite : ");
        P_Affiche(P_identite(dim), dim);

        int* tmp1 = P_Lire(dim);
        P_Affiche(tmp1, dim);
        printf("Permutation correcte ? : %d \n", P_Verifie(tmp1, dim));

        int* tmp2 = P_Lire(dim);
        int* tmp3 = (int*)malloc(dim * sizeof(int));
        P_Compose(tmp1, tmp2, tmp3, dim);
        printf("Composee des permutations en input :");
        P_Affiche(tmp3, dim);

        int* tmp = P_random(dim);
        printf("permutation aleatoire :");
        P_Affiche(tmp, dim);

        printf("puissances 0, 1, 2, 3, dim, dim! \n");
        for (int v = 1; v <= VersionsPuissance; v++) {
            printf("version puissance %d \n", v);
            P_Affiche(P_power(tmp, dim, 0, v),
                      dim);  // note : fuite memoire ici....
            P_Affiche(P_power(tmp, dim, 1, v), dim);
            P_Affiche(P_power(tmp, dim, 2, v), dim);
            P_Affiche(P_power(tmp, dim, 3, v), dim);
            P_Affiche(P_power(tmp, dim, dim, v), dim);
            P_Affiche(P_power(tmp, dim, fact1(dim), v), dim); // REMARQUE: peut causer des crash
                                                              // si la dimension est trop grande. Par exemple pour dim=10,
                                                              // il y a 10! = 3 628 800 appels récursifs. Pour dim=9 ça
                                                              // plante aussi. Pour dim<=8 c'est ok.
        }

        free(tmp1);
        free(tmp2);
        free(tmp3);
        free(tmp);

        printf("\n");
    }

    // BONUS: pour tester la fameuse remarque sur les crash lié au nombre d'appels récursifs.
    if (false) {
        int max_depth = 362880; // 9!
        recursive(max_depth);
    }

    return 0;
}
