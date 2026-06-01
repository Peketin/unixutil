#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/*
 * Lukee annetusta virrasta rivejä ja tulostaa ne rivit,
 * joissa hakusana esiintyy. Haku on kirjainkoon huomioiva.
 */
static void hae_virrasta(FILE *virta, const char *hakusana) {
    char *rivi = NULL;
    size_t rivin_koko = 0;

    /*
     * getline() osaa käsitellä myös erittäin pitkät rivit,
     * koska se kasvattaa muistia tarvittaessa automaattisesti.
     */
    while (getline(&rivi, &rivin_koko, virta) != -1) {
        if (strstr(rivi, hakusana) != NULL) {
            printf("%s", rivi);
        }
    }

    free(rivi);
}

int main(int argumenttien_maara, char *argumentit[]) {
    /* Jos hakusanaa ei anneta, tulostetaan tehtävänannon mukainen ohje. */
    if (argumenttien_maara < 2) {
        printf("my-grep: searchterm [file ...]\n");
        return 1;
    }

    const char *hakusana = argumentit[1];

    /*
     * Jos tiedostoja ei anneta, luetaan syöte standard inputista.
     * Tämä mahdollistaa käytön esimerkiksi putkessa:
     * echo "teksti" | ./my-grep teksti
     */
    if (argumenttien_maara == 2) {
        hae_virrasta(stdin, hakusana);
        return 0;
    }

    /* Muuten käydään kaikki annetut tiedostot läpi. */
    for (int indeksi = 2; indeksi < argumenttien_maara; indeksi++) {
        FILE *tiedosto = fopen(argumentit[indeksi], "r");

        if (tiedosto == NULL) {
            printf("my-grep: cannot open file\n");
            return 1;
        }

        hae_virrasta(tiedosto, hakusana);
        fclose(tiedosto);
    }

    return 0;
}

