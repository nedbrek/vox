#version 130

uniform sampler2D tex;

/*
 * normalization

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
*/

bool doesRayHit(ivec2 pixelCoord, vec2 position, vec2 direction)
{
	int stepX = direction.x < 0 ? -1 : 1;
	int stepY = direction.y < 0 ? -1 : 1;

	bool xMovement = pixelCoord.x < position.x && direction.x < 0 ||
	                 pixelCoord.x > position.x && direction.x > 0;

	bool yMovement = pixelCoord.y < position.y && direction.y < 0 ||
	                 pixelCoord.y > position.y && direction.y > 0;

	vec2 sideDist = direction;

	while (xMovement || yMovement)
	{
		if (sideDist.x < sideDist.y)
		{
			position.x += stepX;
			sideDist.x += direction.x;
		}
		else
		{
			position.y += stepY;
			sideDist.y += direction.y;
		}

		xMovement = pixelCoord.x < position.x && direction.x < 0 ||
		            pixelCoord.x > position.x && direction.x > 0;

		yMovement = pixelCoord.y < position.y && direction.y < 0 ||
		            pixelCoord.y > position.y && direction.y > 0;
	}

	return position == pixelCoord;
}

void main()
{
	// TODO: adjust for camera
	// remember to subtact .5, so it's [-.5, .5]
	vec2 xzDirection = vec2(.3, .5);

	// TODO: adjust for camera
	vec2 xzPosition = vec2(410., 205.);

	ivec2 pixelCoord = ivec2(floor(gl_TexCoord[0].x * 1092),
	  floor(gl_TexCoord[0].y * 614));

	// calculate if ray hits pixel
	bool rayHit = doesRayHit(pixelCoord, xzPosition, xzDirection);

	//gl_FragColor = texture2D(tex, gl_TexCoord[0].st);
	//gl_FragColor = vec4(1., 0., 0., 1.);
	//gl_FragColor = gl_TexCoord[0].stpq;

	// if ray hit and grid -> blue
	// if ray hit and not grid -> red
	// if ray no hit and grid -> white
	// if ray no hit and not grid -> black
	if (rayHit)
	{
		if (pixelCoord.x % 25 == 0 || pixelCoord.y % 25 == 0)
			gl_FragColor = vec4(0., 0., 1., 1.);
		else
			gl_FragColor = vec4(1., 0., 0., 1.);
	}
	else
	{
		if (pixelCoord.x % 25 == 0 || pixelCoord.y % 25 == 0)
			gl_FragColor = vec4(1., 1., 1., 1.);
		else
			gl_FragColor = vec4(0., 0., 0., 1.);
	}
}

