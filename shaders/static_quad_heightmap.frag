#version 400

in vec2 texcoord;
in float depth;
out vec4 fragment;

uniform sampler2D u_heightMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_diffuseMap;
uniform float u_heightScale; 
uniform mat4 u_mvInvTrans;
uniform vec2 u_numPatches;
uniform float u_gridSpacing;

vec3 incident = normalize(vec3(1.0, 5.2, 0.5));
vec4 light = vec4(1.0, 0.95, 0.9, 1.0) * 1.1;

float sampleHeight(vec2 coord)
{
	return u_heightScale*texture(u_heightMap, coord).r;
}

vec3 sampleNormal(vec2 coord)
{
	return texture(u_normalMap, coord).xyz;
}

vec3 sampleDiffuse(vec2 coord)
{
	return texture(u_diffuseMap, coord).rgb;
}

void main(){
    //vec3 normal = normalize(texture(terrain, texcoord).xyz);
    //vec4 color = texture(diffuse, texcoord);
    //float noise_factor = texture(noise_tile, texcoord*32).r+0.1;

    //float dot_surface_incident = max(0, dot(normal, incident));

    //color = color * light * noise_factor * (max(0.1, dot_surface_incident)+0.05)*1.5;
    //fragment = mix(color, color*0.5+vec4(0.5, 0.5, 0.5, 1.0), depth*2.0);

	// compute normal

	//vec2 stepSize = 1.0 / u_numPatches;
	vec2 stepSize = 1.0 / textureSize(u_heightMap, 0);

	float h21 = sampleHeight(vec2(texcoord.s + stepSize.s, texcoord.t));
	float h01 = sampleHeight(vec2(texcoord.s - stepSize.s, texcoord.t));
	float h12 = sampleHeight(vec2(texcoord.s, texcoord.t + stepSize.t));
	float h10 = sampleHeight(vec2(texcoord.s, texcoord.t - stepSize.t));

	//h21 = h01;
	//h12 = h10;

	float diff1 = h21 - h01;
	float diff2 = h12 - h10;

	vec3 slopeX = vec3(
		2.0 * u_gridSpacing,
		0.0,
		h21 - h01
	);

	vec3 slopeY = vec3(
		0.0,
		2.0 * u_gridSpacing,
		h12 - h10
	);

	vec3 normal  = normalize(u_mvInvTrans * vec4(normalize(cross(slopeX, slopeY)), 0.0)).xyz;
	
	// using normal map
	normal = sampleNormal(texcoord);

	vec3 color = sampleDiffuse(texcoord);

	




	float intensity = max(dot(incident, normal), 0.0);
	color = color * intensity * light.xyz;

	float avgSlope = (diff1 + diff2) / 2.0;


	fragment = vec4(color,1.0);
	//fragment = vec4(diff1, 0, 0, 1.0);
	//fragment = vec4(diff2, 0, 0, 1.0);
	//fragment = vec4(avgSlope, 0, 0, 1.0);

	//fragment = vec4(slopeY, 1.0);
	//fragment = vec4(normal, 1.0);
	//fragment = vec4(1,1,1,1);
}