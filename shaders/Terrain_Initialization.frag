#version 400

uniform sampler2D u_heightMap;
uniform float u_heightScale;

//Noise params
uniform float uH = 1.0;
uniform float uLacunarity=4.0;
uniform int uOctaves = 16;

in vec3 fs_Position;
in vec2 fs_Texcoord;

// using layout to enforce the location of each out
layout (location = 0) out vec4 out_flux;
layout (location = 1) out vec4 out_terrainAttr;
layout (location = 2) out vec4 out_velocity;

vec4 sampleHeightMap(vec2 texcoord)
{
	return texture(u_heightMap, texcoord);
}

//
// Description : Array and textureless GLSL 2D simplex noise function.
// Author : Ian McEwan, Ashima Arts.
// Maintainer : ijm
// Lastmod : 20110822 (ijm)
// License : Copyright (C) 2011 Ashima Arts. All rights reserved.
// Distributed under the MIT License. See LICENSE file.
// https://github.com/ashima/webgl-noise
//

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187, // (3.0-sqrt(3.0))/6.0
                      0.366025403784439, // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626, // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i = floor(v + dot(v, C.yy) );
  vec2 x0 = v - i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
                + i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x = a0.x * x0.x + h.x * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

//https://github.com/jdupuy/fractalTerrain
// ridged multifractal terrain (p.504)
float fBm3(vec2 p, float H, float lacunarity, int octaves, float offset, float gain) {
        float result, frequency, signal, weight;

        frequency = 1.0;

        signal = offset - abs(snoise(p));
        signal*= signal;
        result = signal;
        weight = 1.0;

        for(int i=1; i<octaves; ++i) {
                p*= lacunarity;
                weight = clamp(signal*gain, 0.0,1.0);
                signal = offset - abs(snoise(p));
                signal*= signal*weight;
                result+= signal * pow(frequency, -H);
                frequency*= lacunarity;
        }

        return result;

}


void main (void)
{
	vec4 height = u_heightScale * sampleHeightMap(fs_Texcoord);
	//float h = fBm3(fs_Texcoord, uH, uLacunarity, uOctaves, 0.8, 2.1)*u_heightScale;
	out_flux = vec4(0,0,0,0);
	out_terrainAttr = vec4(height.r,0,0,1);
	out_velocity = vec4(0,0,0,1);
}
