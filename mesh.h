#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include "vertex.h"

using namespace std;

class mesh {
    public:
        // mesh data
        vector<vertex> vertices;
        vector<unsigned int> indices;
        mesh(vector<vertex> vertices, vector<unsigned int> indices);
        mesh(string filename);
        void draw();
    private:
        // render data
        unsigned int vao, vbo, ebo;
        void setup_mesh();
        void setup_mesh_test();
};
#endif /* MESH_H */
