#include <glut.h>
#include <iostream>

#define PI 3.1415926

class Animation
{
private:
	// cos系数，sin系数
	float factor[2];
	// 单位时间PI份数增加量
	int step;
	// 旋转轴
	float axis[3];
public:
	Animation()
	{
		factor[0] = 0;
		factor[1] = 0;
		step = 0;
		axis[0] = 0;
		axis[1] = 0;
		axis[2] = 0;
	}
	Animation(float fac1, float fac2, int s, float axis_x, float axis_y, float axis_z)
	{
		factor[0] = fac1;
		factor[1] = fac2;
		step = s;
		axis[0] = axis_x;
		axis[1] = axis_y;
		axis[2] = axis_z;
	}
	float getFactor(int i)
	{
		return factor[i];
	}
	int getStep()
	{
		return step;
	}
	float getaxis(int i)
	{
		return axis[i];
	}
};


class PartClass
{
private:
	int id;
	float color[3];
	float position[3];
	float scale[3];
	float joint[3];
	float animation[4];
	int num;
	PartClass* parts;
	Animation anim;
public:
	PartClass()
	{
		parts = nullptr;
		anim = Animation();
	}
	PartClass(int i, float r, float g, float b, float pos_x, float pos_y, float pos_z, float scale_x, float scale_y, float scale_z, float joint_x, float joint_y, float joint_z, float angle, float rotate_x, float rotate_y, float rotate_z)
	{
		id = i;
		num = 0;
		color[0] = r;
		color[1] = g;
		color[2] = b;
		position[0] = pos_x;
		position[1] = pos_y;
		position[2] = pos_z;
		scale[0] = scale_x;
		scale[1] = scale_y;
		scale[2] = scale_z;
		joint[0] = joint_x;
		joint[1] = joint_y;
		joint[2] = joint_z;
		animation[0] = angle;
		animation[1] = rotate_x;
		animation[2] = rotate_y;
		animation[3] = rotate_z;
		parts = nullptr;
		anim = Animation();
	}
	// 添加子部分
	void add(PartClass part)
	{
		// 动态分配内存给parts数组
		PartClass* new_parts = new PartClass[num + 1];

		// 将已有的部分复制到新的数组中
		for (int i = 0; i < num; i++) {
			new_parts[i] = parts[i];
		}

		// 添加新的部分到新的数组中
		new_parts[num] = part;

		// 释放旧的数组内存，并将指针指向新的数组
		delete[] parts;
		parts = new_parts;

		// 增加部分计数
		num++;
	}
	// 获得id
	int getID()
	{
		return id;
	}
	// 获取颜色
	float getColor(int i)
	{
		return color[i];
	}
	// 获取初始位置
	float getPosition(int i)
	{
		return position[i];
	}
	// 获取缩放
	float getScale(int i)
	{
		return scale[i];
	}
	// 获取连接点位置
	float getJoint(int i)
	{
		return joint[i];
	}
	// 获取动画数值
	float getAnimation2(int i)
	{
		return animation[i];
	}
	Animation getAnimation()
	{
		return anim;
	}
	// 获取子部分数量
	int getNum()
	{
		return num;
	}
	PartClass getPartClass(int i)
	{
		return parts[i];
	}
	// 修改动画
	void modifyAnimation(float angle, float x_axis, float y_axis, float z_axis)
	{
		animation[0] = angle;
		animation[1] = x_axis;
		animation[2] = y_axis;
		animation[3] = z_axis;
	}
	void addAnimation(Animation a)
	{
		anim = a;
	}
};

PartClass root(0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
PartClass* animationList[12];
int frame = 0;

typedef struct materialStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;
} materialStruct;

typedef struct lightingStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
} lightingStruct;

static materialStruct *currentMaterials;
static lightingStruct *currentLighting;

materialStruct redPlasticMaterials = {
	{ 0.3, 0.0, 0.0, 1.0 },
	{ 0.6, 0.0, 0.0, 1.0 },
	{ 0.8, 0.6, 0.6, 1.0 },
	32.0
};

lightingStruct whiteLighting = {
	{ 0.1, 0.1, 0.1, 1.0 },
	{ 1.0, 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0, 1.0 }
};

float eyex, eyey, eyez, atx, aty, atz = 0;
GLfloat light0_pos[4] = { 10, 10, 10, 1.0 };

