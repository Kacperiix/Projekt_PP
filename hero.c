#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hero.h"

// Wskaźnik na pierwszy element listy
// Na starcie jest NULL, bo lista jest pusta.
ElementListy* glowa = NULL;

// Tablica pomocnicza do wyświetlania nazw statusów zamiast cyferek 0-4
const char* opisyStatusow[] = {
    "Aktywny", "Na misji", "Ranny", "Zaginiony", "Zawieszony"
};

// Funkcja czyszcząca "śmieci" z wejścia klawiatury
void wyczyscBufor() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void dodajBohatera() {
    // 1. Alokacja pamięci (tworzymy nowy element na stercie)
    ElementListy* nowy = (ElementListy*)malloc(sizeof(ElementListy));
    
    // Sprawdzenie, czy komputer ma pamięć
    if (nowy == NULL) {
        printf("BLAD: Brak pamieci RAM!\n");
        return;
    }

    // 2. Pobieranie danych od użytkownika
    printf("\n--- DODAWANIE BOHATERA ---\n");
    
    printf("Podaj imie (max 100 znakow): ");
    scanf("%100s", nowy->dane.imie);
    wyczyscBufor(); // Usunięcie Entera po wpisaniu tekstu

    printf("Podaj rase: ");
    scanf("%49s", nowy->dane.rasa);
    wyczyscBufor();

    printf("Podaj klase: ");
    scanf("%49s", nowy->dane.klasa);
    wyczyscBufor();

    printf("Podaj poziom (liczba): ");
    scanf("%d", &nowy->dane.poziom);
    wyczyscBufor();

    printf("Podaj reputacje (0-100): ");
    scanf("%d", &nowy->dane.reputacja);
    wyczyscBufor();

    printf("Status (0-Aktywny, 1-Misja, 2-Ranny, 3-Zaginiony, 4-Zawieszony): ");
    int wyborStatusu;
    scanf("%d", &wyborStatusu);
    wyczyscBufor();

    // Zabezpieczenie, żeby nie wpisać statusu spoza zakresu
    if (wyborStatusu < 0 || wyborStatusu > 4) {
        wyborStatusu = 0; 
    }
    // Rzutowanie liczby na nasz typ enum
    nowy->dane.status = (StatusBohatera)wyborStatusu;

    // Nowy element będzie ostatni, więc nie ma następnika
    nowy->nastepny = NULL;

    // 3. Dodanie do listy
    if (glowa == NULL) {
        // Jeśli lista była pusta, nowy element staje się pierwszym
        glowa = nowy;
    } else {
        // Jeśli lista nie jest pusta, szukamy końca
        ElementListy* temp = glowa;
        while (temp->nastepny != NULL) {
            temp = temp->nastepny;
        }
        temp->nastepny = nowy;
    }

    printf("SUKCES: Bohater %s zostal dodany do Rejestru!\n", nowy->dane.imie);
}

void wyswietlListe() {
    if (glowa == NULL) {
        printf("\nRejestr jest pusty.\n");
        return;
    }

    printf("\n%-20s %-15s %-15s %-5s %-15s\n", "Imie", "Rasa", "Klasa", "Poziom", "Status");
    printf("---------------------------------------------------------------------------\n");

    ElementListy* temp = glowa;
    while (temp != NULL) {
        printf("%-20s %-15s %-15s %-5d %-15s\n",
            temp->dane.imie,
            temp->dane.rasa,
            temp->dane.klasa,
            temp->dane.poziom,
            opisyStatusow[temp->dane.status] 
        );
        
        temp = temp->nastepny;
    }
    printf("\n");
}