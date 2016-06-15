uniform mat4 u_projection;
uniform float u_zdepth;

void main()
{
    // transform the vertex position
    //gl_Position = u_projection * gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_Position = u_projection * vec4(gl_Vertex.xy, u_zdepth, 1.0);

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    gl_FrontColor = gl_Color;
}