#include "StdAfx.h"
#include "Explosion.h"

//爆炸类实现

//静态图像列表
CImageList CExplosion::images;
//设置爆炸处位置、起始下标
CExplosion::CExplosion(int x, int y) :imagesIndex(EXPLOSION_START)
{
	bossDie = FALSE;
	mPoint.x = x;
	mPoint.y = y;
}

CExplosion::~CExplosion(void)
{
}
//绘制，按顺序显示爆炸的八个图像，形成动画效果
int CExplosion::Draw(CDC *pDC, BOOL bPause)
{
	if (bPause == 0) {
		if (imagesIndex < EXPLOSION_NUM) {
			BOOL b = images.Draw(pDC, imagesIndex, mPoint, ILD_TRANSPARENT);
			/*if (!b)
				return b;*/
			imagesIndex++;
			return EXPLOSION_STATUS_SUCCESS;
		}
		else return EXPLOSION_STATUS_STOP;
	}
	else return EXPLOSION_STATUS_PAUSE;
}
//加载爆炸图像
BOOL CExplosion::LoadImage()
{
	return  CGameObject::LoadImage(images, IDB_BMP_EXPLOSION, RGB(0, 0, 0), EXPLOSION_WIDTH, EXPLOSION_WIDTH, EXPLOSION_NUM);
}