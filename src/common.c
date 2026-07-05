#include "common.h"

struct HSL *hsl_cache[COLORS_AMOUNT];

int color_to_idx(struct Pixel *pixel) {
    union ColorIndex cIdx;
    cIdx.color.r = (pixel->r & COLORS_MASK) >> (8 - COLORS_DEPTH);
    cIdx.color.g = (pixel->g & COLORS_MASK) >> (8 - COLORS_DEPTH);
    cIdx.color.b = (pixel->b & COLORS_MASK) >> (8 - COLORS_DEPTH);

    return cIdx.idx;
}

void idx_to_color(int idx, struct Pixel *pixel) {
    union ColorIndex cIdx;
    cIdx.idx = idx;

    pixel->r = cIdx.color.r << (8 - COLORS_DEPTH);
    pixel->g = cIdx.color.g << (8 - COLORS_DEPTH);
    pixel->b = cIdx.color.b << (8 - COLORS_DEPTH);
}

void idx_to_hsl(int idx, double* h, double* s, double* l) {
    static PixelWand* pw;
    if (!pw) {
        pw = NewPixelWand();
    }

    if (hsl_cache[idx]) {
        *h = hsl_cache[idx]->h;
        *s = hsl_cache[idx]->s;
        *l = hsl_cache[idx]->l;
        return;
    }

    double r = idx_to_double(idx, r);
    double g = idx_to_double(idx, g);
    double b = idx_to_double(idx, b);

    PixelSetRed(pw, r);
    PixelSetGreen(pw, g);
    PixelSetBlue(pw, b);

    PixelGetHSL(pw, h, s, l);
}
