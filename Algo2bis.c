/*
 * Arthur PINGARD --- arthur.pingard@universite-paris-saclay.fr
 * Pierre LEANDRI --- pierre.leandri@universite-paris-saclay.fr
 */

#include <stdio.h>
#include <stdlib.h>

#define CHECK(test)                                                       \
    if (!(test))                                                          \
        fprintf(stderr, "Test failed in file %s line %d: %s\n", __FILE__, \
                __LINE__, #test);

// Type Booleen
typedef enum { false, true } bool;

/* Definition type liste */

typedef struct Bloc {
    int valeur;
    struct Bloc* suite;
} Bloc;

typedef Bloc* Liste;

/*******/

Liste ajoute(int x, Liste l) {
    Liste tmp = (Liste)malloc(sizeof(Bloc));
    tmp->valeur = x;
    tmp->suite = l;
    return tmp;
}

/*****************************/
/*       Affiche             */
/*****************************/

void affiche_iter(Liste l) {
    Liste L2 = l;
    printf("[ ");
    while (L2 != NULL) {
        printf("%d ", L2->valeur);
        L2 = L2->suite;
    }
    printf("]");
}

Liste copier_liste(Liste L) {
    if (L == NULL) return NULL;
    return ajoute(L->valeur, copier_liste(L->suite));
}

/* Type L_Liste : Liste de listes */

typedef struct LBloc {
    Liste L;
    struct LBloc* suite;
} LBloc;

typedef LBloc* LList;

LList ajoute_LList(
    LList LL, Liste L) {  // Ajoute la liste L à la fin de la liste de listes LL
    LList b = malloc(sizeof(LBloc));
    b->L = L;
    b->suite = NULL;

    if (LL == NULL) {
        return b;
    } else {
        LList p = LL;
        while (p->suite != NULL) {
            p = p->suite;
        }
        p->suite = b;
        return LL;
    }
}

LList concatene_LList(LList L1, LList L2) {  // Concatene deux listes de listes
    if (L1 == NULL) return L2;
    LList p = L1;
    while (p->suite != NULL) p = p->suite;
    p->suite = L2;
    return L1;
}

LList ajoute_val_LList(int x,
                       LList LL) {  // Ajoute la valeur x au début de chaque
                                    // liste dans la liste de liste LL
    LList R = NULL;
    for (LList p = LL; p != NULL; p = p->suite)
        R = ajoute_LList(R, ajoute(x, copier_liste(p->L)));
    return R;
}

LList PPQ(int p1, int p2, int q) {
    if (q == 0) {
        LList R = NULL;
        R = ajoute_LList(R, NULL);
        return R;
    }

    if (q < p1) return NULL;

    LList resultat = NULL;

    for (int x = p1; x <= p2; x++) {
        if (q < x) break;

        LList suite = PPQ(p1, p2, q - x);
        if (suite == NULL)
            continue;  // Si x empêche les autres solutions, on ne le prend pas

        LList xList = ajoute_val_LList(x, suite);
        resultat = concatene_LList(resultat, xList);
    }

    return resultat;
}

void affiche_LList(LList LL) {
    printf("[");
    LList p = LL;

    while (p != NULL) {
        printf(" ");
        affiche_iter(p->L);  // on affiche la sous-liste
        p = p->suite;
        if (p != NULL) printf(",");  // séparateur entre les sous-listes
    }

    printf(" ]");
}

int main(void) {
    affiche_LList(PPQ(2, 4, 9));

    return 0;
}