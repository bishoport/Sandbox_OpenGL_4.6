#version 450 core


struct PointLight {
    vec3 position;
    vec3 color;
    float strength;
};

struct AmbientLight {
    vec3 color;
    float intensity;
};


in vec2 fragmentTexCoords;
in vec3 fragmentPosition;
in vec3 fragmentNormal;

uniform sampler2D basicTexture;
uniform PointLight[8] lights;
uniform AmbientLight ambientLight;
uniform vec3 cameraPosition;

out vec4 finalColor;

vec3 calculatePointLight(int i);




void main()
{
    //ambient
    vec3 temp = 0.2 * texture(basicTexture, fragmentTexCoords).rgb;
    //vec3 temp = 0.2 * texture(basicTexture, fragmentTexCoords).rgb;
    //vec3 temp = ambientLight.color * ambientLight.intensity;

    //lighting
    for (int i = 0; i < 8; i++) {
        temp += calculatePointLight(i);
    }

    finalColor = vec4(temp, 1.0);
}



vec3 calculatePointLight(int i) {

    vec3 baseTexture = texture(basicTexture, fragmentTexCoords).rgb;

    //geometric data
    vec3 fragmentLight = lights[i].position - fragmentPosition;
    float distance = length(fragmentLight);
    fragmentLight = normalize(fragmentLight);
    vec3 fragmentCamera = normalize(cameraPosition - fragmentPosition);
    vec3 halfVec = normalize(fragmentLight + fragmentCamera);
    
    //diffuse
    vec3 result = lights[i].color * baseTexture * max(0.0, dot(fragmentNormal, fragmentLight)) * lights[i].strength / (distance * distance);

    //specular
    result += vec3(1.0) * pow(max(0.0, dot(fragmentNormal, halfVec)), 32) * lights[i].strength / (distance * distance);

    return result;
}