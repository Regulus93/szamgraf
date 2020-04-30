#version 130

uniform mat4 mvp;
uniform vec3 points[10]; //uniform t�mb (nem dinamikus), manu�lisan kell megoldani, hogy kevesebb adatot k�ldj�nk le

void main()
{
	gl_Position = mvp * vec4(points[gl_VertexID], 1) ;
}