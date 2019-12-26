#include "Mesh_Painter.h"
#include "FreeImage.h"

Mesh_Painter::Mesh_Painter()
{
}


Mesh_Painter::~Mesh_Painter()
{
}


void Mesh_Painter::draw_meshes(mat4 viewMatrix)	// ��ͼ������Ϊ��Ҫ���ϱ仯��������Ϊ��������
{
	for (unsigned int i = 0; i < this->m_my_meshes_.size(); i++)
	{
		glUseProgram(this->program_all[i]);//ָ��ʹ����Ⱦ������ͬ��ģ�Ϳ���ָ����ͬ����Ⱦ������������ʹ�õ���Ⱦ��������ͬ���ļ���ѧ�����Ը����Լ��İ��öԲ�ͬ��ģ���趨��ͬ����Ⱦ��
		
		vec3 scale_rate = m_my_meshes_[i]->get_scale_rate();
		mat4 scaleMatrix = Scale(scale_rate[0], scale_rate[1], scale_rate[2]);
		mat4 translateMatrix = Translate(m_my_meshes_[i]->get_translate());
		mat4 rotate_x = RotateX(m_my_meshes_[i]->get_rotate_angel()[0]);
		mat4 rotate_y = RotateY(m_my_meshes_[i]->get_rotate_angel()[1]);
		mat4 rotate_z = RotateZ(m_my_meshes_[i]->get_rotate_angel()[2]);
		mat4 rotateMatrix = rotate_z * rotate_y * rotate_x;
		mat4 totalMatrix = modelMatrix_ * rotateMatrix * scaleMatrix * translateMatrix;
		glUniformMatrix4fv(glGetUniformLocation(program_all[i], "modelMatrix"), 1, GL_TRUE, &totalMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(program_all[i], "viewMatrix"), 1, GL_TRUE, &viewMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(program_all[i], "projMatrix"), 1, GL_TRUE, &projMatrix_[0][0]);
		glUniform1i(glGetUniformLocation(program_all[i], "isShadow"), 0);	// ������ɫ������ģʽ
		glUniform1i(glGetUniformLocation(program_all[i], "isShadow"), 0);	// ������ɫ������ģʽ
		glUniform3fv(glGetUniformLocation(program_all[i], "lightPos"), 1, &lightPos_[0]);	// ����Դ������ɫ��
		glBindVertexArray(this->vao_all[i]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->textures_all[i]);//�������룬����ֻʹ��ͬһ���������л���
		glDrawArrays(GL_TRIANGLES, 0, this->m_my_meshes_[i]->num_faces() * 3);
		glBindVertexArray(0);
		
		// ������Ӱ
		if (m_my_meshes_[i]->get_is_need_shadow()) {
			float lx = lightPos_[0];
			float ly = lightPos_[1];
			float lz = lightPos_[2];
			mat4 shadowMatrix;
			shadowMatrix[3][3] = 0.0;
			shadowMatrix[3][1] = -1.0 / ly;
			mat4 translate1 = Translate(-lx, -ly, -lz);
			mat4 translate2 = Translate(lx, ly, lz);

			vec3 scale_rate = m_my_meshes_[i]->get_scale_rate();
			//mat4 scaleMatrix = Scale(scale_rate[0], scale_rate[1], scale_rate[2]);
			//mat4 translateMatrix = Translate(m_my_meshes_[i]->get_translate());
			mat4 rotate_x = RotateX(m_my_meshes_[i]->get_rotate_angel()[0]);
			mat4 rotate_y = RotateY(m_my_meshes_[i]->get_rotate_angel()[1]);
			mat4 rotate_z = RotateZ(m_my_meshes_[i]->get_rotate_angel()[2]);
			mat4 rotateMatrix = rotate_z * rotate_y * rotate_x;
			mat4 totalMatrix;
			totalMatrix = modelMatrix_ * translate2 * shadowMatrix * translate1 * rotateMatrix * scaleMatrix * translateMatrix;
			glUniformMatrix4fv(glGetUniformLocation(program_all[i], "modelMatrix"), 1, GL_TRUE, &totalMatrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(program_all[i], "viewMatrix"), 1, GL_TRUE, &viewMatrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(program_all[i], "projMatrix"), 1, GL_TRUE, &projMatrix_[0][0]);
			glUniform1i(glGetUniformLocation(program_all[i], "isShadow"), 1);	// ������ɫ������ģʽ
			glUniform1i(glGetUniformLocation(program_all[i], "isShadow"), 1);	// ������ɫ������ģʽ
			glUniform3fv(glGetUniformLocation(program_all[i], "lightPos"), 1, &lightPos_[0]);	// ����Դ������ɫ��
			glBindVertexArray(this->vao_shadow_all[i]);
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, this->textures_all[i]);//�������룬����ֻʹ��ͬһ���������л���
			glDrawArrays(GL_TRIANGLES, 0, this->m_my_meshes_[i]->num_faces() * 3);
			glBindVertexArray(0);
		}
		
		glUseProgram(0);
	}
};


