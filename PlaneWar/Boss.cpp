#include "stdafx.h"
#include "Boss.h"

//Boss静态共享成员，Boss图片列表
CImageList CBoss::imagesBoss;
CBoss::CBoss(int direction, int WINDOWS_WIDTH, int WINDOWS_HEIGHT)
	:CEnemy(direction, WINDOWS_WIDTH, WINDOWS_HEIGHT)
{
	this->life = BOSS_LIFE;
	mPoint.x = WINDOWS_WIDTH / 2;
	mPoint.y = -BOSS_HEIGHT;
	bossLoadOK = FALSE;
	speed = BOSS_SPEED;
}
CBoss::~CBoss()
{
}
//绘制Boss，根据关卡数选择Boss图标
BOOL CBoss::Draw(CDC * pDC, int passNum, BOOL bPause)
{
	if (bPause == 0) {
		int index = passNum % 5;
		//绘制Boss
		if (bossLoadOK == FALSE) {
			//还未出场完毕，只改纵坐标
			mPoint.y += speed;
			if (mPoint.y >= 30)
				bossLoadOK = TRUE;
			imagesBoss.Draw(pDC, index, mPoint, ILD_TRANSPARENT);
			return FALSE;
		}
		else {
			//Boss出场完毕，只改横坐标
			mPoint.x += BOSS_SPEED * direction;
			if (mPoint.x > WINDOWS_WIDTH - BOSS_WIDTH) {
				direction = -1;
				mPoint.x = WINDOWS_WIDTH - BOSS_WIDTH;
			}
			if (mPoint.x < 0) {
				direction = 1;
				mPoint.x = 0;
			}
			imagesBoss.Draw(pDC, index, mPoint, ILD_TRANSPARENT);
			return TRUE;
		}
	}
	else return FALSE;
}
//绘制Boss
BOOL CBoss::Draw(CDC * pDC, BOOL bPause)
{
	if (bPause == 0) {
		//绘制Boss
		if (bossLoadOK == FALSE) {
			//还未出场完毕，只改纵坐标
			mPoint.y += speed;
			if (mPoint.y >= 30)
				bossLoadOK = TRUE;
			imagesBoss.Draw(pDC, 0, mPoint, ILD_TRANSPARENT);
			return FALSE;
		}
		else {
			//Boss出场完毕，只改横坐标
			mPoint.x += BOSS_SPEED * direction;
			if (mPoint.x > WINDOWS_WIDTH - BOSS_WIDTH) {
				direction = -1;
				mPoint.x = WINDOWS_WIDTH - BOSS_WIDTH;
			}
			if (mPoint.x < 0) {
				direction = 1;
				mPoint.x = 0;
			}
			imagesBoss.Draw(pDC, 0, mPoint, ILD_TRANSPARENT);
			return TRUE;
		}
	}
	else return FALSE;
}
//加载Boss图标
BOOL CBoss::LoadImage()
{
	return CGameObject::LoadImage(imagesBoss, IDB_BMP_BOSS, RGB(0, 0, 0), BOSS_WIDTH, BOSS_HEIGHT, 5);
}

BOOL CBoss::isFire()
{
	return TRUE;
}

