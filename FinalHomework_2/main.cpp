#include <cassert>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Angel.h"
#include "Robot.h"
#include "Mesh_Painter.h"
using std::vector;
using std::cout;
using std::endl;



#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "FreeImage.lib")


std::vector<My_Mesh*> my_meshs;
Mesh_Painter* mp_;


const GLfloat PI = 3.1415926;

// 建模类
My_Model m_model;

// 各个着色器变量对应的id
GLuint ShadowMatrix;
GLuint ModelView;
GLuint Projection;
GLuint LightPos;
GLuint IsShadow;
GLuint program;
GLuint draw_color;
GLuint vao;

vector<point4> points;
vector<color4> colors;
vector<point4> normals;

mat4 modelView;
mat4 projection;




// 机器人身体各个部位对应的索引号
enum {
	Torso,
	Head1,
	Head2,
	RightUpperArm,
	RightLowerArm,
	LeftUpperArm,
	LeftLowerArm,
	RightUpperLeg,
	RightLowerLeg,
	LeftUpperLeg,
	LeftLowerLeg,
	AttAngel,
	NumJointAngles,
	Quit
};

// 机器人各个部位的初始角度
GLfloat theta[12] = {
	0.0,    // Torso
	0.0,    // Head1
	0.0,    // Head2
	180.0,    // RightUpperArm
	0.0,    // RightLowerArm
	180.0,    // LeftUpperArm
	0.0,    // LeftLowerArm
	180.0,  // RightUpperLeg
	0.0,    // RightLowerLeg
	180.0,  // LeftUpperLeg
	0.0,     // LeftLowerLeg
	0.0
};

// 用于指定机器人当前转动的部位
GLint angle = Head2;

// 模型的移动向量
vec3f model_translation(0.0);

// 机器人移动向量
vec3f human_translateion(0.0);
const float step = 2;//移动速度
const float speed = 0.5;


// 建筑参数
vec3 scale_rate_build = vec3(2, 2, 2);
vec3 translate_build = vec3(-50, 0, 0);		// z控制地面左右，y控制地面上下
vec3 rotate_angel_build = vec3(0, 0, 0);

// 地面参数
vec3 scale_rate_ground = vec3(2, 2, 2);
vec3 translate_ground = vec3(-50, -1.01, 0);		// z控制地面左右，y控制地面上下
vec3 rotate_angel_ground = vec3(0, 0, 0);

// 房子参数
vec3 scale_rate_house = vec3(0.2, 0.2, 0.2);
vec3 translate_house = vec3(0, 0, 200);		// z控制地面左右，y控制地面上下
vec3 rotate_angel_house = vec3(0, 180, 0);

// 树参数
vec3 scale_rate_tree = vec3(1, 1, 1);
vec3 translate_tree = vec3(15, 0, -40);		// z控制地面左右，y控制地面上下
vec3 rotate_angel_tree = vec3(0, 0, 0);

// 皮卡丘参数
vec3 scale_rate_pikaqiu = vec3(0.2, 0.2, 0.2);
vec3 translate_pikaqiu = vec3(15 * 5, 0, -30 * 5);		// z控制地面左右，y控制地面上下
vec3 rotate_angel_pikaqiu = vec3(0, 0, 0);
My_Mesh* my_mesh_pikaqiu;

// 太阳参数
float sun_angel = PI / 2;
const int light_height = 50, zpos = -30;//光源和主体有一定的角度
vec3 lightPos(light_height * cos(sun_angel), light_height * sin(sun_angel), zpos);//光源

vec3 scale_rate_sun = vec3(10, 10, 10);
vec3 translate_sun = vec3(light_height * cos(sun_angel), light_height * sin(sun_angel), zpos);		// z控制地面左右，y控制地面上下
vec3 rotate_angel_sun = vec3(0, 0, 0);
My_Mesh* my_mesh_sun;


// 计算阴影投影矩阵，绘制投影之后的三角形（用黑色表示）
mat4 shadowProjMatrix(
	-lightPos[1], 0, 0, 0,
	lightPos[0], 0, lightPos[2], 1,
	0, 0, -lightPos[1], 0,
	0, 0, 0, -lightPos[1]);
