#version 130

uniform mat4 mvp;
uniform vec3 points[10]; //uniform tömb (nem dinamikus), manuálisan kell megoldani, hogy kevesebb adatot küldjünk le

void main()
{
	gl_Position = mvp * vec4(points[gl_VertexID], 1) ;
}