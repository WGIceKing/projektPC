#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define KOLORY 6
#define COLORSWITHGREEN 7
#define GREEN 1
#define BLUE 2
#define RED 3
#define VIOLET 4
#define YELLOW 5
#define WHITE 6
#define BLACK 7

struct card {
    const char* kolor;
    int wartosc;
};

typedef struct list {
    card karta;
    struct list* next;
}list_t;

typedef struct intlist {
    int value;
    struct intlist* next;
}intlist_t;

void init(list_t* head) {
    head->next = NULL;
}

void initInt(intlist_t* head) {
    head->next = NULL;
}

void addElementInt(intlist_t* l, int value) {
    intlist_t* x = (intlist_t*)malloc(sizeof(intlist_t));
    x->value = value;
    x->next = l->next;
    l->next = x;
}

void addElement(list_t* l, int value, const char* color) {
    list_t* x = (list_t*)malloc(sizeof(list_t));
    x->karta.wartosc = value;
    x->karta.kolor = color;
    x->next = l->next;
    l->next = x;
}

void removeElementFront(list_t* l) {
    list_t* head = l;
    list_t* deleteObject;
    if (l == NULL) {
        cout << "List already empty" << endl;
    }
    else {
        head = head->next;
        deleteObject = head;
        head = head->next;
        l->next = head;
        free(deleteObject);
    }

}

void print(list_t* h) {
    h = h->next;
    while (h != NULL) {
        cout << h->karta.wartosc << " " << h->karta.kolor << " ";
        h = h->next;
    }
}

void printInt(intlist_t* h) {
    h = h->next;
    while (h != NULL) {
        cout << h->value << " ";
        h = h->next;
    }
}

void printToFile(list_t* h, int i, FILE* fp) {
    h = h->next;
    while (h != NULL) {
        fprintf(fp, "%i %s ", h->karta.wartosc, h->karta.kolor);
        h = h->next;
    }
}

void sprawdzZmienne(int n, int k, int g, int gv, int o) {
    if (n < 2 or n > 6) {
        cout << "n wrong" << endl;
        cin >> n;
    }
    if (k < 1 or k > 6) {
        cout << "k wrong" << endl;
        cin >> k;
    }
    if (g < 1 or g > 9) {
        cout << "g wrong" << endl;
        cin >> g;
    }
    if (gv < 1 or gv > 10) {
        cout << "gv wrong" << endl;
        cin >> gv;
    }
    if (o < 1 or o > 20) {
        cout << "o wrong" << endl;
        cin >> o;
    }
}

void wczytajWartosci(int tabela[], int o, int k, card tabelaKart[]) {

    for (int i = 0; i < o; i++) {
        int wartosc;
        cin >> wartosc;
        tabela[i] = wartosc;
    }

    for (int j = 0; j < k; j++) {
        for (int i = 0; i < o; i++) {
            tabelaKart[i + j * (k - 1)].wartosc = tabela[i];
        }
    }
}


void stworzTalie(int k, int g, int gv, int o, const char* colors[KOLORY], card tabelaKart[], int* tabelaWartosci) {
    for (int i = 0; i < g; i++) {
        tabelaKart[i].wartosc = gv;
        tabelaKart[i].kolor = "green";
    }

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < o; j++) {
            tabelaKart[j + (i * o) + g].wartosc = tabelaWartosci[j];
            tabelaKart[j + (i * o) + g].kolor = colors[i];
        }
    }
}

void reverseList(list_t* l) {
    list_t* prev = NULL;
    list_t* cur = l->next;
    list_t* next = NULL;
    while (cur != NULL) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    l->next = prev;
}

void dajDoRak(int o, int k, int g, int n, list_t** players, card* tabelaKart) {
    list_t* cur = NULL;
    for (int i = 0; i < o * k + g; i++) {
        cur = players[i % n];
        addElement(cur, tabelaKart[i].wartosc, tabelaKart[i].kolor);
        cur = cur->next;
    }
    for (int i = 0; i < n; i++) {
        reverseList(players[i]);
    }
}

void wypiszStanDoPliku(int numberOfPlayers, int k, list_t** players, FILE* fp, int* activePlayer, list_t** deckCards, list_t** pileCards, int exploTreshold) {

    fprintf(fp, "active player = %i\n", (*activePlayer));
    fprintf(fp, "players number = %i\n", numberOfPlayers);
    fprintf(fp, "explosion treshold = %i\n", exploTreshold);

    for (int i = 0; i < numberOfPlayers; i++) {
        fprintf(fp, "%i player hand cards: ", i + 1);
        printToFile(players[i], i, fp);

        fprintf(fp, "\n");
        fprintf(fp, "%i player deck cards: ", i + 1);
        printToFile(deckCards[i], i, fp);
        fprintf(fp, "\n");
    }
    for (int i = 0; i < k; i++) {
        fprintf(fp, "%i pile cards: ", i + 1);
        printToFile(pileCards[i], i, fp);
        fprintf(fp, "\n");
    }
    fclose(fp);
    free(fp);
}

int LiczbaGraczy() {
    FILE* loadFile;
    loadFile = fopen("save.txt", "r");

    for (int i = 0; i < 2; i++) {
        char line[1000];
        fgets(line, 1000, loadFile);
        int index = 0;
        if (i == 1) {
            char nGraczy;
            int numberOfPlayers;
            while (line[index] != '\n') {
                nGraczy = line[index];
                index++;
            }
            numberOfPlayers = (int)nGraczy - '0';;
            return numberOfPlayers;
        }
    }
    fclose(loadFile);
    free(loadFile);
}

