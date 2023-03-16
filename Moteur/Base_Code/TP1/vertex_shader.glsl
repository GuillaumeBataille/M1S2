#version 330 core


// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout (location = 1) in vec2 textureCoordinates;
layout(location = 2) in vec3 normals;



//TODO create uniform transformations matrices Model View Projection
// Matrice de transfo
uniform mat4 transform_mat_plane;
uniform mat4 view_mat;
uniform mat4 project_mat;
uniform mat4 transform_;
uniform bool isMesh;
//HeightMap qu'on récupère
uniform sampler2D texture0;
//Output vers fragmentshader
out vec2 TexCoord; // Les coordonnées UV


//out vec4 normals;

void main(){
        vec3 pos = vertices_position_modelspace;
        // TODO : Output position of the vertex, in clip space : MVP * position
        
        if(isMesh) // Si on a affaire a un mesh, c'est ça propre matrice de transformation qui est utilisé
        {
        gl_Position = project_mat * view_mat * transform_ * vec4(pos,1);        
        }
        else // Sinon, pour l'instant y'a que le plan donc on le traite comme un plan
        {
        float height = texture(texture0,textureCoordinates).r;
        pos.y += height;
        gl_Position =view_mat * transform_mat_plane * vec4(pos,1);
        TexCoord = textureCoordinates;
        gl_Position = project_mat * gl_Position;
        }
}

