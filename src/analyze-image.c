#include "analyze-image.h"
#include "common.h"

void load_image(MagickWand *mw, char *path) {
    MagickBooleanType status;

    status = MagickReadImage(mw, path);
    if (status == MagickFalse)
        ThrowWandException(mw);
}

void resize_image(MagickWand *mw, int width, int height) {
    MagickBooleanType status;

    status = MagickResizeImage(mw, width, height, LanczosFilter);
    if (status == MagickFalse)
        ThrowWandException(mw);
}

void get_image_pixels(MagickWand *mw, struct Pixel *pixels) {
    MagickBooleanType status;

    status = MagickSetImageDepth(mw, 8);
    if (status == MagickFalse)
        ThrowWandException(mw);

    size_t width = MagickGetImageWidth(mw);
    size_t height = MagickGetImageHeight(mw);

    status = MagickExportImagePixels(mw, 0, 0, width, height, "RGB", CharPixel, pixels);
    if (status == MagickFalse)
        ThrowWandException(mw);
}

void analyze_pixels(struct ImageAnalysis* analysis, struct Pixel* pixels, size_t pixels_count, struct ImageAnalysisConfig* config) {
    for (int i = 0; i < pixels_count; i++) {
        int r = pixels[i].r & COLORS_MASK;
        int g = pixels[i].g & COLORS_MASK;
        int b = pixels[i].b & COLORS_MASK;

        int cIdx = color_to_idx(&pixels[i]);
        analysis->pixelCounts[cIdx] += 1;

        analysis->colorSum += r;
        analysis->colorSum += g;
        analysis->colorSum += b;
    }

    PixelWand *pw = NewPixelWand();
    for (int i = 0; i < COLORS_AMOUNT; i++) {
        double r = idx_to_double(i, r);
        double g = idx_to_double(i, g);
        double b = idx_to_double(i, b);

        PixelSetRed(pw, r);
        PixelSetGreen(pw, g);
        PixelSetBlue(pw, b);

        double h, s, l;
        PixelGetHSL(pw, &h, &s, &l);

        if (
            s > config->minSaturation &&
            s < config->maxSaturation &&
            l > config->minLightness &&
            l < config->maxLightness
        ) {
            analysis->colorScores[i] =
                (double)analysis->pixelCounts[i]
                * (
                    (config->saturationWeight * s)
                    + (config->lightnessWeight * l)
                );
        }
    }
    DestroyPixelWand(pw);

    analysis->preferedColor.idx = 0;
    analysis->preferedColorScore = analysis->colorScores[0];
    for (int i = 1; i < COLORS_AMOUNT; i++) {
        double score = analysis->colorScores[i];
        if (score > analysis->preferedColorScore) {
            analysis->preferedColor.idx = i;
            analysis->preferedColorScore = score;
        }
    }

    analysis->averageBrightness = analysis->colorSum / (3.0 * pixels_count * COLORS_MASK / 100.0);
}
