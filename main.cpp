#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <fstream>

using namespace std;

typedef struct structData
{
    int dzien;
    int miesiac;
    int rok;

} Data;

typedef struct structOsoba
{
    // dane osoby
    long int Id;
    char nazwisko[15];
    char imie [15];
    Data data;
    char adres [15];
    char miasto [15];

    // wskazanie na elementy
    structOsoba *nastepny;
    structOsoba *poprzedni;
    // sortowanie po nazwisku
    structOsoba *sortnastepny;
    structOsoba *sortpoprzedni;
} Osoba;

Osoba *poczatek = NULL;
Osoba *koniec = NULL;
Osoba *sortpoczatek = NULL;
Osoba *sortkoniec = NULL;

void szukajOsobe();
void wstrzymaj();
void dodajNaKoniecListy(Osoba * osoba);
void dodajOsobe();
void wyswietlListePoId();
void wyswietlListeWgNazwisk();
void wyswietlListePosortowana();
int porownaj(const void * a, const void *b);
char * strupper(char *s);
void zapiszDoPliku(const char * nazwaPliku);
void odczytajZPliku(const char * nazwaPliku);
void wyswietlOsoba(Osoba * dane);
long int dajId();
long int policzElementy();
void liczbaElementow();
void dodajDoListyPosortowanej(Osoba *osoba);
void wyszukajIwyswietl();
void wyszukajIusun();
void usunOsoba(Osoba *osoba);
Osoba * szukajOsobe(char nazwisko [15]);
void znajdzImodyfikuj();
Osoba * wyszukajPoId (int id);
Osoba * modyfikujOsobe(Osoba *osoba);
void podajIzapisz();
void podajIodczytaj();

int main()
{
    int opcja = 0;
    while(1)
    {
        printf("____________________________\n");
        printf("Menu:\n");
        printf(" 1. Dodaj nowa osobe\n");
        printf(" 2. Usun osobe\n");
        printf(" 3. Wyszukaj osobe\n");
        printf(" 4. Wyswietl baze osob wg id\n");
        printf(" 5. Wyswietl baze osob wg nazwisk (tablica)\n");
        printf(" 6. Wyswietl baze osob wg nazwisk (wskazniki)\n");
        printf(" 7. Modyfikuj osobe po Id\n");
        printf(" 8. Zapisz do pliku\n");
        printf(" 9. Odczytaj z pliku\n");
        printf("10. Liczba elementow\n");
        printf(" 0. Koniec pracy\n");
        printf("____________________________\n");
        printf("\nWybierz polecenie: ");
        scanf("%d", &opcja);
        printf("\n");
        if(opcja==0)
            break;
        switch(opcja)
        {
        case 1:
            dodajOsobe();
            break;
        case 2:
            wyszukajIusun();
            wstrzymaj();
            break;
        case 3:
            wyszukajIwyswietl();
            wstrzymaj();
            break;
        case 4:
            wyswietlListePoId();
            wstrzymaj();
            break;
        case 5:
            wyswietlListeWgNazwisk();
            wstrzymaj();
            break;
        case 6:
            wyswietlListePosortowana();
            wstrzymaj();
            break;
        case 7:
            znajdzImodyfikuj();
            wstrzymaj();
            break;
        case 8:
            podajIzapisz();
            wstrzymaj();
            break;
        case 9:
            podajIodczytaj();
            wstrzymaj();
            break;
        case 10:
            liczbaElementow();
            wstrzymaj();
            break;
        default:
            printf("\n\nNie ma takiej opcji!\n\n");
            break;
        }
    }
}

void wstrzymaj()
{

    printf("\nNacisnij dowolny klawisz aby kontynuowac\n");
    getch();
}


void liczbaElementow()
{
    long int ileElementow =0;

    ileElementow = policzElementy();
    printf("Liczba elementow na liscie: %ld\n", ileElementow);

}


long int policzElementy()
{
    // policz ile jest elementow
    long int liczbaElementow = 0;
    Osoba *aktualny = poczatek;
    while(aktualny != NULL)
    {
        liczbaElementow = liczbaElementow + 1;
        aktualny = aktualny->nastepny;
    }
    return liczbaElementow;
}


