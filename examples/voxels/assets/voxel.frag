#version 410 core
in vec4 ourColor;
in vec3 pixelPos;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;

struct Highlighter {
    bool on;
    ivec3 position;
};

uniform Highlighter highlight;
uniform vec3 player_position;

float BUFF = 0.005;

uniform vec4 BG_COLOR;
uniform float FOG_DIST;
uniform float FOG_DIST_0;
uniform bool FOG_ON;

void main()
{
    vec4 color = ourColor;
    vec3 highlight_center = highlight.position + 0.5;
    vec3 dist_to_center = abs(highlight_center - pixelPos);

    if (highlight.on
    && dist_to_center.x <= 0.5 + BUFF
    && dist_to_center.y <= 0.5 + BUFF
    && dist_to_center.z <= 0.5 + BUFF
    ) {
        // Now have fun
        BUFF *= sqrt(length(highlight_center - player_position));

        int goodx = int(dist_to_center.x >= 0.5 - BUFF);
        int goody = int(dist_to_center.y >= 0.5 - BUFF);
        int goodz = int(dist_to_center.z >= 0.5 - BUFF);
        if (goodx + goody + goodz >= 2)
        color *= vec4(vec3(2.0), 1);
    }

    FragColor = texture(ourTexture, TexCoord) * color;

    if (FOG_ON) {
        float dist_to_player = length(pixelPos - player_position);
        if (dist_to_player >= FOG_DIST) {
            if (dist_to_player >= FOG_DIST_0) {
                FragColor = BG_COLOR;
            } else {
                float color_scale = (dist_to_player - FOG_DIST) / (FOG_DIST_0 - FOG_DIST);
                FragColor = vec4(vec3(1.0 - color_scale), 1) * FragColor + vec4(vec3(color_scale), 1) * BG_COLOR;
            }
        }
    }
}
