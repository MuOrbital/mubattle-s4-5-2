#version 330 core

in vec2 TexCoord; // Coordenadas de textura que vienen del vertex shader
out vec4 FragColor; // Color de salida del fragmento

uniform sampler2D textTexture; // Textura del texto
uniform vec4 textColor;        // Color del texto
uniform vec4 shadowColor;      // Color de la sombra
uniform vec2 shadowOffset;     // Desplazamiento de la sombra

void main()
{
    // Cargar la textura del texto
    float alpha = texture(textTexture, TexCoord).r;

    // Cargar la textura de la sombra (desplazada)
    float shadowAlpha = texture(textTexture, TexCoord + shadowOffset).r;

    // Color de la sombra
    vec4 shadow = shadowColor * shadowAlpha;

    // Color del texto
    vec4 text = textColor * alpha;

    // Combinar sombra y texto
    FragColor = max(shadow, text);
}
