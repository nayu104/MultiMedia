#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 500
#define BIN 10

void DisplayHistogram(unsigned char (*Red)[MAX], unsigned char (*Green)[MAX], unsigned char (*Blue)[MAX], int size) {
    int hist[BIN] = {0};

    for (int i = 0; i < size; i++) {
        float brightness = ((*Red)[i] + (*Green)[i] + (*Blue)[i]) / (3.0 * 255.0);
        int bin = (int)(brightness * BIN);
        if (bin >= BIN) bin = BIN - 1;
        hist[bin]++;
    }

    printf("■ 明度ヒストグラム\n");
    for (int i = 0; i < BIN; i++) {
        printf("%.1f\t%d\n", (float)i / BIN, hist[i]);
    }
}

int ReadImage(const char* filename, unsigned char (*Red)[MAX], unsigned char (*Green)[MAX], unsigned char (*Blue)[MAX]) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;

    char header[3];
    int width, height, maxval;
    fscanf(file, "%s %d %d %d", header, &width, &height, &maxval);

    int total = width * height;
    for (int i = 0; i < total; i++) {
        int r, g, b;
        fscanf(file, "%d %d %d", &r, &g, &b);
        (*Red)[i] = r;
        (*Green)[i] = g;
        (*Blue)[i] = b;
    }
    fclose(file);
    return total;
}

void WriteImage(const char* filename, unsigned char (*Red)[MAX], unsigned char (*Green)[MAX], unsigned char (*Blue)[MAX], int size, int width, int height) {
    FILE* file = fopen(filename, "w");
    fprintf(file, "P3\n%d %d\n255\n", width, height);
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d %d %d\n", (*Red)[i], (*Green)[i], (*Blue)[i]);
    }
    fclose(file);
}

void BinarizeImage(unsigned char (*Red)[MAX], unsigned char (*Green)[MAX], unsigned char (*Blue)[MAX],
                   unsigned char (*outRed)[MAX], unsigned char (*outGreen)[MAX], unsigned char (*outBlue)[MAX],
                   int size, float threshold) {
    for (int i = 0; i < size; i++) {
        float brightness = ((*Red)[i] + (*Green)[i] + (*Blue)[i]) / (3.0 * 255.0);
        unsigned char value = (brightness >= threshold) ? 255 : 0;
        (*outRed)[i] = (*outGreen)[i] = (*outBlue)[i] = value;
    }
}

void PosterizeImage(unsigned char (*Red)[MAX], unsigned char (*Green)[MAX], unsigned char (*Blue)[MAX],
                    unsigned char (*outRed)[MAX], unsigned char (*outGreen)[MAX], unsigned char (*outBlue)[MAX],
                    int size, int levels) {
    for (int i = 0; i < size; i++) {
        float r = (*Red)[i] / 255.0;
        float g = (*Green)[i] / 255.0;
        float b = (*Blue)[i] / 255.0;
        float brightness = (r + g + b) / 3.0;

        float level_step = 1.0 / (levels - 1);
        float quant = round(brightness / level_step) * level_step;

        unsigned char value = (unsigned char)(fmin(quant * 255, 255));
        (*outRed)[i] = (*outGreen)[i] = (*outBlue)[i] = value;
    }
}

int main() {
    unsigned char Red[MAX], Green[MAX], Blue[MAX];
    unsigned char outRed[MAX], outGreen[MAX], outBlue[MAX];
    const int width = 1000, height = 750;

    // 入力ファイル
    const char* input = "photo_yacht_std.ppm";
    int size = ReadImage(input, &Red, &Green, &Blue);
    if (size < 0) {
        printf("画像ファイルが読み込めませんでした。\n");
        return 1;
    }

    // 課題1: 二値化処理
    float threshold = 0.5;
    BinarizeImage(&Red, &Green, &Blue, &outRed, &outGreen, &outBlue, size, threshold);
    WriteImage("output_binary.ppm", &outRed, &outGreen, &outBlue, size, width, height);
    DisplayHistogram(&outRed, &outGreen, &outBlue, size);

    // 課題2: ポスタリゼーション処理（5段階）
    int steps = 5;
    PosterizeImage(&Red, &Green, &Blue, &outRed, &outGreen, &outBlue, size, steps);
    WriteImage("output_posterize.ppm", &outRed, &outGreen, &outBlue, size, width, height);
    DisplayHistogram(&outRed, &outGreen, &outBlue, size);

    printf("✅ 処理が完了しました。\n");
    return 0;
}
