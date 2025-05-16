#include <stdio.h>

#define MAXS (100)//1000にすると巨大すぎて動きません・・・

// 画像読み込み関数
void ReadImage(char *Filename,
               unsigned char red[MAXS][MAXS],
               unsigned char green[MAXS][MAXS],
               unsigned char blue[MAXS][MAXS],
               long *pWidth, long *pHeight) {
    FILE *pFile = NULL;
    char Line1[100], Line2[100];
    long Width = 0, Height = 0;
    long Maximum = 0;
    long i, j;
    unsigned char R, G, B;

    pFile = fopen(Filename, "r");

    fgets(Line1, sizeof(Line1), pFile);   // P3
    fgets(Line2, sizeof(Line2), pFile);   // #Comment
    fscanf(pFile, "%ld %ld", &Width, &Height); // サイズ
    fscanf(pFile, "%ld", &Maximum);            // 最大値

    // 画像データ読み込み
    for (i = 0; i < Height; i++) {
        for (j = 0; j < Width; j++) {
            if (fscanf(pFile, "%hhu", &R) != 1) break;
            if (fscanf(pFile, "%hhu", &G) != 1) break;
            if (fscanf(pFile, "%hhu", &B) != 1) break;
            red[i][j] = R;
            green[i][j] = G;
            blue[i][j] = B;
        }
    }

    *pWidth = Width;
    *pHeight = Height;

    fclose(pFile);
    pFile = fopen(Filename, "r");

}

void WriteImage(char *Filename,
                unsigned char red[MAXS][MAXS],
                unsigned char green[MAXS][MAXS],
                unsigned char blue[MAXS][MAXS],
                long Width, long Height) {
    FILE *pFile = NULL;
    long i, j;

    pFile = fopen(Filename, "w");
   

    fprintf(pFile, "P3\n");
    fprintf(pFile, "#Output Image\n");
    fprintf(pFile, "%ld %ld\n", Width, Height);
    fprintf(pFile, "255\n");

    for (i = 0; i < Height; i++) {
        for (j = 0; j < Width; j++) {
            fprintf(pFile, "%d %d %d\n", red[i][j], green[i][j], blue[i][j]);
        }
    }

    fclose(pFile);
}

// メイン関数
int main() {

    unsigned char red[MAXS][MAXS], green[MAXS][MAXS], blue[MAXS][MAXS];
    long width, height;

    ReadImage("sample.ppm", red, green, blue, &width, &height);
    WriteImage("sample_out.ppm", red, green, blue, width, height);

    return 0;
}
