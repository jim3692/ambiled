#include "common.h"

struct ImageAnalysisConfig {
    double minSaturation;
    double maxSaturation;
    double saturationWeight;

    double minLightness;
    double maxLightness;
    double lightnessWeight;
};

struct ImageAnalysis {
    int pixelCounts[COLORS_AMOUNT];
    double colorScores[COLORS_AMOUNT];

    uint64_t colorSum;
    double averageBrightness;

    union ColorIndex preferedColor;
    double preferedColorScore;
};

void load_image(MagickWand *mw, char* path);
void resize_image(MagickWand *mw, int width, int height);
void get_image_pixels(MagickWand *mw, struct Pixel* pixels);

void analyze_pixels(struct ImageAnalysis* analysis, struct Pixel* pixels, size_t pixels_count, struct ImageAnalysisConfig* config);