void Mesh_Painter::update_texture()
{
	this->textures_all.clear();

	for (unsigned int i = 0; i < this->m_my_meshes_.size(); i++)
	{
		//ʵ�����������͸�ֵ
		GLuint textures;

		glGenTextures(1, &textures);
		//����FreeImage��������
		load_texture_FreeImage(this->m_my_meshes_[i]->get_texture_file(), textures);

		//ָ�������С���˷����������д���������ӣ������޷���ʾ����
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//�����ɵ���������shader
		glBindTexture(GL_TEXTURE_2D, textures);
		glUniform1i(glGetUniformLocation(this->program_all[i], "texture"), 0);
		this->textures_all.push_back(textures);
	}
};


void Mesh_Painter::load_texture_FreeImage(std::string file_name, GLuint& m_texName)
{
	//1 ��ȡͼƬ��ʽ
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(file_name.c_str(), 0);

	//2 ����ͼƬ
	FIBITMAP *dib = FreeImage_Load(fifmt, file_name.c_str(), 0);

	//3 ת��Ϊrgb 24ɫ;
	dib = FreeImage_ConvertTo24Bits(dib);

	//4 ��ȡ����ָ��
	BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);

	int width = FreeImage_GetWidth(dib);
	int height = FreeImage_GetHeight(dib);

	/**
	* ����һ������Id,������Ϊ���������������Ĳ����������������id
	*/

	/**
	* ʹ���������id,���߽а�(����)
	*/
	glBindTexture(GL_TEXTURE_2D, m_texName);
	/**
	* ָ�������ķŴ�,��С�˲���ʹ�����Է�ʽ������ͼƬ�Ŵ��ʱ���ֵ��ʽ
	*/
	/**
	* ��ͼƬ��rgb�����ϴ���opengl.
	*/
	glTexImage2D(
		GL_TEXTURE_2D, //! ָ���Ƕ�άͼƬ
		0, //! ָ��Ϊ��һ��������������mipmap,��lod,����ľͲ��ü����ģ�Զ��ʹ�ý�С������
		GL_RGB, //! ������ʹ�õĴ洢��ʽ
		width, //! ���ȣ���һ����Կ�����֧�ֲ�����������������Ⱥ͸߶Ȳ���2^n��
		height, //! ���ȣ���һ����Կ�����֧�ֲ�����������������Ⱥ͸߶Ȳ���2^n��
		0, //! �Ƿ�ı�
		GL_BGR_EXT, //! ���ݵĸ�ʽ��bmp�У�windows,����ϵͳ�д洢��������bgr��ʽ
		GL_UNSIGNED_BYTE, //! ������8bit����
		pixels
	);
	/**
	* �ͷ��ڴ�
	*/
	FreeImage_Unload(dib);
};

