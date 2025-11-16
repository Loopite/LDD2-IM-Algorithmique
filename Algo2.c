/*
 * Arthur PINGARD --- arthur.pingard@universite-paris-saclay.fr
 * Pierre LEANDRI --- pierre.leandri@universite-paris-saclay.fr
 *
 * REMARQUE:
 * On utilise CHECK défini ci-dessous pour tester des valeurs
 * rentrées car la 2ème partie s'y prête mieux. Les print associés
 * sont mis en commentaire juste au-dessus du CHECK si jamais
 * celui-ci échoue. Ainsi, vous pourrez obtenir la valeur avant que ça plante
 * juste en décommentant au niveau du print.
 */

#include <stdlib.h>
#include <stdio.h>

#define CHECK(test) \
    if (!(test)) \
        fprintf(stderr, "Test failed in file %s line %d: %s\n", __FILE__, __LINE__, #test);

// Type Booleen
typedef enum { false, true } bool;

/*************************************************/
/*                                               */
/*          definition type liste                */
/*                                               */
/*************************************************/

typedef struct Bloc {
    int valeur;
    struct Bloc* suite;
} Bloc;

typedef Bloc* Liste;

/*************************************************/
/*                                               */
/*                briques de base                */
/*                                               */
/*************************************************/

/*** les 5 fonctionnalit�s suivantes sont plus du sucre syntaxique que du code
 * utile  ***/
/*** sauf � vouloir pouvoir basculer � moindre frais sur une impl�menation des
 * listes ***/
/**** diff�rentes des listes chain�es propos�es dans le cadre de ce projet ***/

// Liste Vide() { return NULL ; }
// void initVide(Liste *L) { *L = NULL ; }
// bool estVide(Liste l) { return l == NULL ; }
// int premier(Liste l) { return l->valeur ; }
// Liste suite(Liste l) { return l->suite ; }

/****************/

void depile(Liste* L) {
    Liste tmp = *L;
    *L = (*L)->suite;
    free(tmp);
}

/*******/

Liste ajoute(int x, Liste l) {
    Liste tmp = (Liste)malloc(sizeof(Bloc));
    tmp->valeur = x;
    tmp->suite = l;
    return tmp;
}

/*******/

void empile(int x, Liste* L) { *L = ajoute(x, *L); }

/*****************************/
/*                           */
/*       Affiche             */
/*                           */
/*****************************/

void affiche_rec(Liste l) {
    if (l == NULL)
        printf("\n");
    else {
        printf("%d ", l->valeur);
        affiche_rec(l->suite);
    }
}

/*******/

void affiche_iter(Liste l) {
    Liste L2 = l;
    while (L2 != NULL) {
        printf("%d ", L2->valeur);
        L2 = L2->suite;
    }
    printf("\n");
}

/****************************/
/*                          */
/*       Longueur           */
/*                          */
/****************************/

int longueur_rec(Liste l) {
    if (l == NULL)
        return 0;
    else
        return (1 + longueur_rec(l->suite));
}

/*******/

int longueur_iter(Liste l) {
    Liste P = l;
    int cpt = 0;
    while (P != NULL) {
        P = P->suite;
        cpt++;
    }
    return cpt;
}

/*****************************************/
/*                                       */
/*                 VireDernier           */
/*     avec un depile                    */
/* � la main opportuniste (version iter) */
/* ou en utilisant depile (version rec ) */
/*                                       */
/*****************************************/

void VD(Liste* L)
// *L non NULL ie liste non vide
{
    if ((*L)->suite == NULL)
        depile(L);  // moralement : depile(& (*L)) ;
    else
        VD(&(*L)->suite);
}

void VireDernier_rec(Liste* L) {
    if (*L != NULL) VD(L);  // moralement : VD(& (*L)) ;
}

/*************/

void VireDernier_iter(Liste* L) {
    if (*L != NULL) {
        while ((*L)->suite != NULL) L = &(*L)->suite;  //  &  (**L).suite  ;
        free(*L);
        *L = NULL;
    }
}

/*************************************************/
/*                                               */
/*       Libere la memoire                       */
/*                                               */
/*************************************************/

void VideListe(Liste* L) {
    if (*L != NULL) {
        depile(L);
        VideListe(L);
    }
}

/********************************************/
/*                                          */
/*          UnPlusDeuxgalTrois              */
/*                                          */
/********************************************/

bool UnPlusDeuxEgalTrois(Liste L) {
    if (L == NULL) return true;                   // 0 + 0 = 0
    if (L->suite == NULL) return L->valeur == 0;  // x + 0 = 0 <=> x = 0
    if (L->suite->suite == NULL)
        return L->valeur == -(L->suite->valeur);  // x + y = 0 <=> x = -y
    return L->valeur + L->suite->valeur == L->suite->suite->valeur;
}

/********************************************/
/*                                          */
/*             PlusCourte                   */
/*                                          */
/********************************************/

bool PlusCourteRec(Liste L1, Liste L2) {
    if (L1 == NULL && L2 != NULL) return true;
    if (L2 == NULL) return false;

    return PlusCourteRec(L1->suite, L2->suite);
}

/*******/
bool PlusCourteIter(Liste L1, Liste L2) {
	while (L1 != NULL && L2 != NULL) {
        if (L1 == NULL && L2 != NULL) return true;
        if (L1 != NULL && L2 == NULL) return false;

        L1 = L1->suite;
        L2 = L2->suite;
    }

	return L1 == NULL && L2 != NULL;
}

/********************************************/
/*                                          */
/*              Verifiek0                   */
/*                                          */
/********************************************/

bool VerifiekORec(Liste L, int k) {
    if (k < 0) return false;
    if (L == NULL) return k == 0;

    if (L->valeur == 0) return VerifiekORec(L->suite, k - 1);

    return VerifiekORec(L->suite, k);
}

/*******/

bool VerifiekOIter(Liste L, int k) {
    int compteur = 0;
    while (L != NULL) {
        if (L->valeur == 0) {
            compteur++;
            if (compteur > k) return false;
        }

        L = L->suite;
    }

    return compteur == k;
}

/********************************************/
/*                                          */
/*     NombreTermesAvantZero                */
/*                                          */
/********************************************/

int NTAZ_It(Liste L) {
	int compteur = 0;

	while (L != NULL && L->valeur != 0) {
		L = L->suite;
		compteur++;
	}

	return compteur;
}

/*******/

int NTAZ_Rec(Liste L) {
    if (L == NULL || L->valeur == 0) return 0;
    return 1 + NTAZ_Rec(L->suite);
}

/*******/
int NTAZ_SF(Liste L, int compteur) {
    if (L == NULL || L->valeur == 0) return compteur;
    return NTAZ_SF(L->suite, compteur + 1);
}

int NTAZ_RTSF(Liste L) {
    return NTAZ_SF(L, 0);
}

/*******/
void NTAZ_SP(Liste L, int* compteur) {
    if (L == NULL || L->valeur == 0) return;
    (*compteur)++;
    NTAZ_SP(L->suite, compteur);
}

int NTAZ_RTSP(Liste L) {
    int compteur = 0;
    NTAZ_SP(L, &compteur);
    return compteur;
}

/********************************************/
/*                                          */
/*              TuePos                      */
/*                                          */
/********************************************/

Liste TuePosRecAux(Liste L, int compteur) {

    if (L == NULL) return NULL; // Cas de base
    L->suite = TuePosRecAux(L->suite, compteur + 1); // On traite la suite de la liste d'abord puis la valeur actuelle

    if (L->valeur == compteur) {        // Si la valeur est egale a la position, on supprime
        Liste suite = L->suite;
        free(L);
        return suite;
    } else {
        return L;
    }
}


void TuePosRec(Liste* L) {
    *L = TuePosRecAux(*L, 1);
}

/*******/

void TuePosIt(Liste* L) {

    Liste actuel = *L;
    Liste precedent = NULL;
    int compteur = 1;

    while (actuel != NULL) {                        // On s'arrete quand on a plus rien a traiter
        if (actuel->valeur == compteur) {           // Si la valeur est egale a la position, on supprime

            if (precedent == NULL) {                // Si on est au debut de la liste, on decale la tete de liste
                *L = actuel->suite;
                free(actuel);
                actuel = *L;
            } else {                                // On supprime l'element au milieu de liste
                precedent->suite = actuel->suite;
                free(actuel);
                actuel = precedent->suite;
            }

        } else {                                    // Si la valeur n'est pas egale a la position, on passe simplement a l'element d'apres
            precedent = actuel;
            actuel = actuel->suite;
        }
        compteur++;
    }
}

/********************************************/
/*                                          */
/*            TueRetroPos                   */
/*                                          */
/********************************************/

Liste TueRetroPosAUX(Liste L, int *compteur_retro) {
    if (L == NULL) {
        *compteur_retro = 0; // On compte les positions depuis la fin en remontant la recursivite
        return NULL;
    }

    L->suite = TueRetroPosAUX(L->suite, compteur_retro);
    *compteur_retro = *compteur_retro + 1;

    if (L->valeur == *compteur_retro) {
        Liste suite = L->suite;
        free(L);
        return suite;
    } else {
        return L;
    }
}
void TueRetroPos(Liste* L) {
    int compteur_retro = 0;
    *L = TueRetroPosAUX(*L, &compteur_retro);
}

/*************************************************/
/*                                               */
/*           Main                                */
/*                                               */
/*************************************************/

int main(void) {
    if (true) {
        Liste L1 = NULL;
        L1 = ajoute(2, L1);
        L1 = ajoute(4, L1);
        L1 = ajoute(42, L1);
        L1 = ajoute(19, L1);
        L1 = ajoute(23, L1);
        //printf("UnPlusDeuxEgalTrois(L1) = %d\n", UnPlusDeuxEgalTrois(L1));
		CHECK(UnPlusDeuxEgalTrois(L1) == 1);

        Liste L2 = NULL;
        L2 = ajoute(-2, L2);
        L2 = ajoute(2, L2);
        //printf("UnPlusDeuxEgalTrois(L2) = %d\n", UnPlusDeuxEgalTrois(L2));
		CHECK(UnPlusDeuxEgalTrois(L2) == 1);

        Liste L3 = NULL;
        L3 = ajoute(1, L3);
        L3 = ajoute(27, L3);
        L3 = ajoute(3, L3);
        L3 = ajoute(2, L3);
        //printf("UnPlusDeuxEgalTrois(L3) = %d\n", UnPlusDeuxEgalTrois(L3));
		CHECK(UnPlusDeuxEgalTrois(L3) == 0);

        Liste L4 = NULL;
        L4 = ajoute(2, L4);
        //printf("UnPlusDeuxEgalTrois(L4) = %d\n", UnPlusDeuxEgalTrois(L4));
		CHECK(UnPlusDeuxEgalTrois(L4) == 0);
    }

    if (true) {
        {
            Liste L1 = NULL;
            Liste L2 = NULL;
            //printf("PlusCourteRec(L1, L2) = %d, PlusCourteIter(L1, L2) = %d\n",
            //       PlusCourteRec(L1, L2), PlusCourteIter(L1, L2));
			CHECK(PlusCourteRec(L1, L2) == 0 && PlusCourteIter(L1, L2) == 0);
        }

        {
            Liste L1 = NULL;
            L1 = ajoute(2, L1);

            Liste L2 = NULL;
            //printf("PlusCourteRec(L1, L2) = %d, PlusCourteIter(L1, L2) = %d\n",
            //       PlusCourteRec(L1, L2), PlusCourteIter(L1, L2));
			CHECK(PlusCourteRec(L1, L2) == 0 && PlusCourteIter(L1, L2) == 0);
        }

        {
            Liste L1 = NULL;

            Liste L2 = NULL;
            L2 = ajoute(2, L2);
            //printf("PlusCourteRec(L1, L2) = %d, PlusCourteIter(L1, L2) = %d\n",
            //       PlusCourteRec(L1, L2), PlusCourteIter(L1, L2));
			CHECK(PlusCourteRec(L1, L2) == 1 && PlusCourteIter(L1, L2) == 1);
        }

        {
            Liste L1 = NULL;
            L1 = ajoute(1, L1);

            Liste L2 = NULL;
            L2 = ajoute(2, L2);
            L2 = ajoute(2, L2);
            //printf("PlusCourteRec(L1, L2) = %d, PlusCourteIter(L1, L2) = %d\n",
            //       PlusCourteRec(L1, L2), PlusCourteIter(L1, L2));
			CHECK(PlusCourteRec(L1, L2) == 1 && PlusCourteIter(L1, L2) == 1);
        }

        {
            Liste L1 = NULL;
            L1 = ajoute(2, L1);
            L1 = ajoute(1, L1);

            Liste L2 = NULL;
            L2 = ajoute(3, L2);
            L2 = ajoute(2, L2);
            L2 = ajoute(1, L2);
            //printf("PlusCourteRec(L1, L2) = %d, PlusCourteIter(L1, L2) = %d\n",
            //       PlusCourteRec(L1, L2), PlusCourteIter(L1, L2));
			CHECK(PlusCourteRec(L1, L2) == 1 && PlusCourteIter(L1, L2) == 1);
        }
    }

    if (true) {
        Liste L1 = NULL;
        //printf("VerifiekORec(L1, 1) = %d, VerifiekOIter(L1, 1) = %d\n",
        //       VerifiekORec(L1, 1), VerifiekOIter(L1, 1));
		CHECK(VerifiekORec(L1, 1) == 0 && VerifiekOIter(L1, 1) == 0);

        //printf("VerifiekORec(L1, 0) = %d, VerifiekOIter(L1, 0) = %d\n",
        //       VerifiekORec(L1, 0), VerifiekOIter(L1, 0));
		CHECK(VerifiekORec(L1, 0) == 1 && VerifiekOIter(L1, 0) == 1);

        Liste L2 = NULL;
        L2 = ajoute(0, L2);
        //printf("VerifiekORec(L2, 1) = %d, VerifiekOIter(L2, 1) = %d\n",
        //       VerifiekORec(L2, 1), VerifiekOIter(L2, 1));
		CHECK(VerifiekORec(L2, 1) == 1 && VerifiekOIter(L2, 1) == 1);

        //printf("VerifiekORec(L2, 0) = %d, VerifiekOIter(L2, 0) = %d\n",
        //       VerifiekORec(L2, 0), VerifiekOIter(L2, 0));
		CHECK(VerifiekORec(L2, 0) == 0 && VerifiekOIter(L2, 0) == 0);

        Liste L3 = NULL;
        L3 = ajoute(0, L3);
        L3 = ajoute(4, L3);
        L3 = ajoute(2, L3);
        L3 = ajoute(6, L3);
        L3 = ajoute(0, L3);
        L3 = ajoute(7, L3);
        L3 = ajoute(0, L3);
        L3 = ajoute(0, L3);
        L3 = ajoute(2, L3);
        //printf("VerifiekORec(L3, 4) = %d, VerifiekOIter(L3, 4) = %d\n",
        //       VerifiekORec(L3, 4), VerifiekOIter(L3, 4));
		CHECK(VerifiekORec(L3, 4) == 1 && VerifiekOIter(L3, 4) == 1);

        //printf("VerifiekORec(L3, 3) = %d, VerifiekOIter(L3, 3) = %d\n",
        //       VerifiekORec(L3, 3), VerifiekOIter(L3, 3));
		CHECK(VerifiekORec(L3, 3) == 0 && VerifiekOIter(L3, 3) == 0);

        //printf("VerifiekORec(L3, 1) = %d, VerifiekOIter(L3, 1) = %d\n",
        //       VerifiekORec(L3, 1), VerifiekOIter(L3, 1));
		CHECK(VerifiekORec(L3, 1) == 0 && VerifiekOIter(L3, 1) == 0);

        //printf("VerifiekORec(L3, 0) = %d, VerifiekOIter(L3, 0) = %d\n",
        //       VerifiekORec(L3, 0), VerifiekOIter(L3, 0));
		CHECK(VerifiekORec(L3, 0) == 0 && VerifiekOIter(L3, 0) == 0);
    }

	if (true) {
		{
			Liste L = NULL;
        	L = ajoute(0, L);
			L = ajoute(6, L);
			L = ajoute(0, L);
			L = ajoute(5, L);
			L = ajoute(9, L);
			L = ajoute(2, L);
			L = ajoute(3, L);

			//printf("NTAZ_It(L) = %d\n", NTAZ_It(L));
		    //printf("NTAZ_Rec(L) = %d\n", NTAZ_Rec(L));
		    //printf("NTAZ_RTSF(L) = %d\n", NTAZ_RTSF(L));
		    //printf("NTAZ_RTSP(L) = %d\n", NTAZ_RTSP(L));
			CHECK(NTAZ_It(L) == 4);
            CHECK(NTAZ_Rec(L) == 4);
            CHECK(NTAZ_RTSF(L) == 4);
            CHECK(NTAZ_RTSP(L) == 4);
		}

        {
		    Liste L = NULL;
		    L = ajoute(0, L);

		    //printf("NTAZ_It(L) = %d\n", NTAZ_It(L));
		    //printf("NTAZ_Rec(L) = %d\n", NTAZ_Rec(L));
		    //printf("NTAZ_RTSF(L) = %d\n", NTAZ_RTSF(L));
		    //printf("NTAZ_RTSP(L) = %d\n", NTAZ_RTSP(L));
		    CHECK(NTAZ_It(L) == 0);
		    CHECK(NTAZ_Rec(L) == 0);
		    CHECK(NTAZ_RTSF(L) == 0);
		    CHECK(NTAZ_RTSP(L) == 0);
        }

        {
		    Liste L = NULL;
		    L = ajoute(1, L);
		    L = ajoute(2, L);
		    L = ajoute(3, L);
		    L = ajoute(4, L);
            L = ajoute(5, L);
		    L = ajoute(6, L);
		    L = ajoute(7, L);

		    //printf("NTAZ_It(L) = %d\n", NTAZ_It(L));
		    //printf("NTAZ_Rec(L) = %d\n", NTAZ_Rec(L));
		    //printf("NTAZ_RTSF(L) = %d\n", NTAZ_RTSF(L));
		    //printf("NTAZ_RTSP(L) = %d\n", NTAZ_RTSP(L));
		    CHECK(NTAZ_It(L) == 7);
		    CHECK(NTAZ_Rec(L) == 7);
		    CHECK(NTAZ_RTSF(L) == 7);
		    CHECK(NTAZ_RTSP(L) == 7);
        }
	}

    if (true) {
        Liste LPos = NULL;
        LPos = ajoute(0, LPos);
		LPos = ajoute(6, LPos);
		LPos = ajoute(0, LPos);
		LPos = ajoute(4, LPos);
		LPos = ajoute(5, LPos);
		LPos = ajoute(9, LPos);
		LPos = ajoute(1, LPos);


        TuePosIt(&LPos);
        affiche_rec(LPos);

        Liste LPos2 = NULL;
        LPos2 = ajoute(0, LPos2);
		LPos2 = ajoute(6, LPos2);
		LPos2 = ajoute(0, LPos2);
		LPos2 = ajoute(4, LPos2);
		LPos2 = ajoute(5, LPos2);
		LPos2 = ajoute(9, LPos2);
		LPos2 = ajoute(1, LPos2);


        TuePosRec(&LPos2);
        affiche_rec(LPos2);

        Liste LPos3 = NULL;
        LPos3 = ajoute(1, LPos3);
		LPos3 = ajoute(6, LPos3);
		LPos3 = ajoute(0, LPos3);
		LPos3 = ajoute(4, LPos3);
		LPos3 = ajoute(5, LPos3);
		LPos3 = ajoute(9, LPos3);
		LPos3 = ajoute(7, LPos3);

        TueRetroPos(&LPos3);
        affiche_rec(LPos3);
    }

    if (true) {
        // Tests ici pour la fonction TueRetroPos
    }

    return 0;
}
