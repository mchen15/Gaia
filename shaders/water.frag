#version 330

in vec2 v_Texcoord;
in vec3 v_Position;

uniform sampler2D u_heightMap;
uniform sampler2D u_normalMap;
uniform vec4 u_lightColor;
uniform vec3 u_lightDirection;
uniform float u_fresnelR0;
uniform vec3 u_cameraPosition;

out vec4 fragColor;

vec3 sampleNormal(vec2 texcoord)
{
	return texture(u_normalMap, texcoord).xyz;
}

float sampleHeight(vec2 texcoord)
{
	return texture(u_heightMap, texcoord).x;
}

float computeFresnelTerm(vec3 normal, vec3 eyeVec)
{
	float cosIncidentAngle = dot(normal, eyeVec);
	return clamp(u_fresnelR0 + (1.0 - u_fresnelR0) * pow(1 - cosIncidentAngle, 5), 0, 1);
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
	vec3 eyeVector = normalize(u_cameraPosition - position);
	vec3 reflectedEyeVec = reflect(-eyeVector, normal);	
	float fresnel = computeFresnelTerm(normal, eyeVector);

	float shininess = 0.5;

	float dotSpec = clamp(dot(reflectedEyeVec, -u_lightDirection.xyz) * 0.5 + 0.5, 0.0, 1.0);
	vec3 specular = (1.0 - fresnel) * clamp(-u_lightDirection.y, 0 , 1) * ((pow(dotSpec, 512.0)) * (shininess * 1.8 + 0.2)) * u_lightColor.xyz;
	specular += specular * 25 * clamp(shininess - 0.05, 0, 1);
	
	fragColor = vec4(diffuse * color + specular, 1.0);
}
