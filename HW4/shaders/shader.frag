#version 330 core
// This is a sample fragment shader.

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float attenuationPara;
};

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 vertNormal;
in vec3 fragPos;


uniform samplerCube skybox;
uniform vec3 cameraPos;
uniform int type;
//uniform vec3 color;
//uniform int colorType;
//uniform vec3 viewPos;

//uniform Light light;
//uniform Material material;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    vec3 objColor;
    if(type == 1)
    {
        vec3 I = normalize(fragPos - cameraPos);
        vec3 R = reflect(I, normalize(vertNormal));
        objColor = texture(skybox, R).rgb;
    }
    else if(type == 2)
    {
        objColor = vec3(1,0,0);
    }
    else if(type == 3)
    {
        objColor = vec3(0,1,0);
    }
    else if(type == 4)
    {
        objColor = vec3(0,0,1);
    }
    else if(type == 5)
    {
        objColor = vec3(1,0,0);
    }
    
    //objColor = (vertNormal + 1 )/ 2;
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    //fragColor = vec4(color, sampleExtraOutput);
    fragColor = vec4(objColor, sampleExtraOutput);
}