// 坐标轴常量
enum {
	X_axis = 0,
	Y_axis = 1,
	Z_axis = 2
};





bool JUMP_FLAG = false;//跳跃动画
bool MOVE_FLAG = false;//移动手臂摆动动画
bool TURN_LEFT_FLAG = false;//左转
bool TURN_RIGHT_FLAG = false;//右转
bool NO_TURN_FLAG = true;//不转
bool Att_FLAG = false;//攻击动画
int Dir = 1;//皮卡丘跳跃动画
//相机参数


// 相机参数设置
namespace Camera
{
	GLfloat phi = PI / 2;	// 绕x轴的角度
	GLfloat theta =  0.0872665;	// 绕y轴的角度
	GLfloat radius = 15;
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;

	mat4 ortho(const GLfloat left, const GLfloat right,
		const GLfloat bottom, const GLfloat top,
		const GLfloat zNear, const GLfloat zFar)
	{
		mat4 m;
		m[0][0] = 2 / (right - left);
		m[0][3] = -(right + left) / (right - left);
		m[1][1] = 2 / (top - bottom);
		m[1][3] = -(top + bottom) / (top - bottom);
		m[2][2] = -2 / (zFar - zNear);
		m[2][3] = -(zFar + zNear) / (zFar - zNear);
		m[3][3] = 1;
		return m;
	}

	mat4 perspective(const GLfloat fovy, const GLfloat aspect,
		const GLfloat zNear, const GLfloat zFar)
	{
		GLfloat top = tan(fovy * DegreesToRadians / 2) * zNear;
		GLfloat right = top * aspect;

		mat4 m;
		m[0][0] = zNear / right;
		m[1][1] = zNear / top;
		m[2][2] = -(zFar + zNear) / (zFar - zNear);
		m[2][3] = -(2 * zFar * zNear) / (zFar - zNear);
		m[3][2] = -1;
		return m;
	}

	mat4 lookAt(const vec4& eye, const vec4& at, const vec4& up)
	{
		vec4 n = normalize(eye - at);
		vec3 uu = normalize(cross(up, n));
		vec4 u = vec4(uu.x, uu.y, uu.z, 0.0);
		vec3 vv = normalize(cross(n, u));
		vec4 v = vec4(vv.x, vv.y, vv.z, 0.0);
		vec4 t = vec4(0.0, 0.0, 0.0, 1.0);
		mat4 c = mat4(u, v, n, t);
		return c * Translate(-eye);
	}
}

//添加模型给my_meshs
My_Mesh* add_obj(string obj,string jpg,vec3 scale,vec3 trans,vec3 rotate,int shadow)
{
	My_Mesh* my_mesh_obj = new My_Mesh;//新建一个对象
	my_mesh_obj->load_obj(obj);//读取模型
	my_mesh_obj->set_texture_file(jpg);//贴图
	my_mesh_obj->set_scale_rate(scale);//缩放
	my_mesh_obj->set_translate(trans);//移动
	my_mesh_obj->set_rotate_angel(rotate);//旋转
	my_mesh_obj->set_shadow(shadow);//阴影
	my_mesh_obj->set_shader("v_texture_shader.glsl", "f_texture_shader.glsl");//着色器
	my_meshs.push_back(my_mesh_obj);
	mp_->add_mesh(my_mesh_obj);
	return my_mesh_obj;
}



