#version 400

in vec2 texcoord;
in float depth;
in vec3 fs_Position;
out vec4 fragment;

uniform sampler2D u_heightMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_computedNormalMap;
uniform sampler2D u_diffuseMap;
uniform float u_heightScale; 
uniform mat4 u_mvInvTrans;
uniform vec2 u_numPatches;
uniform float u_gridSpacing;
uniform vec4 u_lightColor;
uniform vec3 u_lightDirection;
uniform int u_toggleNormal;
uniform int u_userInteraction;

// water shading
uniform samplerCube u_cubemap;
uniform float u_fresnelR0;
uniform vec3 u_cameraPosition;

//Terrain Manipulator
uniform vec2 u_manipCenter;
uniform float u_manipRadius;

const float manipWidth = 0.0001;

float sampleHeight(vec2 coord)
{
	return u_heightScale*texture(u_heightMap, coord).r;
}

vec3 sampleComputedNormal (vec2 coord)
{
	return texture(u_computedNormalMap, coord).xyz;
}

vec3 sampleNormal(vec2 coord)
{
	return texture(u_normalMap, coord).xyz;
}

vec3 sampleDiffuse(vec2 coord)
{
	return texture(u_diffuseMap, coord).rgb;
}

vec3 getNormalSobel()
{
	const ivec3 off = ivec3(-1.0,0.0,1.0);
	vec2 tSize = 1.0/textureSize(u_heightMap,0);
	float topLeft = sampleHeight( vec2( texcoord + tSize*off.xz));
	float top = sampleHeight( vec2 (texcoord + tSize*off.yz));
	float topRight = sampleHeight( vec2( texcoord + tSize*off.zz));
	float left = sampleHeight( vec2( texcoord + tSize*off.xy));
	float right = sampleHeight( vec2( texcoord + tSize*off.zy));
	float bottomLeft = sampleHeight( vec2( texcoord + tSize*off.xx));
	float bottom = sampleHeight( vec2( texcoord + tSize*off.yx));
	float bottomRight = sampleHeight( vec2( texcoord + tSize*off.zx));


	float dx = (topLeft + 2.0*left+ bottomLeft) - (topRight + 2.0*right+bottomRight) ;
	float dy = (topLeft + 2.0*top+ topRight) - (bottomLeft + 2.0*bottom+bottomRight);
	float dz = 4.0;

	vec3 normal = normalize ( vec3(dx,dy,dz));
	return normal;

}

vec2 tSize = textureSize(u_heightMap,0);
const float samples = 32;
const float occlusionRadius = 1.0;

vec3 getWorldPos( vec2 texcoord)
{
	float h = texture(u_heightMap,texcoord).r;
	return vec3( texcoord*tSize,h);
}

float getOcclusionFactor()
{
	float occlusion = 0.0;
	
	vec3 normal =  sampleComputedNormal(texcoord);
	vec3 tangent = normalize(cross( vec3(0.0,0.0,1.0),normal));
	vec3 bitangent = normalize( cross(tangent,normal));

	mat3 orthobasis = mat3(tangent,normal,bitangent);
	vec3 pos = getWorldPos(texcoord);
	for(int i=1; i<samples+1;++i)
	{
		float s = float(i)/samples;
		float a = sqrt(s* 512.0);
		float b = sqrt(s);

		float x = sin(a)*b*occlusionRadius;
		float y = cos(a)*b*occlusionRadius;

		vec3 sample_uv = orthobasis*vec3(x,y,0.0);
		vec3 sample_pos = getWorldPos( sample_uv.xy);

		vec3 sample_dir = normalize(sample_pos - pos);
		float lambert = clamp( dot(normal,sample_dir),0.0,1.0);

		float dist_factor =40.0/sqrt(length(sample_pos-pos));
		occlusion += dist_factor*lambert;
	}
	return (1.0 - occlusion/samples);
}


///////////////////////
// water shading stuff
float computeFresnelTerm(vec3 normal, vec3 eyeVec)
{
	float cosIncidentAngle = dot(normal, eyeVec);
	return clamp(u_fresnelR0 + (1.0 - u_fresnelR0) * pow(1 - cosIncidentAngle, 5), 0, 1);
}

