#version 330

uniform sampler2D texture;
uniform float time;

in vec2 TexCoord;
out vec4 FragColor;

void main()
{
    // Obtener el valor de brillo del ne�n
    float neonBrightness = texture2D(texture, TexCoord).r;

    // Calcular la oscilaci�n de parpadeo
    float blink = abs(sin(time * 2.0));

    // Aplicar el desenfoque alrededor del borde del ne�n
    float edge = smoothstep(0.0, 0.1, neonBrightness);

    // Calcular el color del ne�n final
    vec3 neonColor = vec3(1.0, 0.0, 0.0) * neonBrightness * blink;
    neonColor = mix(neonColor, vec3(1.0, 1.0, 1.0), edge);

    FragColor = vec4(neonColor, 1.0);
}