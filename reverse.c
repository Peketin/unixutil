#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

/*
 * Tätä funktiota kutsutaan, jos muistin varaaminen epäonnistuu.
 */
static void muisti_virhe(void) {
    fprintf(stderr, "malloc failed\n");
    exit(1);
}

/*
 * Tätä funktiota kutsutaan, jos tiedoston avaaminen epäonnistuu.
 */
static void tiedosto_virhe(const char *tiedostonimi) {
    fprintf(stderr, "error: cannot open file '%s'\n", tiedostonimi);
    exit(1);
}

/*
 * Tarkistaa, viittaavatko kaksi polkua samaan tiedostoon.
 *
 * Tämä on tärkeää, koska tulostiedoston avaaminen moodissa "w"
 * tyhjentää tiedoston. Jos syöte ja tuloste ovat sama tiedosto,
 * alkuperäinen sisältö voisi kadota ennen kuin se ehditään lukea.
 */
static int sama_tiedosto(const char *polku1, const char *polku2) {
    struct stat tila1;
    struct stat tila2;

    /*
     * Yksinkertainen tapaus:
     * ./reverse input.txt input.txt
     */
    if (strcmp(polku1, polku2) == 0) {
        return 1;
    }

    /*
     * Monimutkaisempi tapaus:
     * ./reverse input.txt ./input.txt
     *
     * stat() antaa tietoa varsinaisesta tiedostosta.
     * Jos st_dev ja st_ino ovat samat, polut viittaavat
     * samaan tiedostoon.
     */
    if (stat(polku1, &tila1) == 0 && stat(polku2, &tila2) == 0) {
        return tila1.st_dev == tila2.st_dev &&
               tila1.st_ino == tila2.st_ino;
    }

    /*
     * Jos emme pysty todistamaan, että tiedostot ovat samat,
     * käsittelemme ne eri tiedostoina.
     */
    return 0;
}

