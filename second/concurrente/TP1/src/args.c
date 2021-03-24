#include "args.h"

KernelParams *alloc_kernel_params()
{
    return (KernelParams *)malloc(sizeof(KernelParams));
}
