Unix Utilities

Tassa osassa toteutetaan nelja yksinkertaista UNIX tyylista komentorivityokalua C kielella. Ohjelmat ovat my cat, my grep, my zip ja my unzip. Jokainen ohjelma on omassa C lahdekooditiedostossaan.

Tarkoitus

Tehtavan tarkoituksena on harjoitella C ohjelmointia, komentorivin kayttoa, tiedostojen lukemista ja kirjoittamista seka virhetilanteiden kasittelya. Ohjelmat ovat yksinkertaistettuja versioita tavallisista UNIX tyokaluista.

Kaikki ohjelmat kaannetaan gcc kaantajalla asetuksilla Wall ja Werror. Nailla asetuksilla ohjelmien taytyy kaantya ilman varoituksia.

my cat:

my cat lukee komentorivilla annetut tiedostot ja tulostaa niiden sisallon standard outputiin. Jos tiedostoja annetaan useita, ne tulostetaan samassa jarjestyksessa kuin ne annetaan komentorivilla.

Jos tiedostoja ei anneta, ohjelma ei tulosta mitaan ja palauttaa arvon 0. Jos tiedostoa ei voida avata, ohjelma tulostaa tehtavanannon mukaisen virheilmoituksen ja palauttaa arvon 1.

my grep:

my grep etsii hakusanaa annetusta tiedostosta tai standard inputista. Ohjelma kay lapi syotteen rivi kerrallaan. Jos rivi sisaltaa hakusanan, koko rivi tulostetaan.

Haku on kirjainkoon huomioiva. Tama tarkoittaa, etta esimerkiksi foo ja Foo ovat eri merkkijonoja.

Ohjelmassa kaytetaan getline funktiota, jotta myos hyvin pitkat rivit voidaan kasitella oikein. Jos kayttaja ei anna hakusanaa, ohjelma tulostaa kayttoohjeen ja palauttaa arvon 1.

my zip:

my zip pakkaa tiedoston kayttaen run length encoding menetelmaa. Siina perakkaiset samat merkit tallennetaan yhtena jaksona. Jokainen pakattu jakso sisaltaa neljan tavun kokonaisluvun ja yhden merkin.

Ohjelma kirjoittaa pakatun datan standard outputiin binarimuodossa. Tulos voidaan ohjata tiedostoon komentorivin uudelleenohjauksella.

Jos my zip saa useita tiedostoja, se kasittelee ne yhtena jatkuvana syotevirtana. Tiedostojen valiset rajat eivat saily pakatussa datassa.

my unzip:

my unzip purkaa my zip ohjelman tuottamaa pakattua dataa. Se lukee syotteesta toistuvasti neljan tavun kokonaisluvun ja yhden merkin. Kokonaisluku kertoo, kuinka monta kertaa merkki tulostetaan.

my unzip ei pura oikeita zip arkistoja. Se toimii vain taman tehtavan oman RLE formaatin kanssa.

Testaus:

Ohjelmien toiminta voidaan tarkistaa luomalla testitiedosto, pakkaamalla se, purkamalla se ja vertaamalla purettua tiedostoa alkuperaiseen.

Jos alkuperainen tiedosto ja purettu tiedosto ovat samat, ohjelmat toimivat oikein.

Tarkea huomio:

Virheilmoitusten taytyy olla tasmalleen tehtavanannon mukaiset, koska automaattiset testit voivat vertailla ohjelman tulostetta merkkijonona. Siksi virheilmoituksia ei ole muutettu suomen kielelle, vaikka muuttujien nimet ja kommentit ovat suomeksi.

Dokumentointi:

Lahdekoodissa on kommentteja, jotka selittavat ohjelmien keskeiset vaiheet. Kommentit kertovat esimerkiksi miten tiedosto avataan, miten riveja luetaan, miten hakusanaa etsitaan, miten RLE pakkaus toimii ja miten pakattu data puretaan.

Yhteenveto:

Unix Utilities osuudessa on toteutettu kaikki vaaditut perustoiminnot. Ohjelmat kaantyvat, kasittelevat tiedostoja komentorivilta ja palauttavat oikeat paluuarvot onnistumisessa ja virhetilanteissa.
