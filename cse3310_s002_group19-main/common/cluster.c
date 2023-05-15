#include <stddef.h>
#include <stdio.h>
#include "cluster.h"

void *cluster(Main_Boot *MBR, int n)
{
    // given a number n, this function returns a pointer to cluster n in the mmapped file
    // n must be 2 or greater
    if (MBR && n > 1)
    {
        int bytesPerSector = 2 << (MBR->BytesPerSectorShift - 1);
        int sectorsPerCluster = 2 << (MBR->SectorsPerClusterShift - 1);
        return (((void *)MBR + MBR->ClusterHeapOffset * bytesPerSector) + ((n - 2) * bytesPerSector * sectorsPerCluster));
    }
    return NULL;
}