void wczytajStanWartosci(int* cardCount, int* greenCount, int* greenValue, int* greenValueCheck, int* deckCardCount, int* iloscPile, int* iloscPileKart, int* exploTreshold, int* activePlayer) {

    FILE* loadFile;
    loadFile = fopen("save.txt", "r");

    const char* colorName = NULL;
    int value = 0;
    int indexKolorow = 0;
    const char* green = "green";
    char line[1000];
    int i = 0;
    int playerNumber = 0;
    int greenValueBufor = 0;

    while (fgets(line, 1000, loadFile) != NULL) {
        int x = 0;
        if (i == 0) {
            (*activePlayer) = (int)line[16] - '0';
        }
        if (i == 1) {
            playerNumber = (int)line[17] - '0';
            //(*pileCount) -= 3;
        }
        if (i == 2) {
            if ((int)line[23] >= 48 and (int)line[23] <= 57) {
                (*exploTreshold) = 10 * ((int)line[22] - '0') + (int)line[23] - '0';
            }
            else {
                (*exploTreshold) = (int)line[22] - '0';
            }
        }
        (*iloscPile) = (int)line[0] - '0';
        if (i > 2 and i < 2 * playerNumber + 3) {
            int z = 0;
            int flaga = 0;
            char lineEnd[1000];
            while (line[x] != '\n') {
                if (line[x] == ':') {
                    flaga = flaga + 1;
                }

                if (flaga != 0) {
                    lineEnd[z] = line[x + 1];
                    z++;
                }
                x++;
            }

            lineEnd[z] = '\0';
            if (i % 2 == 1) {

                int y = 0;
                int even = 0;

                while (lineEnd[y + 1] != '\0') {
                    char element[10];
                    int index = 0;
                    int flagaDwa = 0;
                    int valueLength = 0;

                    if (lineEnd[y] != '\n') {
                        while (lineEnd[y] != ' ') {
                            element[index] = lineEnd[y];
                            index++;
                            y++;
                            flagaDwa++;
                            valueLength++;
                        }
                    }

                    if (flagaDwa > 0) {
                        element[index] = '\0';

                        if (even % 2 == 0) {
                            if (valueLength < 2) {
                                value = (int)element[0] - '0';
                            }
                            else {
                                value = 10 * ((int)element[0] - '0') + (int)element[1] - '0';
                            }
                            (*cardCount)++;
                            even++;
                        }
                        else {
                            colorName = element;
                            int compareIndex = 0;
                            int flague = 0;
                            while (element[compareIndex] != '\0') {
                                if (element[compareIndex] == green[compareIndex]) {
                                    compareIndex++;
                                }
                                else {
                                    break;
                                }
                                flague++;
                            }
                            if (flague == 5) {
                                (*greenCount)++;
                                *greenValue = value;
                                if (*greenValue != greenValueBufor) {
                                    greenValueBufor = *greenValue;
                                    (*greenValueCheck)++;
                                }
                            }
                            even++;
                        }
                    }
                    y++;
                }
            }
            if (i % 2 == 0) {

                int y = 0;
                int even = 0;

                while (lineEnd[y + 1] != '\0') {
                    char element[10];
                    int index = 0;
                    int flagaDwa = 0;
                    int valueLength = 0;

                    if (lineEnd[y] != '\n') {
                        while (lineEnd[y] != ' ') {
                            element[index] = lineEnd[y];
                            index++;
                            y++;
                            flagaDwa++;
                            valueLength++;
                        }
                    }

                    const char* colorName = NULL;

                    if (flagaDwa > 0) {
                        element[index] = '\0';

                        if (even % 2 == 0) {
                            if (valueLength < 2) {
                                value = (int)element[0] - '0';
                            }
                            else {
                                value = 10 * ((int)element[0] - '0') + (int)element[1] - '0';
                            }
                            (*deckCardCount)++;
                            even++;
                        }
                        else {
                            colorName = element;
                            int compareIndex = 0;
                            int flague = 0;
                            while (element[compareIndex] != '\0') {
                                if (element[compareIndex] == green[compareIndex]) {
                                    compareIndex++;
                                }
                                else {
                                    break;
                                }
                                flague++;
                            }
                            if (flague == 5) {
                                (*greenCount)++;
                                *greenValue = value;
                                if (*greenValue != greenValueBufor) {
                                    greenValueBufor = *greenValue;
                                    (*greenValueCheck)++;
                                }
                            }
                            even++;
                        }
                    }
                    y++;
                }
            }
        }
        if (i >= 2 * playerNumber + 3) {
            int z = 0;
            int flaga = 0;
            char lineEnd[1000];
            while (line[x] != '\n') {
                if (line[x] == ':') {
                    flaga = flaga + 1;
                }

                if (flaga != 0) {
                    lineEnd[z] = line[x + 1];
                    z++;
                }
                x++;
            }

            lineEnd[z] = '\0';

            int y = 0;
            int even = 0;

            while (lineEnd[y + 1] != '\0') {
                char element[10];
                int index = 0;
                int flagaDwa = 0;
                int valueLength = 0;

                if (lineEnd[y] != '\n') {
                    while (lineEnd[y] != ' ') {
                        element[index] = lineEnd[y];
                        index++;
                        y++;
                        flagaDwa++;
                        valueLength++;
                    }
                }

                const char* colorName = NULL;

                if (flagaDwa > 0) {
                    element[index] = '\0';

                    if (even % 2 == 0) {
                        if (valueLength < 2) {
                            value = (int)element[0] - '0';
                        }
                        else {
                            value = 10 * ((int)element[0] - '0') + (int)element[1] - '0';
                        }
                        if (value >= 0 and value <= 20) {
                            (*iloscPileKart)++;
                        }
                        even++;
                    }
                    else {
                        colorName = element;
                        int compareIndex = 0;
                        int flague = 0;
                        while (element[compareIndex] != '\0') {
                            if (element[compareIndex] == green[compareIndex]) {
                                compareIndex++;
                            }
                            else {
                                break;
                            }
                            flague++;
                        }
                        if (flague == 5) {
                            (*greenCount)++;
                            *greenValue = value;
                            if (*greenValue != greenValueBufor) {
                                greenValueBufor = *greenValue;
                                (*greenValueCheck)++;
                            }
                        }
                        even++;
                    }
                }
                y++;
            }

        }
        char line[1000];
        i++;
    }
    fclose(loadFile);
    free(loadFile);
}


