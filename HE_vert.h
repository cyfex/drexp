#ifndef HE_VERT_H
#define HE_VERT_H
#include "HE_edge.h"

struct HE_vert {
    float x, y, z; // the vertex coordinates
    HE_edge* edge; // one of the half-edges emanating from the vertex
};
#endif /* HE_VERT_H */
