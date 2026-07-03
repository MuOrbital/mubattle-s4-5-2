#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTex;
layout(location = 3) in uint aBone;

uniform mat4 uProj;
uniform mat4 uView;
uniform vec4 u_Bones[200*3];

uniform vec4 u_bodyLight;
uniform vec4 u_lightPosition;
uniform vec4 u_meshUV;
uniform bool u_enableLight;

out vec4 v_color0;
out vec2 v_texcoord0;
out float v_fogDepth;


void main() {
    int offset = int(aBone);
	vec4 BoneTransform;
    BoneTransform.x = dot(u_Bones[offset], vec4(aPos, 1.0));
    BoneTransform.y = dot(u_Bones[offset + 1], vec4(aPos, 1.0));
    BoneTransform.z = dot(u_Bones[offset + 2], vec4(aPos, 1.0));
    BoneTransform.w = 1.0;
	
    vec4 tmpvar_2;
    tmpvar_2 = BoneTransform;
	
	vec4 coreViewPosition = uView * tmpvar_2;
    gl_Position = uProj * coreViewPosition;
    v_fogDepth = abs(coreViewPosition.z);
	
	vec3 NorTransform;
    NorTransform.x = dot(u_Bones[offset].xyz, aNorm);
    NorTransform.y = dot(u_Bones[offset + 1].xyz, aNorm);
    NorTransform.z = dot(u_Bones[offset + 2].xyz, aNorm);
	
	vec3 tmpvar_3;
	tmpvar_3 = normalize(NorTransform);
	
	if(u_enableLight)
	{
	   	float tmpvar_4;
		tmpvar_4 = max(((((dot(tmpvar_3, u_lightPosition.xyz) * 0.8) + 0.4)* u_lightPosition.w) + (1.0 - u_lightPosition.w)), 0.2);
        vec4 tmpvar_5;
        tmpvar_5.w = u_lightPosition.w;
        tmpvar_5.x = tmpvar_4;
        tmpvar_5.y = tmpvar_4;
        tmpvar_5.z = tmpvar_4;
		v_color0 = clamp((u_bodyLight * tmpvar_5), 0.0, 1.0);
	}
	else
	{
	  	v_color0 = clamp(u_bodyLight, 0.0, 1.0);	
	}
	vec2 tmpvar_6;
	tmpvar_6.x = tmpvar_3.x + u_meshUV.x;
	tmpvar_6.y = tmpvar_3.y + u_meshUV.y;
	v_texcoord0 = tmpvar_6;
}
