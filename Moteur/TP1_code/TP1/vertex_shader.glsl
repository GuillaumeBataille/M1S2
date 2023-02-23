#version 330 core


// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;

layout (location = 0) in vec2 textureCoordinates;



//TODO create uniform transformations matrices Model View Projection
// NEW
uniform mat4 transform_mat;
uniform mat4 view_mat;
uniform mat4 project_mat;
// Values that stay constant for the whole mesh.

out vec2 TexCoord;
//out sampler2D texOut;


out vec2 UV;
uniform sampler2D TextureSampler;


void main(){

        // TODO : Output position of the vertex, in clip space : MVP * position
        gl_Position =project_mat * view_mat * transform_mat * vec4(vertices_position_modelspace,1);
        TexCoord = vec2(textureCoordinates.x, textureCoordinates.y);
        UV = TexCoord;
        //TexCoord = aTexCoord;
        //texture = texture0;
        //textureout = texture;
        //Heightmap min max 

}

