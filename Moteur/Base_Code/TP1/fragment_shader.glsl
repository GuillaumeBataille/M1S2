#version 330 core


// Ouput data
out vec4 color;
in vec2 TexCoord;


uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;


void main(){
// Sample the textures at the current texture coordinate
vec4 tex1 = texture(texture1, TexCoord);
vec4 tex2 = texture(texture2, TexCoord);
vec4 tex3 = texture(texture3, TexCoord);

// Calculate the weights for each texture based on the height value
float height = texture(texture0,TexCoord).g;

float weight1 = smoothstep(0, 0.5, height);
float weight2 = smoothstep(0.5, 0.8, height);
float weight3 = smoothstep(0.8, 1, height);

// Blend the textures together based on the weights
vec4 blendedColor = mix(tex1, tex2, weight1);
blendedColor = mix(blendedColor, tex3, weight2 + weight3);

// Output the final color
color = blendedColor;
}
