#version 410 core

layout (location = 0) in int triData;

uniform ivec3 chunk_position;
uniform mat4 transform;
uniform mat4 camera;

out vec3 pixelPos;  // We want to interpolate this
flat out int direction;
flat out int block_type;
void main()
{
    int position_z = triData & 63;
    int position_y = (triData & (63 << 6)) >> 6;
    int position_x = (triData & (63 << 12)) >> 12;
    direction = (triData >> 18) & 7;
    block_type = (triData >> 21) & 15;
    ivec3 pos = ivec3(position_x, position_y, position_z);
    gl_Position = camera * vec4(chunk_position + pos, 1.0);
    pixelPos = chunk_position + pos;
}