void init()
{
	eyex = 12;

	glEnable(GL_DEPTH_TEST); // 启用深度测试
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);//启用第0个光源
	glEnable(GL_COLOR_MATERIAL);
	//glDisable(GL_COLOR_MATERIAL);

	glClearColor(1, 1, 1, 0);
	currentLighting = &whiteLighting;
	glLightfv(GL_LIGHT0, GL_AMBIENT, currentLighting->ambient);//setting color of ambient light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, currentLighting->diffuse);//setting color of diffuse light
	glLightfv(GL_LIGHT0, GL_SPECULAR, currentLighting->specular);//setting color of specular light
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);//setting position of light source

	// 身体
	PartClass body(1, 0.8, 0.5, 0.5, 0.0, 0.0, 0.0, 1.2, 3.0, 2.0, 0, -0.5, 0, 0, 0, 0, 1);
	// 头
	PartClass head(2, 0.5, 0.5, 0.3, 0, 2, 0, 1, 1, 1, 0, -0.5, 0, 0, 0, 0, 0);
	// 眼睛
	PartClass leftEye(3, 0, 0, 0, 0.5, 0.25, 0.25, 0.25, 0.25, 0.25, -0.5, 0, 0, 0, 0, 0, 0);
	PartClass rightEye(4, 0, 0, 0, 0.5, 0.25, -0.25, 0.25, 0.25, 0.25, -0.5, 0, 0, 0, 0, 0, 0);
	head.add(leftEye);
	head.add(rightEye);
	// 左手臂
	PartClass leftBigArm(5, 0.2, 0.2, 0.5, 0, 0.25, -1.25, 0.5, 2.5, 0.5, 0, 0.5, 0.5, 0, 0, 0, 1);
	Animation leftBigArmAnim(45, 0, 5, 0, 0, 1);
	leftBigArm.addAnimation(leftBigArmAnim);
	// 右手臂
	PartClass rightBigArm(7, 0.2, 0.2, 0.5, 0, 0.25, 1.25, 0.5, 2.5, 0.5, 0, 0.5, -0.5, 0, 0, 0, 1);
	Animation rightBigArmAnim(-45, 0, 5, 0, 0, 1);
	rightBigArm.addAnimation(rightBigArmAnim);
	body.add(head);
	body.add(leftBigArm);
	body.add(rightBigArm);
	// 左大腿
	PartClass leftBigLeg(9, 0.2, 0.2, 0.5, 0, -2.75, -0.5, 0.5, 2.5, 0.5, 0, 0.5, 0, 0, 0, 0, 0);
	Animation leftBigLegAnim(-45, 0, 5, 0, 0, 1);
	leftBigLeg.addAnimation(leftBigLegAnim);
	// 右大腿
	PartClass rightBigLeg(11, 0.2, 0.2, 0.5, 0, -2.75, 0.5, 0.5, 2.5, 0.5, 0, 0.5, 0, 0, 0, 0, 0);
	Animation rightBigLegAnim(45, 0, 5, 0, 0, 1);
	rightBigLeg.addAnimation(rightBigLegAnim);
	root.add(body);
	root.add(leftBigLeg);
	root.add(rightBigLeg);

	//root.add(test);
}

void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, w / h, 2, 1000);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
// 变换矩阵
GLfloat transformMatrix[16];
float points[3];
// 根据旋转角度和轴向创建旋转矩阵
void createRotationMatrix(float angle, float x, float y, float z) {
	angle = angle*PI / 180.0f;
	float c = cos(angle);
	float s = sin(angle);
	float t = 1.0f - c;

	// 归一化轴向
	float len = sqrt(x*x + y*y + z*z);
	x /= len;
	y /= len;
	z /= len;

	// 创建旋转矩阵
	transformMatrix[0] = t*x*x + c;
	transformMatrix[1] = t*x*y - s*z;
	transformMatrix[2] = t*x*z + s*y;
	transformMatrix[3] = 0.0f;

	transformMatrix[4] = t*x*y + s*z;
	transformMatrix[5] = t*y*y + c;
	transformMatrix[6] = t*y*z - s*x;
	transformMatrix[7] = 0.0f;

	transformMatrix[8] = t*x*z - s*y;
	transformMatrix[9] = t*y*z + s*x;
	transformMatrix[10] = t*z*z + c;
	transformMatrix[11] = 0.0f;

	transformMatrix[12] = 0.0f;
	transformMatrix[13] = 0.0f;
	transformMatrix[14] = 0.0f;
	transformMatrix[15] = 1.0f;
}


