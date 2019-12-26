#include "Robot.h"

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
	AttAngles,
	NumJointAngles,
	Quit
};

// ****************************************************************************** 
//							 分别绘制机器人的各个部位

void torso()
{
	// 对边长为1的正方体进行变换得到机器人的某一部分
	mat4 instance = (Translate(0.0, 0.5 * TORSO_HEIGHT, 0.0) *
		Scale(TORSO_WIDTH, TORSO_HEIGHT, TORSO_WIDTH));//本节点局部变换矩阵

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, Tshirt);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void head()
{
	// 对边长为1的正方体进行变换得到机器人的某一部分
	mat4 instance = (Translate(0.0, 0.5 * HEAD_HEIGHT, 0.0) *
		Scale(HEAD_WIDTH, HEAD_HEIGHT, HEAD_WIDTH));//本节点局部变换矩阵

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, face);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}


void hair()
{
	// 对边长为1的正方体进行变换得到机器人的某一部分
	mat4 instance = (Translate(0.0, 0.5 * HAIR_HEIGHT, 0.0) *
		Scale(HAIR_WIDTH, HAIR_HEIGHT, HAIR_WIDTH));//本节点局部变换矩阵

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, black);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void left_upper_arm()
{
	// 对边长为1的正方体进行变换得到机器人的某一部分
	mat4 instance = (Translate(0.0, 0.5 * UPPER_ARM_HEIGHT, 0.0) *
		Scale(UPPER_ARM_WIDTH, UPPER_ARM_HEIGHT, UPPER_ARM_WIDTH));//本节点局部变换矩阵
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, Tshirt);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void left_lower_arm()
{
	// 对边长为1的正方体进行变换得到机器人的某一部分
	mat4 instance = (Translate(0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0) *
		Scale(LOWER_ARM_WIDTH, LOWER_ARM_HEIGHT, LOWER_ARM_WIDTH));//本节点局部变换矩阵
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, skin);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void right_upper_arm()
{
	// 对边长为1的正方体进行变换得到机器人的某一部分
	mat4 instance = (Translate(0.0, 0.5 * UPPER_ARM_HEIGHT, 0.0) *
		Scale(UPPER_ARM_WIDTH, UPPER_ARM_HEIGHT, UPPER_ARM_WIDTH));//本节点局部变换矩阵
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, Tshirt);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void right_lower_arm()
{
	// 对边长为1的正方体进行变换得到机器人的某一部分
	mat4 instance = (Translate(0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0) *
		Scale(LOWER_ARM_WIDTH, LOWER_ARM_HEIGHT, LOWER_ARM_WIDTH));//本节点局部变换矩阵
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, skin);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void left_upper_leg()
{
	// 对边长为1的正方体进行变换得到机器人的某一部分
	mat4 instance = (Translate(0.0, 0.5 * UPPER_LEG_HEIGHT, 0.0) *
		Scale(UPPER_LEG_WIDTH, UPPER_LEG_HEIGHT, UPPER_LEG_WIDTH));//本节点局部变换矩阵
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, pants);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void left_lower_leg()
{
	// 对边长为1的正方体进行变换得到机器人的某一部分
	mat4 instance = (Translate(0.0, 0.5 * LOWER_LEG_HEIGHT, 0.0) *
		Scale(LOWER_LEG_WIDTH, LOWER_LEG_HEIGHT, LOWER_LEG_WIDTH));//本节点局部变换矩阵
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, skin);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void right_upper_leg()
{
	// 对边长为1的正方体进行变换得到机器人的某一部分
	mat4 instance = (Translate(0.0, 0.5 * UPPER_LEG_HEIGHT, 0.0) *
		Scale(UPPER_LEG_WIDTH, UPPER_LEG_HEIGHT, UPPER_LEG_WIDTH));//本节点局部变换矩阵
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, pants);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void right_lower_leg()
{
	// 对边长为1的正方体进行变换得到机器人的某一部分
	mat4 instance = (Translate(0.0, 0.5 * LOWER_LEG_HEIGHT, 0.0) *
		Scale(LOWER_LEG_WIDTH, LOWER_LEG_HEIGHT, LOWER_LEG_WIDTH));//本节点局部变换矩阵
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, skin);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}
//					以上函数绘制机器人的各个部位
// **************************************************************************************