void Mesh_Painter::update_vertex_buffer()
{
	this->vao_all.clear();
	this->buffer_all.clear();
	this->vPosition_all.clear();
	this->vColor_all.clear();
	this->vTexCoord_all.clear();
	this->vNormal_all.clear();

	//ˢ�»���������
	for (unsigned int m_i = 0; m_i < this->m_my_meshes_.size(); m_i++)
	{
		int num_face = this->m_my_meshes_[m_i]->num_faces();
		int num_vertex = this->m_my_meshes_[m_i]->num_vertices();

		const VertexList& vertex_list = this->m_my_meshes_[m_i]->get_vertices();
		const NormalList& normal_list = this->m_my_meshes_[m_i]->get_normals();
		const FaceList&  face_list = this->m_my_meshes_[m_i]->get_faces();
		const STLVectorf& color_list = this->m_my_meshes_[m_i]->get_colors();
		const VtList& vt_list = this->m_my_meshes_[m_i]->get_vts();


		// Create a vertex array object
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);


		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(vec3)*num_face * 3
			+ sizeof(vec3)*num_face * 3
			+ sizeof(vec3)*num_face * 3
			+ sizeof(vec2)*num_face * 3
			,
			NULL, GL_STATIC_DRAW)
			;
		//����㹻�Ŀռ�洢���꣬��ɫ�������Լ���������ȵ�
		// Specify an offset to keep track of where we're placing data in our
		//   vertex array buffer.  We'll use the same technique when we
		//   associate the offsets with vertex attribute pointers.

		vec3* points = new vec3[num_face * 3];
		point3f  p_center_ = this->m_my_meshes_[m_i]->get_center();
		point3f p_min_box_, p_max_box_;
		this->m_my_meshes_[m_i]->get_boundingbox(p_min_box_, p_max_box_);
		float d = p_min_box_.distance(p_max_box_);

		for (int i = 0; i < num_face; i++)
		{
			int index = face_list[3 * i];
			points[3 * i] = vec3(
				(vertex_list[index * 3 + 0] - p_center_.x),
				(vertex_list[index * 3 + 1] - p_center_.y),
				(vertex_list[index * 3 + 2] - p_center_.z)
			);

			index = face_list[3 * i + 1];
			points[3 * i + 1] = vec3(
				(vertex_list[index * 3 + 0] - p_center_.x),
				(vertex_list[index * 3 + 1] - p_center_.y),
				(vertex_list[index * 3 + 2] - p_center_.z)
			);

			index = face_list[3 * i + 2];
			points[3 * i + 2] = vec3(
				(vertex_list[index * 3 + 0] - p_center_.x),
				(vertex_list[index * 3 + 1] - p_center_.y),
				(vertex_list[index * 3 + 2] - p_center_.z)
			);

		}
		GLintptr offset = 0;
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(vec3)*num_face * 3, points);
		//�������괫��shader
		offset += sizeof(vec3)*num_face * 3;//��ʼλ�ú���
		delete[] points;
		/************************************************************************/
		/* ����face_list�е������洢��points��ַ���ڻ������洢���������        */
		/************************************************************************/
		points = new vec3[num_face * 3];
		for (int i = 0; i < num_face; i++)
		{
			int index = face_list[3 * i];
			points[3 * i] = vec3(
				(normal_list[index * 3 + 0]),
				(normal_list[index * 3 + 1]),
				(normal_list[index * 3 + 2])
			);

			index = face_list[3 * i + 1];
			points[3 * i + 1] = vec3(
				(normal_list[index * 3 + 0]),
				(normal_list[index * 3 + 1]),
				(normal_list[index * 3 + 2])
			);

			index = face_list[3 * i + 2];
			points[3 * i + 2] = vec3(
				(normal_list[index * 3 + 0]),
				(normal_list[index * 3 + 1]),
				(normal_list[index * 3 + 2])
			);
		}
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(vec3) * num_face * 3, points);
		offset += sizeof(vec3) * num_face * 3;
		delete[] points;
		//���ߴ���shader

		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		points = new vec3[num_face * 3];
		for (int i = 0; i < num_face; i++)
		{
			int index = face_list[3 * i];
			points[3 * i] = vec3(
				(color_list[index * 3 + 0]),
				(color_list[index * 3 + 1]),
				(color_list[index * 3 + 2])
			);

			index = face_list[3 * i + 1];
			points[3 * i + 1] = vec3(
				(color_list[index * 3 + 0]),
				(color_list[index * 3 + 1]),
				(color_list[index * 3 + 2])
			);

			index = face_list[3 * i + 2];
			points[3 * i + 2] = vec3(
				(color_list[index * 3 + 0]),
				(color_list[index * 3 + 1]),
				(color_list[index * 3 + 2])
			);
		}
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(vec3) * num_face * 3, points);
		//��ɫ����shader
		offset += sizeof(vec3) * num_face * 3;
		delete[] points;


		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		vec2* points_2 = new vec2[num_face * 3];
		for (int i = 0; i < num_face; i++)
		{
			points_2[i * 3] = vec2(vt_list[i * 6 + 0], vt_list[i * 6 + 1]);
			points_2[i * 3 + 1] = vec2(vt_list[i * 6 + 2], vt_list[i * 6 + 3]);
			points_2[i * 3 + 2] = vec2(vt_list[i * 6 + 4], vt_list[i * 6 + 5]);
		}
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(vec2) * num_face * 3, points_2);
		offset += sizeof(vec2) * num_face * 3;
		delete points_2;
		//�������괫��shader
		/****************************************************************************/
		/*����������洢�ṹ:˳����ÿ���߶������ꡢ����������ɫ��������ͼ�������� */
		/****************************************************************************/

		// Load shaders and use the resulting shader program


		// set up vertex arrays
		offset = 0;
		GLuint vPosition;
		vPosition = glGetAttribLocation(this->program_all[m_i], "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(offset));
		offset += sizeof(vec3) * num_face * 3;
		//ָ��vPosition��shader��ʹ��ʱ��λ��

		GLuint vNormal;
		vNormal = glGetAttribLocation(this->program_all[m_i], "vNormal");
		glEnableVertexAttribArray(vNormal);
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(offset));
		offset += sizeof(vec3) * num_face * 3;
		//ָ��vNormal��shader��ʹ��ʱ��λ��

		GLuint vColor;
		vColor = glGetAttribLocation(this->program_all[m_i], "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(offset));
		offset += sizeof(vec3) * num_face * 3;
		//ָ��vColor��shader��ʹ��ʱ��λ��

		GLuint vTexCoord;
		vTexCoord = glGetAttribLocation(this->program_all[m_i], "vTexCoord");
		glEnableVertexAttribArray(vTexCoord);
		glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(offset));
		//ָ��vTexCoord��shader��ʹ��ʱ��λ��

		/************************************************************************/
		/*                                                                      */
		/************************************************************************/

		this->vao_all.push_back(vao);
		this->buffer_all.push_back(buffer);
		this->vPosition_all.push_back(vPosition);
		this->vColor_all.push_back(vColor);
		this->vTexCoord_all.push_back(vTexCoord);
		this->vNormal_all.push_back(vNormal);
	}

	// ������Ӱ�ĵ�����
	for (unsigned int m_i = 0; m_i < this->m_my_meshes_.size(); m_i++)
	{
		int num_face = this->m_my_meshes_[m_i]->num_faces();

		const VertexList& vertex_list = this->m_my_meshes_[m_i]->get_vertices();
		const FaceList&  face_list = this->m_my_meshes_[m_i]->get_faces();

		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);


		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*num_face * 3, NULL, GL_STATIC_DRAW);
		vec3* points = new vec3[num_face * 3];
		point3f  p_center_ = this->m_my_meshes_[m_i]->get_center();

		for (int i = 0; i < num_face; i++)
		{
			int index = face_list[3 * i];
			points[3 * i] = vec3(
				(vertex_list[index * 3 + 0] - p_center_.x),
				(vertex_list[index * 3 + 1] - p_center_.y),
				(vertex_list[index * 3 + 2] - p_center_.z)
			);

			index = face_list[3 * i + 1];
			points[3 * i + 1] = vec3(
				(vertex_list[index * 3 + 0] - p_center_.x),
				(vertex_list[index * 3 + 1] - p_center_.y),
				(vertex_list[index * 3 + 2] - p_center_.z)
			);

			index = face_list[3 * i + 2];
			points[3 * i + 2] = vec3(
				(vertex_list[index * 3 + 0] - p_center_.x),
				(vertex_list[index * 3 + 1] - p_center_.y),
				(vertex_list[index * 3 + 2] - p_center_.z)
			);

		}
		GLintptr offset = 0;
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*num_face * 3, points, GL_STATIC_DRAW);

		//glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(vec3)*num_face * 3, points);
		//�������괫��shader
		delete[] points;

		offset = 0;
		GLuint vPosition;
		vPosition = glGetAttribLocation(this->program_all[m_i], "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offset));

		this->vao_shadow_all.push_back(vao);
	}
};

