#ifndef LEDTRIX_CHARS_H
#define LEDTRIX_CHARS_H

const int NUMBER_0[5][3] = {{1, 1, 1},{1, 0, 1},{1, 0, 1},{1, 0, 1},{1, 1, 1}};
const int NUMBER_1[5][3] = {{0, 1, 0},{0, 1, 0},{0, 1, 0},{0, 1, 0},{0, 1, 0}};
const int NUMBER_2[5][3] = {{1, 1, 1},{0, 0, 1},{1, 1, 1},{1, 0, 0},{1, 1, 1}};
const int NUMBER_3[5][3] = {{1, 1, 1},{0, 0, 1},{0, 1, 1},{0, 0, 1},{1, 1, 1}};
const int NUMBER_4[5][3] = {{1, 0, 0},{1, 0, 1},{1, 1, 1},{0, 0, 1},{0, 0, 1}};
const int NUMBER_5[5][3] = {{1, 1, 1},{1, 0, 0},{1, 1, 1},{0, 0, 1},{1, 1, 1}};
const int NUMBER_6[5][3] = {{1, 1, 1},{1, 0, 0},{1, 1, 1},{1, 0, 1},{1, 1, 1}};
const int NUMBER_7[5][3] = {{1, 1, 1},{0, 0, 1},{0, 1, 1},{0, 0, 1},{0, 0, 1}};
const int NUMBER_8[5][3] = {{1, 1, 1},{1, 0, 1},{1, 1, 1},{1, 0, 1},{1, 1, 1}};
const int NUMBER_9[5][3] = {{1, 1, 1},{1, 0, 1},{1, 1, 1},{0, 0, 1},{1, 1, 1}};
const int LETTER_A[5][3] = {{1, 1, 1},{1, 0, 1},{1, 1, 1},{1, 0, 1},{1, 0, 1}};
const int LETTER_B[5][3] = {{1, 1, 1},{1, 0, 1},{1, 1, 0},{1, 0, 1},{1, 1, 1}};
const int LETTER_C[5][3] = {{1, 1, 1},{1, 0, 0},{1, 0, 0},{1, 0, 0},{1, 1, 1}};
const int LETTER_D[5][3] = {{1, 1, 0},{1, 0, 1},{1, 0, 1},{1, 0, 1},{1, 1, 0}};
const int LETTER_E[5][3] = {{1, 1, 1},{1, 0, 0},{1, 1, 1},{1, 0, 0},{1, 1, 1}};
const int LETTER_F[5][3] = {{1, 1, 1},{1, 0, 0},{1, 1, 1},{1, 0, 0},{1, 0, 0}};
const int LETTER_G[5][3] = {{1, 1, 1},{1, 0, 0},{1, 0, 1},{1, 0, 1},{1, 1, 1}};
const int LETTER_H[5][3] = {{1, 0, 1},{1, 0, 1},{1, 1, 1},{1, 0, 1},{1, 0, 1}};
const int LETTER_I[5][3] = {{0, 1, 0},{0, 1, 0},{0, 1, 0},{0, 1, 0},{0, 1, 0}};


void loadChar(int c[5][5], const int v[5][3]) {
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            c[y][x] = (x == 0 || x == 4) ? 0 : v[y][x - 1];
        }
    }
}

int getChar(int character[5][5], int code) {
    switch (code) {
        case 0:
            loadChar(character, NUMBER_0);
            break;
        case 1:
            loadChar(character, NUMBER_1);
            break;
        case 2:
            loadChar(character, NUMBER_2);
            break;
        case 3:
            loadChar(character, NUMBER_3);
            break;
        case 4:
            loadChar(character, NUMBER_4);
            break;
        case 5:
            loadChar(character, NUMBER_5);
            break;
        case 6:
            loadChar(character, NUMBER_6);
            break;
        case 7:
            loadChar(character, NUMBER_7);
            break;
        case 8:
            loadChar(character, NUMBER_8);
            break;
        case 9:
            loadChar(character, NUMBER_9);
            break;
        case 10:
            loadChar(character, LETTER_A);
            break;
        case 11:
            loadChar(character, LETTER_B);
            break;
        case 12:
            loadChar(character, LETTER_C);
            break;
        case 13:
            loadChar(character, LETTER_D);
            break;
        case 14:
            loadChar(character, LETTER_E);
            break;
        case 15:
            loadChar(character, LETTER_F);
            break;
        case 16:
            loadChar(character, LETTER_G);
            break;
        case 17:
            loadChar(character, LETTER_H);
            break;
        case 18:
            loadChar(character, LETTER_I);
            break;
        default:
            printf("char not found!\n");
            return 0;
    }
    return 1;
}

#endif //LEDTRIX_CHARS_H
