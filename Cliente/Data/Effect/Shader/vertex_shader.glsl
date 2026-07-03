#version 330 core

layout(location = 0) in vec2 aPos;     // Posición del vértice
layout(location = 1) in vec2 aTexCoord; // Coordenadas de textura

out vec2 TexCoord; // Coordenadas que pasarán al fragment shader

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0); // Posición final en la pantalla
    TexCoord = aTexCoord;               // Pasar las coordenadas de textura al fragment shader
}
