#ifdef GL_ES
precision highp float;
#endif

void main()
{
/*
	vec2 position = ( gl_FragCoord.xy / resolution.xy );
	float color = 0.0;
	float r = 0.3;
	vec2 center;
	center.x = 0.5; center.y = 0.5;

	if( (position.x - center.x)* (position.x - center.x) + (position.y - center.y) *(position.y - center.y) <= r*r)
		color = 1.0;
	if ((position.x - center.x ) >= 0.0)
	{
		if ((position.y-center.y)> 0.0 && (position.x - center.x) > (position.y - center.y) )
			color = 0.0;	
		if ((position.y - center.y ) < 0.0 && (position.x - center.x) > (center.y - position.y))
			color = 0.0;
	}

	r = 0.05; 
	center.x = 0.8;
	if( (position.x - center.x)* (position.x - center.x) + (position.y - center.y) *(position.y - center.y) <= r*r)
		color = 1.0;
	center.x = 0.95;
	if( (position.x - center.x)* (position.x - center.x) + (position.y - center.y) *(position.y - center.y) <= r*r)
		color = 1.0;	

	gl_FragColor = vec4( vec3( color*0.8, color*0.8, 0), 1.0 );
	*/
	if (gl_FragCoord.x < 1 )
		gl_FragColor = vec4(0,0,0,0);
	else
	gl_FragColor = vec4(gl_FragCoord.xyz,1.0);
}