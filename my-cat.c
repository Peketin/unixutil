#include <stdio.h>
#include <stdlib.h>

int main(int argumenttien_maara, char *argumentit[]) {
    char puskuri[4096];

    /* Jos tiedostoja ei anneta, ohjelma lopettaa onnistuneesti. */
    if (argumenttien_maara == 1) {
        return 0;
    }

    /* Käydään kaikki komentorivillä annetut tiedostot läpi järjestyksessä. */
    for (int indeksi = 1; indeksi < argumenttien_maara; indeksi++) {
        FILE *tiedosto = fopen(argumentit[indeksi], "r");

        /* Tehtävänannon vaatima virheilmoitus täsmälleen tässä muodossa. */
        if (tiedosto == NULL) {
            printf("my-cat: cannot open file\n");
            return 1;
        }

        /* Luetaan tiedostoa rivi kerrallaan ja tulostetaan sisältö sellaisenaan. */
        while (fgets(puskuri, sizeof(puskuri), tiedosto) != NULL) {
            printf("%s", puskuri);
        }

        fclose(tiedosto);
    }

    return 0;
}

