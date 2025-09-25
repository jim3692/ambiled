#include "common.h"

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
