#include <iostream>
#include <fstream>
#include <png.h>
#include <string>
#include <thread>

#include "interface.hpp"
#include "function.hpp"

void exec_multi()
{
    int width = 1024;
    int height = 1024;

    double x_min = -2.2;
    double x_max = 0.8;
    double y_min = -1.5;
    double y_max = 1.5;

    int color_hue_base = 0;
    int max_iterations = 30;

    // 画像のデータを格納する配列を確保する
    png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++)
    {
        row_pointers[y] = (png_byte *)malloc(sizeof(png_byte) * width * 4);
    }

    int thread_num = 4;
    int thread_height = height / thread_num;
    std::thread threads[thread_num];
    for (int i = 0; i < thread_num; i++)
    {
        threads[i] = std::thread(
            [i, thread_height, width, height, x_min, x_max, y_min, y_max, color_hue_base, max_iterations, row_pointers]()
            {
                for (int y = i * thread_height; y < (i + 1) * thread_height; y++)
                {
                    png_bytep row = row_pointers[y];
                    for (int x = 0; x < width; x++)
                    {
                        png_bytep px = &(row[x * 4]);

                        double x0 = x_min + (x_max - x_min) * x / width;
                        double y0 = y_min + (y_max - y_min) * y / height;
                        double x1 = 0.0;
                        double y1 = 0.0;
                        int i = 0;
                        while (x1 * x1 + y1 * y1 <= 2 * 2 && i < max_iterations)
                        {
                            double x2 = x1 * x1 - y1 * y1 + x0;
                            double y2 = 2 * x1 * y1 + y0;
                            x1 = x2;
                            y1 = y2;
                            i++;
                        }

                        // 計算結果を色に変換する
                        int color = i * 255 / max_iterations + color_hue_base;
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
            });
    }

    for (int i = 0; i < thread_num; i++)
    {
        threads[i].join();
    }

    // 画像をファイルに出力する
    ofstream output_file("mandelbrot-multi-thread.png", ios::binary);
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    png_set_write_fn(
        png, &output_file, [](png_structp png, png_bytep data, png_size_t length)
        { ((ostream *)png_get_io_ptr(png))->write((char *)data, length); },
        nullptr);
    png_set_IHDR(
        png,
        info,
        width, height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png, info);
    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    // メモリを解放する
    for (int y = 0; y < height; y++)
    {
        free(row_pointers[y]);
    }
    free(row_pointers);

    return;
}