int main(int argc, char *argv[]) {
    /*
     * Oletuksena:
     * - luetaan standardisyötteestä eli stdinistä
     * - kirjoitetaan standarditulosteeseen eli stdoutiin
     *
     * Tämä hoitaa tapauksen:
     * ./reverse
     */
    FILE *syote = stdin;
    FILE *tuloste = stdout;

    /*
     * rivit on dynaaminen merkkijonotaulukko.
     *
     * Jokainen alkio osoittaa yhteen syötteestä luettuun riviin.
     *
     * Esimerkki:
     * rivit[0] -> "hello\n"
     * rivit[1] -> "this\n"
     * rivit[2] -> "is\n"
     */
    char **rivit = NULL;

    /*
     * rivien_maara kertoo, kuinka monta riviä on tallennettu.
     * kapasiteetti kertoo, kuinka monelle riville taulukossa
     * on tällä hetkellä tilaa.
     */
    size_t rivien_maara = 0;
    size_t kapasiteetti = 0;

    /*
     * Sallitut tavat ajaa ohjelma:
     *
     * ./reverse
     * ./reverse input.txt
     * ./reverse input.txt output.txt
     *
     * Joten argc voi olla 1, 2 tai 3.
     */
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    /*
     * Jos sekä syöte- että tulostiedosto annetaan,
     * ne eivät saa olla sama tiedosto.
     */
    if (argc == 3) {
        if (sama_tiedosto(argv[1], argv[2])) {
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }
    }

    /*
     * Jos käyttäjä antoi syötetiedoston, avataan se lukemista varten.
     *
     * argv[1] on syötetiedoston nimi.
     */
    if (argc >= 2) {
        syote = fopen(argv[1], "r");

        if (syote == NULL) {
            tiedosto_virhe(argv[1]);
        }
    }

    /*
     * Jos käyttäjä antoi tulostiedoston, avataan se kirjoittamista varten.
     *
     * argv[2] on tulostiedoston nimi.
     *
     */
    if (argc == 3) {
        tuloste = fopen(argv[2], "w");

        if (tuloste == NULL) {
            tiedosto_virhe(argv[2]);
        }
    }

    /*
     * Luetaan syöte yksi rivi kerrallaan.
     *
     * getline() on hyödyllinen, koska se pystyy lukemaan minkä tahansa
     * pituisia rivejä. Se varaa muistia riville automaattisesti.
     */
    while (1) {
        char *rivi = NULL;
        size_t rivin_koko = 0;
        ssize_t luettu_maara;

        /*
         * Nollataan errno ennen getline()-kutsua.
         * Jos getline() epäonnistuu muistin takia,
         * errno voi saada arvon ENOMEM.
         */
        errno = 0;

        /*
         * getline() lukee yhden rivin syötteestä.
         *
         * Onnistuessaan:
         * - rivi osoittaa luettuun riviin
         * - luettu_maara sisältää luettujen merkkien määrän
         *
         * Tiedoston lopussa tai virheessä:
         * - getline() palauttaa -1
         */
        luettu_maara = getline(&rivi, &rivin_koko, syote);

        /*
         * Lopetetaan lukeminen, jos getline() saavutti tiedoston lopun
         * tai jos se epäonnistui.
         */
        if (luettu_maara == -1) {
            /*
             * free(NULL) on turvallinen, joten tämä toimii myös silloin,
             * jos getline() ei varannut muistia.
             */
            free(rivi);

            /*
             * Jos getline() epäonnistui muistin loppumisen takia,
             * tulostetaan vaadittu malloc-virhe.
             */
            if (errno == ENOMEM) {
                muisti_virhe();
            }

            break;
        }

        /*
         * Jos taulukko on täynnä, kasvatetaan sitä.
         */
        if (rivien_maara == kapasiteetti) {
            size_t uusi_kapasiteetti;

            /*
             * Ensimmäinen varaus antaa tilaa 16 riville.
             * Sen jälkeen kapasiteetti kaksinkertaistetaan aina tarvittaessa.
             */
            if (kapasiteetti == 0) {
                uusi_kapasiteetti = 16;
            } else {
                uusi_kapasiteetti = kapasiteetti * 2;
            }

            /*
             * Muutetaan dynaamisen taulukon kokoa.
             *
             * Tarvitsemme tilaa uusi_kapasiteetti kappaleelle char*-osoittimia.
             */
            char **uudet_rivit = realloc(
                rivit,
                uusi_kapasiteetti * sizeof(char *)
            );

            /*
             * Jos realloc epäonnistuu, vapautetaan juuri luettu rivi
             * ja lopetetaan ohjelma virheeseen.
             */
            if (uudet_rivit == NULL) {
                free(rivi);
                muisti_virhe();
            }

            /*
             * Tallennetaan uusi taulukon osoitin ja uusi kapasiteetti.
             */
            rivit = uudet_rivit;
            kapasiteetti = uusi_kapasiteetti;
        }

        /*
         * Tallennetaan luettu rivi taulukkoon.
         */
        rivit[rivien_maara] = rivi;
        rivien_maara++;
    }

    /*
     * Tulostetaan rivit käänteisessä järjestyksessä.
     *
     * Jos rivien_maara on 4, tämä tulostaa:
     * rivit[3]
     * rivit[2]
     * rivit[1]
     * rivit[0]
     */
    for (size_t i = rivien_maara; i > 0; i--) {
        fprintf(tuloste, "%s", rivit[i - 1]);
    }

    /*
     * Vapautetaan jokainen getline()-funktion varaama rivi.
     */
    for (size_t i = 0; i < rivien_maara; i++) {
        free(rivit[i]);
    }

    /*
     * Vapautetaan itse dynaaminen taulukko.
     */
    free(rivit);

    /*
     * Suljetaan syötetiedosto, jos avasimme sellaisen.
     * stdin:iä ei tarvitse sulkea.
     */
    if (syote != stdin) {
        fclose(syote);
    }

    /*
     * Suljetaan tulostiedosto, jos avasimme sellaisen.
     * stdout:ia ei tarvitse sulkea.
     */
    if (tuloste != stdout) {
        fclose(tuloste);
    }

    /*
     * return 0 tarkoittaa, että ohjelma onnistui.
     */
    return 0;
}
