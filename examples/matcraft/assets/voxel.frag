#version 410 core
in vec3 pixelPos;
flat in int direction;
flat in int block_type;
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
uniform float TEXTURE_WIDTH;
uniform float TEXTURE_HEIGHT;
uniform bool FOG_ON;

uniform float TIME;

vec2 getBaseTexCoords() {
    vec3 dec;
    modf(pixelPos.x, dec.x);
    modf(pixelPos.y, dec.y);
    modf(pixelPos.z, dec.z);

    vec3 rems = pixelPos - dec;

    if(rems.x < 0) rems.x += 1;
    if(rems.y < 0) rems.y += 1;
    if(rems.z < 0) rems.z += 1;

    // NORTH = 0, EAST, SOUTH, WEST, UP, DOWN = 5
    switch (direction) {
        case 0:
            return vec2(1.0 - rems.x, 1.0 - rems.y);
        case 1:
            return vec2(1.0 - rems.z, 1.0 - rems.y);
        case 2:
            return vec2(1.0 - rems.x, 1.0 - rems.y);
        case 3:
            return vec2(rems.z, 1.0 - rems.y);
        case 4:
            return vec2(1.0 - rems.z, rems.x);
        case 5:
            return vec2(1.0 - rems.z, rems.x);
    }
}

vec2 getTexCoords() {
    vec2 base = getBaseTexCoords();

//     Move water
    if (block_type == 9) {
        base.x += (sin(TIME * 0.9) - 1) / 6.0;
        base.y += (cos(TIME * 1.2) - 1) / 6.0;
        if (base.x > 1.0) base.x -= 1.0;
        if (base.x < 0.0) base.x += 1.0;
        if (base.y > 1.0) base.y -= 1.0;
        if (base.y < 0.0) base.y += 1.0;
    }

    base.x += block_type;


    switch (direction) {
        case 0:
        case 1:
        case 2:
        case 3:
            base.y += 1;
            break;
        case 4:
            break;
        case 5:
            base.y += 2;
            break;
    }
    return base * 16.0 / vec2(TEXTURE_WIDTH, TEXTURE_HEIGHT);
}

void main()
{
    vec4 color = vec4(1);
    vec2 tex_coord = getTexCoords();

    vec3 highlight_center = highlight.position + 0.5;
    vec3 dist_to_center = abs(highlight_center - pixelPos);
    float len_to_cam = length(highlight_center - player_position);

    if (highlight.on
    && dist_to_center.x <= 0.5 + BUFF
    && dist_to_center.y <= 0.5 + BUFF
    && dist_to_center.z <= 0.5 + BUFF
    ) {
        // Now have fun
        BUFF *= sqrt(len_to_cam);

        int goodx = int(dist_to_center.x >= 0.5 - BUFF);
        int goody = int(dist_to_center.y >= 0.5 - BUFF);
        int goodz = int(dist_to_center.z >= 0.5 - BUFF);
        if (goodx + goody + goodz >= 2)
        color *= vec4(vec3(2.0), 1);
    }

    FragColor = texture(ourTexture, tex_coord) * color;

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
