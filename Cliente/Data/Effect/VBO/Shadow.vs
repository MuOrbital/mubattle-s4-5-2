#version 330

layout(location = 0) in vec3 aPos;
layout(location = 3) in uvec2 aBone;

uniform mat4 uMVP;
uniform vec4 u_Bones[200*3];
uniform vec4 u_bodyLight;
uniform vec4 u_setting1;
uniform vec4 u_setting2;
out vec4 v_color0;

void main(){

    int offset = int(aBone.x);
	vec4 BoneTransform;
    BoneTransform.x = dot(u_Bones[offset], vec4(aPos, 1.0));
    BoneTransform.y = dot(u_Bones[offset + 1], vec4(aPos, 1.0));
    BoneTransform.z = dot(u_Bones[offset + 2], vec4(aPos, 1.0));
    BoneTransform.w = 1.0;
	
	vec4 Temp1 = BoneTransform - u_setting2;
    vec4 Temp2 = Temp1 + u_setting1;
    
    float rcp_z = 1.0 / Temp2.z;
    Temp2.w = Temp2.x * rcp_z;
    Temp1.x = (Temp1.z * Temp2.w) + Temp1.x;
    
    Temp1.z = u_setting1.y;
    Temp1 = Temp1 + u_setting2;
	
	v_color0 = u_bodyLight;

    gl_Position = uMVP * Temp1;
}