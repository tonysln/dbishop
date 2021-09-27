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
#include <ctype.h>

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
            printf("%c", arr[i][j] < 32 ? ' ' : arr[i][j]);
        }
        puts("|");
    }

    puts("+-----------------+");
}

void insert_vals(int valc, struct Points vals[valc], char arr[h][w])
{ 
    char CHARS[] = " .o+=*BOX@%&#/^SE";
    int counts[h][w];

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            counts[i][j] = 0;
        }
    }

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
    int str_size = strlen(hex_str);
    // Sisendi läbimine kahe sümboli kaupa
    for (int i = 0; i < str_size; i += 2) {
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

int main(int argc, char *argv[]) 
{
    // Olgu sisendi maksimaalne pikkus 32 sümbolit
    char hex_str[33];
    int hex_len = 32;

    if (argc == 2) {
        // Hex väärtus on antud kasutaja poolt
        int input_len = strlen(argv[1]);

        if (input_len > 32) {
            puts("Sisendi pikkus ei saa olla > 32 symbolit.");
            return(EXIT_FAILURE);
        }
        
        for (int i = 0; i < input_len; i++) {
            char c = argv[1][i];
            if (isxdigit(c) == 0) {
                puts("Sisend peab olema hex value kujul.");
                return(EXIT_FAILURE);
            }
        }

        strncpy(hex_str, argv[1], input_len);
        hex_len = input_len;
    } else {
        // Loo juhuslik väärtus
        // Salvesta vahepealne väärtus
        char *rand_hex = malloc((2*sizeof(char)*16)+1);
        FILE *fp;
        fp = fopen("/dev/urandom", "r");
        if (fp == NULL) {
            puts("Failed to open /dev/urandom.");
            return(EXIT_FAILURE);
        }
        // Salvesta väärtused hex kujul
        for (int i = 0; i < 16; i++) {
            sprintf(rand_hex, "%s%02x", rand_hex, fgetc(fp));
        }
        // Kopeeri väärtus üle
        strncpy(hex_str, rand_hex, hex_len);
        free(rand_hex);
    }

    // Väljundi maatriks, esialgu kõik tühikud
    char arr[h][w];
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            arr[i][j] = ' ';
        }
    }

    hex_str[hex_len] = '\0';

    // Sisendi läbimiseks punktide (koordinaatide) arv
    int step_count = hex_len * 2 + 1;
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

