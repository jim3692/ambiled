#include "common.h"
#include "analyze-image.h"

void handle_image(MagickWand *magick_wand, struct Config* cfg);

int main(int argc, char *argv[]) {
    struct Config config = {
        .stdin = 0,

        .image = "x:root",

        .should_resize = 1,
        .resize_w = 300,
        .resize_h = 200,

        .min_saturation = 0.3,
        .max_saturation = INFINITY,
        .saturation_weight = 3.0,

        .min_lightness = 0.2,
        .max_lightness = 0.8,
        .lightness_weight = 1.2,
    };

    for (int i = 1; i < argc; i++) {
        if (is_arg("--stdin")) {
            config.stdin = 1;
            continue;
        }

        if (is_arg("--image")) {
            config.image = argv[i + 1];
            i += 1;
            continue;
        }

        if (is_arg("--no-resize")) {
            config.should_resize = 0;
            config.resize_w = atoi(argv[i + 1]);
            config.resize_h = atoi(argv[i + 2]);
            i += 2;
            continue;
        }

        if (is_arg("--min-saturation")) {
            config.min_saturation = atof(argv[i + 1]);
            i += 1;
            continue;
        }

        if (is_arg("--max-saturation")) {
            config.max_saturation = atof(argv[i + 1]);
            i += 1;
            continue;
        }

        if (is_arg("--saturation-weight")) {
            config.saturation_weight = atof(argv[i + 1]);
            i += 1;
            continue;
        }

        if (is_arg("--min-lightness")) {
            config.min_lightness = atof(argv[i + 1]);
            i += 1;
            continue;
        }

        if (is_arg("--max-lightness")) {
            config.max_lightness = atof(argv[i + 1]);
            i += 1;
            continue;
        }

        if (is_arg("--lightness-weight")) {
            config.lightness_weight = atof(argv[i + 1]);
            i += 1;
            continue;
        }
    }

    MagickWandGenesis();
    MagickWand *mw = NewMagickWand();

    if (!config.stdin) {
        handle_image(mw, &config);
        return 0;
    }

    size_t line_len;
    ssize_t read;
    config.image = 0;
    while ((read = getline(&config.image, &line_len, stdin)) > 0) {
        config.image[read - 1] = 0;
        handle_image(mw, &config);
        free(config.image);
        config.image = 0;
    }

    return 0;
}

void handle_image(MagickWand *mw, struct Config* cfg) {
    load_image(mw, cfg->image);

    if (cfg->should_resize) {
        resize_image(mw, cfg->resize_w, cfg->resize_h);
    }

    size_t w = MagickGetImageWidth(mw);
    size_t h = MagickGetImageHeight(mw);
    int total_pixels = w * h;

    struct Pixel pixels[total_pixels];
    get_image_pixels(mw, pixels);

    struct ImageAnalysis analysis;
    analyze_pixels(&analysis, pixels, total_pixels, cfg);

    printf(
        "{ \"state\": \"{ \\\"rgb_color\\\": [ %d, %d, %d ], \\\"brightness_pct\\\": %d, \\\"transition\\\": 0 }\" }",
        idx_to_int(analysis.preferedColor, r),
        idx_to_int(analysis.preferedColor, g),
        idx_to_int(analysis.preferedColor, b),
        (int)analysis.averageBrightness
    );
}
