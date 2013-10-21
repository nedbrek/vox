#version 130

uniform sampler2D tex;

void main()
{
	vec2 tmp;
	if (gl_TexCoord[0].s == 0 || gl_TexCoord[0].s == 1.)
	{
		tmp.s = gl_TexCoord[0].t;
		tmp.t = gl_TexCoord[0].p;
	}
	else
	{
		tmp.s = gl_TexCoord[0].s;
		if (gl_TexCoord[0].t == 0 || gl_TexCoord[0].t == 1.)
			tmp.t = gl_TexCoord[0].p;
		else
			tmp.t = gl_TexCoord[0].t;
	}

	gl_FragColor = texture2D(tex, tmp);
	//gl_FragColor = texture2D(tex, gl_TexCoord[0].st);
	//gl_FragColor = texture2D(tex, vec2(0., 0.));
	//gl_FragColor = vec4(1., 0., 0., 1.);
	//gl_FragColor = gl_TexCoord[0].stpq;
}

