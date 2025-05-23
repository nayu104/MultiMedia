#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 1000000
#define BIN 10

// 入出力画像バッファ
unsigned char Red[MAX], Green[MAX], Blue[MAX];
unsigned char outRed[MAX], outGreen[MAX], outBlue[MAX];

// PPM画像の読み込み
int ReadImage(const char* filename, unsigned char Red[], unsigned char Green[], unsigned char Blue[]) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;

    char header[3];
    int width, height, maxval;
    fscanf(file, "%s %d %d %d", header, &width, &height, &maxval);

    int total = width * height;
    for (int i = 0; i < total; i++) {
        int r, g, b;
        fscanf(file, "%d %d %d", &r, &g, &b);
        Red[i] = r;
        Green[i] = g;
        Blue[i] = b;
    }
    fclose(file);
    return total;
}

// PPM画像の書き込み
void WriteImage(const char* filename, unsigned char Red[], unsigned char Green[], unsigned char Blue[],
                int size, int width, int height) {
    FILE* file = fopen(filename, "w");
    fprintf(file, "P3\n%d %d\n255\n", width, height);
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d %d %d\n", Red[i], Green[i], Blue[i]);
    }
    fclose(file);
}

// 平均化ぼかし（課題1）
void BlurImage(unsigned char inRed[], unsigned char inGreen[], unsigned char inBlue[],
               unsigned char outRed[], unsigned char outGreen[], unsigned char outBlue[],
               int width, int height, int filterSize) {
    int half = filterSize / 2;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int sumR = 0, sumG = 0, sumB = 0, count = 0;

            for (int dy = -half; dy <= half; dy++) {
                for (int dx = -half; dx <= half; dx++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        int index = ny * width + nx;
                        sumR += inRed[index];
                        sumG += inGreen[index];
                        sumB += inBlue[index];
                        count++;
                    }
                }
            }

            int index = y * width + x;
            outRed[index] = sumR / count;
            outGreen[index] = sumG / count;
            outBlue[index] = sumB / count;
        }
    }
}

// 微分（エッジ抽出）（課題2）
void DifferentiateImage(unsigned char inRed[], unsigned char inGreen[], unsigned char inBlue[],
                        unsigned char outRed[], unsigned char outGreen[], unsigned char outBlue[],
                        int width, int height) {
    int kernelX[3][3] = {
        { -1, 0, 1 },
        { -2, 0, 2 },
        { -1, 0, 1 }
    };
    int kernelY[3][3] = {
        { -1, -2, -1 },
        {  0,  0,  0 },
        {  1,  2,  1 }
    };

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int gx = 0, gy = 0;

            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int pixel = inRed[(y + dy) * width + (x + dx)];
                    gx += kernelX[dy + 1][dx + 1] * pixel;
                    gy += kernelY[dy + 1][dx + 1] * pixel;
                }
            }

            int val = sqrt(gx * gx + gy * gy);
            if (val > 255) val = 255;
            if (val < 0) val = 0;

            int index = y * width + x;
            outRed[index] = outGreen[index] = outBlue[index] = val;
        }
    }
}

// メイン関数
int main() {
    const char* inputFile = "photo_yacht_std.ppm";
    int width = 1000, height = 750;
    int size = ReadImage(inputFile, Red, Green, Blue);
    if (size < 0) {
        printf("❌ 入力画像の読み込みに失敗しました。\n");
        return 1;
    }

    // 課題1：平均化ぼかし
    int filterSize = 5;
    BlurImage(Red, Green, Blue, outRed, outGreen, outBlue, width, height, filterSize);
    WriteImage("output_blur.ppm", outRed, outGreen, outBlue, size, width, height);
    printf("✅ 課題1（平均化）完了：output_blur.ppm\n");

    // 課題2：微分（エッジ抽出）
    DifferentiateImage(Red, Green, Blue, outRed, outGreen, outBlue, width, height);
    WriteImage("output_edge.ppm", outRed, outGreen, outBlue, size, width, height);
    printf("✅ 課題2（エッジ検出）完了：output_edge.ppm\n");

    return 0;
}
