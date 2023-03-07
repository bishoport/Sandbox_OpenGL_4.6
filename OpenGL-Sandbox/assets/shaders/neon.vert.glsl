#version 330

uniform sampler2D texture;
uniform float time;

in vec2 TexCoord;
out vec4 FragColor;

void main()
{
    // Obtener el valor de brillo del neón
    float neonBrightness = texture2D(texture, TexCoord).r;

    // Calcular la oscilación de parpadeo
    float blink = abs(sin(time * 2.0));

    // Aplicar el desenfoque alrededor del borde del neón
    float edge = smoothstep(0.0, 0.1, neonBrightness);

    // Calcular el color del neón final
    vec3 neonColor = vec3(1.0, 0.0, 0.0) * neonBrightness * blink;
    neonColor = mix(neonColor, vec3(1.0, 1.0, 1.0), edge);

    FragColor = vec4(neonColor, 1.0);
}