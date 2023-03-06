#version 330 core


// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout (location = 1) in vec2 textureCoordinates;
layout(location = 2) in vec3 normals;



//TODO create uniform transformations matrices Model View Projection
// Matrice de transfo
uniform mat4 transform_mat;
uniform mat4 view_mat;
uniform mat4 project_mat;

//HeightMap qu'on récupère
uniform sampler2D texture0;

//Output vers fragmentshader
out vec2 TexCoord; // Les coordonnées UV


//out vec4 normals;

void main(){

        // TODO : Output position of the vertex, in clip space : MVP * position
        vec3 pos = vertices_position_modelspace;
        float height = texture(texture0,textureCoordinates).r;
        pos.y += height;
        gl_Position =view_mat * transform_mat * vec4(pos,1);
        TexCoord = textureCoordinates;
        gl_Position = project_mat * gl_Position;

}

