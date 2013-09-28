#version 130

uniform sampler2D tex;

void main()
{
	gl_FragColor = texture2D(tex, gl_TexCoord[0].st);
	//gl_FragColor = texture2D(tex, vec2(0., 0.));
	//gl_FragColor = vec4(1., 0., 0., 1.);
	//gl_FragColor = gl_TexCoord[0].stpq;
}

