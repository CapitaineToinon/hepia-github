#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include "ppm.h"
#include "ker.h"
#include "args.h"

pixel_t get_value_img(img_t *img, int x, int y);
void linear_ker_segment(img_t *img, ker_t *kernel, int size, int offset, img_t *result);
ker_t *alloc_blur(int size);

ker_t *alloc_blur(int size)
{
    ker_t *ker = alloc_ker(size);

    for (int i = 0; i < (size * size); i++)
        ker->data[i] = 1.0 / (size * size);

    return ker;
}

void *linear_kernel_thread(void *vargs)
{
    KernelParams *args = (KernelParams *)vargs;

    if (args == NULL)
    {
        fprintf(stderr, "vargs cannot be NULL\n");
        exit(EXIT_FAILURE);
    }

    img_t *img = args->img;
    ker_t *kernel = args->kernel;
    int size = args->size;
    int offset = args->offset;
    img_t *result = args->result;

    printf("Doing from %d to %d\n", offset, offset + size);
    linear_ker_segment(img, kernel, size, offset, result);
    return NULL;
}

void linear_ker_segment(img_t *img, ker_t *kernel, int size, int offset, img_t *result)
{
    for (int i = offset; i < offset + size; i++)
    {
        int x = i % img->width;
        int y = i / img->width;

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

img_t *linear_ker(img_t *img, ker_t *kernel)
{
    if (kernel->size % 2 == 0)
    {
        fprintf(stderr, "The kernel size needs to be odd.\n");
        exit(EXIT_FAILURE);
    }

    img_t *result = alloc_img(img->width, img->height);
    linear_ker_segment(img, kernel, img->width * img->height, 0, result);
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

    struct timespec start, finish;
    double elapsed_ms;

    char *input = argv[1];
    img_t *img = load_ppm(input);
    img_t *result = alloc_img(img->width, img->height);
    ker_t *blur = alloc_blur(3);

    printf("width %d and height %d\n", img->width, img->height);

    clock_gettime(CLOCK_MONOTONIC, &start);
    free_img(linear_ker(img, blur));
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed_ms = 1000 * (finish.tv_sec - start.tv_sec);
    elapsed_ms += (finish.tv_nsec - start.tv_nsec) / 1000000.0;
    printf("linear_ker took %f ms\n", elapsed_ms);

    int threads = 6;
    pthread_t threads_ids[threads];
    KernelParams **arguments = (KernelParams **)malloc(threads * sizeof(KernelParams *));

    for (int i = 0; i < threads; i++)
    {
        arguments[i] = alloc_kernel_params();
        arguments[i]->img = img;
        arguments[i]->kernel = blur;
        arguments[i]->size = (img->height * img->width) / threads;
        arguments[i]->offset = arguments[i]->size * i;
        arguments[i]->result = result;
    }

    arguments[threads - 1]->size += (img->height * img->width) % threads; 

    // clock_gettime(CLOCK_MONOTONIC, &start);
    // for (int i = 0; i < threads; i++)
    // {
    //     img_t *img = arguments[i]->img;
    //     ker_t *kernel = arguments[i]->kernel;
    //     int size = arguments[i]->size;
    //     int offset = arguments[i]->offset;
    //     img_t *result = arguments[i]->result;
    //     linear_ker_segment(img, kernel, size, offset, result);
    // }
    // clock_gettime(CLOCK_MONOTONIC, &finish);
    // elapsed_ms = 1000 * (finish.tv_sec - start.tv_sec);
    // elapsed_ms += (finish.tv_nsec - start.tv_nsec) / 1000000.0;
    // printf("linear_ker_segment took %f ms\n", elapsed_ms);

    // call the threads
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int thread = 0; thread < threads; thread++)
    {
        if (pthread_create(&threads_ids[thread], NULL, &linear_kernel_thread, arguments[thread]) != 0)
        {
            fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    // join the threads
    for (int i = 0; i < threads; i++)
    {
        if (pthread_join(threads_ids[i], NULL) != 0)
        {
            fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed_ms = 1000 * (finish.tv_sec - start.tv_sec);
    elapsed_ms += (finish.tv_nsec - start.tv_nsec) / 1000000.0;
    printf("threads took %f ms\n", elapsed_ms);

    write_ppm("output.ppm", result);

    for (int i = 0; i < threads; i++)
    {
        free(arguments[i]);
    }

    free(arguments);
    free_img(img);
    free_img(result);
    free_ker(blur);
    return 0;
}