vec3 computeWaterColor()
{
	vec3 position = fs_Position;
	vec3 shallow = vec3(0, 0.2, 0.5) * 0.1;
	vec3 deep = vec3(0.16, 0.83, 1.0) * 0.9;
	vec3 normal = vec3(0,0,0);
	
	if (u_toggleNormal == 0)
		//normal = getNormalSobel();
		normal = sampleComputedNormal(texcoord);
	else
		normal = sampleNormal(texcoord);
	
	float waterHeight = sampleHeight(texcoord);

	float mixFactor = clamp(waterHeight / 0.00075, 0, 1);
	vec3 color = mix(shallow, deep, sqrt(mixFactor));  
	
	// lighting computation
	float diffuse = max(dot(u_lightDirection, normal),0);
	vec3 eyeVector = normalize(u_cameraPosition - position);
	vec3 reflectedEyeVec = reflect(-eyeVector, normal);	
	float fresnel = computeFresnelTerm(normal, eyeVector);

	float shininess = 0.5;

	float dotSpec = clamp(dot(reflectedEyeVec, -u_lightDirection.xyz) * 0.5 + 0.5, 0.0, 1.0);
	vec3 specular = (1.0 - fresnel) * clamp(u_lightDirection.y, 0 , 1) * ((pow(dotSpec,4.5)) * (shininess * 1.8 + 0.2)) * u_lightColor.xyz;
	specular += specular * 25 * clamp(shininess - 0.05, 0, 1);
	
	vec3 envColor = texture(u_cubemap, normalize(reflectedEyeVec)).rgb;
		
	vec4 waterColor = clamp(vec4(diffuse * color * envColor + specular, 1.0), 0, 1);
	//waterColor = vec4(diffuse * color, 1.0);
	//waterColor = u_lightColor;
	//waterColor = vec4(specular, 1.0);
	//waterColor = vec4(color,1.0);
	//waterColor = vec4(specular + envColor , 1.0);
	//waterColor = vec4(dotSpec, dotSpec, dotSpec, 1.0);
	//waterColor = clamp(vec4(pow(dotSpec, 2.0),pow(dotSpec, 2.0),pow(dotSpec, 2.0),1.0), 0, 1);
	//waterColor = vec4(clamp(u_lightDirection.y, 0 , 1),clamp(u_lightDirection.y, 0 , 1),clamp(u_lightDirection.y, 0 , 1),1);
	//waterColor = vec4((1.0 - fresnel),(1.0 - fresnel),(1.0 - fresnel),1.0);
	//waterColor = vec4(envColor, 1.0);

	return waterColor.rgb;
}



void main(){

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

	if (u_toggleNormal == 0)
		//normal = getNormalSobel();
		normal = sampleComputedNormal(texcoord);
	else
		normal = sampleNormal(texcoord);
	//vec3 color = sampleDiffuse(texcoord);

	vec3 waterColor = computeWaterColor();

	float intensity = max(dot(u_lightDirection, normal), 0.0);
	//vec3 color = color * intensity * u_lightColor.xyz;	//vec3 color = mix( vec3(0.54,0.27,0), vec3(0,0,1), texture(u_heightMap, texcoord).g);
	vec3 terrainColor = vec3(0.54,0.27,0)*intensity*u_lightColor.xyz;
	vec3 color = mix( terrainColor, waterColor, texture(u_heightMap, texcoord).g);
	
	float avgSlope = (diff1 + diff2) / 2.0;

	if (u_userInteraction==1 && u_manipCenter.x >=0.0 && u_manipCenter.x <=1.0 && u_manipCenter.y >=0.0 && u_manipCenter.y <=1.0)
	{
		float distSq = (texcoord.x-u_manipCenter.x)*(texcoord.x-u_manipCenter.x) + 
			(texcoord.y-u_manipCenter.y)*(texcoord.y-u_manipCenter.y);
		float radSq = u_manipRadius*u_manipRadius;

		if ( distSq >= radSq-manipWidth && distSq<=radSq+manipWidth)
		{
			color = vec3(1.0,0.0,0.0);
		}
	}

	float occ = getOcclusionFactor();
	fragment = vec4(color,1.0);
}