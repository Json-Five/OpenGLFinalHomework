#version 330 core

in  vec4 vPosition;
in  vec4 vColor;
in  vec4 vNormal;

out vec4 color;
out vec3 N;
out vec3 V;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform mat4 shadowMatrix;
uniform vec4 draw_color;
uniform int isShadow;

void main() 
{
    //color = vColor;
	color = draw_color;
	vec4 t = Projection * ModelView * vPosition;
    //gl_Position = vec4(t.xyz / t.w, 1.0);

	// �ж��Ƿ�����Ӱ
	if (isShadow == 1.0)
	{
		color = vec4(0.1,0.1,0.1,1.0);
		gl_Position = vec4(t.xyz / t.w, 1.0);
	}
	else
	{
		gl_Position = t;
		// TODO ������任���������ϵ��
		vec4 vertPos_cameraspace = ModelView * vPosition;
	
		// TODO ���������任���������ϵ�²�����ƬԪ��ɫ��
		N = (ModelView * vNormal).xyz;
	
		// �Զ���������͸��ͶӰ
		V = vertPos_cameraspace.xyz / vertPos_cameraspace.w;
	}	
} 