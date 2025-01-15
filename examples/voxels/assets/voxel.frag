#version 410 core
in vec4 ourColor;
in vec2 TexCoord;
out vec4 FragColor;

void main()
{
    FragColor = ourColor;
}

//#version 410 core
//in vec4 ourColor;
//in vec2 TexCoord;
//out vec4 FragColor;
//
//uniform sampler2D ourTexture;
//
//void main()
//{
//   FragColor = texture(ourTexture, TexCoord) * ourColor;
//}

