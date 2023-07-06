#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
uniform vec3 u_Transform;
uniform float sineDriver;

void main()
{
	float radius = 0.1;
	float speed = 50.0;
	float angle = sineDriver * speed;
	vec3 offset = vec3(radius * sin(angle), radius * cos(angle), 0.0);
	gl_Position = vec4(position.x + u_Transform.x + offset.x, position.y + u_Transform.y + offset.y, position.z + u_Transform.z, 1.0f);
};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
uniform vec3 u_Color;

void main()
{
	color = vec4(u_Color, 1.0);
};
