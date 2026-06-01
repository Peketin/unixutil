#include <stdio.h>
#include <stdlib.h>

/*
 * Kirjoittaa yhden pakatun jakson standard outputiin.
 * Formaatti on tehtävänannon mukainen:
 * 4 tavun kokonaisluku + 1 tavun merkki.
 */
static void kirjoita_jakso(int maara, char merkki) {
    fwrite(&maara, sizeof(int), 1, stdout);
    fwrite(&merkki, sizeof(char), 1, stdout);
}

int main(int argumenttien_maara, char *argumentit[]) {
    if (argumenttien_maara < 2) {
        printf("my-zip: file1 [file2 ...]\n");
        return 1;
    }

    int maara = 0;
    int edellinen_merkki = 0;
    int onko_edellinen = 0;

    /*
     * Jos tiedostoja on useita, pakkaus jatkuu tiedostorajojen yli.
     * Siksi laskuria ei nollata jokaisen tiedoston alussa.
     */
    for (int indeksi = 1; indeksi < argumenttien_maara; indeksi++) {
        FILE *tiedosto = fopen(argumentit[indeksi], "r");

        if (tiedosto == NULL) {
            return 1;
        }

        int nykyinen_merkki;

        while ((nykyinen_merkki = fgetc(tiedosto)) != EOF) {
            if (!onko_edellinen) {
                edellinen_merkki = nykyinen_merkki;
                maara = 1;
                onko_edellinen = 1;
            } else if (nykyinen_merkki == edellinen_merkki) {
                maara++;
            } else {
                kirjoita_jakso(maara, (char)edellinen_merkki);
                edellinen_merkki = nykyinen_merkki;
                maara = 1;
            }
        }

        fclose(tiedosto);
    }

    /* Kirjoitetaan viimeinen jakso, jos tiedostoissa oli sisältöä. */
    if (onko_edellinen) {
        kirjoita_jakso(maara, (char)edellinen_merkki);
    }

    return 0;
}

