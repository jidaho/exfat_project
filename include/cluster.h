#pragma once
#include "extfat.h"

void *cluster(Main_Boot *MBR, int n);
// returns a pointer to a specific cluster in the file image.
// the cluster is specified by n. n must be 2 or greater
// note that the MBR starts at offset 0