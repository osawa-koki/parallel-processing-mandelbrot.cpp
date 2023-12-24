#include <iostream>
#include <fstream>
#include <png.h>
#include <string>

#include "interface.hpp"
#include "function.hpp"
#include "const.hpp"

void exec_single()
{
    // 画像のデータを格納する配列を確保する
    png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * HEIGHT);
    for (int y = 0; y < HEIGHT; y++)
    {
        row_pointers[y] = (png_byte *)malloc(sizeof(png_byte) * WIDTH * 4);
    }

    for (int y = 0; y < HEIGHT; y++)
    {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < WIDTH; x++)
        {
            png_bytep px = &(row[x * 4]);

            double x0 = X_MIN + (X_MAX - X_MIN) * x / WIDTH;
            double y0 = Y_MIN + (Y_MAX - Y_MIN) * y / HEIGHT;
            double x1 = 0.0;
            double y1 = 0.0;
            int i = 0;
            while (x1 * x1 + y1 * y1 <= 2 * 2 && i < MAX_ITERATIONS)
            {
                double x2 = x1 * x1 - y1 * y1 + x0;
                double y2 = 2 * x1 * y1 + y0;
                x1 = x2;
                y1 = y2;
                i++;
            }

            // 計算結果を色に変換する
            int color = i * 255 / MAX_ITERATIONS + COLOR_HUE_BASE;
            HSL *hsl = new HSL();
            hsl->h = color;
            hsl->s = 100;
            hsl->l = 50;
            RGB *rgb = hsl_to_rgb(hsl);
            px[0] = rgb->r;
            px[1] = rgb->g;
            px[2] = rgb->b;
            px[3] = 255;
        }
    }

    // 画像をファイルに出力する
    ofstream output_file("mandelbrot-single-thread.png", ios::binary);
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    png_set_write_fn(
        png, &output_file, [](png_structp png, png_bytep data, png_size_t length)
        { ((ostream *)png_get_io_ptr(png))->write((char *)data, length); },
        nullptr);
    png_set_IHDR(
        png,
        info,
        WIDTH,
        HEIGHT,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png, info);
    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    // メモリを解放する
    for (int y = 0; y < HEIGHT; y++)
    {
        free(row_pointers[y]);
    }
    free(row_pointers);

    return;
}
