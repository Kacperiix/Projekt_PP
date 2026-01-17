#include <stdio.h>
#include "hero.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("BLAD: Nie podano nazwy pliku z baza danych!\n");
        printf("Uzycie: %s <nazwa_pliku.txt>\n", argv[0]);
        return 1;
    }

    char* nazwaPliku = argv[1];
    printf("Uruchomiono system Gildii. Plik danych: %s\n", nazwaPliku);

    int wybor;
    do {
        printf("\n=== REJESTR BOHATEROW GILDII ===\n");
        printf("1. Dodaj bohatera\n");
        printf("2. Wyswietl wszystkich\n");
        printf("3. Usun bohatera\n");
        printf("4. Wyjscie\n");
        printf("Wybierz opcje: ");
        
        if (scanf("%d", &wybor) != 1) {
            printf("To nie jest liczba!\n");
            while(getchar() != '\n'); 
            wybor = 0; 
        }
        
        while(getchar() != '\n');

        switch (wybor) {
            case 1:
                dodajBohatera(); 
                break;
            case 2:
                wyswietlListe();
                break;
            case 3:
                usunBohatera();
                break;
            case 4:
                printf("Konczenie pracy...\n");
                break;
            default:
                printf("Nieznana opcja. Sprobuj ponownie.\n");
        }
    } while (wybor != 4);

    return 0;
}