// 初始化函数，初始化一些参数信息
void init()
{
	//初始化相机变换参数
	using Camera::phi;
	using Camera::theta;
	using Camera::radius;

	point4  eye(human_translateion.x + radius*cos(theta)*cos(phi),
		radius*sin(theta),
		human_translateion.z + radius*cos(theta)*sin(phi),
		1.0);
	point4  at(human_translateion.x, 6, human_translateion.z, 1.0);
	vec4    up(0.0, 1.0, 0.0, 0.0);
	Camera::modelMatrix = mat4(1.0);
	Camera::viewMatrix = Camera::lookAt(eye, at, up);
	Camera::projMatrix = Camera::perspective(45, 1.0, 0.1, 250);

	//添加光照，模型
	mp_ = new Mesh_Painter;
	mp_->set_lightPos(lightPos);
	mp_->set_matrix(Camera::modelMatrix, Camera::projMatrix);

	// 建筑
	add_obj("texture/myworld.obj", "texture/myworld.jpg", scale_rate_build,
		translate_build, rotate_angel_build, 1);
	//房子
	add_obj("texture/house.obj", "texture/house.jpg", scale_rate_house,
		translate_house, rotate_angel_house, 1);
	//树
	add_obj("texture/tree.obj", "texture/tree.jpg", scale_rate_tree,
		translate_tree, rotate_angel_tree, 1);
	//皮卡丘
	my_mesh_pikaqiu = add_obj("texture/pikaqiu.obj", "texture/pikaqiu.jpg", scale_rate_pikaqiu,
		translate_pikaqiu, rotate_angel_pikaqiu, 1);
	//地面
	add_obj("texture/ground.obj", "texture/ground.jpg", scale_rate_ground,
		translate_ground, rotate_angel_ground, 0);
	//太阳
	my_mesh_sun = add_obj("texture/sun.obj", "texture/sun.jpg", scale_rate_sun, 
		translate_sun, rotate_angel_sun,0);

	//加载模型
	mp_->init_shaders();
	mp_->update_vertex_buffer();
	mp_->update_texture();

	//绘制单位立方体
	m_model.init_cube(red);

	//或者单位立方体参数
	points = m_model.get_points();
	colors = m_model.get_colors();
	normals = m_model.get_normals();

	// Create a vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point4) * points.size() + sizeof(color4) * colors.size() +
		sizeof(point4) * normals.size(), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point4) * points.size(), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * points.size(), sizeof(color4) * colors.size(),
		&colors[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * points.size() + sizeof(color4) * colors.size(),
		sizeof(point4) * normals.size(), &normals[0]);

	// Load shaders and use the resulting shader program
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point4) * points.size()));

	// 法向量
	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point4) * points.size() + sizeof(color4) * colors.size()));


	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");
	draw_color = glGetUniformLocation(program, "draw_color");
	LightPos = glGetUniformLocation(program, "lightPos");
	IsShadow = glGetUniformLocation(program, "isShadow");
	ShadowMatrix = glGetUniformLocation(program, "shadowMatrix");
	//glUseProgram(0);

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glDepthFunc(GL_LESS);		// 深度测试函数

	//glEnable(GL_CULL_FACE);	// 面剔除
	glClearColor(135/255.0, 206/255.0, 250/255.0, 0.2);
}


// 渲染函数，负责更新图形界面
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	lightPos = vec3(light_height * cos(sun_angel), light_height * sin(sun_angel), zpos);//重新计算阴影矩阵
	shadowProjMatrix = mat4(
		-lightPos[1], 0, 0, 0,
		lightPos[0], 0, lightPos[2], 1,
		0, 0, -lightPos[1], 0,
		0, 0, 0, -lightPos[1]);

	//重新配置皮卡丘的位置变换矩阵
	my_mesh_pikaqiu->set_translate(translate_pikaqiu);
	//重新配置太阳的位置变换矩阵
	translate_sun = vec3(light_height * cos(sun_angel), light_height * sin(sun_angel), zpos);
	my_mesh_sun->set_translate(translate_sun);
	//重新配置光源
	mp_->set_lightPos(lightPos);
	//重新配置相机观察矩阵
	mp_->draw_meshes(Camera::viewMatrix);

	// 切换着色器
	glUseProgram(program);
	glBindVertexArray(vao);

	glUniform3fv(LightPos, 1, &lightPos[0]);


	// 绘制人，初始参数
	modelView = Scale(1, 1, 1) * Translate(0, 1.25, 0);
	// 键盘控制移动机器人
	modelView = Translate(human_translateion) * modelView;
	m_model.draw_human();
	glUniform1i(IsShadow, 1);	// 绘制阴影
	modelView = shadowProjMatrix * modelView;
	m_model.draw_human();
	glUniform1i(IsShadow, 0);	// 停止绘制阴影
	glutSwapBuffers();
}

