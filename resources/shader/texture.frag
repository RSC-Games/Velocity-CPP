#version 330 core

in vec4 color;
in vec2 texCoord;
in float texID;

uniform sampler2D u_Textures[32];

void main()
{
    vec4 chosenTex;
    switch (int(texID)) {
        case  1: chosenTex = texture(u_Textures[ 1], texCoord); break;
        case  2: chosenTex = texture(u_Textures[ 2], texCoord); break;
        case  3: chosenTex = texture(u_Textures[ 3], texCoord); break;
        case  4: chosenTex = texture(u_Textures[ 4], texCoord); break;
        case  5: chosenTex = texture(u_Textures[ 5], texCoord); break;
        case  6: chosenTex = texture(u_Textures[ 6], texCoord); break;
        case  7: chosenTex = texture(u_Textures[ 7], texCoord); break;
        case  8: chosenTex = texture(u_Textures[ 8], texCoord); break;
        case  9: chosenTex = texture(u_Textures[ 9], texCoord); break;
        case 10: chosenTex = texture(u_Textures[10], texCoord); break;
        case 11: chosenTex = texture(u_Textures[11], texCoord); break;
        case 12: chosenTex = texture(u_Textures[12], texCoord); break;
        case 13: chosenTex = texture(u_Textures[13], texCoord); break;
        case 14: chosenTex = texture(u_Textures[14], texCoord); break;
        case 15: chosenTex = texture(u_Textures[15], texCoord); break;
        case 16: chosenTex = texture(u_Textures[16], texCoord); break;
        case 17: chosenTex = texture(u_Textures[17], texCoord); break;
        case 18: chosenTex = texture(u_Textures[18], texCoord); break;
        case 19: chosenTex = texture(u_Textures[19], texCoord); break;
        case 20: chosenTex = texture(u_Textures[20], texCoord); break;
        case 21: chosenTex = texture(u_Textures[21], texCoord); break;
        case 22: chosenTex = texture(u_Textures[22], texCoord); break;
        case 23: chosenTex = texture(u_Textures[23], texCoord); break;
        case 24: chosenTex = texture(u_Textures[24], texCoord); break;
        case 25: chosenTex = texture(u_Textures[25], texCoord); break;
        case 26: chosenTex = texture(u_Textures[26], texCoord); break;
        case 27: chosenTex = texture(u_Textures[27], texCoord); break;
        case 28: chosenTex = texture(u_Textures[28], texCoord); break;
        case 29: chosenTex = texture(u_Textures[29], texCoord); break;
        case 30: chosenTex = texture(u_Textures[30], texCoord); break;
        case 31: chosenTex = texture(u_Textures[31], texCoord); break;
        default: chosenTex = texture(u_Textures[ 0], texCoord); break;
    }
    gl_FragColor = chosenTex * color;
}
