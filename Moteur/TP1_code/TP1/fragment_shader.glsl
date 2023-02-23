#version 330 core


// Ouput data
out vec4 color;
in vec2 UV;


uniform sampler2D TextureSampler;

void main(){

        //color = vec4(0.2, 0.3,0,1) ;
        color = texture(TextureSampler, UV);




}
