#include <math.h>

#include "common.h"
#include "analyze-image.h"

const int resize_width = 300;
const int resize_height = 200;
const int total_pixels = resize_width * resize_height;

int main() {
    MagickWandGenesis();
    MagickWand *magick_wand = NewMagickWand();

    load_image(magick_wand, "x:root");
    resize_image(magick_wand, resize_width, resize_height);

    struct Pixel *pixels = (struct Pixel*)malloc(sizeof(struct Pixel) * total_pixels);
    get_image_pixels(magick_wand, pixels);

    struct ImageAnalysisConfig cfg;

    cfg.minSaturation = 0.3;
    cfg.maxSaturation = INFINITY;
    cfg.saturationWeight = 3.0;

    cfg.minLightness = 0.2;
    cfg.maxLightness = 0.8;
    cfg.lightnessWeight = 1.2;

    struct ImageAnalysis* analysis = (struct ImageAnalysis *)calloc(1, sizeof(struct ImageAnalysis));
    analyze_pixels(analysis, pixels, total_pixels, &cfg);

    printf(
        "{ \"state\": \"{ \\\"rgb_color\\\": [ %d, %d, %d ], \\\"brightness_pct\\\": %d, \\\"transition\\\": 0 }\" }",
        idx_to_int(analysis->preferedColor, r),
        idx_to_int(analysis->preferedColor, g),
        idx_to_int(analysis->preferedColor, b),
        (int)analysis->averageBrightness
    );
}
