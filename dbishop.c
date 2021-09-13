/*
 * MTAT.03.219 Programmeerimine C-keeles ja assembleris
 * Programm C keeles (projekt)
 * Anton Slavin
 * 2021/2022
 *
 * Algoritmi seletus ja analüüs:
 * http://www.dirk-loss.de/sshvis/drunken_bishop.pdf
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Kahe arvu võrdlus
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

// Laua suurus
static int w = 17;
static int h = 9;
// Jalutuskäigu liikumise suunad
static int moves[2] = {-1, 1};

struct Points {
    int x;
    int y;
};

void draw_board(char arr[h][w])
{
    puts("+-----------------+");

    for (int i = 0; i < h; i++) {
        printf("%s", "|");
        for (int j = 0; j < w; j++) {
            printf("%c", arr[i][j]);
        }
        puts("|");
    }

    puts("+-----------------+");
}

void insert_vals(int valc, struct Points vals[valc], char arr[h][w])
{
    // ...
    char CHARS[] = " .o+=*BOX@%&#/^SE";
    int counts[h][w];

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            counts[i][j] = 0;
        }
    }

    // ...
    for (int i = 0; i < valc; i++) {
        int y = vals[i].y;
        int x = vals[i].x;

        // Esimene sümbol
        if (i == 0) {
            arr[y][x] = CHARS[15];
            continue;
        }

        // Viimane sümbol
        if (i == valc-1) {
            arr[y][x] = CHARS[16];
            continue;
        }

        counts[y][x]++;
        arr[y][x] = CHARS[counts[y][x] % 14];
    }
}

void walk(int valc, struct Points vals[valc], char hex_str[])
{
    int validx = 1;
    // Sisendi läbimine kahe sümboli kaupa
    for (int i = 0; i < strlen(hex_str); i += 2) {
        // Valime sümbolid
        char hex_pair[3];
        strncpy(hex_pair, hex_str+i, 2);
        hex_pair[2] = '\0';
        // Konverteerime binaryks ...
        long int val = strtol(hex_pair, NULL, 16);

        // Eraldi bitide salvestamine
        int bits[8];
        for (int k = 0; k < 8; k++) {
            bits[k] = (val >> k) & 1;
        }

        // Kahe biti kaupa järgmise sammu arvutamine
        for (int bit = 1; bit < 8; bit += 2) {
            // Arvuta suunad
            int x = moves[bits[bit-1]] + vals[validx-1].x;
            int y = moves[bits[bit]] + vals[validx-1].y;

            // Paranda, kui läks lauast välja
            vals[validx].x = MIN(MAX(x, 0), 16);
            vals[validx].y = MIN(MAX(y, 0), 8);
            validx++;
        }
    }
}

int main() 
{
    // Sisend -> hex väärtus
    char hex_str[] = "d433fdc564d3ee9e97ca54213be4bae9";
    
    // Väljundi maatriks, esialgu kõik tühikud
    char arr[h][w];
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            arr[i][j] = ' ';
        }
    }

    // Sisendi läbimiseks punktide (koordinaatide) arv
    // ...
    int step_count = strlen(hex_str) * 2 + 1;
    // Koordinaadid 
    struct Points steps[step_count];

    // Esialgne punkt (keskel)
    steps[0].x = 8;
    steps[0].y = 4;

    // Punktide loomine -> jalutus
    walk(step_count, steps, hex_str);
    // Visualiseerimine
    insert_vals(step_count, steps, arr);
    draw_board(arr);

    return 0;
}


