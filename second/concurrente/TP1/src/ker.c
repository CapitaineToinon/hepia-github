#include "ker.h"

ker_t *alloc_ker(int size)
{
    if (size < 3)
    {
        fprintf(stderr, "The kernel size needs 3 or greater.\n");
        exit(EXIT_FAILURE);
    }

    if (size % 2 == 0)
    {
        fprintf(stderr, "The kernel size needs to be odd.\n");
        exit(EXIT_FAILURE);
    }

    ker_t *ker = malloc(sizeof(ker_t));
    ker->size = size;
    ker->data = malloc(sizeof(double) * size * size);
    return ker;
}

ker_t *alloc_identity()
{
    ker_t *ker = alloc_ker(3);

    for (int i = 0; i < 9; i++)
        ker->data[i] = 0.0;

    ker->data[ker->size + 1] = 1.0;
    return ker;
}

ker_t *alloc_sharpen()
{
    ker_t *ker = alloc_ker(3);

    for (int i = 0; i < 9; i += 2)
        ker->data[i] = 0.0;

    for (int i = 1; i < 9; i += 2)
        ker->data[i] = -1.0;

    ker->data[5] = 5.0;
    return ker;
}

ker_t *alloc_edge()
{
    ker_t *ker = alloc_ker(3);

    for (int i = 0; i < 9; i++)
        ker->data[i] = -1.0;

    ker->data[ker->size + 1] = 8.0;
    return ker;
}

ker_t *alloc_blur(int size)
{
    ker_t *ker = alloc_ker(size);

    for (int i = 0; i < (size * size); i++)
        ker->data[i] = 1.0 / (size * size);

    return ker;
}

ker_t *alloc_gauss()
{
    ker_t *ker = alloc_ker(5);

    ker->data[0]  = 1.0 / 256.0;
    ker->data[1]  = 4.0 / 256.0;
    ker->data[2]  = 6.0 / 256.0;
    ker->data[3]  = 4.0 / 256.0;
    ker->data[4]  = 1.0 / 256.0;

    ker->data[5]  = 4.0 / 256.0;
    ker->data[6]  = 16.0 / 256.0;
    ker->data[7]  = 24.0 / 256.0;
    ker->data[8]  = 16.0 / 256.0;
    ker->data[9]  = 4.0 / 256.0;

    ker->data[10] = 6.0 / 256.0;
    ker->data[11] = 24.0 / 256.0;
    ker->data[12] = 36.0 / 256.0;
    ker->data[13] = 24.0 / 256.0;
    ker->data[14] = 6.0 / 256.0;

    ker->data[15] = ker->data[5];
    ker->data[16] = ker->data[6];
    ker->data[17] = ker->data[7];
    ker->data[18] = ker->data[8];
    ker->data[19] = ker->data[9];

    ker->data[20] = ker->data[0];
    ker->data[21] = ker->data[1];
    ker->data[22] = ker->data[2];
    ker->data[23] = ker->data[3];
    ker->data[24] = ker->data[4];

    return ker;
}

ker_t *alloc_unsharp()
{
    ker_t *ker = alloc_ker(5);

    ker->data[0]  = -1.0 / 256.0;
    ker->data[1]  = -4.0 / 256.0;
    ker->data[2]  = -6.0 / 256.0;
    ker->data[3]  = -4.0 / 256.0;
    ker->data[4]  = -1.0 / 256.0;

    ker->data[5]  = -4.0 / 256.0;
    ker->data[6]  = -16.0 / 256.0;
    ker->data[7]  = -24.0 / 256.0;
    ker->data[8]  = -16.0 / 256.0;
    ker->data[9]  = -4.0 / 256.0;

    ker->data[10] = -6.0 / 256.0;
    ker->data[11] = -24.0 / 256.0;
    ker->data[12] = 476 / 256.0;
    ker->data[13] = -24.0 / 256.0;
    ker->data[14] = -6.0 / 256.0;

    ker->data[15] = ker->data[5];
    ker->data[16] = ker->data[6];
    ker->data[17] = ker->data[7];
    ker->data[18] = ker->data[8];
    ker->data[19] = ker->data[9];

    ker->data[20] = ker->data[0];
    ker->data[21] = ker->data[1];
    ker->data[22] = ker->data[2];
    ker->data[23] = ker->data[3];
    ker->data[24] = ker->data[4];

    return ker;
}

void free_ker(ker_t *ker)
{
    free(ker->data);
    free(ker);
}