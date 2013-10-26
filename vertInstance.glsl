#version 130

void main()
{
	int x = gl_InstanceID & 15;
	int y = (gl_InstanceID >> 4) & 15;
	int z = (gl_InstanceID >> 8) & 15;

	// feed forward the uv coordinate
	gl_TexCoord[0] = gl_Vertex;

	vec4 position = gl_Vertex + ivec4(x, y, z, 0);
	gl_Position = gl_ModelViewProjectionMatrix * position;
} 