// 计算点变换后的坐标
void computeTransformedPoint() {
	// 计算变换后的坐标
	float transformedPoint[4] = { 0 };
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			transformedPoint[i] += transformMatrix[i * 4 + j] * points[j];
		}
	}

	// 输出变换后的坐标
	points[0] = transformedPoint[0];
	points[1] = transformedPoint[1];
	points[2] = transformedPoint[2];
}

// 执行旋转后的位移
void moveAfterRotation(float angle, int x_axis, int y_axis, int z_axis, float x, float y, float z)
{
	if (angle == 0)
	{
		return;
	}
	createRotationMatrix(angle, x_axis, y_axis, z_axis);
	// 计算旋转矩阵
	points[0] = x;
	points[1] = y;
	points[2] = z;
	// 获取旋转后坐标
	computeTransformedPoint();
	glTranslatef(x - points[0], y - points[1], z - points[2]);
	glRotatef(angle, x_axis, y_axis, z_axis);
}

void animationControl(PartClass& p)
{
	int angle = p.getAnimation().getFactor(0)*cos(p.getAnimation().getStep()*frame*PI / 180.f) + p.getAnimation().getFactor(1)*sin(p.getAnimation().getStep()*frame*PI / 180.f);
	p.modifyAnimation(angle, p.getAnimation().getaxis(0), p.getAnimation().getaxis(1), p.getAnimation().getaxis(2));
}
//绘制
void draw(PartClass p)
{
	glPushMatrix();
	glColor3f(p.getColor(0), p.getColor(1), p.getColor(2));
	glTranslatef(p.getPosition(0), p.getPosition(1), p.getPosition(2));
	animationControl(p);
	moveAfterRotation(p.getAnimation2(0), p.getAnimation2(1), p.getAnimation2(2), p.getAnimation2(3), p.getJoint(0)*p.getScale(0), p.getJoint(1)*p.getScale(1), p.getJoint(2)*p.getScale(2));
	glPushMatrix();
	glScalef(p.getScale(0), p.getScale(1), p.getScale(2));
	glutSolidCube(1);
	glPopMatrix();
	for (int i = 0; i < p.getNum(); i++)
	{
		draw(p.getPartClass(i));
	}
	glPopMatrix();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, atx, aty, atz, 0.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.5, 0.5);
	glVertex3f(5, -4, -5);
	glVertex3f(-5, -4, -5);
	glVertex3f(-5, -4, 5);
	glVertex3f(5, -4, 5);
	glEnd();
	// 整体变换
	glTranslatef(3 * sin(2 * frame*PI / 180.f), 0, 3 * cos(2 * frame*PI / 180.f));
	glRotatef(2 * (180 + frame % 360), 0, 1, 0);
	draw(root);
	glutSwapBuffers();
}

void timer(int value)
{
	frame++;
	glutPostRedisplay();
	glutTimerFunc(33, timer, 10);
}

void keyboard(unsigned char key, int x, int y)
{


	switch (key)
	{
	case 'd':
		eyez -= 1;
		atz -= 1;
		break;
	case 'a':
		eyez += 1;
		atz += 1;
		break;
	case 'w':
		eyex -= 1;
		atx -= 1;
		break;
	case 's':
		eyex += 1;
		atx += 1;
		break;
	case'q':
		eyey += 0.5;
		aty += 0.5;
		break;
	case'e':
		eyey -= 0.5;
		aty -= 0.5;
		break;
	}

	glutPostRedisplay();
}

int main(int argc, char * argv[])
{

	glutInit(&argc, argv);    //初始化GLUT库；
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);   //设置显示模式；（缓冲，颜色类型）
	glutInitWindowSize(1200, 1000);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("简单机器人");  //创建窗口”；
	glutReshapeFunc(reshape);
	init();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);  //用于绘制当前窗口；
	glutTimerFunc(33, timer, 10);
	glutMainLoop();   //表示开始运行程序，用于程序的结尾；

	return 0;
}