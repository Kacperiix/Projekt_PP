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

void usunBohatera() {
    // 1. Sprawdzenie, czy lista jest pusta
    if (glowa == NULL) {
        printf("Rejestr jest pusty, nikogo nie mozna usunac.\n");
        return;
    }

    // 2. Pobranie imienia do usunięcia
    char szukaneImie[101];
    printf("Podaj imie bohatera do usuniecia: ");
    scanf("%100s", szukaneImie);
    wyczyscBufor();

    // 3. Specjalny przypadek: Usuwanie PIERWSZEGO elementu
    // strcmp(a, b) == 0 oznacza, że napisy są identyczne
    if (strcmp(glowa->dane.imie, szukaneImie) == 0) {
        
        // Sprawdzenie statusu czy jest na misji
        if (glowa->dane.status == NA_MISJI) {
            printf("BLAD: Nie mozna usunac bohatera '%s' - jest NA MISJI!\n", szukaneImie);
            return;
        }

        // Zapamiętujemy adres starej głowy, żeby ją skasować
        ElementListy* doUsuniecia = glowa;
        // Nową głową zostaje drugi element
        glowa = glowa->nastepny;
        
        free(doUsuniecia); // Zwolnienie pamięci RAM
        printf("Bohater usuniety pomyslnie.\n");
        return;
    }

    // 4. Przypadek ogólny: Szukanie w reszcie listy
    // Potrzebujemy dwóch wskaźników: "obecny" i "poprzedni"
    ElementListy* obecny = glowa->nastepny;
    ElementListy* poprzedni = glowa;

    while (obecny != NULL) {
        if (strcmp(obecny->dane.imie, szukaneImie) == 0) {
            // Sprawdzamy status
            if (obecny->dane.status == NA_MISJI) {
                printf("BLAD: Nie mozna usunac bohatera '%s' - jest NA MISJI!\n", szukaneImie);
                return;
            }

            // Przepinamy wskaźniki (omijamy usuwany element)
            poprzedni->nastepny = obecny->nastepny;
            
            free(obecny); // Usuwamy element z pamięci
            printf("Bohater usuniety pomyslnie.\n");
            return;
        }

        poprzedni = obecny;
        obecny = obecny->nastepny;
    }

    // 5. Jeśli pętla się skończyła i nic nie znaleźliśmy
    printf("Nie znaleziono bohatera o imieniu: %s\n", szukaneImie);
}

// --- OBSŁUGA PLIKÓW ---

void zapiszDoPliku(const char* nazwaPliku) {
    // 1. Otwarcie pliku w trybie write
    // Ten tryb kasuje starą zawartość pliku i wpisuje nową
    FILE* plik = fopen(nazwaPliku, "w");
    
    if (plik == NULL) {
        printf("BLAD: Nie udalo sie otworzyc pliku do zapisu!\n");
        return;
    }

    // 2. Przejście przez całą listę
    ElementListy* temp = glowa;
    while (temp != NULL) {
        // fprintf wysyła tekst do pliku
        fprintf(plik, "%s %s %s %d %d %d\n",
            temp->dane.imie,
            temp->dane.rasa,
            temp->dane.klasa,
            temp->dane.poziom,
            temp->dane.reputacja,
            (int)temp->dane.status
        );
        
        temp = temp->nastepny;
    }

    // 3. Zamknięcie pliku
    fclose(plik);
    printf("Dane zostaly zapisane do pliku '%s'.\n", nazwaPliku);
}

void wczytajZPliku(const char* nazwaPliku) {
    // 1. Otwarcie pliku w trybie read
    FILE* plik = fopen(nazwaPliku, "r");
    
    if (plik == NULL) {
        printf("Plik '%s' nie istnieje. Zostanie utworzony przy zapisie.\n", nazwaPliku);
        return;
    }

    printf("Wczytywanie danych...\n");

    // 2. Pętla nieskończona
    while (1) {
        // Rezerwujemy pamięć na nowego bohatera
        ElementListy* nowy = (ElementListy*)malloc(sizeof(ElementListy));
        
        if (nowy == NULL) break; 

        int statusInt; // Zmienna pomocnicza do wczytania statusu

        // fscanf próbuje pobrać 6 wartości z pliku
        // Zwraca liczbę poprawnie wczytanych elementów
        int wynik = fscanf(plik, "%s %s %s %d %d %d",
            nowy->dane.imie,
            nowy->dane.rasa,
            nowy->dane.klasa,
            &nowy->dane.poziom,
            &nowy->dane.reputacja,
            &statusInt
        );

        // 3. Sprawdzenie czy to koniec pliku
        if (wynik != 6) {
            // Jeśli fscanf nie znalazł 6 elementów to znaczy że plik się skończył
            free(nowy);
            break;     
        }

        // Przypisanie wczytanych danych
        nowy->dane.status = (StatusBohatera)statusInt;
        nowy->nastepny = NULL;

        // 4. Dodanie wczytanego bohatera na koniec listy
        if (glowa == NULL) {
            glowa = nowy;
        } else {
            ElementListy* temp = glowa;
            while (temp->nastepny != NULL) {
                temp = temp->nastepny;
            }
            temp->nastepny = nowy;
        }
    }

    fclose(plik);
    printf("Wczytano dane z pliku '%s'.\n", nazwaPliku);
}