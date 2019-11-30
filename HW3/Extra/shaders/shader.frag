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

//uniform vec3 color;
//uniform int colorType;
//uniform vec3 viewPos;
uniform int color;

//uniform Light light;
//uniform Material material;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    vec3 objColor;
    /*
    if(colorType == 1)
    {
        //Ambient
        vec3 ambient = light.ambient * material.ambient;

        //Diffuse
        vec3 lightDir = normalize(light.position - fragPos);
        float diff = max(dot(vertNormal, lightDir), 0.0);
        vec3 diffuse = light.diffuse * (diff * material.diffuse);

        //Specular
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, vertNormal);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * (spec * material.specular);  

        //Attenuation
        float distance = length(light.position - fragPos);
        float parameter = 1.0f / distance / light.attenuationPara;

        objColor = parameter * (ambient + diffuse + specular);
    }
    else if(colorType == 2)
    {
        objColor = (vertNormal + 1 )/ 2;
    }
    */
    if(color == 2)
    {
        objColor = vec3(1,0,0);
    }
    else
    {
        objColor = (vertNormal + 1 )/ 2;
    }
    
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    //fragColor = vec4(color, sampleExtraOutput);
    fragColor = vec4(objColor, sampleExtraOutput);
}