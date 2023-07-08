#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main()
{
   gl_Position = position;
};



#shader fragment
#version 330 core

#define MARKER_RADIUS 50.0

layout(location = 0) out vec4 out_color;
uniform vec4 u_Color;
uniform vec2 u_Mouse;
uniform float u_Time;

void marker(vec2 center, vec4 color)
{	
	if (length(gl_FragCoord.xy - center) < MARKER_RADIUS)
	{
		out_color += color;			
	}
}

void main()
{
	vec2 iResolution = vec2(1600.0, 1600.0);
	vec2 p = gl_FragCoord.xy / iResolution.xy;

	vec3 col = vec3(0.8, 0.2, 0.4);		
	col.x *= p.x * p.y + sin(u_Time);
	//col += length(p - vec2(0.5, 0.25));
	out_color = vec4(col, 1.0);
	//marker(u_Mouse, vec4(1.0, 0.0, 0.0, 1.0));
};
