/*
 * Arthur PINGARD --- arthur.pingard@universite-paris-saclay.fr
 * Pierre LEANDRI --- pierre.leandri@universite-paris-saclay.fr
 */

#include <stdio.h>
#include <stdlib.h>

// Type Booleen
typedef enum { false, true } bool;

/*************************************************/
/*                                               */
/*          definition type bloc_image           */
/*                                               */
/*************************************************/

typedef struct bloc_image
{
	bool blanc;
	struct bloc_image *Im[4];
} bloc_image;

typedef bloc_image *image;

/*************************************************/
/*                                               */
/*           Fonctionalités à écrire             */
/*                                               */
/*************************************************/

image Wht() {
    image img = malloc(sizeof(bloc_image));
    img->blanc = true;
    img->Im[0] = NULL;
    img->Im[1] = NULL;
    img->Im[2] = NULL;
    img->Im[3] = NULL;
	return img;
}

image Blk() {
    return NULL;
}

image Cut(image i1, image i2, image i3, image i4) {
	image img = malloc(sizeof(bloc_image));
	img->blanc = false;
	img->Im[0] = i1;
	img->Im[1] = i2;
	img->Im[2] = i3;
	img->Im[3] = i4;
	return img;
}

void AfficheAux(image i) {
	if (i == NULL) {
		printf("Z");
	}
	else if (i->blanc) {
		printf("o");
	}
	else {
		printf("*");
		AfficheAux(i->Im[0]);
		AfficheAux(i->Im[1]);
		AfficheAux(i->Im[2]);
		AfficheAux(i->Im[3]);
	}
}

void Affiche(image i) {
	AfficheAux(i);
	printf("\n"); // Pour un affichage propre à chaque fois.
}

void ProfAfficheAux(image i, int acc) {
	if (i == NULL) {
		printf("Z%d ", acc);
	}
	else if (i->blanc) {
		printf("o%d ", acc);
	}
	else {
		printf("*%d ", acc);
		ProfAfficheAux(i->Im[0], acc + 1);
		ProfAfficheAux(i->Im[1], acc + 1);
		ProfAfficheAux(i->Im[2], acc + 1);
		ProfAfficheAux(i->Im[3], acc + 1);
	}
}

void ProfAffiche(image i) {
	ProfAfficheAux(i, 0);
	printf("\n"); // Pareil ici pour l'affichage propre.
}

image LectureAux(char *str, int *pos) {
    char ch = str[*pos];

    if (ch == '\0') {
        return NULL;
    }

    (*pos)++; // on consomme le caractère.

    if (ch == 'o') {
        return Wht();
    }

    if (ch == 'Z') {
        return Blk();
    }

    if (ch == '*') {
        image img = malloc(sizeof(bloc_image));
        img->blanc = false;
        img->Im[0] = LectureAux(str, pos);
        img->Im[1] = LectureAux(str, pos);
        img->Im[2] = LectureAux(str, pos);
        img->Im[3] = LectureAux(str, pos);
        return img;
    }

    return NULL; // Valeur de retour non définie.
}

image Lecture() {
    image img = NULL;

    //
    // Lire d'abord l'image entrée à la main.
    // On essaie de ne PAS être limité par le nombre de caractères saisis.
    //
    printf("Entrer le quadtree à la main: ");
    const size_t alloc = 16; // On allouera 16 octets de plus à chaque fois pour se donner de la marge.
    size_t nAlloc = alloc;
    size_t len = 0;

    char *str = malloc(nAlloc); // Allocation initiale.
    if (!str) {
        return img;
    }

    int c = 0;
    while ((c = getchar()) != '\n') {
        if (len + 1 >= nAlloc) { // +1 pour '\0' (le byte-terminator).
            nAlloc += alloc;
            char *tmp = realloc(str, nAlloc); // Nouvelle allocation si besoin.
            if (!tmp) {
                free(str);
                return img;
            }
            str = tmp;
        }
        str[len] = (char) c;
        len++;
    }
    str[len] = '\0'; // On n'oublie pas le byte-terminator.
    printf("Vous avez entré: %s\n", str);

    // On lit les caractères pour construire l'image résultante
    // grâce à un appel récursif.
    int pos = 0;
    img = LectureAux(str, &pos);

    free(str); // On n'oublie pas de free l'entrée.
    return img;
}

bool DessinNoir(image img) {
    if (img == NULL)
        return true;

    if (img->blanc)
        return false;

    return DessinNoir(img->Im[0]) &&
           DessinNoir(img->Im[1]) &&
           DessinNoir(img->Im[2]) &&
           DessinNoir(img->Im[3]);
}

bool DessinBlanc(image img) {
    if (img == NULL)
        return false;

    if (img->blanc)
        return true;

    return DessinBlanc(img->Im[0]) &&
           DessinBlanc(img->Im[1]) &&
           DessinBlanc(img->Im[2]) &&
           DessinBlanc(img->Im[3]);
}

