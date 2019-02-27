#ifndef HE_FACE_H
#define HE_FACE_H
#include "HE_vert.h"

struct HE_face {
    HE_edge* edge; // one of the half-edges bordering the face
};
#endif /* HE_FACE_H */
