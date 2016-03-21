#include "StdAfx.h"
#include "Enemy.h"

//CEnemy实现
//静态图像列表
CImageList CEnemy::imagesUp;
CImageList CEnemy::imagesDown;
//指定敌机位置，飞行方向，等待时间，速度
CEnemy::CEnemy(int direction , int WINDOWS_WIDTH, int WINDOWS_HEIGHT) : wait(WAIT), speed(ENEMY_SPEED)
{
	this->direction = direction;
	this->WINDOWS_WIDTH = WINDOWS_WIDTH;
	this->WINDOWS_HEIGHT = WINDOWS_HEIGHT;
	this->life = ENEMY_LIFE;
	//横坐标在窗口宽范围内
	mPoint.x = rand() % WINDOWS_WIDTH;
	//纵坐标在窗口高范围内
	if (direction == -1)//控制敌机速度方向敌机向上飞
	{
		mPoint.y = WINDOWS_HEIGHT;
		imageIndex = 1;
	}
	else//敌机向下飞
	{
		mPoint.y = 0;
		imageIndex = 0;
	}
}

CEnemy::~CEnemy(void)
{
}

//Boss下返回FALSE表示Boss正在出场，此时战机不能进行攻击
//绘制当前敌机位置
BOOL CEnemy::Draw(CDC *pDC, int passNum,BOOL bPause)
{
	if (bPause == 0) {
		int index = passNum % 5;
		//敌机位置随机变化,只改变纵坐标，随机数为了让敌机不匀速飞行
		//+ rand() % 10
		mPoint.y += (speed)*direction;
		//imageIndex为0代表向下飞的敌机，为1代表向上飞的敌机
		if (imageIndex == 0)
			imagesDown.Draw(pDC, index, mPoint, ILD_TRANSPARENT);
		else imagesUp.Draw(pDC, index, mPoint, ILD_TRANSPARENT);
		return TRUE;
	}
	else return FALSE;
}

BOOL CEnemy::Draw(CDC *pDC, BOOL bPause) {
	if (bPause == 0) {
		//敌机位置随机变化,只改变纵坐标，随机数为了让敌机不匀速飞行
		//+ rand() % 10
		mPoint.y += (speed)*direction;
		//为0代表向下飞的敌机，为1代表向上飞的敌机
		if (imageIndex == 0)
			imagesDown.Draw(pDC, 0, mPoint, ILD_TRANSPARENT);
		else imagesUp.Draw(pDC, 0, mPoint, ILD_TRANSPARENT);
		return TRUE;
	}
	else return FALSE;
}

//加载图像
BOOL CEnemy::LoadImage()
{
	//加载游戏对象的图标对象
	CBitmap enemy1;
	if (!enemy1.LoadBitmapW(IDB_BMP_ENEMYDOWN))
		return FALSE;
	//创建CImageList对象
	if (!imagesDown.Create(ENEMY_WIDTH, ENEMY_HEIGHT, ILC_COLOR24 | ILC_MASK, 5, 0))//nInitial初始个数
		return FALSE;//cx,cy 图片的宽度
	//图像链表中加入对象对应的图标对象，之后直接通过该链表访问图标对象
	imagesDown.Add(&enemy1, RGB(0,0,0));

	CBitmap enemy2;
	if (!enemy2.LoadBitmapW(IDB_BMP_ENEMYUP))
		return FALSE;
	if (!imagesUp.Create(ENEMY_WIDTH, ENEMY_HEIGHT, ILC_COLOR24 | ILC_MASK, 5, 0))//nInitial初始个数
		return FALSE;//cx,cy 图片的宽度
	imagesUp.Add(&enemy2, RGB(0,0,0));
	return TRUE;
}

//敌机是否开火
BOOL CEnemy::isFire()
{
	return TRUE;
}