void wczytajStan(int n, int* tabelaWartosciLoaded, int cardCount, int* tabelaKolorowLoaded, list_t** players, const char** colorsWithGreen, list_t** deckCards, int* tabelaWartosciDeck, int* tabelaKolorowDeck, list_t** pileCards, int* tabelaWartosciPile, int* tabelaKolorowPile, int iloscPile, int* tabelaWszystkichKolory, int* tabelaWszystkichWartosci, FILE* loadFile) {

    const char* colorName = NULL;
    int value = 0;
    int indexWartosci = 0;
    int indexWartosciDeck = 0;
    int indexWartosciPile = 0;
    int indexWartosciAll = 0;
    int indexKolorow = 0;
    int indexKolorowDeck = 0;
    int indexKolorowPile = 0;
    int indexKolorowAll = 0;
    const char* green = "green";
    const char* blue = "blue";
    const char* red = "red";
    const char* violet = "violet";
    const char* yellow = "yellow";
    const char* white = "white";
    const char* black = "black";
    int i = 0;

    for (i; i < 3 + (2 * n) + iloscPile; i++) {
        char line[1000];
        fgets(line, 1000, loadFile);
        int x = 0;
        if (i > 2) {
            list_t* cur = players[i / 2 - 1];
            list_t* curDeck = deckCards[i / 2 - 2];
            list_t* curPile = pileCards[i - (3 + 2 * n)];
            int z = 0;
            int flaga = 0;
            char lineEnd[1000];
            while (line[x] != '\n') {
                if (line[x] == ':') {
                    flaga = flaga + 1;
                }

                if (flaga != 0) {
                    lineEnd[z] = line[x + 1];
                    z++;
                }
                x++;
            }

            lineEnd[z] = '\0';
            if (i % 2 == 1 and i < 2 * n + 3) {

                int y = 0;
                int even = 0;

                while (lineEnd[y + 1] != '\0') {
                    char element[10];
                    int index = 0;
                    int flagaDwa = 0;
                    int valueLength = 0;

                    if (lineEnd[y] != '\n') {
                        while (lineEnd[y] != ' ') {
                            element[index] = lineEnd[y];
                            index++;
                            y++;
                            flagaDwa++;
                            valueLength++;
                        }
                    }

                    if (flagaDwa > 0) {
                        element[index] = '\0';

                        if (even % 2 == 0) {
                            if (valueLength < 2) {
                                value = (int)element[0] - '0';
                            }
                            else {
                                value = 10 * ((int)element[0] - '0') + (int)element[1] - '0';
                            }
                            tabelaWartosciLoaded[indexWartosci] = value;
                            tabelaWszystkichWartosci[indexWartosciAll] = value;
                            indexWartosciAll++;
                            indexWartosci++;
                            even++;
                        }
                        else {
                            if (element[0] != 'b') {
                                if (element[0] == 'g') {
                                    tabelaKolorowLoaded[indexKolorow] = GREEN;
                                    tabelaWszystkichKolory[indexKolorowAll] = GREEN;
                                    indexKolorowAll++;
                                    addElement(cur, value, colorsWithGreen[tabelaKolorowLoaded[indexKolorow] - 1]);
                                    cur = cur->next;
                                    indexKolorow++;
                                }
                                if (element[0] == 'r') {
                                    tabelaKolorowLoaded[indexKolorow] = RED;
                                    tabelaWszystkichKolory[indexKolorowAll] = RED;
                                    indexKolorowAll++;
                                    addElement(cur, value, colorsWithGreen[tabelaKolorowLoaded[indexKolorow] - 1]);
                                    cur = cur->next;
                                    indexKolorow++;
                                }
                                if (element[0] == 'y') {
                                    tabelaKolorowLoaded[indexKolorow] = YELLOW;
                                    tabelaWszystkichKolory[indexKolorowAll] = YELLOW;
                                    indexKolorowAll++;
                                    addElement(cur, value, colorsWithGreen[tabelaKolorowLoaded[indexKolorow] - 1]);
                                    cur = cur->next;
                                    indexKolorow++;
                                }
                                if (element[0] == 'w') {
                                    tabelaKolorowLoaded[indexKolorow] = WHITE;
                                    tabelaWszystkichKolory[indexKolorowAll] = WHITE;
                                    indexKolorowAll++;
                                    addElement(cur, value, colorsWithGreen[tabelaKolorowLoaded[indexKolorow] - 1]);
                                    cur = cur->next;
                                    indexKolorow++;
                                }
                                if (element[0] == 'v') {
                                    tabelaKolorowLoaded[indexKolorow] = VIOLET;
                                    tabelaWszystkichKolory[indexKolorowAll] = VIOLET;
                                    indexKolorowAll++;
                                    addElement(cur, value, colorsWithGreen[tabelaKolorowLoaded[indexKolorow] - 1]);
                                    cur = cur->next;
                                    indexKolorow++;
                                }
                            }
                            else if (element[2] == 'u') {
                                tabelaKolorowLoaded[indexKolorow] = BLUE;
                                tabelaWszystkichKolory[indexKolorowAll] = BLUE;
                                indexKolorowAll++;
                                addElement(cur, value, colorsWithGreen[tabelaKolorowLoaded[indexKolorow] - 1]);
                                cur = cur->next;
                                indexKolorow++;
                            }
                            else {
                                tabelaKolorowLoaded[indexKolorow] = BLACK;
                                tabelaWszystkichKolory[indexKolorowAll] = BLACK;
                                indexKolorowAll++;
                                addElement(cur, value, colorsWithGreen[tabelaKolorowLoaded[indexKolorow] - 1]);
                                cur = cur->next;
                                indexKolorow++;
                            }
                            even++;
                        }
                    }
                    y++;
                }
            }
            if (i % 2 == 0 and i < 2 * n + 3) {

                int y = 0;
                int even = 0;

                while (lineEnd[y + 1] != '\0') {
                    char element[10];
                    int index = 0;
                    int flagaDwa = 0;
                    int valueLength = 0;

                    if (lineEnd[y] != '\n') {
                        while (lineEnd[y] != ' ') {
                            element[index] = lineEnd[y];
                            index++;
                            y++;
                            flagaDwa++;
                            valueLength++;
                        }
                    }

                    if (flagaDwa > 0) {
                        element[index] = '\0';

                        if (even % 2 == 0) {
                            if (valueLength < 2) {
                                value = (int)element[0] - '0';
                            }
                            else {
                                value = 10 * ((int)element[0] - '0') + (int)element[1] - '0';
                            }
                            tabelaWartosciDeck[indexWartosciDeck] = value;
                            tabelaWszystkichWartosci[indexWartosciAll] = value;
                            indexWartosciAll++;
                            indexWartosciDeck++;
                            even++;
                        }
                        else {
                            if (element[0] != 'b') {
                                if (element[0] == 'g') {
                                    tabelaKolorowDeck[indexKolorowDeck] = GREEN;
                                    tabelaWszystkichKolory[indexKolorowAll] = GREEN;
                                    indexKolorowAll++;
                                    addElement(curDeck, value, colorsWithGreen[tabelaKolorowDeck[indexKolorowDeck] - 1]);
                                    curDeck = curDeck->next;
                                    indexKolorowDeck++;
                                }
                                if (element[0] == 'r') {
                                    tabelaKolorowDeck[indexKolorowDeck] = RED;
                                    tabelaWszystkichKolory[indexKolorowAll] = RED;
                                    indexKolorowAll++;
                                    addElement(curDeck, value, colorsWithGreen[tabelaKolorowDeck[indexKolorowDeck] - 1]);
                                    curDeck = curDeck->next;
                                    indexKolorowDeck++;
                                }
                                if (element[0] == 'y') {
                                    tabelaKolorowDeck[indexKolorowDeck] = YELLOW;
                                    tabelaWszystkichKolory[indexKolorowAll] = YELLOW;
                                    indexKolorowAll++;
                                    addElement(curDeck, value, colorsWithGreen[tabelaKolorowDeck[indexKolorowDeck] - 1]);
                                    curDeck = curDeck->next;
                                    indexKolorowDeck++;
                                }
                                if (element[0] == 'w') {
                                    tabelaKolorowDeck[indexKolorowDeck] = WHITE;
                                    tabelaWszystkichKolory[indexKolorowAll] = WHITE;
                                    indexKolorowAll++;
                                    addElement(curDeck, value, colorsWithGreen[tabelaKolorowDeck[indexKolorowDeck] - 1]);
                                    curDeck = curDeck->next;
                                    indexKolorowDeck++;
                                }
                                if (element[0] == 'v') {
                                    tabelaKolorowDeck[indexKolorowDeck] = VIOLET;
                                    tabelaWszystkichKolory[indexKolorowAll] = VIOLET;
                                    indexKolorowAll++;
                                    addElement(curDeck, value, colorsWithGreen[tabelaKolorowDeck[indexKolorowDeck] - 1]);
                                    curDeck = curDeck->next;
                                    indexKolorowDeck++;
                                }
                            }
                            else if (element[2] == 'u') {
                                tabelaKolorowDeck[indexKolorowDeck] = BLUE;
                                tabelaWszystkichKolory[indexKolorowAll] = BLUE;
                                indexKolorowAll++;
                                addElement(curDeck, value, colorsWithGreen[tabelaKolorowDeck[indexKolorowDeck] - 1]);
                                curDeck = curDeck->next;
                                indexKolorowDeck++;
                            }
                            else {
                                tabelaKolorowDeck[indexKolorowDeck] = BLACK;
                                tabelaWszystkichKolory[indexKolorowAll] = BLACK;
                                indexKolorowAll++;
                                addElement(curDeck, value, colorsWithGreen[tabelaKolorowDeck[indexKolorowDeck] - 1]);
                                curDeck = curDeck->next;
                                indexKolorowDeck++;
                            }
                            even++;
                        }
                    }
                    y++;
                }
            }
            else if (i >= 2 * n + 3) {
                int y = 0;
                int even = 0;

                while (lineEnd[y + 1] != '\0') {
                    char element[10];
                    int index = 0;
                    int flagaDwa = 0;
                    int valueLength = 0;

                    if (lineEnd[y] != '\n') {
                        while (lineEnd[y] != ' ') {
                            element[index] = lineEnd[y];
                            index++;
                            y++;
                            flagaDwa++;
                            valueLength++;
                        }
                    }

                    if (flagaDwa > 0) {
                        element[index] = '\0';

                        if (even % 2 == 0) {
                            if (valueLength < 2) {
                                value = (int)element[0] - '0';
                            }
                            else {
                                value = 10 * ((int)element[0] - '0') + (int)element[1] - '0';
                            }

                            if (value >= 0 and value <= 20) {
                                tabelaWartosciPile[indexWartosciPile] = value;
                                tabelaWszystkichWartosci[indexWartosciAll] = value;
                                indexWartosciAll++;
                                indexWartosciPile++;
                            }
                            even++;
                        }
                        else if (value >= 0 and value <= 20) {
                            if (element[0] != 'b') {
                                if (element[0] == 'g') {
                                    tabelaKolorowPile[indexKolorowPile] = GREEN;
                                    tabelaWszystkichKolory[indexKolorowAll] = GREEN;
                                    indexKolorowAll++;
                                    addElement(curPile, value, colorsWithGreen[tabelaKolorowPile[indexKolorowPile] - 1]);
                                    curPile = curPile->next;
                                    indexKolorowPile++;
                                }
                                if (element[0] == 'r') {
                                    tabelaKolorowPile[indexKolorowPile] = RED;
                                    tabelaWszystkichKolory[indexKolorowAll] = RED;
                                    indexKolorowAll++;
                                    addElement(curPile, value, colorsWithGreen[tabelaKolorowPile[indexKolorowPile] - 1]);
                                    curPile = curPile->next;
                                    indexKolorowPile++;
                                }
                                if (element[0] == 'y') {
                                    tabelaKolorowPile[indexKolorowPile] = YELLOW;
                                    tabelaWszystkichKolory[indexKolorowAll] = YELLOW;
                                    indexKolorowAll++;
                                    addElement(curPile, value, colorsWithGreen[tabelaKolorowPile[indexKolorowPile] - 1]);
                                    curPile = curPile->next;
                                    indexKolorowPile++;
                                }
                                if (element[0] == 'w') {
                                    tabelaKolorowPile[indexKolorowPile] = WHITE;
                                    tabelaWszystkichKolory[indexKolorowAll] = WHITE;
                                    indexKolorowAll++;
                                    addElement(curPile, value, colorsWithGreen[tabelaKolorowPile[indexKolorowPile] - 1]);
                                    curPile = curPile->next;
                                    indexKolorowPile++;
                                }
                                if (element[0] == 'v') {
                                    tabelaKolorowPile[indexKolorowPile] = VIOLET;
                                    tabelaWszystkichKolory[indexKolorowAll] = VIOLET;
                                    indexKolorowAll++;
                                    addElement(curPile, value, colorsWithGreen[tabelaKolorowPile[indexKolorowPile] - 1]);
                                    curPile = curPile->next;
                                    indexKolorowPile++;
                                }
                            }
                            else if (element[2] == 'u') {
                                tabelaKolorowPile[indexKolorowPile] = BLUE;
                                tabelaWszystkichKolory[indexKolorowAll] = BLUE;
                                indexKolorowAll++;
                                addElement(curPile, value, colorsWithGreen[tabelaKolorowPile[indexKolorowPile] - 1]);
                                curPile = curPile->next;
                                indexKolorowPile++;
                            }
                            else {
                                tabelaKolorowPile[indexKolorowPile] = BLACK;
                                tabelaWszystkichKolory[indexKolorowAll] = BLACK;
                                indexKolorowAll++;
                                addElement(curPile, value, colorsWithGreen[tabelaKolorowPile[indexKolorowPile] - 1]);
                                curPile = curPile->next;
                                indexKolorowPile++;
                            }
                            even++;
                        }
                    }
                    y++;
                }
            }
        }
    }

    fclose(loadFile);
    free(loadFile);
}

