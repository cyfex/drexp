#version 400
in vec3 vertex_normal;
in vec3 frag_position;
out vec4 frag_colour;
uniform vec3 light_position;
uniform vec3 view_position; // used for specular light
void main() {
    /*
     * Basic light params
     */
    vec3 ambient_light_colour = vec3(0.5, 0.5, 0.5); // light colour used for ambient only
    vec3 light_colour = vec3(0.5, 0.0, 0.0); // light colour used for both diffuse and specular light
    vec3 object_colour = vec3(1.0, 1.0, 1.0);
    /*
     * Secondary light params
     */
    float ambient_strength = 0.1; // strength of ambient light
    float specular_strength = 0.5; // strenght of specular light
    float specular_power = 32;  // power of specular light
    
    // ambient light
    vec3 ambient = ambient_strength * ambient_light_colour;
    // diffuse light
    vec3 light_direction = normalize(frag_position - light_position); // negative light direction
    float diffuse_rate = max(dot(vertex_normal, light_direction), 0.0);
    vec3 diffuse = diffuse_rate * light_colour;
    // specular light
    vec3 view_direction = normalize(view_position - frag_position);
    vec3 reflect_direction = normalize(reflect(light_direction, vertex_normal));
    float specular_rate = pow(max(dot(view_direction, reflect_direction), 0.0 ), specular_power);
    vec3 specular = specular_strength * specular_rate * light_colour;

    // result from multiple lights
    vec3 result = (ambient + diffuse + specular) * object_colour;
    frag_colour = vec4(result, 1.0);


    //vec3 object_colour = vec3(0.5, 0.0, 0.5);
    //vec3 light_colour = vec3(0.0, 0.5, 0.0);
    //vec3 light_direction = normalize(light_pos - frag_pos);
    //float diff = max(dot(vertex_normal, light_direction), 0.0);
    //vec3 diffuse = diff * light_colour;
    //vec3 result = diffuse * object_colour;

    //frag_colour = vec4(vertex_colour, 1.0);
};