void wyswietlListeWgNazwisk()
{
    long int licznik = 0;
    long int liczbaElementow = 0;
    Osoba *aktualny = NULL;

    liczbaElementow = policzElementy();
    if(liczbaElementow == 0)
    {
        printf("Brak elementow do wyswietlenia\n");
        return;
    }
    /* utworz tablice i przepisz do niej */
    Osoba * tablica[liczbaElementow];
    aktualny = poczatek;
    licznik = 0;
    while(aktualny != NULL)
    {
        tablica[licznik] = aktualny;
        aktualny = aktualny->nastepny;
        licznik++;
    }

    /* posortuj */
    qsort(tablica, liczbaElementow, sizeof(Osoba *), porownaj);

    /* wyswietl elementy z tablicy */
    for(long int i=0; i < liczbaElementow; i++)
    {
        wyswietlOsoba(tablica[i]);
    }
}


char * strupper(char *s)
{
    while (*s)
    {
        if ((*s >= 'a' ) && (*s <= 'z')) *s -= ('a'-'A');
        s++;
    }

    return s;
}

// Porownanie elementow
// Porownanie jest po duzych literach aby prawidlowo sortowal
// zarowno duze jak i male
int porownaj(const void *a, const void *b)
{
    char anazw[15];
    char bnazw[15];

    int wynik = 0;
    const Osoba **aEL = (const Osoba **) a;
    const Osoba **bEL = (const Osoba **) b;
    // manewry w celu nierozrozniania welkosci liter
    strcpy(anazw, (*aEL)->nazwisko);
    strcpy(bnazw, (*bEL)->nazwisko);
    strupper(anazw);
    strupper(bnazw);
    wynik = strcmp( anazw, bnazw);

    return wynik;
}


void wyswietlListePoId()
{
    Osoba *aktualny = poczatek;
    if(aktualny == NULL)
    {
        printf("Brak elementow do wyswietlenia\n");
    }
    else
    {
        while(aktualny != NULL)
        {
            wyswietlOsoba(aktualny);
            aktualny = aktualny->nastepny;
        }
    }

}

void wyswietlListePosortowana()
{
    // sortowanie listy przed wyswietleniem
    Osoba * doSortowania = poczatek;
    sortpoczatek = NULL;
    sortkoniec = NULL;
    while(doSortowania != NULL)
    {
        dodajDoListyPosortowanej(doSortowania);
        doSortowania = doSortowania->nastepny;
    }


    // wyswietlenie posortowanej listy
    Osoba *aktualny = sortpoczatek;
    if(aktualny == NULL)
    {
        printf("Brak elementow do wyswietlenia\n");
    }
    else
    {
        while(aktualny != NULL)
        {
            wyswietlOsoba(aktualny);
            aktualny = aktualny->sortnastepny;
        }
    }

}


void wyswietlOsoba(Osoba * aktualny)
{
    printf("Id: %ld Nazwisko: %s imie: %s data: %02d/%02d/%02d adres: %s miasto: %s\n ",aktualny->Id, aktualny->nazwisko, aktualny -> imie, aktualny-> data.dzien, aktualny->data.miesiac, aktualny->data.rok, aktualny ->adres, aktualny -> miasto);
}



Osoba *wyszukajPoId (int id)
{
    Osoba *aktualny = poczatek;

    while(aktualny != NULL && aktualny->Id != id)
    {
        aktualny = aktualny->nastepny;
    }
    return aktualny;
}


void znajdzImodyfikuj ()
{
    int id = 0;
    Osoba *osoba;

    printf("Podaj ID osoby, ktorej chcesz modyfikowac dane: ");
    scanf("%d", &id);
    printf("\n");
    osoba = wyszukajPoId(id);

    if (osoba == NULL)
    {
        printf("Nie ma osoby o takim Id !\n");
    }
    else
    {
        wyswietlOsoba(osoba);
        modyfikujOsobe(osoba);
        printf("Osoba zostala zmodyfikowana pomyslnie.\n");
    }

}

Osoba* modyfikujOsobe(Osoba *osoba)
{
    char nazwisko[15];
    char imie [15];
    Data data;
    char adres [15];
    char miasto [15];
    printf("Podaj nazwisko: ");
    scanf("%s", nazwisko);
    printf("Podaj imie: ");
    scanf("%s", imie);
    printf("Podaj date urodzin:\n");
    printf("  dzien (dd): ");
    scanf("%d", &data.dzien);
    printf("  miesiac (mm): ");
    scanf("%d", &data.miesiac);
    printf("  rok (rr): ");
    scanf("%d", &data.rok);
    printf("Podaj adres: ");
    scanf("%s", adres);
    printf("Podaj miasto: ");
    scanf("%s", miasto);


    // wpisanie elementow do struktury
    strcpy(osoba->nazwisko,nazwisko);
    strcpy(osoba->imie,imie);
    osoba->data.dzien = data.dzien;
    osoba->data.miesiac = data.miesiac;
    osoba->data.rok = data.rok;
    strcpy(osoba->adres,adres);
    strcpy(osoba->miasto,miasto);

    return osoba;
}


