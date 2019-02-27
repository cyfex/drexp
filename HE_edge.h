#ifndef HE_EDGE_H
#define HE_EDGE_H
#include "HE_vert.h"
#include "HE_face.h"

struct HE_edge {
    HE_vert* vert; // vertex at the end of the half-edge
    HE_edge* pair; // oppositely oriented half-edge
    HE_face* face; // the incident face
    HE_edge* prev; // previous half-edge around the face
    HE_edge* next; // next half-edge around the face
};
#endif /* HE_EDGE_H */
