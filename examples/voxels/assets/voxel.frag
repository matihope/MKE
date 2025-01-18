#version 410 core
in vec4 ourColor;
in vec3 pixelPos;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;

struct Highlighter {
    bool on;
    vec3 position;
};

uniform Highlighter highlight;
uniform vec3 player_position;

float BUFF = 0.005;

void main()
{
    vec4 color = ourColor;
    vec3 voxel_center = highlight.position + 0.5;
    vec3 test_highlight = abs(voxel_center - pixelPos);

    if (highlight.on &&
    test_highlight.x <= 0.5 &&
    test_highlight.y <= 0.5 &&
    test_highlight.z <= 0.5
    ) {
        // Now have fun
        BUFF *= sqrt(length(abs(voxel_center - player_position)));
        int goodx = int(test_highlight.x >= 0.5 - BUFF);
        int goody = int(test_highlight.y >= 0.5 - BUFF);
        int goodz = int(test_highlight.z >= 0.5 - BUFF);
        if (goodx + goody + goodz >= 2)
            color *= vec4(vec3(2.0), 1);
    }
    FragColor = texture(ourTexture, TexCoord) * color;
}