void colorNumber(list_t* allCards, const char** colors, int sposob, int* equal, int* equalSize, int colorCheck[KOLORY]) {

    //int blueCount = 0;
    //int redCount = 0;
    //int yellowCount = 0;
    //int violetCount = 0;
    //int whiteCount = 0;
    //int blackCount = 0;

    int colorCount[KOLORY];//blue, red, violet, yellow, white, black

    for (int i = 0; i < KOLORY; i++) {
        colorCount[i] = 0;
    }

    allCards = allCards->next;
    while (allCards != NULL) {
        if (allCards->karta.kolor[0] != 'b') {

            if (allCards->karta.kolor[0] == 'r') {
                colorCount[1]++;
            }
            if (allCards->karta.kolor[0] == 'y') {
                colorCount[3]++;
            }
            if (allCards->karta.kolor[0] == 'w') {
                colorCount[4]++;
            }
            if (allCards->karta.kolor[0] == 'v') {
                colorCount[2]++;
            }
        }
        else if (allCards->karta.kolor[2] == 'u') {
            colorCount[0]++;
        }
        else {
            colorCount[5]++;
        }
        allCards = allCards->next;
    }
    int ilosc;
    int flaga = 0;
    for (int i = 0; i < KOLORY; i++) {
        if (colorCount[i] > 0) {
            ilosc = colorCount[i];
        }
    }

    for (int i = 0; i < KOLORY; i++) {
        if (colorCount[i] > 0 and colorCount[i] != ilosc) {
            cout << "At least two colors with a different number of cards were found:" << endl;
            flaga++;
            break;
        }

    }

    if (flaga > 0) {
        for (int i = 0; i < KOLORY; i++) {
            if (colorCount[i] > 0) {
                cout << colors[i] << " cards are " << colorCount[i] << endl;
            }
        }
        cout << endl;
    }
    else {
        cout << "The number cards of all colors is equal: " << ilosc << endl;
        (*equal) = 1;
        (*equalSize) = ilosc;

        for (int i = 0; i < KOLORY; i++) {
            if (colorCount[i] > 0) {
                colorCheck[i] = i + 1;
            }
        }
    }
}

