#include <stdio.h>
#include <stdlib.h>

int main(int argumenttien_maara, char *argumentit[]) {
    if (argumenttien_maara < 2) {
        printf("my-unzip: file1 [file2 ...]\n");
        return 1;
    }

    /* Puretaan kaikki komentorivillä annetut pakatut tiedostot järjestyksessä. */
    for (int indeksi = 1; indeksi < argumenttien_maara; indeksi++) {
        FILE *tiedosto = fopen(argumentit[indeksi], "r");

        if (tiedosto == NULL) {
            return 1;
        }

        int maara;
        char merkki;

        /*
         * Jokainen pakattu jakso koostuu 4 tavun kokonaisluvusta
         * ja sitä seuraavasta yhdestä merkistä.
         */
        while (fread(&maara, sizeof(int), 1, tiedosto) == 1) {
            if (fread(&merkki, sizeof(char), 1, tiedosto) != 1) {
                break;
            }

            for (int laskuri = 0; laskuri < maara; laskuri++) {
                printf("%c", merkki);
            }
        }

        fclose(tiedosto);
    }

    return 0;
}

