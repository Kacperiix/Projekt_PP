#ifndef HERO_H
#define HERO_H

typedef enum {
    AKTYWNY,
    NA_MISJI,    
    RANNY,
    ZAGINIONY,
    ZAWIESZONY
} StatusBohatera;

typedef struct {
    char imie[101];     
    char rasa[50];
    char klasa[50];
    int poziom;          
    int reputacja;    
    StatusBohatera status;
} Bohater;

typedef struct ElementListy {
    Bohater dane;                
    struct ElementListy* nastepny; 
} ElementListy;

void dodajBohatera();
void wyswietlListe();
void usunBohatera();
void zapiszDoPliku(const char* nazwaPliku);
void wczytajZPliku(const char* nazwaPliku);
void sortujPoImieniu();
void sortujPoPoziomie();

#endif