void swapTwo(intlist_t* a, intlist_t* b) {
    int bufor = a->value;
    a->value = b->value;
    b->value = bufor;
}

void bubbleSort(intlist_t* l) {
    l = l->next;
    int swapped;

    intlist_t* leftPointer;
    intlist_t* rightPointer = NULL;

    if (l != NULL) {
        do {
            swapped = 0;
            leftPointer = l;
            while (leftPointer->next != rightPointer) {
                if (leftPointer->value > leftPointer->next->value) {
                    swapTwo(leftPointer, leftPointer->next);
                    swapped = 1;
                }
                leftPointer = leftPointer->next;
            }
            rightPointer = leftPointer;
        } while (swapped == 1);
    }
}

void rozlozNaKolory(intlist_t** listOfColorCards, int* tabelaWszystkichWartosci, int* tabelaWszystkichKolorow, int allCardsTogether) {
    intlist_t* curZero = listOfColorCards[0];
    intlist_t* curOne = listOfColorCards[1];
    intlist_t* curTwo = listOfColorCards[2];
    intlist_t* curThree = listOfColorCards[3];
    intlist_t* curFour = listOfColorCards[4];
    intlist_t* curFive = listOfColorCards[5];

    for (int i = 0; i < allCardsTogether; i++) {
        if (tabelaWszystkichKolorow[i] == BLUE) {
            addElementInt(curZero, tabelaWszystkichWartosci[i]);
            curZero = curZero->next;
        }
        if (tabelaWszystkichKolorow[i] == RED) {
            addElementInt(curOne, tabelaWszystkichWartosci[i]);
            curOne = curOne->next;
        }
        if (tabelaWszystkichKolorow[i] == VIOLET) {
            addElementInt(curTwo, tabelaWszystkichWartosci[i]);
            curTwo = curTwo->next;
        }
        if (tabelaWszystkichKolorow[i] == YELLOW) {
            addElementInt(curThree, tabelaWszystkichWartosci[i]);
            curThree = curThree->next;
        }
        if (tabelaWszystkichKolorow[i] == WHITE) {
            addElementInt(curFour, tabelaWszystkichWartosci[i]);
            curFour = curFour->next;
        }
        if (tabelaWszystkichKolorow[i] == BLACK) {
            addElementInt(curFive, tabelaWszystkichWartosci[i]);
            curFive = curFive->next;
        }
    }

    for (int i = 0; i < KOLORY; i++) {
        bubbleSort(listOfColorCards[i]);
    }
}