void QuotaNoirAux(image img, double* currentQuota, double quotaProfondeur) {
    if (img == NULL) {
        *currentQuota += quotaProfondeur;
        return;
    }

    if (!img->blanc) {
        QuotaNoirAux(img->Im[0], currentQuota, quotaProfondeur / 4);
        QuotaNoirAux(img->Im[1], currentQuota, quotaProfondeur / 4);
        QuotaNoirAux(img->Im[2], currentQuota, quotaProfondeur / 4);
        QuotaNoirAux(img->Im[3], currentQuota, quotaProfondeur / 4);
    }
}

double QuotaNoir(image img) {
    double currentQuota = 0.0;
    QuotaNoirAux(img, &currentQuota, 1.0);
    return currentQuota;
}

image Copie(image img) {
	if (img == NULL)
		return Blk();

	if (img->blanc)
		return Wht();

	return Cut(Copie(img->Im[0]), Copie(img->Im[1]), Copie(img->Im[2]), Copie(img->Im[3]));
}

image DiagonalAux(int p, int currentp) {
	if (currentp == p) return NULL;
	return Cut(DiagonalAux(p, currentp + 1), Wht(), Wht(), DiagonalAux(p, currentp + 1));
}

image Diagonal(int p) { // On suppose p >= 0.
	return DiagonalAux(p, 0);
}

void SimplifieProfPAux(image img, int p, int prof){
    if (img == NULL || img->blanc) return;
    if (prof < p - 1) {
        SimplifieProfPAux(img->Im[0], p, prof+1);
        SimplifieProfPAux(img->Im[1], p, prof+1);
        SimplifieProfPAux(img->Im[2], p, prof+1);
        SimplifieProfPAux(img->Im[3], p, prof+1);
        return;
    }
    else {
        for (int i = 0; i < 4; i++) {

            if (DessinBlanc(img->Im[i])) {
                image t = img->Im[i];
                img->Im[i] = Wht();
                free(t);

            } else if (DessinNoir(img->Im[i])) {
                image t = img->Im[i];
                img->Im[i] = Blk();
                free(t);
            }
        }
        return;
    }
}

void SimplifieProfP(image img, int p){
    return SimplifieProfPAux(img, p, 0);
}




bool Incluse(image img1, image img2){
    if (img1 == NULL) return img2 == NULL;
    if (img1->blanc) return true;
    if (img2 == NULL || img2->blanc) {
        return Incluse(img1->Im[0], img2) 
        && Incluse(img1->Im[1], img2) 
        && Incluse(img1->Im[2], img2) 
        && Incluse(img1->Im[3], img2);
    }
    else {
        return Incluse(img1->Im[0], img2->Im[0]) 
        && Incluse(img1->Im[1], img2->Im[1]) 
        && Incluse(img1->Im[2], img2->Im[2]) 
        && Incluse(img1->Im[3], img2->Im[3]);
    }
}

int CompteSousImagesGrises(image img){
    if (img == NULL || img->blanc) return 0;
    float TauxNoir = QuotaNoir(img);
    if (TauxNoir >= 1/3 && 2/3 >= TauxNoir) {
        return 1 + CompteSousImagesGrises(img->Im[0]) 
                 + CompteSousImagesGrises(img->Im[1]) 
                 + CompteSousImagesGrises(img->Im[2]) 
                 + CompteSousImagesGrises(img->Im[3]);
    }
    else {
        return CompteSousImagesGrises(img->Im[0]) 
                 + CompteSousImagesGrises(img->Im[1]) 
                 + CompteSousImagesGrises(img->Im[2]) 
                 + CompteSousImagesGrises(img->Im[3]);
    }
}


/*************************************************/
/*                                               */
/*           Main                                */
/*                                               */
/*************************************************/

int main(void) {
	// QUESTION 2 & 3
	if (true) {
		bloc_image main;
		main.blanc = false;

		bloc_image b1;
		b1.blanc = true;
		bloc_image b2;
		b2.blanc = true;
		bloc_image b3;
		b3.blanc = true;
		bloc_image b4;
		b4.blanc = true;

		main.Im[0] = &b1;
		main.Im[1] = &b2;
		main.Im[2] = &b3;
		main.Im[3] = &b4;

		image i = &main;
		Affiche(i);
		ProfAffiche(i);
	}

    // QUESTION 4, 5, 6
    if (true) {
        image img = Lecture();
        Affiche(img); // EXEMPLE avec **ZooZoZoo, **oooZ*ooZo*oZoo*Zooo
        printf("Le dessin est-il noir?  %s\n", DessinNoir(img) == true ? "OUI" : "NON");
        printf("Le dessin est-il blanc? %s\n", DessinBlanc(img) == true ? "OUI" : "NON");
        printf("Le quota de noir est: %f\n", QuotaNoir(img));
    }

	// Question 8
	if (true) {
		image img = Diagonal(3); // Comme dans l'énoncé.
		Affiche(img);
	}

    return 0;
}