// 正方体的一个面
void My_Model::quad(int a, int b, int c, int d, color4 color)
{
	points.push_back(vertices[a]);
	points.push_back(vertices[b]);
	points.push_back(vertices[c]);
	points.push_back(vertices[b]);
	points.push_back(vertices[c]);
	points.push_back(vertices[d]);
	for (int i = 0;i < 6;i++)
		colors.push_back(color);
}

My_Model::My_Model()
{
}


// 绘制边长为1的正方体
void My_Model::init_cube(color4 color)
{
	quad(0, 1, 2, 3, color);
	for (int i = 0;i < 6;i++) normals.push_back(vec4(0, -1, 0, 0));
	quad(4, 5, 6, 7, color);
	for (int i = 0;i < 6;i++) normals.push_back(vec4(0, 1, 0, 0));
	quad(0, 1, 4, 5, color);
	for (int i = 0;i < 6;i++) normals.push_back(vec4(-1, 0, 0, 0));
	quad(2, 3, 6, 7, color);
	for (int i = 0;i < 6;i++) normals.push_back(vec4(1, 0, 0, 0));
	quad(0, 2, 4, 6, color);
	for (int i = 0;i < 6;i++) normals.push_back(vec4(0, 0, -1, 0));
	quad(1, 3, 5, 7, color);
	for (int i = 0;i < 6;i++) normals.push_back(vec4(0, 0, 1, 0));

	pointSize += 36;
	colorSize += 36;
}


pointVec My_Model::get_points()
{
	return points;
}

colorVec My_Model::get_colors()
{
	return colors;
}

pointVec My_Model::get_normals()
{
	return normals;
}

// 绘制机器人
void My_Model::draw_human()
{
	MatrixStack mvstack;
	mvstack.push(modelView);

	modelView *= RotateY(theta[Torso]);//躯干变换矩阵
	torso();//躯干绘制

	mvstack.push(modelView);//保存躯干变换矩阵
	modelView *= Translate(0, TORSO_HEIGHT, 0) * RotateY(theta[Head1]);
	head();//头部绘制
	modelView *= Translate(0, HEAD_HEIGHT, 0);
	hair();//头发绘制
	modelView = mvstack.pop();//恢复躯干变换矩阵

	mvstack.push(modelView); //保存躯干变换矩阵
	modelView *= Translate(-0.5*TORSO_WIDTH - 0.5*UPPER_ARM_WIDTH, TORSO_HEIGHT, 0) * RotateX(theta[LeftUpperArm]);
	left_upper_arm();//左上臂绘制
	modelView *= Translate(0, UPPER_ARM_HEIGHT, 0) * RotateX(theta[LeftLowerArm]);
	left_lower_arm();//左下臂绘制
	modelView = mvstack.pop();//恢复躯干变换矩阵

	mvstack.push(modelView); //保存躯干变换矩阵
	modelView *= Translate(0.5*TORSO_WIDTH + 0.5*UPPER_ARM_WIDTH, TORSO_HEIGHT, 0) * RotateX(theta[RightUpperArm])* RotateZ(theta[AttAngles]);
	right_upper_arm();//右上臂绘制
	modelView *= Translate(0, UPPER_ARM_HEIGHT, 0) * RotateX(theta[RightLowerArm]);
	right_lower_arm();//右下臂绘制
	modelView = mvstack.pop();//恢复躯干变换矩阵

	mvstack.push(modelView); //保存躯干变换矩阵
	modelView *= Translate(-0.25*TORSO_WIDTH, 0, 0) * RotateX(theta[LeftUpperLeg]);
	left_upper_leg();//左上腿绘制
	modelView *= Translate(0, UPPER_LEG_HEIGHT, 0) *
		RotateX(theta[LeftLowerLeg]);
	left_lower_leg();//左下腿绘制
	modelView = mvstack.pop();//恢复躯干变换矩阵

	mvstack.push(modelView); //保存躯干变换矩阵
	modelView *= Translate(0.25*TORSO_WIDTH, 0, 0) * RotateX(theta[RightUpperLeg]);
	right_upper_leg();//右上腿绘制
	modelView *= Translate(0, UPPER_LEG_HEIGHT, 0) *
		RotateX(theta[RightLowerLeg]);
	right_lower_leg();//右下腿绘制
	modelView = mvstack.pop();//恢复躯干变换矩阵

	modelView = mvstack.pop();//恢复父节点矩阵
}


int My_Model::get_points_size()
{
	return pointSize;
}

int My_Model::get_colors_size()
{
	return colorSize;
}