void sprawdzKolory(intlist_t** listOfColorCards, int equal, const char** colors, int colorCheck[KOLORY], intlist_t** listOfBeg) {

    int equalValues = 0;
    int flaga = 0;
    int firstNotEmpty = 0;
    int notEqual = 0; //1 if not equal, 0 if equal

    for (int i = 0; i < KOLORY; i++) {
        listOfBeg[i] = listOfColorCards[i];
        if (colorCheck[i] > 0 and flaga == 0) {
            firstNotEmpty = colorCheck[i] - 1;
            flaga = 1;
        }
    }

    if (equal == 1) {
        for (int i = 0; i < KOLORY; i++) {
            listOfColorCards[firstNotEmpty] = listOfBeg[firstNotEmpty]->next;
            listOfColorCards[i] = listOfBeg[i]->next;

            if (notEqual == 0) {
                if (listOfBeg[i]->next != NULL and colorCheck[i] >= 0) {
                    while (listOfColorCards[firstNotEmpty] != NULL) {
                        if (listOfColorCards[firstNotEmpty]->value == listOfColorCards[i]->value) {
                            listOfColorCards[firstNotEmpty] = listOfColorCards[firstNotEmpty]->next;
                            listOfColorCards[i] = listOfColorCards[i]->next;
                        }
                        else {
                            notEqual = 1;
                            break;
                        }
                    }
                }
            }
            else {
                cout << "The values of cards of all colors are not identical: " << endl;
                for (int j = 0; j < KOLORY; j++) {
                    if (listOfBeg[j]->next != NULL) {
                        cout << colors[j] << " cards values: ";
                        printInt(listOfBeg[j]);
                        cout << endl;
                    }
                }
                break;
            }
        }
        if (notEqual == 0) {
            if (notEqual == 0) {
                cout << endl;
                cout << "The values of cards of all colors are identical: " << endl;
                printInt(listOfBeg[firstNotEmpty]);
                cout << endl;
            }
        }
    }
    else {
        cout << "The values of cards of all colors are not identical: " << endl;
        for (int j = 0; j < KOLORY; j++) {
            if (listOfBeg[j]->next != NULL) {
                cout << colors[j] << " cards values: ";
                printInt(listOfBeg[j]);
                cout << endl;
            }
        }
    }
}

int listSize(list_t* h) {
    h = h->next;
    int size = 0;
    while (h != NULL) {
        size++;
        h = h->next;
    }
    return size;
}

void sprawdzStan(list_t** players, int exploTreshold, list_t** pileCards, int numOfPlayers, int iloscPile, list_t** poczatekPile, int* flagaOne, int* flagaTwo, int* flagaThree) {
    const char* green = "green";


    for (int i = 0; i < iloscPile; i++) {
        poczatekPile[i] = pileCards[i];
    }

    for (int i = 1; i < numOfPlayers; i++) {
        if (abs(listSize(players[i - 1]) - listSize(players[i])) > 2) {
            cout << "The number of players cards on hand is wrong" << endl;
            (*flagaThree) = 1;
            break;
        }
    }
    for (int i = 0; i < iloscPile; i++) {
        pileCards[i] = pileCards[i]->next;
        while (pileCards[i]->next != NULL) {
            if (pileCards[i]->karta.kolor != pileCards[i]->next->karta.kolor) {
                if (pileCards[i]->karta.kolor != green and pileCards[i]->next->karta.kolor != green) {
                    cout << "Two different colors were found on the " << i + 1 << " pile" << endl;
                    (*flagaOne) = 1;
                    break;
                }
            }
            pileCards[i] = pileCards[i]->next;
        }
    }
    for (int i = 0; i < iloscPile; i++) {
        int sumaWartosci = 0;
        poczatekPile[i] = poczatekPile[i]->next;
        while (poczatekPile[i] != NULL) {
            sumaWartosci += poczatekPile[i]->karta.wartosc;
            poczatekPile[i] = poczatekPile[i]->next;
        }
        if (sumaWartosci > exploTreshold) {
            cout << "Pile number " << i + 1 << " should explode earlier" << endl;
            (*flagaTwo) = 1;
        }
    }
    if ((*flagaOne) + (*flagaTwo) + (*flagaThree) == 0) {
        cout << "Current state of the game is ok" << endl;
    }
}

