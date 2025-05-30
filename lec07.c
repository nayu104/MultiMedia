#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXS 1000

unsigned char red[MAXS][MAXS], green[MAXS][MAXS], blue[MAXS][MAXS];
unsigned char outR[MAXS][MAXS], outG[MAXS][MAXS], outB[MAXS][MAXS];

void ReadImage(char *Filename, unsigned char r[MAXS][MAXS], unsigned char g[MAXS][MAXS], unsigned char b[MAXS][MAXS], long *w, long *h) {
    FILE *fp = fopen(Filename, "r");
    char buf[100];
    long width, height, maxval;

    fgets(buf, 100, fp); 
    do {
        fgets(buf, 100, fp); 
    } while (buf[0] == '#');
    sscanf(buf, "%ld %ld", &width, &height);
    fscanf(fp, "%ld", &maxval);

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            int r_, g_, b_;
            fscanf(fp, "%d %d %d", &r_, &g_, &b_);
            r[i][j] = r_;
            g[i][j] = g_;
            b[i][j] = b_;
        }

    fclose(fp);
    *w = width;
    *h = height;
}

// 出力画像保存
void WriteImage(char *Filename, unsigned char r[MAXS][MAXS], unsigned char g[MAXS][MAXS], unsigned char b[MAXS][MAXS], long w, long h) {
    FILE *fp = fopen(Filename, "w");
    fprintf(fp, "P3\n#Output Image\n%ld %ld\n255\n", w, h);
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            fprintf(fp, "%d %d %d\n", r[i][j], g[i][j], b[i][j]);
    fclose(fp);
}

// ラプラシアンフィルタによるエッジ抽出
void LaplacianImage(unsigned char Red[MAXS][MAXS], unsigned char Green[MAXS][MAXS], unsigned char Blue[MAXS][MAXS],
                    unsigned char RedOut[MAXS][MAXS], unsigned char GreenOut[MAXS][MAXS], unsigned char BlueOut[MAXS][MAXS],
                    long width, long height, long alpha) {
    long LapR[MAXS][MAXS], LapG[MAXS][MAXS], LapB[MAXS][MAXS];

    for (int j = 1; j < height - 1; j++) {
        for (int i = 1; i < width - 1; i++) {
            LapR[j][i] = -4 * Red[j][i] + Red[j][i - 1] + Red[j][i + 1] + Red[j - 1][i] + Red[j + 1][i];
            LapG[j][i] = -4 * Green[j][i] + Green[j][i - 1] + Green[j][i + 1] + Green[j - 1][i] + Green[j + 1][i];
            LapB[j][i] = -4 * Blue[j][i] + Blue[j][i - 1] + Blue[j][i + 1] + Blue[j - 1][i] + Blue[j + 1][i];
        }
    }

    for (int j = 1; j < height - 1; j++) {
        for (int i = 1; i < width - 1; i++) {
            if ((LapR[j][i] * LapR[j][i + 1] < 0 && abs(Red[j][i] - Red[j][i + 1]) >= alpha) ||
                (LapR[j][i] * LapR[j + 1][i] < 0 && abs(Red[j][i] - Red[j + 1][i]) >= alpha)) {
                RedOut[j][i] = GreenOut[j][i] = BlueOut[j][i] = 255;
            } else {
                RedOut[j][i] = GreenOut[j][i] = BlueOut[j][i] = 0;
            }
        }
    }
}

int main() {
    long width, height;
    long alpha = 10;

    ReadImage("photo_yacht_std.ppm", red, green, blue, &width, &height);
    LaplacianImage(red, green, blue, outR, outG, outB, width, height, alpha);
    WriteImage("output_laplacian.ppm", outR, outG, outB, width, height);

    return 0;
}

