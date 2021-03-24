#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ppm.h"
#include "ker.h"

pixel_t get_value_img(img_t *img, int x, int y);

ker_t *alloc_blur(int size)
{
    ker_t *ker = alloc_ker(size);

    for (int i = 0; i < (size * size); i++)
        ker->data[i] = 1.0 / (size * size);

    return ker;
}

img_t *linear_ker(img_t *img, ker_t *kernel)
{
    if (kernel->size % 2 == 0)
    {
        fprintf(stderr, "The kernel size needs to be odd.\n");
        exit(EXIT_FAILURE);
    }

    img_t *result = alloc_img(img->width, img->height);

    for (int x = 0; x < img->width; x++)
    {
        for (int y = 0; y < img->height; y++)
        {
            int half = kernel->size / 2;
            int dest_i = (img->width * y) + x;

            double r = 0.0;
            double g = 0.0;
            double b = 0.0;

            for (int kernel_i = 0; kernel_i < (kernel->size * kernel->size); kernel_i++)
            {
                int kernel_x = kernel_i % kernel->size;
                int kernel_y = kernel_i / kernel->size;
                int target_x = x + kernel_x - half;
                int target_y = y + kernel_y - half;

                pixel_t current_p = get_value_img(img, target_x, target_y);

                r += current_p.r * kernel->data[kernel_i];
                g += current_p.g * kernel->data[kernel_i];
                b += current_p.b * kernel->data[kernel_i];
            }

            result->data[dest_i] = (pixel_t){r, g, b};
        }
    }

    return result;
}

pixel_t get_value_img(img_t *img, int x, int y)
{
    int newX = x % img->width;
    int newY = y % img->height;

    if (newX < 0)
    {
        // ex : -2 -> img->width - 2
        newX = img->width - abs(newX);
    }

    if (newY < 0)
    {
        // ex : -2 -> img->height - 2
        newY = img->height - abs(newY);
    }

    return img->data[(img->width * newY) + newX];
}

int main(int argc, char *argv[])
{
    // @TODO change this
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s input output\n"
                        "where input and output are PPM files\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    char *input = argv[1];
    img_t *img = load_ppm(input);

    // if ((img = load_ppm(input)) == NULL)
    // {
    //     fprintf(stderr, "Failed to open image %s.\n", input);
    //     exit(EXIT_FAILURE);
    // }

    printf("width %d and height %d\n", img->width, img->height);

    ker_t *blur = alloc_blur(15);
    img_t *result = linear_ker(img, blur);
    write_ppm("output.ppm", result);

    // for (int x = 0; x < img->width; x++)
    // {
    //     for (int y = 0; y < img->height; y++)
    //     {
    //         pixel_t p = get_value_img(img, img->width, img->height);
    //         printf("%d %d %d\n", p.r, p.g, p.b);

    //         p = get_value_img(img, 0, 0);
    //         printf("%d %d %d\n", p.r, p.g, p.b);
    //     }
    // }

    printf("%d\n", -1 % 710);

    free_img(img);
    free_img(result);
    free_ker(blur);
    return 0;
}