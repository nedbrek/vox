#version 130

uniform ivec3 blockOffset;

void main()
{
	// feed forward the uv coordinate
	gl_TexCoord[0] = gl_Vertex;

	// translate the block
	vec4 position = gl_Vertex + ivec4(blockOffset, 0);
	
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * position;
} 

