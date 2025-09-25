#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
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

extern int color_to_idx(struct Pixel *pixel);

extern void idx_to_color(int idx, struct Pixel *pixel);

#define idx_to_double(idx, channel) (((*(union ColorIndex *)&idx).color.channel << (8 - COLORS_DEPTH)) / (double)COLORS_MASK)
#define idx_to_int(idx, channel) ((*(union ColorIndex *)&idx).color.channel << (8 - COLORS_DEPTH))

#endif