// ���޸ģ���Ϊ��ͬģ�Ϳ��Լ��ز�ͬ��ɫ��
void Mesh_Painter::init_shaders()
{
	this->program_all.clear();
	for (unsigned int i = 0; i < this->m_my_meshes_.size(); i++)
	{
		GLuint program = InitShader(m_my_meshes_[i]->get_vshader_file_name().c_str(), m_my_meshes_[i]->get_fshader_file_name().c_str());
		//glUseProgram(program);
		this->program_all.push_back(program);
	}

};
void Mesh_Painter::add_mesh(My_Mesh* m)
{
	this->m_my_meshes_.push_back(m);
};
void Mesh_Painter::clear_mehs()
{
	this->m_my_meshes_.clear();

	this->textures_all.clear();
	this->program_all.clear();
	this->vao_all.clear();
	this->buffer_all.clear();
	this->vPosition_all.clear();
	this->vColor_all.clear();
	this->vTexCoord_all.clear();
	this->vNormal_all.clear();
};

void Mesh_Painter::set_lightPos(vec3 lightPos) {
	this->lightPos_ = lightPos;
}

void Mesh_Painter::set_matrix(mat4 modelMatrix, mat4 projMatrix) {
	// ֻ��������������Ϊ��ͼ����᲻�ϱ仯
	this->modelMatrix_ = modelMatrix;
	this->projMatrix_ = projMatrix;
}
