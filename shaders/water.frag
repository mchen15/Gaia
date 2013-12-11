#version 330

in vec2 v_Texcoord;
in vec3 v_Position;

uniform sampler2D u_heightMap;
uniform sampler2D u_normalMap;
uniform vec4 u_lightColor;
uniform vec3 u_lightDirection;
uniform float fresnelR0 = 0.5;

out vec4 fragColor;

vec3 sampleNormal(vec2 texcoord)
{
	return texture(u_normalMap, texcoord).xyz;
}

float sampleHeight(vec2 texcoord)
{
	return texture(u_heightMap, texcoord).x;
}

float computeFresnelTerm(float incidentAngle)
{
	return fresnelR0 + (1.0 - fresnelR0) * pow(1 - cos(incidentAngle), 5);
}

void main(void)
{
	vec3 position = v_Position;
	vec3 shallow = vec3(0, 0.2, 0.5) * 0.5;
	vec3 deep = vec3(0.16, 0.83, 1.0) * 0.9;
	vec3 normal = sampleNormal(v_Texcoord);
	
	float waterHeight = sampleHeight(v_Texcoord);

	float mixFactor = clamp(waterHeight / 0.00075, 0, 1);
	vec3 color = mix(shallow, deep, sqrt(mixFactor));  
	
	// lighting computation
	float diffuse = max(dot(u_lightDirection, normal),0);


	fragColor = vec4(diffuse * color, 1.0);
}
