#include "mesh.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cstddef>
#include <fstream>
#include <cstdio>
#include <set>
#include <unordered_map>
#include <algorithm>

using namespace glm;

mesh::mesh(vector<Vertex> vertices, vector<unsigned int> indices){
    this->vertices = vertices;
    this->indices = indices;

    setup_mesh();
}

// comparison functions to find maximum and minimum
static bool compare_x(Vertex a, Vertex b){
    return a.position.x < b.position.x;
}

static bool compare_y(Vertex a, Vertex b){
    return a.position.y < b.position.y;
}

static bool compare_z(Vertex a, Vertex b){
    return a.position.z < b.position.z;
}

// For the .m file that without vertex normal
mesh::mesh(string filename){
    // open file
    ifstream m_file(filename);
    if(!m_file.is_open()) {
        printf("unable to open the mesh file\n");
        exit(1);
    }
    // Temporary structure for vertex to calculate vertex normal.
    unordered_map<unsigned int, vector<vec3>> v_face_normals; // storing normals of all neighbouring faces of each vertex

    // iterate through lines
    string line;
    bool is_vertex=false;
    bool is_face=false;
    while(getline(m_file, line)) {
        if(!is_face&&!is_vertex) {
            if(line.substr(0,6).compare("Vertex")==0) {
                float x=0,y=0,z=0;
                // fist vertex
                unsigned int useless;
                sscanf(line.c_str(),"Vertex%u%f%f%f",&useless,&x,&y,&z);
                vertices.push_back(Vertex{vec3(x, y, z), vec3(0.0)});
                // loop
                is_vertex=true;
            }
        }
        else if(is_face){
            unsigned int v1=0, v2=0, v3=0;
            unsigned int useless;
            sscanf(line.c_str(),"Face%u%u%u%u",&useless,&v1,&v2,&v3);
            indices.push_back(v1-1);
            indices.push_back(v2-1);
            indices.push_back(v3-1);

            // assign face normal to the neighbouring vertices
            // 1. calculate face normal
            vec3 face_normal = cross(vertices[v2-1].position-vertices[v1-1].position, 
                                    vertices[v3-1].position-vertices[v1-1].position);
            // 2. assign
            v_face_normals[v1-1].push_back(face_normal);
            v_face_normals[v2-1].push_back(face_normal);
            v_face_normals[v3-1].push_back(face_normal);
        }
        else { // is vertex
            if(line.substr(0,4).compare("Face")==0) {
                //first face
                unsigned int v1=0, v2=0, v3=0;
                unsigned int useless;
                sscanf(line.c_str(),"Face%u%u%u%u",&useless,&v1,&v2,&v3);
                indices.push_back(v1-1);
                indices.push_back(v2-1);
                indices.push_back(v3-1);

                // assign face normal to the neighbouring vertices
                // 1. calculate face normal
                vec3 face_normal = cross(vertices[v2-1].position-vertices[v1-1].position, 
                                        vertices[v3-1].position-vertices[v1-1].position);
                // 2. assign
                v_face_normals[v1-1].push_back(face_normal);
                v_face_normals[v2-1].push_back(face_normal);
                v_face_normals[v3-1].push_back(face_normal);

                // loop
                is_face=true;
            }
            else {
                // vertex
                float x=0,y=0,z=0;
                unsigned int useless;
                sscanf(line.c_str(),"Vertex%u%f%f%f",&useless,&x,&y,&z);
                vertices.push_back(Vertex{vec3(x, y, z), vec3(0.0)});
            }
        }
    }
    // calculate vertex normals
    for(int i=0; i<vertices.size(); ++i){
        vec3 sum_face_normals(0.0);
        for (vec3 face_normal : v_face_normals[i]){
            sum_face_normals += face_normal;
        }
        vertices[i].normal = normalize(sum_face_normals);
    }

    // normalize vertex coordinates
    // 1. find max and min
    float min_x = min_element(vertices.begin(), vertices.end(), compare_x)->position.x;
    float max_x = max_element(vertices.begin(), vertices.end(), compare_x)->position.x;
    float min_y = min_element(vertices.begin(), vertices.end(), compare_y)->position.y;
    float max_y = max_element(vertices.begin(), vertices.end(), compare_y)->position.y;
    float min_z = min_element(vertices.begin(), vertices.end(), compare_z)->position.z;
    float max_z = max_element(vertices.begin(), vertices.end(), compare_z)->position.z;
    // 2. normalize
    float centroid_x = (max_x+min_x)/2;
    float range_x = max_x - centroid_x;
    float centroid_y = (max_y+min_y)/2;
    float range_y = max_y - centroid_y;
    float centroid_z = (max_z+min_z)/2;
    float range_z = max_z - centroid_z;
    float temp_range;
    float max_ratio = (temp_range=range_x>range_y?range_x:range_y)>range_z?temp_range:range_z;
    printf("%f,%f,%f,%f,%f,%f\n", min_x, max_x, min_y, max_y, min_z, max_z);
    printf("%f,%f,%f,%f,%f,%f,%f\n",centroid_x, range_x, centroid_y, range_y, centroid_z, range_z, max_ratio);
    for (Vertex &a : vertices){
        a.position.x = (a.position.x-centroid_x)/max_ratio;
        a.position.y = (a.position.y-centroid_y)/max_ratio;
        a.position.z = (a.position.z-centroid_z)/max_ratio;
    }

    // setup the mesh
    setup_mesh();
}

void mesh::setup_mesh_test(){
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glBindVertexArray(0);

}

void mesh::setup_mesh(){
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glBindVertexArray(0);
}

void mesh::draw(){
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}