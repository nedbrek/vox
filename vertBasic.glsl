#version 130

void main()
{
	gl_TexCoord[0] = gl_Vertex;
	gl_Position = ftransform();
	//gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
} 