void addLast(list_t* l, int value, const char* color) {
    list_t* beg = l;
    list_t* newNode = (list_t*)malloc(sizeof(list_t));
    newNode->karta.wartosc = value;
    newNode->karta.kolor = color;
    newNode->next = NULL;
    while (beg->next != NULL) {
        beg = beg->next;
    }
    beg->next = newNode;
    l = (list_t*)malloc(sizeof(list_t));
    l->next = beg;
}

void turn(list_t** players, list_t** pileCards, int numOfPlayers, int* activePlayer, int iloscPile) {
    const char* green = "green";
    players[(*activePlayer) - 1] = players[(*activePlayer) - 1]->next;
    if (players[(*activePlayer) - 1]->karta.kolor == green) {
        addLast(pileCards[0], players[(*activePlayer) - 1]->karta.wartosc, players[(*activePlayer) - 1]->karta.kolor);
        removeElementFront(players[(*activePlayer) - 1]);
        (*activePlayer)++;
        if ((*activePlayer) > numOfPlayers) {
            (*activePlayer) = 1;
        }
    }
}

int main() {

    int n; //l. graczy
    int k; //l. kotlow
    int g; //l. zielonych kart
    int gv; //wartosc zielonych kart
    int o; //l. kart innych kolorow
    int exploTreshold;
    int activePlayer;
    int iloscPile = 0;
    card* tabelaKart = NULL;
    int* tabelaWartosci = NULL;
    int* tabelaWartosciLoaded = NULL;
    int* tabelaKolorowLoaded = NULL;
    int* tabelaPileLoaded = NULL;
    int* tabelaWartosciDeck = NULL;
    int* tabelaKolorowDeck = NULL;
    int* tabelaWartosciPile = NULL;
    int* tabelaKolorowPile = NULL;
    int* tabelaWszystkichWartosci = NULL;
    int* tabelaWszystkichKolory = NULL;
    list_t** players = NULL;
    list_t** deckCards = NULL;
    list_t** pileCards = NULL;
    intlist_t** listOfColorCards = NULL;
    intlist_t** listOfBeg = NULL;
    list_t** poczatekPile = NULL;
    list_t allCards;

    const char* colors[KOLORY] = { "blue", "red", "violet", "yellow", "white", "black" };
    const char* colorsWithGreen[COLORSWITHGREEN] = { "green", "blue", "red", "violet", "yellow", "white", "black" };

    char option;

    cout << "l - wczytaj | g - wygeneruj" << endl;
    cin >> option;

    int sposob;
    switch (option) {
    case 'g':
        activePlayer = 1;
        sposob = 0;
        FILE* fpclear;
        fpclear = fopen("save.txt", "w");
        fclose(fpclear);
        free(fpclear);

        FILE* fp;
        fp = fopen("save.txt", "a");

        cin >> n >> k >> g >> gv >> o >> exploTreshold;
        sprawdzZmienne(n, k, g, gv, o);

        iloscPile = k;

        tabelaKart = (card*)malloc(o * k * sizeof(card) + g * sizeof(card));
        if (tabelaKart == NULL) {
            cout << "Allocation error" << endl;
            return 1;
        }

        tabelaWartosci = (int*)malloc(o * k * sizeof(int) + g * sizeof(int));
        if (tabelaWartosci == NULL) {
            cout << "Allocation error" << endl;
            return 1;
        }

        wczytajWartosci(tabelaWartosci, o, k, tabelaKart);
        stworzTalie(k, g, gv, o, colors, tabelaKart, tabelaWartosci);

        players = (list_t**)malloc(n * sizeof(list_t*));
        for (int i = 0; i < n; i++) {
            players[i] = (list_t*)malloc(sizeof(list_t));
            init(players[i]);
        }

        deckCards = (list_t**)malloc(n * sizeof(list_t*));
        for (int i = 0; i < n; i++) {
            deckCards[i] = (list_t*)malloc(sizeof(list_t));
            init(deckCards[i]);
        }

        pileCards = (list_t**)malloc(k * sizeof(list_t*));
        for (int i = 0; i < k; i++) {
            pileCards[i] = (list_t*)malloc(sizeof(list_t));
            init(pileCards[i]);
        }

        dajDoRak(o, k, g, n, players, tabelaKart);
        wypiszStanDoPliku(n, k, players, fp, &activePlayer, deckCards, pileCards, exploTreshold);

        fclose(fp);
        free(fp);

        break;

    case 'l':
        sposob = 1;
        int greenCount = 0;
        int cardCount = 0;
        int iloscKartPile = 0;
        int deckCardCount = 0;
        int greenValue;
        int greenValueCheck = 0;
        int equal = 0;
        int equalSize;
        int colorCheck[KOLORY]; //blue, red, violet, yellow, white, black
        int flagaOne = 0;
        int flagaTwo = 0;
        int flagaThree = 0;

        for (int i = 0; i < KOLORY; i++) {
            colorCheck[i] = -1;
        }

        wczytajStanWartosci(&cardCount, &greenCount, &greenValue, &greenValueCheck, &deckCardCount, &iloscPile, &iloscKartPile, &exploTreshold, &activePlayer);

        tabelaWartosciLoaded = (int*)malloc(cardCount * sizeof(int));
        if (tabelaWartosciLoaded == NULL) {
            cout << "Allocation error" << endl;
            return 1;
        }

        tabelaKolorowLoaded = (int*)malloc(cardCount * sizeof(int));
        if (tabelaKolorowLoaded == NULL) {
            cout << "Allocation error" << endl;
            return 1;
        }

        tabelaKolorowDeck = (int*)malloc(deckCardCount * sizeof(int));
        if (tabelaKolorowDeck == NULL) {
            cout << "Allocation error" << endl;
            return 1;
        }

        tabelaWartosciDeck = (int*)malloc(deckCardCount * sizeof(int));
        if (tabelaWartosciDeck == NULL) {
            cout << "Allocation error" << endl;
            return 1;
        }

        tabelaKolorowPile = (int*)malloc(iloscKartPile * sizeof(int));
        if (tabelaKolorowPile == NULL) {
            cout << "Allocation error" << endl;
            return 1;
        }

        tabelaWartosciPile = (int*)malloc(iloscKartPile * sizeof(int));
        if (tabelaWartosciDeck == NULL) {
            cout << "Allocation error" << endl;
            return 1;
        }

        int allCardsTogether = iloscKartPile + deckCardCount + cardCount;

        tabelaWszystkichWartosci = (int*)malloc(allCardsTogether * sizeof(int));
        if (tabelaWszystkichWartosci == NULL) {
            cout << "Allocation error" << endl;
            return 1;
        }

        tabelaWszystkichKolory = (int*)malloc(allCardsTogether * sizeof(int));
        if (tabelaWszystkichKolory == NULL) {
            cout << "Allocation error" << endl;
            return 1;
        }

        n = LiczbaGraczy();

        players = (list_t**)malloc(n * sizeof(list_t*));
        for (int i = 0; i < n; i++) {
            players[i] = (list_t*)malloc(sizeof(list_t));
            init(players[i]);
        }

        deckCards = (list_t**)malloc(n * sizeof(list_t*));
        for (int i = 0; i < n; i++) {
            deckCards[i] = (list_t*)malloc(sizeof(list_t));
            init(deckCards[i]);
        }

        pileCards = (list_t**)malloc(iloscPile * sizeof(list_t*));
        for (int i = 0; i < iloscPile; i++) {
            pileCards[i] = (list_t*)malloc(sizeof(list_t));
            init(pileCards[i]);
        }

        listOfColorCards = (intlist_t**)malloc(KOLORY * sizeof(intlist_t*));
        for (int i = 0; i < KOLORY; i++) {
            listOfColorCards[i] = (intlist_t*)malloc(sizeof(intlist_t));
            initInt(listOfColorCards[i]);
        }

        listOfBeg = (intlist_t**)malloc(KOLORY * sizeof(intlist_t*));
        for (int i = 0; i < KOLORY; i++) {
            listOfBeg[i] = (intlist_t*)malloc(sizeof(intlist_t));
            initInt(listOfBeg[i]);
        }

        poczatekPile = (list_t**)malloc(sizeof(list_t*));
        for (int i = 0; i < iloscPile; i++) {
            poczatekPile[i] = (list_t*)malloc(sizeof(list_t));
            init(poczatekPile[i]);
        }

        FILE* loadFileOga;
        loadFileOga = fopen("save.txt", "r");

        wczytajStan(n, tabelaWartosciLoaded, cardCount, tabelaKolorowLoaded, players, colorsWithGreen, deckCards, tabelaWartosciDeck, tabelaKolorowDeck, pileCards, tabelaWartosciPile, tabelaKolorowPile, iloscPile, tabelaWszystkichKolory, tabelaWszystkichWartosci, loadFileOga);

        if (greenCount == 0) {
            cout << "Green cards does not exist" << endl;
        }
        else {
            if (greenValueCheck > 1) {
                cout << "Different green cards values occurred" << endl;
            }
            else {
                cout << "Green number: " << greenCount << endl;
                cout << "Green value: " << greenValue << endl;
            }
        }

        cout << "Deck card number: " << deckCardCount << endl;
        cout << "Card number: " << cardCount << endl;
        cout << "Pile number: " << iloscKartPile << endl;
        cout << endl;

        cout << "hands: " << endl;
        for (int i = 0; i < n; i++) {
            print(players[i]);
            cout << endl;
        }
        cout << endl;
        cout << "decks: " << endl;
        for (int i = 0; i < n; i++) {
            print(deckCards[i]);
            cout << endl;
        }
        cout << endl;
        cout << "piles: " << endl;
        for (int i = 0; i < iloscPile; i++) {
            print(pileCards[i]);
            cout << endl;
        }
        cout << endl;

        init(&allCards);

        list_t* cur = &allCards;
        for (int i = 0; i < allCardsTogether; i++) {
            addElement(cur, tabelaWszystkichWartosci[i], colorsWithGreen[tabelaWszystkichKolory[i] - 1]);
            cur = cur->next;
        }

        colorNumber(&allCards, colors, sposob, &equal, &equalSize, colorCheck);
        rozlozNaKolory(listOfColorCards, tabelaWszystkichWartosci, tabelaWszystkichKolory, allCardsTogether);

        sprawdzKolory(listOfColorCards, equal, colors, colorCheck, listOfBeg);

        sprawdzStan(players, exploTreshold, pileCards, n, iloscPile, poczatekPile, &flagaOne, &flagaTwo, &flagaThree);

        cout << endl;
        break;
    }

    FILE* save;
    save = fopen("save.txt", "w");

    turn(players, pileCards, n, &activePlayer, iloscPile);

    wypiszStanDoPliku(n, iloscPile, players, save, &activePlayer, deckCards, pileCards, exploTreshold);

    free(tabelaKart);
    free(tabelaWartosci);
    free(tabelaWartosciLoaded);
    free(tabelaKolorowDeck);
    free(tabelaWartosciDeck);
    free(tabelaKolorowLoaded);
    free(tabelaKolorowPile);
    free(tabelaWartosciPile);
    free(tabelaWszystkichKolory);
    free(tabelaWszystkichWartosci);


    return 0;
}