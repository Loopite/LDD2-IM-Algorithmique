/*
 * Arthur PINGARD --- arthur.pingard@universite-paris-saclay.fr
 * Pierre LEANDRI --- pierre.leandri@universite-paris-saclay.fr
 */

#include <stdlib.h>
#include <stdio.h>

typedef struct Bloc {
    int valeur;
    struct Bloc* suite;
} Bloc;

typedef Bloc** File;

void entree(int x, File* file)
{
    if (file == NULL) {
        printf("ATTENTION: Le pointeur vers la file est nul (dans l'appel entree).");
        return;
    }

    // Création du nouvel élément.
    Bloc *tmp = (Bloc*) malloc(sizeof(Bloc));
    tmp->valeur = x;

    if (*file == NULL) // En cas de liste vide.
    {
        tmp->suite = tmp; // Comme on a une liste vide,
                          // le seul élément pointe vers lui même.
                          // pour la propriété de liste circulaire.
    }
    else // La liste n'est pas vide.
    {
        tmp->suite = (*(*file)); // On conserve la propriété de liste circulaire.
        *(*file) = tmp; // On met à jour le dernier élément dans notre file.
    }

    *file = &(tmp->suite); // On modifie l'élément en tête.
}

void sortie(int* x, File* file)
{
    if (file == NULL) {
        printf("ATTENTION: Le pointeur vers la file est nul (dans l'appel sortie).");
        return;
    }

    if (*file == NULL) { // La liste est vide.
        return;
    }

    *x = (*(*file))->valeur; // On récupère la valeur dans le bloc qui va être retiré.

    Bloc *item = *(*file); // L'élément à enlever.
    Bloc *next = item->suite; // On récupère le pointeur vers le second élément.
    free(item); // On dégage de la mémoire l'élément qui a été inséré en tout premier.

    if (item == next) {
        // On est dans le cas ou il y a un seul et unique élément.
        // Donc, il faut mettre la file à nulle.
        *file = NULL;
    }
    else {
        *(*file) = next; // On met à jour notre file.
    }
}

void affiche_iter(File* file) {
    if (file == NULL) {
        printf("ATTENTION: Le pointeur vers la file est nul (dans l'appel affiche_iter).");
        return;
    }

    if (*file == NULL) { // La liste est vide.
        printf("La liste est vide.\n");
        return;
    }

    printf("Les éléments de la liste sont: ");

    Bloc *debut = (*(*file));
    Bloc *current = (*(*file));
    do {
        printf("%d ", current->valeur);
        current = current->suite;
    }
    while(current != debut);
    printf("\n");
}

/*************************************************/
/*                                               */
/*           Main                                */
/*                                               */
/*************************************************/

int main(void) {
    { // Cas de liste vide.
        printf("******************************\n");
        File file;
        file = NULL;
        affiche_iter(&file);
        int s = 0;
        sortie(&s, &file);
        printf("L'élément sorti est: %d\n", s);
        affiche_iter(&file);
        printf("******************************\n");
    }

    {
        printf("******************************\n");
        File file;
        file = NULL;
        entree(23, &file);
        entree(17, &file);
        entree(93, &file);
        entree(777, &file);
        affiche_iter(&file);
        int s = 0;
        sortie(&s, &file);
        printf("L'élément sorti est: %d\n", s);
        affiche_iter(&file);
        sortie(&s, &file);
        printf("L'élément sorti est: %d\n", s);
        affiche_iter(&file);
        sortie(&s, &file);
        printf("L'élément sorti est: %d\n", s);
        affiche_iter(&file);
        entree(999, &file);
        affiche_iter(&file);
        printf("******************************\n");
    }

    {
        printf("******************************\n");
        File file;
        file = NULL;
        entree(1, &file);
        affiche_iter(&file);
        int s = 0;
        sortie(&s, &file);
        printf("L'élément sorti est: %d\n", s);
        affiche_iter(&file);
        entree(2, &file);
        affiche_iter(&file);
        entree(3, &file);
        affiche_iter(&file);
        sortie(&s, &file);
        printf("L'élément sorti est: %d\n", s);
        affiche_iter(&file);
        sortie(&s, &file);
        printf("L'élément sorti est: %d\n", s);
        affiche_iter(&file);
        printf("******************************\n");
    }

    return 0;
}
