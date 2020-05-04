attribute highp vec3 vertex;
attribute mediump vec4 texCoord;

varying mediump vec4 texc;
uniform mediump mat4 matrix;

void main(void)
{
    gl_Position = matrix * vec4(vertex, 1);
    texc = texCoord;
}
