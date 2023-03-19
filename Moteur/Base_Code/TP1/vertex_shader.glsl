#version 330 core


// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout (location = 1) in vec2 textureCoordinates;
layout(location = 2) in vec3 textureDirectionCube;



//TODO create uniform transformations matrices Model View Projection
// Matrice de transfo


uniform mat4 transform_GO;
uniform int type;
uniform float heightcoeff;
//HeightMap qu'on récupère
uniform sampler2D texture0;
//Output vers fragmentshader
out vec2 TexCoord; // Les coordonnées UV
out vec3 TexDirCube;// Les coordonnées si CubeTextured
//out vec4 normals;
uniform mat4 matv;
uniform mat4 matv_static;
uniform mat4 matp;
uniform int Texid;
void main(){

        vec3 pos = vertices_position_modelspace;
        TexCoord = textureCoordinates;
        TexDirCube = vec3(pos.x,pos.y,pos.z);
        if(type == 1)
        {
        float height = texture(texture0,textureCoordinates).r;
        pos.y = height ;
        gl_Position =matp * matv* transform_GO * vec4(pos,1);
        }
        else
        if(type == 4 && Texid == 1)
        {
        vec4 cubepos = matp * matv_static* transform_GO * vec4(pos,1);
        gl_Position = vec4(cubepos.x,cubepos.y,cubepos.w,cubepos.w);
        TexDirCube = vec3(pos.x,pos.y,pos.z);
        }
        else
        gl_Position = matp * matv *transform_GO *vec4(pos,1);


}