// 当窗口大小改变时调用此函数，根据长宽变化调整一些参数信息，
// 使整个t图形界面保持美观、结构等
void reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	GLfloat left = -6.0, right = 6.0;
	GLfloat bottom = -3.0, top = 9.0;
	GLfloat zNear = -6.0, zFar = 6.0;

	// 调整视窗体，使之适应窗口而不变形
	GLfloat aspect = GLfloat(width) / height;
	if (aspect > 1.0) {
		left *= aspect;
		right *= aspect;
	}
	else {
		bottom /= aspect;
		top /= aspect;
	}
	

	using Camera::phi;
	using Camera::theta;
	using Camera::radius;
	point4  eye(human_translateion.x+radius*cos(theta)*cos(phi),
		radius*sin(theta),
		human_translateion.z+radius*cos(theta)*sin(phi),
		1.0);
	point4  at(human_translateion.x, 6, human_translateion.z, 1.0);
	vec4    up(0.0, 1.0, 0.0, 0.0);
	Camera::viewMatrix = Camera::lookAt(eye, at, up);
	projection = Camera::projMatrix *  Camera::viewMatrix;   //透视投影矩阵
	//projection = Camera::ortho(left, right, bottom, top, zNear, zFar) *  Camera::lookAt(eye, at, up);
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

}


// 键盘函数
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	// 机器人移动控制
	case 'w':
		MOVE_FLAG = true; 
		if (TURN_LEFT_FLAG)
		{
			theta[Torso] -= 45;
			theta[Head1] += 45;
			TURN_LEFT_FLAG = false;
		}
		if (TURN_RIGHT_FLAG)
		{
			theta[Torso] += 45;
			theta[Head1] -= 45;
			TURN_RIGHT_FLAG = false;
		}
		NO_TURN_FLAG = true;
		human_translateion.x -= step * cos(Camera::phi);
		human_translateion.z -= step * sin(Camera::phi);
		break;
	case 's':
		MOVE_FLAG = true;
		if (TURN_LEFT_FLAG)
		{
			theta[Torso] -= 45;
			theta[Head1] += 45;
			TURN_LEFT_FLAG = false;
		}
		if (TURN_RIGHT_FLAG)
		{
			theta[Torso] += 45;
			theta[Head1] -= 45;
			TURN_RIGHT_FLAG = false;
		}
		NO_TURN_FLAG = true;
		human_translateion.x += step * cos(Camera::phi);
		human_translateion.z += step * sin(Camera::phi);
		break;
	case 'a':
		MOVE_FLAG = true;
		if (NO_TURN_FLAG)
		{
			theta[Torso] += 45;
			theta[Head1] -= 45;
			NO_TURN_FLAG = false;
		}
		if (TURN_RIGHT_FLAG)
		{
			theta[Torso] += 90;
			theta[Head1] -= 90;
			TURN_RIGHT_FLAG = false;
		}
		TURN_LEFT_FLAG = true;
		human_translateion.x -= step * sin(Camera::phi);
		human_translateion.z += step * cos(Camera::phi);

		//human_translateion.x -= step;
		break;
	case 'd':
		MOVE_FLAG = true;
		if (NO_TURN_FLAG)
		{
			theta[Torso] -= 45;
			theta[Head1] += 45;
			NO_TURN_FLAG = false;
		}
		if (TURN_LEFT_FLAG)
		{
			theta[Torso] -= 90;
			theta[Head1] += 90;
			TURN_LEFT_FLAG = false;
		}
		TURN_RIGHT_FLAG = true;
		human_translateion.x += step * sin(Camera::phi);
		human_translateion.z -= step * cos(Camera::phi);

		//human_translateion.x += step;
		break;
	case ' ':
		JUMP_FLAG = true;
		break;

	case '+':
		Camera::radius += 0.5;
		break;
	case '-':
		if (Camera::radius>0.5)
			Camera::radius -= 0.5;
		break;
	}
	
	// 由于projection和相机变换都放在reshape里，所以调用reshape重绘
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	reshape(width, height);
	glutPostRedisplay();
}


// 鼠标
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		Att_FLAG = true;
	}
}


