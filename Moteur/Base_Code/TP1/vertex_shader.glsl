#version 330 core


// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout (location = 1) in vec2 textureCoordinates;
layout(location = 2) in vec3 normals;



//TODO create uniform transformations matrices Model View Projection
// Matrice de transfo


uniform mat4 transform_GO;
uniform int type;
uniform float heightcoeff;
//HeightMap qu'on récupère
uniform sampler2D texture0;
//Output vers fragmentshader
out vec2 TexCoord; // Les coordonnées UV

//out vec4 normals;
uniform mat4 matv;
uniform mat4 matp;
void main(){

        vec3 pos = vertices_position_modelspace;
        TexCoord = textureCoordinates;
        if(type == 1)
        {
        float height = texture(texture0,textureCoordinates).r;
        pos.y += height * heightcoeff;
        gl_Position =matp * matv* transform_GO * vec4(pos,1);
        }
        else
        gl_Position = matp * matv *transform_GO *vec4(pos,1);
}


