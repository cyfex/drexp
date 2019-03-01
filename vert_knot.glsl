#version 400
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
out vec3 vertex_normal;
out vec3 frag_position;
void main() {
    vertex_normal = vn;
    frag_position = vp;
    gl_Position = vec4(vp, 1.0);
    //vec3 base_colour = vec3(0.5,0.0,0.5);
    //vertex_colour = cross(vn, base_colour);
};