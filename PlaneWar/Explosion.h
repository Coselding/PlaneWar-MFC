#pragma once
#include "GameObject.h"
#include "Resource.h"

//爆炸图像宽度
#define EXPLOSION_WIDTH 66
//爆炸图像总数
#define EXPLOSION_NUM 8
//爆炸首图像下标
#define EXPLOSION_START 0

//爆炸绘制状态结果
#define EXPLOSION_STATUS_SUCCESS 11
#define EXPLOSION_STATUS_PAUSE 12
#define EXPLOSION_STATUS_STOP 13

//爆炸类
class CExplosion :public CGameObject
{
public:
	CExplosion(int x, int y);
	~CExplosion(void);

	int Draw(CDC* pDC, BOOL bPause);

	//加载图标
	static BOOL LoadImage();

	//获取爆炸矩形
	CRect GetRect()
	{
		return CRect(mPoint, CPoint(mPoint.x + EXPLOSION_WIDTH, mPoint.y + EXPLOSION_WIDTH));
	}

	void SetBossDie(BOOL bossDie) {
		this->bossDie = bossDie;
	}
	BOOL GetBossDie() {
		return this->bossDie;
	}
private:
	//炸弹的图片列表
	static CImageList images;
	//图像索引的步进计数
	int   imagesIndex;//取值为0-7
	BOOL bossDie;//标记该爆炸是Boss死亡爆炸
};