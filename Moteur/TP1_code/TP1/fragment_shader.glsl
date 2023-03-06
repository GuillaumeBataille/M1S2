#version 330 core


// Ouput data
out vec4 color;
in vec2 UV;


uniform sampler2D TextureSampler;

void main(){
        color = texture(TextureSampler, UV);
}
