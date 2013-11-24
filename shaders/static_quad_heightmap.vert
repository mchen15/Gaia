#version 400
    in vec4 Position;
    uniform sampler2D u_heightMap;
    
    void main(void){
        vec2 texcoord = Position.xy;
        //float height = texture(u_heightMap, texcoord).r;
		float height = 0.0;
        vec4 displaced = vec4(Position.x, Position.y, height, 1.0);
        gl_Position = displaced;
    }