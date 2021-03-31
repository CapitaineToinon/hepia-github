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
#include "helper.h"

#define BLUR_SIZE 3
#define ARGUMENT_COUNT 5

void *linear_kernel_thread(void *);
void linear_ker_segment(img_t *, ker_t *, int, int, img_t *);
void linear_ker(img_t *, ker_t *, img_t *);
ker_t *find_kernel(char *);
void print_help(FILE *, char *);

void print_help(FILE *dest, char *filename)
{
    fprintf(dest, "Usage: %s <threads count> <source file> <kernel name> <output file>\n", filename);
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

    linear_ker_segment(img, kernel, size, offset, result);
    return NULL;
}

void linear_ker_segment(img_t *img, ker_t *kernel, int size, int offset, img_t *result)
{
    if (kernel->size % 2 == 0)
    {
        fprintf(stderr, "The kernel size needs to be odd.\n");
        exit(EXIT_FAILURE);
    }

    if (kernel->size > img->width || kernel->size > img->height)
    {
        fprintf(stderr, "The kernel can't be bigger than the image.\n");
        exit(EXIT_FAILURE);
    }

    // Only loop through the desired pixels
    for (int i = offset; i < offset + size; i++)
    {
        int x = i % img->width;
        int y = i / img->width;

        int half = kernel->size / 2;
        int target_pixel = (img->width * y) + x;

        double r = 0.0;
        double g = 0.0;
        double b = 0.0;

        for (int kernel_i = 0; kernel_i < (kernel->size * kernel->size); kernel_i++)
        {
            int kernel_x = kernel_i % kernel->size;
            int kernel_y = kernel_i / kernel->size;

            /**
             * get pixel coordinate the kernel will be applied to.
             * target_x and y may be out of bound.
             * 
             * if they are, we mirror the index back into the image.
             */
            int target_x = (x + kernel_x - half);
            int target_y = (y + kernel_y - half);

            if (target_x < 0)
                target_x = abs(target_x % img->width);

            if (target_x >= img->width)
                target_x = img->width - (target_x % img->width) - 1;

            if (target_y < 0)
                target_y = abs(target_y % img->height);

            if (target_y >= img->height)
                target_y = img->height - (target_y % img->height) - 1;

            // Apply the kernel
            pixel_t current_p = img->data[(img->width * target_y) + target_x];
            r += current_p.r * kernel->data[kernel_i];
            g += current_p.g * kernel->data[kernel_i];
            b += current_p.b * kernel->data[kernel_i];
        }

        // ensure values are valid
        r = min(255, max(0, r));
        g = min(255, max(0, g));
        b = min(255, max(0, b));

        result->data[target_pixel] = (pixel_t){r, g, b};
    }
}

void linear_ker(img_t *img, ker_t *kernel, img_t *output)
{
    linear_ker_segment(img, kernel, img->width * img->height, 0, output);
}

ker_t *find_kernel(char *name)
{
    if (strcmp(name, "identity") == 0)
        return alloc_identity();
    if (strcmp(name, "sharpen") == 0)
        return alloc_sharpen();
    if (strcmp(name, "edge") == 0)
        return alloc_edge();
    if (strcmp(name, "blur") == 0)
        return alloc_blur(BLUR_SIZE);
    if (strcmp(name, "gauss") == 0)
        return alloc_gauss();
    if (strcmp(name, "unsharp") == 0)
        return alloc_unsharp();

    return NULL;
}

int main(int argc, char *argv[])
{
    int threads, total_pixels;
    char *input_path, *kernel_name, *output_path;
    img_t *img, *result;
    ker_t *kernel;
    // Used to time how long the program takes
    struct timespec start, finish;
    double elapsed_ms;
    // for threading
    pthread_t *threads_ids;
    KernelParams **arguments;

    if (argc != ARGUMENT_COUNT)
    {
        print_help(stderr, argv[0]);
        exit(EXIT_FAILURE);
    }

    // Get program arguments from command line
    threads = stringtoi(argv[1]);
    input_path = argv[2];
    kernel_name = argv[3];
    output_path = argv[4];

    if (threads <= 0)
    {
        fprintf(stderr, "Thread count needs to be a positive integer.\n");
        exit(EXIT_FAILURE);
    }

    if ((img = load_ppm(input_path)) == NULL)
    {
        fprintf(stderr, "Image with path '%s' does not exist.\n", input_path);
        exit(EXIT_FAILURE);
    }

    // Maximum of 1 thread per pixel
    if ((total_pixels = img->width * img->height) < threads)
    {
        threads = total_pixels;
    }

    if ((kernel = find_kernel(kernel_name)) == NULL)
    {
        fprintf(stderr, "The kernel '%s' does not exist.\n", kernel_name);
        exit(EXIT_FAILURE);
    }

    // Output image that will be modified
    result = alloc_img(img->width, img->height);

    // If thread is 1, then we're not spawning threads
    if (threads == 1)
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        linear_ker(img, kernel, result);
        clock_gettime(CLOCK_MONOTONIC, &finish);
    }
    // otherwise, spawn threads using pthread
    else
    {
        threads_ids = (pthread_t *)malloc(threads * sizeof(pthread_t *));
        arguments = (KernelParams **)malloc(threads * sizeof(KernelParams *));

        // split the 1D pixels array in equal chunks for each thread to process
        for (int i = 0; i < threads; i++)
        {
            arguments[i] = alloc_kernel_params();
            arguments[i]->img = img;
            arguments[i]->kernel = kernel;
            arguments[i]->size = (img->height * img->width) / threads;
            arguments[i]->offset = arguments[i]->size * i;
            arguments[i]->result = result;
        }

        // add remaining pixels to the last thread
        arguments[threads - 1]->size += (img->height * img->width) % threads;

        clock_gettime(CLOCK_MONOTONIC, &start);

        // call the threads
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

        // free thread memory
        free(threads_ids);
        for (int i = 0; i < threads; i++)
            free(arguments[i]);
        free(arguments);
    }

    // We're now done, save the output, show the duration
    if (!write_ppm(output_path, result))
    {
        fprintf(stderr, "Failed to save image with path '%s'.\n", output_path);
        exit(EXIT_FAILURE);
    }

    elapsed_ms = 1000 * (finish.tv_sec - start.tv_sec);
    elapsed_ms += (finish.tv_nsec - start.tv_nsec) / 1000000.0;
    printf("Processing image took %f ms.\n", elapsed_ms);

    free_img(img);
    free_img(result);
    free_ker(kernel);

    return EXIT_SUCCESS;
}