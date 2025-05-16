#include <stdio.h>
#include <math.h>

// RGB → HSL 変換
void RGBtoHSL(unsigned char R, unsigned char G, unsigned char B, double *H, double *S, double *L) {
    double r = R / 255.0, g = G / 255.0, b = B / 255.0;
    double max = fmax(fmax(r, g), b);
    double min = fmin(fmin(r, g), b);
    *L = (max + min) / 2.0;

    if (max == min) {
        *S = 0.0;
        *H = 0.0;
    } else {
        double delta = max - min;
        *S = (*L < 0.5) ? (delta / (max + min)) : (delta / (2.0 - max - min));

        if (max == r)
            *H = 60.0 * fmod(((g - b) / delta), 6.0);
        else if (max == g)
            *H = 60.0 * (((b - r) / delta) + 2.0);
        else
            *H = 60.0 * (((r - g) / delta) + 4.0);

        if (*H < 0.0)
            *H += 360.0;
    }
}

// HSL → RGB 変換
void HSLtoRGB(double H, double S, double L, unsigned char *R, unsigned char *G, unsigned char *B) {
    double C = (1.0 - fabs(2.0 * L - 1.0)) * S;
    double X = C * (1.0 - fabs(fmod(H / 60.0, 2.0) - 1.0));
    double m = L - C / 2.0;
    double r, g, b;

    if (H < 60) { r = C; g = X; b = 0; }
    else if (H < 120) { r = X; g = C; b = 0; }
    else if (H < 180) { r = 0; g = C; b = X; }
    else if (H < 240) { r = 0; g = X; b = C; }
    else if (H < 300) { r = X; g = 0; b = C; }
    else { r = C; g = 0; b = X; }

    *R = (unsigned char)round((r + m) * 255);
    *G = (unsigned char)round((g + m) * 255);
    *B = (unsigned char)round((b + m) * 255);
}

// テスト用 main関数
int main() {
    double h, s, l;
    unsigned char r, g, b;

    // RGB → HSL テスト
    printf("RGB to HSL:\n");
    RGBtoHSL(0, 0, 0, &h, &s, &l);
    printf("RGB(0,0,0) -> HSL(%.1f, %.1f, %.1f)\n", h, s, l);

    RGBtoHSL(255, 0, 0, &h, &s, &l);
    printf("RGB(255,0,0) -> HSL(%.1f, %.1f, %.1f)\n", h, s, l);

    RGBtoHSL(0, 255, 0, &h, &s, &l);
    printf("RGB(0,255,0) -> HSL(%.1f, %.1f, %.1f)\n", h, s, l);

    RGBtoHSL(0, 0, 255, &h, &s, &l);
    printf("RGB(0,0,255) -> HSL(%.1f, %.1f, %.1f)\n", h, s, l);

    RGBtoHSL(255, 255, 255, &h, &s, &l);
    printf("RGB(255,255,255) -> HSL(%.1f, %.1f, %.1f)\n", h, s, l);

    RGBtoHSL(255, 255, 0, &h, &s, &l);
    printf("RGB(255,255,0) -> HSL(%.1f, %.1f, %.1f)\n", h, s, l);

    RGBtoHSL(128, 128, 128, &h, &s, &l);
    printf("RGB(128,128,128) -> HSL(%.1f, %.1f, %.1f)\n", h, s, l);

    // HSL → RGB テスト
    printf("\nHSL to RGB:\n");
    HSLtoRGB(0.0, 1.0, 0.5, &r, &g, &b);
    printf("HSL(0.0, 1.0, 0.5) -> RGB(%d, %d, %d)\n", r, g, b);

    HSLtoRGB(120.0, 1.0, 0.5, &r, &g, &b);
    printf("HSL(120.0, 1.0, 0.5) -> RGB(%d, %d, %d)\n", r, g, b);

    HSLtoRGB(240.0, 1.0, 0.5, &r, &g, &b);
    printf("HSL(240.0, 1.0, 0.5) -> RGB(%d, %d, %d)\n", r, g, b);

    HSLtoRGB(60.0, 1.0, 0.5, &r, &g, &b);
    printf("HSL(60.0, 1.0, 0.5) -> RGB(%d, %d, %d)\n", r, g, b);

    HSLtoRGB(0.0, 0.0, 0.0, &r, &g, &b);
    printf("HSL(0.0, 0.0, 0.0) -> RGB(%d, %d, %d)\n", r, g, b);

    return 0;
}
