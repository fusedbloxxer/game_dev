attribute vec3 a_posL;

void main()
{
vec4 posL = vec4(a_posL, 1);
gl_Position = posL;
}