void dodajOsobe()
{

    Osoba *a;
    a =(Osoba*)malloc(sizeof(Osoba));
    a->Id = dajId();
    modyfikujOsobe(a);
    dodajNaKoniecListy(a);


}


void dodajNaKoniecListy(Osoba *a)
{
    // odpowiednie umieszczenie w strukturze
    if ((poczatek)==NULL && (koniec)==NULL)
    {
        poczatek=a;
        koniec=a;
        a->nastepny=NULL;
        a->poprzedni=NULL;
    }
    else
    {
        a->nastepny = NULL;
        a->poprzedni = koniec;
        (koniec)->nastepny = a;
        (koniec)=a;
    }

}


void dodajDoListyPosortowanej(Osoba *osoba)
{

    if ((sortpoczatek)==NULL && (sortkoniec)==NULL)
    {
        sortpoczatek=osoba;
        sortkoniec=osoba;
        osoba->sortnastepny=NULL;
        osoba->sortpoprzedni=NULL;
    }
    else
    {
        Osoba * osobaprzegl = sortpoczatek;
        char anazw[15];
        char bnazw[15];
        int wynik;
        strcpy(anazw, osoba->nazwisko);
        strcpy(bnazw, osobaprzegl->nazwisko);
        strupper(anazw);
        strupper(bnazw);
        wynik = strcmp( bnazw, anazw);

        while(osobaprzegl != NULL && wynik < 1)
        {
            osobaprzegl = osobaprzegl->sortnastepny;
            if(osobaprzegl != NULL)
            {
                strcpy(anazw, osoba->nazwisko);
                strcpy(bnazw, osobaprzegl->nazwisko);
                strupper(anazw);
                strupper(bnazw);
                wynik = strcmp( bnazw, anazw);
            }
        }

        if(osobaprzegl == NULL)
        {
            // ostatni element
            osobaprzegl = sortkoniec;
            sortkoniec = osoba;
            osoba->sortnastepny = NULL;
            osoba->sortpoprzedni = osobaprzegl;
            osobaprzegl->sortnastepny = osoba;
        }
        else if(osobaprzegl->sortpoprzedni == NULL)
        {
            // pierwszy element
            if(wynik >= 1)
            {
                // wstaw przed elementem
                sortpoczatek = osoba;
                osoba->sortpoprzedni = osobaprzegl->sortpoprzedni;
                osoba->sortnastepny = osobaprzegl;
                osobaprzegl->sortpoprzedni = osoba;

            }
            else
            {
                // wstaw za elementem
                osoba->sortpoprzedni = osobaprzegl;
                osoba->sortnastepny = osobaprzegl->sortnastepny;
                osobaprzegl->sortnastepny = osoba;
                sortkoniec = osoba;
            }
        }
        else
        {
            // srodkowy element - wstaw przed elementem
            osoba->sortnastepny = osobaprzegl;
            osoba->sortpoprzedni = osobaprzegl->sortpoprzedni;
            osobaprzegl->sortpoprzedni = osoba;
            osoba->sortpoprzedni->sortnastepny = osoba;
        }
    }

}


void podajIzapisz()
{
    char nazwa [30];
    printf("Podaj nazwe pliku: ");
    scanf("%s", nazwa);
    zapiszDoPliku(nazwa);

}


void zapiszDoPliku(const char * nazwaPliku)
{
    Osoba *aktualny = poczatek;
    if(aktualny == NULL)
    {
        printf("Brak elementow do zapisu.\n");
    }
    else
    {
        fstream plik;
        plik.open(nazwaPliku, ios::out);
        if(plik.good()==true)
        {
            while(aktualny != NULL)
            {
                char buf[200];
                sprintf(buf, "%ld\t%s\t%s\t%d\t%d\t%d\t%s\t%s",aktualny->Id, aktualny->nazwisko, aktualny->imie, aktualny->data.dzien, aktualny->data.miesiac, aktualny->data.rok, aktualny->adres, aktualny->miasto);
                plik << buf << endl;
                aktualny = aktualny->nastepny;
            }
            plik.close();
            printf("Zapis do pliku zakonczony powodzeniem\n");
        }
    }
}


