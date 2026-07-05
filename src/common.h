#ifndef AMBILED_COMMON_H
#define AMBILED_COMMON_H

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <MagickWand/MagickWand.h>

#define COLORS_DEPTH 3
#define COLORS_AMOUNT (1 << (COLORS_DEPTH * 3))
#define COLORS_MASK (0x100 - (1 << (8 - COLORS_DEPTH)))

#define ThrowWandException(wand) \
{ \
  char \
    *description; \
 \
  ExceptionType \
    severity; \
 \
  description=MagickGetException(wand,&severity); \
  (void) fprintf(stderr,"%s %s %lu %s\n",GetMagickModule(),description); \
  description=(char *) MagickRelinquishMemory(description); \
  exit(-1); \
}

struct Config {
    int stdin;

    char* image;

    int should_resize;
    int resize_w;
    int resize_h;

    double min_saturation;
    double max_saturation;
    double saturation_weight;

    double min_lightness;
    double max_lightness;
    double lightness_weight;
};

struct HSL {
    double h, s, l;
};

struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

union ColorIndex {
    uint16_t idx;
    struct {
        unsigned int r: COLORS_DEPTH;
        unsigned int g: COLORS_DEPTH;
        unsigned int b: COLORS_DEPTH;
    } color;
};

int color_to_idx(struct Pixel *pixel);

void idx_to_color(int idx, struct Pixel *pixel);
void idx_to_hsl(int idx, double* h, double* s, double* l);

#define is_arg(arg) (!strcmp(argv[i], arg))

#define idx_to_double(idx, channel) (((*(union ColorIndex *)&idx).color.channel << (8 - COLORS_DEPTH)) / (double)COLORS_MASK)
#define idx_to_int(idx, channel) ((*(union ColorIndex *)&idx).color.channel << (8 - COLORS_DEPTH))

#endif
