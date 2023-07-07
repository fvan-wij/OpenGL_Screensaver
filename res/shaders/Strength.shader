#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
//uniform vec3 u_Transform;
uniform float sineDriver;

void main()
{
	float radius = 0.1;
	float speed = 50.0;
	float angle = sineDriver * speed;
	vec3 offset = vec3(radius * sin(angle), radius * cos(angle), 0.0);
	gl_Position = vec4(position.x , position.y , position.z , 1.0f);
};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
uniform float u_Time;
uniform vec2 u_Transform;

float random(vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main()
{
    vec2 uv = gl_FragCoord.xy / vec2(800.0, 800.0);  // Adjust the resolution as needed
    
    // Create a colorful gradient background
    vec3 background = vec3(uv.x, 0.1, uv.y);
    
    // Add pulsating circles
    vec2 center1 = vec2(0.5 + u_Transform.x, 0.5 + u_Transform.y);  // Adjust the center position of the first circle
    vec2 center2 = vec2(0.75 + u_Transform.x, 0.5 + u_Transform.y);  // Adjust the center position of the second circle
    vec2 center3 = vec2(1 + u_Transform.x, 0.5 + u_Transform.y);  // Adjust the center position of the second circle
    
    float radius1 = 0.9 + sin(u_Time) * 0.1;  // Adjust the radius of the first circle with pulsation
    float radius2 = 0.2 + cos(u_Time * 0.8) * 0.15;  // Adjust the radius of the second circle with pulsation
    float radius3 = 0.4 + sin(u_Time * 1.5) * 0.15;  // Adjust the radius of the second circle with pulsation

    float dist1 = length(uv - center1);
    float dist2 = length(uv - center2);
    float dist3 = length(uv - center3);
    
    float circle1 = smoothstep(radius1 - 0.01, radius1 + 0.01, dist1);
    float circle2 = smoothstep(radius2 - 0.01, radius2 + 0.01, dist2);
    float circle3 = smoothstep(radius3 - 0.01, radius3 + 0.01, dist3);
    
    vec3 circleColor1 = vec3(1.0, 0.0, 0.0);  // Adjust the color of the first circle
    vec3 circleColor2 = vec3(0.0, 0.0, 1.0);  // Adjust the color of the second circle
    vec3 circleColor3 = vec3(1.0, 0.0, 1.0);  // Adjust the color of the second circle
    
    vec3 circle1Color = circleColor1 * circle1;
    vec3 circle2Color = circleColor2 * sqrt(circle2) / (uv.x / uv.x / u_Time);
    vec3 circle3Color = circleColor3 * sqrt(circle3) / (uv.x / uv.x / u_Time);
    
    // Create a flickering effect with noise
    float noiseIntensity = 0.1;  // Adjust the intensity of the noise flicker
    float noise = random(floor(uv * 1000.0 + u_Time)) * noiseIntensity;
    
    // Combine the effects and add some grain
    vec3 finalColor = mix(background,  sin(circle3Color), 0.8) + vec3(noise);
    
    color = vec4(finalColor, 1.0);
}