// 键盘上下左右键响应函数
void specialKeyboard(int key, int x, int y)
{
	switch (key)
	{
	// 控制相机的上下移动
	case GLUT_KEY_DOWN:
		if (Camera::radius < 25.0)
			Camera::radius += 0.5;
		else if (Camera::theta+ PI / 18 < PI / 2)
		{
			Camera::theta += PI / 18;
		}
		//cout << Camera::theta << " " << Camera::radius << endl;
		break;
	case GLUT_KEY_UP:
		if (Camera::radius >= 25.0&& Camera::theta - PI / 18 > 0)
			Camera::theta -= PI / 18;
		else if (Camera::radius > 5.5)
		{
			Camera::radius -= 0.5;
		}

		break;

	// 控制相机的左右移动
	case GLUT_KEY_RIGHT:
		theta[Torso]-= 10;
		if (theta[angle] > 360.0) { theta[angle] -= 360.0; }
		Camera::phi += PI / 18;
		break;
	case GLUT_KEY_LEFT:
		theta[Torso]+= 10;
		if (theta[Torso] > 360.0) { theta[Torso] -= 360.0; }
		Camera::phi -= PI / 18;
		break;
	case GLUT_KEY_F5:
		sun_angel += PI / 64;
		break;
	}
	// 由于projection和相机变换都放在reshape里，所以调用reshape重绘
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	reshape(width, height);
	glutPostRedisplay();
	
}




// 动画
void idle()
{

	static int cnt1 = 0;
	static int cnt2 = 0;
	if (Att_FLAG)//攻击动画
	{
		if (theta[RightUpperArm] >=180&&theta[RightUpperArm] < 210)
		{
			theta[RightUpperArm] += 0.2*speed;
			theta[AttAngel] -= 0.4*speed;
		}
		else if (theta[RightUpperArm] >= 180 &&theta[RightUpperArm] < 240)
		{
			theta[RightUpperArm] += 0.2*speed;
			theta[AttAngel] += 0.8*speed;
		}
		else
		{
			theta[RightUpperArm] = 180;
			theta[AttAngel] = 0;
			Att_FLAG = false;
		}
	}
	if (JUMP_FLAG)
	{
		if (cnt1 < 300/speed)
		{
			human_translateion.y += 0.01*speed;
			cnt1++;
			glutPostRedisplay();

		}
		else if (cnt1 < 600/speed)
		{
			human_translateion.y -= 0.01*speed;
			cnt1++;
			glutPostRedisplay();
		}
		else
		{
			cnt1 = 0;
			JUMP_FLAG = false;
		}
	}
    if (MOVE_FLAG)
	{
		static bool DirFlag = false;
		if (cnt2 < 300/ speed)
		{
			if (DirFlag)
			{
				theta[LeftUpperArm] += 0.1*speed;
				theta[RightUpperArm] -= 0.1*speed;

				theta[LeftUpperLeg] -= 0.15*speed;
				theta[RightUpperLeg] += 0.15*speed;
			}
			else
			{
				theta[LeftUpperArm] -= 0.1*speed;
				theta[RightUpperArm] += 0.1*speed;

				theta[LeftUpperLeg] += 0.15*speed;
				theta[RightUpperLeg] -= 0.15*speed;
			}
			cnt2++;
			glutPostRedisplay();

		}
		else if (cnt2 < 600/ speed)
		{
			if (DirFlag)
			{
				theta[LeftUpperArm] -= 0.1*speed;
				theta[RightUpperArm] += 0.1*speed;

				theta[LeftUpperLeg] += 0.15*speed;
				theta[RightUpperLeg] -= 0.15*speed;
			}
			else
			{
				theta[LeftUpperArm] += 0.1*speed;
				theta[RightUpperArm] -= 0.1*speed;

				theta[LeftUpperLeg] -= 0.15*speed;
				theta[RightUpperLeg] += 0.15*speed;
			}
			cnt2++;
			glutPostRedisplay();
		}
		else
		{
			cnt2 = 0;
			DirFlag = !DirFlag;
			MOVE_FLAG = false;
		}
	}
	translate_pikaqiu.y += Dir * 0.01;
	if (translate_pikaqiu.y >= 5)Dir *= -1;
	if (translate_pikaqiu.y <= 0)Dir *= -1;
	glutPostRedisplay();

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("2017151028_吴俊生_期末大作业");

	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutSpecialFunc(specialKeyboard);

	glutMainLoop();
	return 0;
}