#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include "Vertex.h"

using namespace std;

class mesh {
    public:
        // mesh data
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        mesh(vector<Vertex> vertices, vector<unsigned int> indices);
        mesh(string filename);
        void draw();
    private:
        // render data
        unsigned int vao, vbo, ebo;
        void setup_mesh();
        void setup_mesh_test();
};
#endif /* MESH_H */