void podajIodczytaj()
{
    char nazwa [30];
    printf("Podaj nazwe pliku: ");
    scanf("%s", nazwa);
    odczytajZPliku(nazwa);

}

void odczytajZPliku(const char * nazwaPliku)
{
    fstream plik;
    string line;
    long int ident = 0;
    char nazwisko[15];
    char imie [15];
    Data data;
    char adres [15];
    char miasto [15];
    plik.open(nazwaPliku, ios::in);
    {
        while(getline(plik,line))
        {
            const char * buf = line.c_str();
            sscanf(buf, "%ld\t%s\t%s\t%d\t%d\t%d\t%s\t%s", &ident,nazwisko,imie, &data.dzien, &data.miesiac, &data.rok, adres, miasto);
            // identyfikator zawsze unikalny w ramach sesji
            Osoba *osoba;
            osoba =(Osoba*)malloc(sizeof(Osoba));
            osoba->Id = dajId();
            strcpy(osoba->nazwisko,nazwisko);
            strcpy(osoba->imie,imie);
            osoba->data.dzien = data.dzien;
            osoba->data.miesiac = data.miesiac;
            osoba->data.rok = data.rok;
            strcpy(osoba->adres,adres);
            strcpy(osoba->miasto,miasto);
            dodajNaKoniecListy(osoba);
        }
    }
    plik.close();
    printf("Odczyt z pliku zakonczony powodzeniem.\n");
}

// pobranie kolejnego identyfikatora
// unikalny w ramach jednego dzialania programu
long int dajId()
{
    static long int ostatnieId = 0;

    ostatnieId = ostatnieId + 1;
    return ostatnieId;
}

Osoba* szukajOsobe(char nazwisko [15])
{

    int wynik = 0;
    char anazw [15];
    char bnazw [15];
    Osoba * osobaprzegl = poczatek;
    strcpy(anazw, nazwisko);


    // jezeli brak elementow to od razu powrot
    if(osobaprzegl == NULL)
        return NULL;

    strcpy(bnazw, osobaprzegl->nazwisko);
    strupper(anazw);
    strupper(bnazw);
    wynik = strcmp( bnazw, anazw);

    while(osobaprzegl != NULL && wynik != 0)
    {
        osobaprzegl = osobaprzegl->nastepny;
        if(osobaprzegl != NULL)
        {

            strcpy(bnazw, osobaprzegl->nazwisko);
            strupper(anazw);
            strupper(bnazw);
            wynik = strcmp( bnazw, anazw);
        }
    }
    return osobaprzegl;
}

void wyszukajIwyswietl()
{
    Osoba *osoba;
    char nazwisko [15];
    printf("Podaj nazwisko: ");
    scanf("%s", nazwisko);

    osoba = szukajOsobe(nazwisko);

    if (osoba == NULL)
    {
        printf("Brak osoby o nazwisku %s\n", nazwisko);

    }
    else
    {
        wyswietlOsoba(osoba);
    }
}


void wyszukajIusun()
{
    Osoba *osoba;
    char nazwisko [15];
    printf("Podaj nazwisko: ");
    scanf("%s", nazwisko);

    osoba = szukajOsobe(nazwisko);

    if (osoba == NULL)
    {
        printf("Brak osoby o nazwisku %s\n", nazwisko);

    }
    else
    {
        usunOsoba(osoba);
    }
}

void usunOsoba(Osoba *osoba)
{
// pojedynczy element
    if (osoba -> poprzedni == NULL && osoba -> nastepny == NULL)
    {
        poczatek = NULL;
        koniec = NULL;
    }
// pierwszy element
    else if (osoba -> poprzedni == NULL)
    {
        Osoba *nastOsoba = osoba -> nastepny;
        poczatek = nastOsoba;
        nastOsoba -> poprzedni = NULL;
    }
//ostatni element
    else if (osoba -> nastepny == NULL)
    {
        Osoba *poprzOsoba = osoba -> poprzedni;
        koniec = poprzOsoba;
        poprzOsoba -> nastepny = NULL;
    }
// element œrodkowy
    else
    {
        Osoba *poprzOsoba = osoba -> poprzedni;
        Osoba *nastOsoba = osoba -> nastepny;
        poprzOsoba -> nastepny = nastOsoba;
        nastOsoba -> poprzedni = poprzOsoba;
    }
    free(osoba);
    printf("Usuwanie osoby zakonczono pomyslnie.\n");
}
