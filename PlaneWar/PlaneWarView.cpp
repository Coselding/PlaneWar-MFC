
// PlaneWarView.cpp : CPlaneWarView 类的实现
//

#include "stdafx.h"
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")
#include <MMSYSTEM.H>
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "PlaneWar.h"
#endif
   
#include "PlaneWarDoc.h"
#include "PlaneWarView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlaneWarView

IMPLEMENT_DYNCREATE(CPlaneWarView, CView)

BEGIN_MESSAGE_MAP(CPlaneWarView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPlaneWarView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CPlaneWarView 构造/析构

CPlaneWarView::CPlaneWarView()
{
	//默认战机飞行速度、战机生命值、战机得分、关卡、是否过关、是否暂停

    // TODO: 在此处添加构造代码
}

CPlaneWarView::~CPlaneWarView()
{
}

BOOL CPlaneWarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPlaneWarView 绘制
void CPlaneWarView::OnDraw(CDC* pDC)
{
	CPlaneWarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

// CPlaneWarView 打印

void CPlaneWarView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPlaneWarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPlaneWarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPlaneWarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CPlaneWarView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// CPlaneWarView 诊断

#ifdef _DEBUG
void CPlaneWarView::AssertValid() const
{
	CView::AssertValid();
}

void CPlaneWarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlaneWarDoc* CPlaneWarView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlaneWarDoc)));
	return (CPlaneWarDoc*)m_pDocument;
}
#endif //_DEBUG

//重写实现
//获得速度
int CPlaneWarView::GetSpeed()
{
	return this->speed;
}
//设置速度
void CPlaneWarView::SetSpeed(int speed)
{
	this->speed = speed;
}

// CPlaneWarView 消息处理程序
//视图创建监听
int CPlaneWarView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	//-----------初始化工作------------
	//加载游戏对象图片
	CMyPlane::LoadImage();
	CMyPlane::LoadImageProtect();
	CEnemy::LoadImage();
	CBoss::LoadImage();
	CBall::LoadImageUp();
	CBall::LoadImageDown();
	CBomb::LoadImage();
	CExplosion::LoadImage();
	CBlood::LoadImage();
	//加载标题图片
	CBitmap startbmp;
	startbmp.LoadBitmapW(IDB_BMP_TITLE);
	startIMG.Create(346, 96, ILC_COLOR24 | ILC_MASK, 1, 0);
	startIMG.Add(&startbmp, RGB(0, 0, 0));
	
	//场景初始化失败
	if (!scene.InitScene())
	{
		AfxMessageBox(L"图片资源加载失败");
		exit(0);
	}

	//参数初始化
	myplane = new CMyPlane(FALSE);
	isBoss = FALSE;
	speed = DEFAULT_SPEED;
	myLife = DEFAULT_LIFE;
	lifeNum = DEFAULT_LIFE_COUNT;
	lifeCount = 1;
	passScore = 0;
	myScore=0;
	bossLoaded = TRUE;
	passNum = DEFAULT_PASS;
	isPass = 0;
	isPause = FALSE;
	magicCount = 0;
	bloodExist = FALSE;
	bossBlood = BOSS_LIFE;
	isProtect = FALSE;
	isUpdate = FALSE;
	test = FALSE;
	isStop = FALSE;
	boss = NULL;
	isStarted = FALSE;

	SetTimer(4, 40, NULL);//背景滚动定时器
	SetMyTimer();
	return 0;
}

//计时器监听
void CPlaneWarView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//双缓冲
	CDC *pDC = GetDC();
	if (pDC == NULL) {
		CView::OnTimer(nIDEvent);
		return;
	}
	//获得客户区矩形区域
	GetClientRect(&rect);
	//内存缓冲CDC
	CDC cdc;

	//内存中承载临时图像的缓冲位图
	CBitmap* cacheBitmap = new CBitmap;
	//用当前设备CDC初始化缓冲CDC
	cdc.CreateCompatibleDC(pDC);
	//绑定pDC和缓冲位图的关系，cdc先输出到缓冲位图中，输出完毕之后再一次性将缓冲位图输出到屏幕
	cacheBitmap->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	//替换cdc原本的缓冲区为缓冲位图，这样cdc输出的内容就写到了缓冲位图中
	CBitmap *pOldBit = cdc.SelectObject(cacheBitmap);

	/*
	//用固定的固体色填充文本矩形框
	cdc.FillSolidRect(rect, RGB(51, 255, 255));
	//添加背景图片位图
	CBitmap bitmap_BackGround;
	//加载背景位图
	if(passNum==1)
		bitmap_BackGround.LoadBitmap(IDB_BMP_BACKGROUND1);
	else if(passNum==2)
		bitmap_BackGround.LoadBitmap(IDB_BMP_BACKGROUND2);
	else if(passNum==3)
		bitmap_BackGround.LoadBitmap(IDB_BMP_BACKGROUND3);
	BITMAP bimap_Background;//背景位图图像
	bitmap_BackGround.GetBitmap(&bimap_Background);
	CDC cdc_BackGround;//背景输出cdc，第三级缓冲
	cdc_BackGround.CreateCompatibleDC(&cdc);//初始化三级缓冲
	//设置三级缓冲，背景图设置为三级缓冲的缓冲区，该区原数据就是背景图，不需要添加数据
	CBitmap*Old = cdc_BackGround.SelectObject(&bitmap_BackGround);
	*/

	//输出背景
	if (isStarted == FALSE)
		scene.StickScene(&cdc, -1, rect);
	else
		scene.StickScene(&cdc, passNum, rect);
	if (nIDEvent == 4) {
		//滚动背景
		scene.MoveBg();
	}
	//欢迎界面
	if (isStarted == FALSE) {
		startIMG.Draw(&cdc, 0, CPoint(rect.right/2-173, 100), ILD_TRANSPARENT);
		HFONT textFont;
		textFont = CreateFont(12, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 10, 0);
		cdc.SelectObject(textFont);
		cdc.SetTextColor(RGB(128, 128, 0));
		//设置透明背景
		cdc.SetBkMode(TRANSPARENT);
		cdc.TextOutW(rect.right/2-120, 210, _T("方向控制：方向键、ASDW、鼠标"));
		cdc.TextOutW(rect.right/ 2 - 120, 225, _T("射击：空格键、鼠标左键"));
		cdc.TextOutW(rect.right / 2 - 120, 240, _T("暂停：Z键"));
		cdc.TextOutW(rect.right / 2 - 120, 255, _T("大招：X键"));
		cdc.TextOutW(rect.right / 2 - 120, 270, _T("防护罩：C键"));
		cdc.TextOutW(rect.right / 2 - 120, 285, _T("战机升级：V键"));
		cdc.TextOutW(rect.right / 2 - 120, 300, _T("无敌模式：Y键"));
		cdc.TextOutW(rect.right / 2 - 120, 315, _T("初始生命值：10"));
		cdc.TextOutW(rect.right / 2 - 120, 330, _T("初始魔法值：0"));
		cdc.TextOutW(rect.right / 2 - 120, 345, _T("敌机生命值：2"));
		cdc.TextOutW(rect.right / 2 - 120, 360, _T("消灭一个敌机加1分，如果分数达到要求即可进入Boss模式，打赢Boss即可进入下一关。"));
		cdc.TextOutW(rect.right / 2 - 120, 375, _T("魔法值随着游戏进程增加，可通过使用魔法值使用防护罩、战机升级、战机大招的使用。"));
		cdc.TextOutW(rect.right / 2 - 120, 390, _T("游戏过程中会有一定程度的血包出现以恢复生命值。"));
		cdc.TextOutW(rect.right / 2 - 120, 405, _T("随着关卡增多，敌机、炮弹速度和数量均增加，通过10关即可通关！"));
		HFONT tipFont;
		tipFont = CreateFont(24, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 10, 0);
		cdc.SelectObject(tipFont);
		cdc.SetTextColor(RGB(255, 0, 0));
		cdc.TextOutW(rect.right / 2 - 120, 420, _T("点击鼠标左键或空格键开始游戏"));
		//将二级缓冲cdc中的数据推送到一级级缓冲pDC中，即输出到屏幕中
		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &cdc, 0, 0, SRCCOPY);
		//释放二级cdc
		cdc.DeleteDC();
		//释放缓冲位图
		cacheBitmap->DeleteObject();
		//释放一级pDC
		ReleaseDC(pDC);
		CView::OnTimer(nIDEvent);
		return;
	}
	//显示暂停信息
	if (myplane != NULL&&isPause == TRUE&&isStop==FALSE) {
		HFONT textFont;
		textFont = CreateFont(24, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 10, 0);
		cdc.SelectObject(textFont);
		cdc.SetTextColor(RGB(255, 0, 0));
		//设置透明背景
		cdc.SetBkMode(TRANSPARENT);
		cdc.TextOutW(rect.right / 2 - 10, 150, _T("暂停"));
		////将二级缓冲cdc中的数据推送到一级级缓冲pDC中，即输出到屏幕中
		//pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &cdc, 0, 0, SRCCOPY);
		////释放二级cdc
		//cdc.DeleteDC();
		////释放缓冲位图
		//cacheBitmap->DeleteObject();
		////释放一级pDC
		//ReleaseDC(pDC);
		//CView::OnTimer(nIDEvent);
		//return;
	}

	//刷新显示战机
	if (myplane != NULL) {
		//reDraw(myplane, TRUE);
		myplane->Draw(&cdc,FALSE,isProtect);
	}
	//随机添加敌机,敌机随机发射炸弹，此时敌机速度与数量和关卡有关
	if (myplane != NULL && isPause == 0&&isBoss==FALSE)
	{
		//敌机产生定时器触发
		if (nIDEvent == 2) {
			//根据关卡数产生敌机
			if (passNum <=5) {
				//前五关只有一个方向的敌机
				int direction = 1;//设置敌机的方向，从上方飞出
				CEnemy *enemy = new CEnemy(direction, rect.right, rect.bottom);
				enemyList.AddTail(enemy);//随机产生敌机
			}
			else if (passNum >5) {//第五关之后，两个方向的敌机
				int direction1 = 1;	//设置敌机的方向，从上方飞出		
				CEnemy *enemy1 = new CEnemy(direction1,rect.right,rect.bottom);
				enemy1->SetSpeed(ENEMY_SPEED+(rand() % 2 + passNum-1));
				enemyList.AddTail(enemy1);//随机产生敌机
				int direction2 = -1;//设置敌机的方向，从下方飞出	
				CEnemy *enemy2 = new CEnemy(direction2, rect.right, rect.bottom);
				enemy2->SetSpeed(ENEMY_SPEED + (rand() % 2 + passNum - 1));
				enemyList.AddTail(enemy2);//随机产生敌机
			}
		}

		//超出边界的敌机进行销毁
		POSITION stPos = NULL, tPos = NULL;
		stPos = enemyList.GetHeadPosition();
		int direction = 1;
		while (stPos != NULL)
		{
			tPos = stPos;
			CEnemy *enemy = (CEnemy *)enemyList.GetNext(stPos);
			//判断敌机是否出界
			if (enemy->GetPoint().x<rect.left || enemy->GetPoint().x>rect.right
				|| enemy->GetPoint().y<rect.top || enemy->GetPoint().y>rect.bottom)
			{
				enemyList.RemoveAt(tPos);
				delete enemy;
			}//if
			else
			{
				//没出界，绘制
				enemy->Draw(&cdc,passNum, FALSE);
				//敌机炸弹产生定时器触发
				if (nIDEvent == 3) {
					//设置定时器产生敌机炸弹
					PlaySound((LPCTSTR)IDR_WAV_BALL, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
					CBall * ball = new CBall(enemy->GetPoint().x + ENEMY_HEIGHT / 2, enemy->GetPoint().y + ENEMY_HEIGHT, enemy->GetDirection());
					ball->SetBallSpeed(BALL_SPEED + (passNum - 1) ); 
					ballList.AddTail(ball);
				}
			}//else
		}//while

		 //判断产生的敌机炸弹是否出界，若已经出界，则删除该敌机炸弹
		POSITION stBallPos=NULL, tBallPos=NULL;
		stBallPos = ballList.GetHeadPosition();
		while (stBallPos != NULL)
		{
			tBallPos = stBallPos;
			ball = (CBall *)ballList.GetNext(stBallPos);
			if (ball->GetPoint().x<rect.left || ball->GetPoint().x>rect.right
				|| ball->GetPoint().y<rect.top || ball->GetPoint().y>rect.bottom)
			{
				ballList.RemoveAt(tBallPos);
				delete  ball;
			}//if
			else
			{
				ball->Draw(&cdc,passNum, FALSE);
			}//else
		}//while
	}
	//Boss产生炮弹
	if (myplane != NULL && isPause == 0 && isBoss == TRUE) {
		//Boss发射子弹
		//敌机炸弹产生定时器触发
		if (nIDEvent == 3) {
			//设置定时器产生敌机炸弹
			PlaySound((LPCTSTR)IDR_WAV_BALL, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			CBall * ball1 = new CBall(boss->GetPoint().x + BOSS_WIDTH / 2, boss->GetPoint().y + BOSS_HEIGHT, 1);
			ball1->SetBallSpeed(BALL_SPEED + (passNum - 1) * 2);
			ballList.AddTail(ball1);
			CBall * ball2 = new CBall(boss->GetPoint().x +5, boss->GetPoint().y + BOSS_HEIGHT, 1);
			ball2->SetBallSpeed(BALL_SPEED + (passNum - 1) * 2);
			ballList.AddTail(ball2);
			CBall * ball3 = new CBall(boss->GetPoint().x + BOSS_WIDTH -5, boss->GetPoint().y + BOSS_HEIGHT,1);
			ball3->SetBallSpeed(BALL_SPEED + (passNum - 1) * 2);
			ballList.AddTail(ball3);
			CBall * ball4 = new CBall(boss->GetPoint().x + BOSS_WIDTH / 2+85, boss->GetPoint().y + BOSS_HEIGHT, 1);
			ball4->SetBallSpeed(BALL_SPEED + (passNum - 1) * 2);
			ballList.AddTail(ball4);
			CBall * ball5 = new CBall(boss->GetPoint().x + BOSS_WIDTH / 2-85, boss->GetPoint().y + BOSS_HEIGHT, 1);
			ball5->SetBallSpeed(BALL_SPEED + (passNum - 1) * 2);
			ballList.AddTail(ball5);
		}
		//显示Boss炸弹
		//判断产生的敌机炸弹是否出界，若已经出界，则删除该敌机炸弹
		POSITION stBallPos = NULL, tBallPos = NULL;
		stBallPos = ballList.GetHeadPosition();
		while (stBallPos != NULL)
		{
			tBallPos = stBallPos;
			ball = (CBall *)ballList.GetNext(stBallPos);
			if (ball->GetPoint().x<rect.left || ball->GetPoint().x>rect.right
				|| ball->GetPoint().y<rect.top || ball->GetPoint().y>rect.bottom)
			{
				ballList.RemoveAt(tBallPos);
				delete  ball;
			}//if
			else
			{
				ball->Draw(&cdc, FALSE);
			}//else
		}//while
	}

	//战机发射子弹
	if (myplane != NULL&& isPause == 0)
	{
		//声明战机子弹位置
		POSITION posBomb = NULL, tBomb = NULL;
		posBomb = bombList.GetHeadPosition();
		while (posBomb != NULL)
		{
			tBomb = posBomb;
			bomb = (CBomb *)bombList.GetNext(posBomb);
			if (bomb->GetPoint().x<rect.left || bomb->GetPoint().x>rect.right
				|| bomb->GetPoint().y<rect.top || bomb->GetPoint().y>rect.bottom)
			{
				//删除越界的子弹
				bombList.RemoveAt(tBomb);
				delete  bomb;
			}//if
			else
				bomb->Draw(&cdc, FALSE);
		}//while
	}//if

	//（1）添加爆炸效果，战机子弹打中敌机
	if (myplane != NULL&& isPause == 0)
	{
		//声明战机子弹位置，敌机位置
		POSITION bombPos, bombTemp, enemyPos, enemyTemp;
		for (bombPos = bombList.GetHeadPosition(); (bombTemp = bombPos) != NULL;)
		{
			bomb = (CBomb *)bombList.GetNext(bombPos);
			for (enemyPos = enemyList.GetHeadPosition(); (enemyTemp = enemyPos) != NULL;)
			{
				enemy = (CEnemy *)enemyList.GetNext(enemyPos);
				//获得战机子弹的矩形区域
				CRect bombRect = bomb->GetRect();
				//获得敌机的矩形区域
				CRect enemyRect = enemy->GetRect();
				//判断两个矩形区域是否有交接
				CRect tempRect;
				if (tempRect.IntersectRect(&bombRect, enemyRect))
				{
					//将爆炸对象添加到爆炸链表中
					CExplosion *explosion = new CExplosion((bomb->GetPoint().x + BOMB_WIDTH / 2 - EXPLOSION_WIDTH / 2), (bomb->GetPoint().y + BOMB_HEIGHT / 2 - EXPLOSION_WIDTH / 2));
					explosionList.AddTail(explosion);
					PlaySound((LPCTSTR)IDR_WAV_EXPLOSION, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
					//爆炸后删除子弹
					bombList.RemoveAt(bombTemp);
					delete  bomb;
					//指向下一个
					bomb = NULL;
					//敌机生命值减少
					enemy->life -= (1 + isUpdate);
					if (enemy->life <= 0) {
						//增加得分
						passScore++;
						//删除敌机
						enemyList.RemoveAt(enemyTemp);
						delete  enemy;
					}
					//炮弹已删除，直接跳出本循环
					break;
				}
			}

			if (isBoss == TRUE&&bomb != NULL) {
				//获得战机子弹的矩形区域
				CRect bombRect = bomb->GetRect();
				//获得Boss的矩形区域
				CRect bossRect = boss->GetRect();
				//判断两个矩形区域是否有交接
				CRect tempRect;
				if (tempRect.IntersectRect(&bombRect, bossRect))
				{
					//将爆炸对象添加到爆炸链表中
					CExplosion *explosion = new CExplosion((bomb->GetPoint().x + BOMB_WIDTH / 2 - EXPLOSION_WIDTH / 2), (bomb->GetPoint().y + BOMB_HEIGHT / 2 - EXPLOSION_WIDTH / 2));
					explosionList.AddTail(explosion);
					PlaySound((LPCTSTR)IDR_WAV_EXPLOSION, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
					//爆炸后删除子弹
					bombList.RemoveAt(bombTemp);
					delete  bomb;
					bomb = NULL;
					//是Boss，不删除敌机，只扣血
					bossBlood -= (1 + isUpdate);
					if (bossBlood <= 0) {
						CExplosion *explosion1 = new CExplosion(boss->GetPoint().x , boss->GetPoint().y );
						explosionList.AddTail(explosion1);
						CExplosion *explosion2 = new CExplosion((boss->GetPoint().x + BOSS_WIDTH ), (boss->GetPoint().y + BOSS_HEIGHT ));
						explosionList.AddTail(explosion2);
						CExplosion *explosion3 = new CExplosion((boss->GetPoint().x + BOSS_WIDTH ), (boss->GetPoint().y ));
						explosionList.AddTail(explosion3);
						CExplosion *explosion4 = new CExplosion((boss->GetPoint().x ), (boss->GetPoint().y + BOSS_HEIGHT ));
						explosionList.AddTail(explosion4);
						CExplosion *explosion5 = new CExplosion((boss->GetPoint().x + BOSS_WIDTH / 2 - EXPLOSION_WIDTH / 2), (boss->GetPoint().y + BOSS_HEIGHT / 2 - EXPLOSION_WIDTH / 2));
						explosion5->SetBossDie(TRUE);//标记最后一个炸弹，炸完之后跳入下一关
						explosionList.AddTail(explosion5);
						PlaySound((LPCTSTR)IDR_WAV_EXPLOSION, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
						
						delete boss;
						boss = NULL;
						//过关的标志变量
						//isPause = TRUE;
						//CMyPlane* temp = myplane;
						//myplane = new CMyPlane(FALSE);
						delete  myplane;
						myplane = NULL;
						isPass = 1;
						isBoss = FALSE;
					}
				}
			}
		}//for
	}//if
   //（2）添加爆炸效果，敌机炸弹打中战机
	if (myplane != NULL&& isPause == 0)
	{
		//声明敌机炸弹位置坐标，战机位置myPlanePos
		POSITION ballPos, ballTemp;
		for (ballPos = ballList.GetHeadPosition(); (ballTemp = ballPos) != NULL;)
		{
			ball = (CBall *)ballList.GetNext(ballPos);
			//获得敌机炸弹的矩形区域
			CRect ballRect = ball->GetRect();
			//获得战机的矩形区域
			CRect myPlaneRect = myplane->GetRect();
			//判断两个矩形区域是否有交接
			CRect tempRect;
			//如果敌机炸弹打中战机，则使战机爆炸
			if (tempRect.IntersectRect(&ballRect, myPlaneRect))
			{
				//将爆炸对象加入爆炸链表中
				CExplosion *explosion = new CExplosion((ball->GetPoint().x + BALL_WIDTH / 2 - EXPLOSION_WIDTH / 2), (ball->GetPoint().y + BALL_HEIGHT / 2 - EXPLOSION_WIDTH / 2));
				PlaySound((LPCTSTR)IDR_WAV_EXPLOSION, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
				explosionList.AddTail(explosion);
				if (isProtect == FALSE&&test == FALSE)
					//战机生命值减1
					myLife--;
				//删除敌机炸弹
				ballList.RemoveAt(ballTemp);
				delete  ball;
				//游戏结束
				if (myLife == 0)
				{
					lifeNum--;
					if (lifeNum <= 0) {
						//删除战机对象
						//isPause = TRUE;
						CMyPlane* temp = myplane;
						myplane = NULL;
						//myplane = new CMyPlane(FALSE);
						delete  temp;
						temp = NULL;
						gameOver(pDC,cdc, cacheBitmap);
						break;
					}
					else {
						myLife = DEFAULT_LIFE;
					}
				}//if
			}//if
		}//for
	}//if
	//（3）添加爆炸效果，敌机与战机相撞
	if (myplane != NULL&& isPause == 0)
	{
		//获得战机的矩形区域
		CRect myPlaneRect = myplane->GetRect();
		POSITION enemyPos, enemyTemp;
		for (enemyPos = enemyList.GetHeadPosition(); (enemyTemp = enemyPos) != NULL;)
		{
			enemy = (CEnemy *)enemyList.GetNext(enemyPos);
			//获得敌机的矩形区域
			CRect enemyRect = enemy->GetRect();
			//判断两个矩形区域是否有交接
			CRect tempRect;
			if (tempRect.IntersectRect(&enemyRect, myPlaneRect))
			{
				CExplosion *explosion = new CExplosion(myplane->GetPoint().x + PLANE_WIDTH / 2 - EXPLOSION_WIDTH / 2, myplane->GetPoint().y + PLANE_HEIGHT / 2 - EXPLOSION_WIDTH / 2);
				PlaySound((LPCTSTR)IDR_WAV_EXPLOSION, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
				explosionList.AddTail(explosion);
				if (isProtect == FALSE&&test == FALSE)
					//战机生命值减1
					myLife--;
				//敌机生命值减少
				enemy->life--;
				if (enemy->life <= 0) {
					//得分
					passScore++;
					//删除敌机
					enemyList.RemoveAt(enemyTemp);
					delete  enemy;
				}
				//游戏结束
				if (myLife == 0)
				{
					lifeNum--;
					if (lifeNum <= 0) {
						//isPause = TRUE;
						//删除战机对象			
						CMyPlane* temp = myplane;
						myplane = NULL;
						//myplane = new CMyPlane(FALSE);
						delete  temp;
						temp = NULL;
						gameOver(pDC,cdc, cacheBitmap);
						break;
					}
					else {
						myLife = DEFAULT_LIFE;
					}
				}//if
			}//if
		}//for
		//Boss和战机相撞
		if (isBoss == TRUE) {
			myPlaneRect = myplane->GetRect();
			//获得Boss的矩形区域
			CRect bossRect = boss->GetRect();
			//判断两个矩形区域是否有交接
			CRect tempRect;
			if (tempRect.IntersectRect(&myPlaneRect, bossRect))
			{
				//将爆炸对象添加到爆炸链表中
				CExplosion *explosion = new CExplosion(myplane->GetPoint().x + PLANE_WIDTH / 2 - EXPLOSION_WIDTH / 2, myplane->GetPoint().y + PLANE_HEIGHT / 2 - EXPLOSION_WIDTH / 2);
				explosionList.AddTail(explosion);
				PlaySound((LPCTSTR)IDR_WAV_EXPLOSION, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
				if (isProtect == FALSE&&test == FALSE)
					//战机生命值减1
					myLife--;
				//是Boss，不删除敌机，只扣血
				bossBlood--;
				myplane->SetPoint(PLANE_X, PLANE_Y);
				if (bossBlood <= 0) {
					CExplosion *explosion1 = new CExplosion(boss->GetPoint().x, boss->GetPoint().y);
					explosionList.AddTail(explosion1);
					CExplosion *explosion2 = new CExplosion((boss->GetPoint().x + BOSS_WIDTH), (boss->GetPoint().y + BOSS_HEIGHT));
					explosionList.AddTail(explosion2);
					CExplosion *explosion3 = new CExplosion((boss->GetPoint().x + BOSS_WIDTH), (boss->GetPoint().y));
					explosionList.AddTail(explosion3);
					CExplosion *explosion4 = new CExplosion((boss->GetPoint().x), (boss->GetPoint().y + BOSS_HEIGHT));
					explosionList.AddTail(explosion4);
					CExplosion *explosion5 = new CExplosion((boss->GetPoint().x + BOSS_WIDTH / 2 - EXPLOSION_WIDTH / 2), (boss->GetPoint().y + BOSS_HEIGHT / 2 - EXPLOSION_WIDTH / 2));
					explosion5->SetBossDie(TRUE);//标记最后一个炸弹，炸完之后跳入下一关
					explosionList.AddTail(explosion5);
					PlaySound((LPCTSTR)IDR_WAV_EXPLOSION, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);

					delete boss;
					boss = NULL;
					//过关的标志变量
					//isPause = TRUE;
					//CMyPlane* temp = myplane;
					//myplane = new CMyPlane(FALSE);
					delete  myplane;
					myplane = NULL;
					isPass = 1;
					isBoss = FALSE;
				}
				//游戏结束
				if (myLife == 0)
				{
					lifeNum--;
					if (lifeNum <= 0) {
						//isPause = TRUE;
						//删除战机对象			
						CMyPlane* temp = myplane;
						myplane = NULL;
						//myplane = new CMyPlane(FALSE);
						delete  temp;
						temp = NULL;
						
						gameOver(pDC,cdc,cacheBitmap);
					}
					else {
						myLife = DEFAULT_LIFE;
						//删除原战机对象			
					}
				}//if
			}
		}//if
	}
	//（4）添加爆炸效果，战机子弹打中敌机炸弹
	if (myplane != NULL&& isPause == 0)
	{
		//声明战机子弹位置，敌机位置
		POSITION bombPos, bombTemp, ballPos, ballTemp;
		for (bombPos = bombList.GetHeadPosition(); (bombTemp = bombPos) != NULL;)
		{
			bomb = (CBomb *)bombList.GetNext(bombPos);
			//获得战机子弹的矩形区域
			CRect bombRect = bomb->GetRect();
			for (ballPos = ballList.GetHeadPosition(); (ballTemp = ballPos) != NULL;)
			{
				ball = (CBall *)ballList.GetNext(ballPos);
				//获得敌机炸弹的矩形区域
				CRect ballRect = ball->GetRect();
				//判断两个矩形区域是否有交接
				CRect tempRect;
				if (tempRect.IntersectRect(&bombRect, ballRect))
				{
					//将爆炸对象添加到爆炸链表中
					CExplosion *explosion = new CExplosion((ball->GetPoint().x + BALL_WIDTH /2-EXPLOSION_WIDTH/2), (ball->GetPoint().y + BOMB_HEIGHT/2 - EXPLOSION_WIDTH / 2));
					explosionList.AddTail(explosion);
					PlaySound((LPCTSTR)IDR_WAV_EXPLOSION, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
					//爆炸后删除战机子弹
					bombList.RemoveAt(bombTemp);
					delete  bomb;
					//删除敌机炸弹
					ballList.RemoveAt(ballTemp);
					delete  ball;
					//打掉敌机炮弹不加分
					//myScore++;
					//战机炮弹释放，直接跳出本循环
					break;
				}//if
			}//for
		}//for
	}//if
	 //（5）添加加血效果，战机吃到血包
	if (myplane != NULL&& isPause == 0)
	{
		//声明血包位置
		POSITION bloodPos, bloodTemp;
		for (bloodPos = bloodList.GetHeadPosition(); (bloodTemp = bloodPos) != NULL;)
		{
			blood = (CBlood *)bloodList.GetNext(bloodPos);
			//获得血包矩形
			CRect bloodbRect = blood->GetRect();
			//获得战机矩形
			CRect planeRect = myplane->GetRect();
			//判断两个矩形区域是否有交接
			CRect tempRect;
			if (tempRect.IntersectRect(&bloodbRect, planeRect))
			{
				//加血效果
				myLife += 5;
				if (myLife > DEFAULT_LIFE)
					myLife = DEFAULT_LIFE;
				// TODO 声音
				PlaySound((LPCTSTR)IDR_WAV_BLOOD, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
				//加血后血包删除
				bloodList.RemoveAt(bloodTemp);
				delete  blood;
				break;
			}//if
		}//for
	}//if

	//显示爆炸效果
	if (myplane != NULL&&isPause == FALSE) {
		POSITION explosionPos, explosionTemp;
		explosionPos = explosionList.GetHeadPosition();
		//检索爆炸链表，非空时在所在位置显示
		while (explosionPos != NULL)
		{
			explosionTemp = explosionPos;
			explosion = (CExplosion *)explosionList.GetNext(explosionPos);
			BOOL flag = explosion->Draw(&cdc, FALSE);
			//爆炸8帧结束，删除爆炸对象
			if (flag == EXPLOSION_STATUS_STOP) {
				//删除爆炸对象
				explosionList.RemoveAt(explosionTemp);
				delete explosion;
			}
		}//while
	}

	//开启血包
	if (myplane != NULL && myLife > 0)
	{
		//关卡打了三分之一三分之二处出现血包
		if (passScore > (PASS_SCORE + passNum * 5)*lifeCount/3)
		{
			//若屏幕中有未吃掉的血包，这次不产生血包
			if (bloodExist == FALSE) {
				lifeCount++;
				//产生血包
				blood = new CBlood(rect.right,rect.bottom);
				bloodList.AddTail(blood);
				bloodExist = TRUE;
				SetTimer(6, 10000, NULL);
			}else lifeCount++;
		}
	}
	//血包定时器，10秒后血包消失
	if (nIDEvent == 6&&isPause==0) {
		KillTimer(6);
		bloodExist = FALSE;
		//声明血包位置
		POSITION bloodPos, bloodTemp;
		for (bloodPos = bloodList.GetHeadPosition(); (bloodTemp = bloodPos) != NULL;)
		{
			blood = (CBlood *)bloodList.GetNext(bloodPos);
			bloodList.RemoveAt(bloodTemp);
			delete  blood;
		}
	}
	//显示血包
	if (myplane != NULL&&isPause == FALSE) 
	{
		POSITION bloodPos;
		bloodPos = bloodList.GetHeadPosition();
		//检索血包链表，非空时在所在位置显示
		while (bloodPos != NULL)
		{
			blood = (CBlood *)bloodList.GetNext(bloodPos);
			blood->Draw(&cdc, FALSE);
		}//while
	}

	//游戏界面输出该游戏当前信息
	if (myplane != NULL)
	{
		HFONT font;
		font = CreateFont(15, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 10, 0);
		cdc.SelectObject(font);
		CString str;
		cdc.SetTextColor(RGB(255, 0, 0));
		//设置透明背景
		cdc.SetBkMode(TRANSPARENT);
		str.Format(_T("当前关卡:%d"), passNum);
		cdc.TextOutW(10, 0, str);
		str.Format(_T("当前命数:%d"), lifeNum);
		cdc.TextOutW(110, 0, str);
		str.Format(_T("当前得分:%d"), passScore);
		cdc.TextOutW(10, 15, str);
		if (test == TRUE) {
			cdc.TextOutW(10, 200, _T("无敌模式！！！"));
		}

		HFONT font1;
		font1 = CreateFont(8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 10, 0);
		cdc.SelectObject(font);
		cdc.SetTextColor(RGB(255,0,0));
		cdc.TextOutW(rect.right- 12 * DEFAULT_LIFE-45, 0, _T("血量："));
		//输出血条
		CBrush brush;
		brush.CreateSolidBrush(RGB(255, 0, 0));//红色刷
		CBrush* oldBrush = cdc.SelectObject(&brush);
		int leftPos, topPos = 0, rightPos, buttomPos = 12;
		leftPos = rect.right-12* DEFAULT_LIFE;
		rightPos = leftPos + 12*myLife;
		cdc.Rectangle(leftPos, topPos, rightPos, buttomPos);
		brush.DeleteObject();

		HFONT font2;
		font2 = CreateFont(8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 10, 0);
		cdc.SelectObject(font);
		cdc.SetTextColor(RGB(0, 255, 0));
		cdc.TextOutW(rect.right - 12 * DEFAULT_LIFE - 45, 15, _T("魔法："));
		CBrush magicBrush;
		magicBrush.CreateSolidBrush(RGB(0, 255, 0));//蓝色刷
		cdc.SelectObject(&magicBrush);
		//输出魔法值
		topPos = 15, buttomPos = 27;
		leftPos = rect.right - 12 * DEFAULT_LIFE;
		rightPos = leftPos + 12 * magicCount;
		cdc.Rectangle(leftPos, topPos, rightPos, buttomPos);
		magicBrush.DeleteObject();

		//输出Boss血条
		if (isBoss == TRUE) {
			CBrush bossBrush;
			bossBrush.CreateSolidBrush(RGB(128, 0, 128));//紫色刷
			cdc.SelectObject(&bossBrush);
			topPos = 2, buttomPos = 12;
			leftPos = rect.right / 2 - 100;
			rightPos = leftPos + bossBlood/(boss->life /10)*20;
			cdc.Rectangle(leftPos, topPos, rightPos, buttomPos);
			bossBrush.DeleteObject();
		}
		cdc.SelectObject(&oldBrush);//把刷子复原原来的样子

		//输出血条中的详细血值
		HFONT textFont;
		textFont = CreateFont(12, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 10, 0);
		cdc.SelectObject(textFont);
		cdc.SetTextColor(RGB(255,255,255));
		str.Format(_T("%d/%d"), DEFAULT_LIFE, myLife);
		cdc.TextOutW(rect.right - 12 * DEFAULT_LIFE+12*4, 0, str);
		str.Format(_T("%d/%d"), DEFAULT_LIFE, magicCount);
		cdc.TextOutW(rect.right - 12 * DEFAULT_LIFE+12*4, 15, str);
		if (isBoss == TRUE) {
			str.Format(_T("%d/%d"), boss->life, bossBlood);
			cdc.TextOutW(rect.right / 2 - 20, 2, str);
		}

		//显示当前能发动的道具
		HFONT toolFont;
		toolFont = CreateFont(24, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 10, 0);
		cdc.SelectObject(toolFont);
		cdc.SetTextColor(RGB(255, 255, 255));
		if (magicCount > 0) {
			str.Format(_T("按C可打开防护罩"));
			cdc.TextOutW(0, rect.bottom - 100, str);
			str.Format(_T("按V可升级战机"));
			cdc.TextOutW(0, rect.bottom - 70, str);
		}
		if (magicCount >= 10) {
			str.Format(_T("按X可使用战机大招"));
			cdc.TextOutW(0, rect.bottom - 40, str);
		}
	}//if

	if (isStop == FLAG_RESTART) {
		HFONT textFont;
		textFont = CreateFont(18, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 10, 0);
		cdc.SelectObject(textFont);
		//设置透明背景
		cdc.SetBkMode(TRANSPARENT);
		cdc.SetTextColor(RGB(255, 255, 255));
		cdc.TextOutW(rect.right/2-150, rect.bottom/2-30, _T("哇，恭喜你已通关！是否重新开始？Y/N"));
	}
	else if (isStop == FLAG_STOP) {
		HFONT textFont;
		textFont = CreateFont(18 , 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 10, 0);
		cdc.SelectObject(textFont);
		//设置透明背景
		cdc.SetBkMode(TRANSPARENT);
		cdc.SetTextColor(RGB(255, 255, 255));
		//显示最后结果
		CString str;
		cdc.TextOutW(rect.right / 2 - 100, rect.bottom / 2 - 30, _T("GAME OVER！"));
		str.Format(_T("您的得分为：%d"), myScore);
		cdc.TextOutW(rect.right / 2 - 100, rect.bottom / 2 - 10, str);
		cdc.TextOutW(rect.right / 2 - 100, rect.bottom / 2 +10, _T("COME ON ！重新开始？Y/N"));
	}

	//控制魔法值
	if (nIDEvent == 5) {
		if (myplane != NULL&&isPause == 0) {
			//防护罩和战机升级没打开，魔法值递增
			if (isProtect==FALSE&&isUpdate==FALSE) {
				magicCount++;
				if (magicCount > 10)
					magicCount = 10;
			}
			//判断是否打开防护罩
			if (isProtect == TRUE) {
				//开启防护罩魔法值递减
				magicCount --;
				if (magicCount <= 0) {
					magicCount = 0;
					isProtect = FALSE;
				}
			}
			//判断是否升级战机
			if (isUpdate == TRUE) {
				//战机升级，魔法值递减
				magicCount--;
				if (magicCount <= 0) {
					magicCount = 0;
					isUpdate = FALSE;
					myplane->SetIsUpdate(isUpdate);
				}
			}
		}
	}

	// 进入下一关界面
	int pScore = PASS_SCORE + passNum * 5;
	// TODO调试条件
	//if (myplane != NULL && passScore >= 3 && isPause == 0&&isBoss==FALSE)
	if (myplane != NULL && passScore >= pScore && isPause == 0 && isBoss == FALSE)
	{
		//进入Boss
		isBoss = TRUE;
		boss = new CBoss(1, rect.right, rect.bottom);
		boss->SetSpeed(BOSS_SPEED+passNum-1);
		boss->life = BOSS_LIFE + passNum * 50;//Boss总血量
		bossBlood= BOSS_LIFE + passNum * 50;//当前Boss血量
		//Boss出场，暂停游戏
		bossLoaded = FALSE;

		//重新设置Boss的子弹产生频率，增强Boss子弹发射频率
		KillTimer(3);
		SetTimer(3, 2000 - passNum * 180, NULL); 
	}

	//显示Boss
	if (myplane != NULL &&boss!=NULL&& isPause == 0 && isBoss == TRUE) {
		BOOL status = boss->Draw(&cdc,passNum, FALSE);
		if (status == TRUE)
			bossLoaded = TRUE;
	}

	if (isPass == 1)
	{
		isPass = FALSE;
		if (passNum ==10)//10关
		{
			//重新初始化数据
			KillTimer(1);
			KillTimer(2);
			KillTimer(3);
			//KillTimer(4);
			KillTimer(5);
			myplane = new CMyPlane(FALSE);
			isPause = TRUE;
			
			isStop = FLAG_RESTART;
			//清屏
			CBitmap* tCache = cacheBitmap;
			cacheBitmap = new CBitmap;
			cacheBitmap->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
			cdc.SelectObject(cacheBitmap);
			delete tCache;
		}//if
		else
		{
			KillTimer(1);
			KillTimer(2);
			KillTimer(3);
			//KillTimer(4);
			KillTimer(5);
			isPause = TRUE;	
			//保存所需数据
			int tScore = myScore + passScore;
			int tPassNum = passNum + 1;
			int tTest = test;
			int magic = magicCount;
			//重新开始游戏
			Restart();
			myScore = tScore;
			passNum = tPassNum;
			magicCount = magic;
			test = tTest;
		}//else
	}//if

	//将二级缓冲cdc中的数据推送到一级级缓冲pDC中，即输出到屏幕中
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &cdc, 0, 0, SRCCOPY);
	//释放二级cdc
	cdc.DeleteDC();
	//释放缓冲位图
	cacheBitmap->DeleteObject();
	//释放一级pDC
	ReleaseDC(pDC);
	CView::OnTimer(nIDEvent);
}

//键盘按下监听
void CPlaneWarView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (myplane != NULL && (GetKeyState(VK_UP) < 0 || GetKeyState('W') < 0) && isPause == 0) {
		//上方向键
		int y = myplane->GetPoint().y - speed;
		if (y < 0)
			y = 0;
		myplane->SetPoint(myplane->GetPoint().x, y);
	}
	else if (myplane != NULL && (GetKeyState(VK_DOWN) < 0 || GetKeyState('S') < 0) && isPause == 0) {
		//下方向键
		int y = myplane->GetPoint().y + speed;
		if (y >rect.bottom-1)
			y = rect.bottom -1;
		myplane->SetPoint(myplane->GetPoint().x, y);
	}
	else if (myplane != NULL && (GetKeyState(VK_LEFT) < 0 || GetKeyState('A') < 0) && isPause == 0 ) {
		//左方向键
		int x = myplane->GetPoint().x - speed;
		if (x < 0)
			x = 0;
		myplane->SetPoint(x, myplane->GetPoint().y);
	}
	else if (myplane != NULL && (GetKeyState(VK_RIGHT) < 0 || GetKeyState('D') < 0) && isPause == 0 ) {
		//右方向键
		int x = myplane->GetPoint().x + speed;
		if (x >rect.right - 1)
			x = rect.right - 1;
		myplane->SetPoint(x, myplane->GetPoint().y);
	}
	else if (myplane != NULL && (GetKeyState(VK_SPACE) < 0) && isPause == 0 && bossLoaded == TRUE) {
		//空格键发射子弹
		CBomb *Bomb1 = new CBomb(myplane->GetPoint().x+20, myplane->GetPoint().y, 1,isUpdate);
		PlaySound((LPCTSTR)IDR_WAV_BOMB, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		bombList.AddTail(Bomb1);
		CBomb *Bomb2 = new CBomb(myplane->GetPoint().x + PLANE_WIDTH-50, myplane->GetPoint().y, 1, isUpdate);
		bombList.AddTail(Bomb2);
		//子弹自动飞行，在Timer中绘制
	}
	else if (myplane != NULL&&GetKeyState('Z') < 0)
	{
		//暂停游戏，开始游戏
		if (isPause == TRUE)
			isPause = FALSE;
		else {
			Pause();
		} 
	}//if
	else if (myplane != NULL&&GetKeyState('X') < 0 && isPause == 0 && bossLoaded == TRUE)
	{
		//战机发大招
		if (magicCount >= 10) {
			magicCount -= 10;
				//清空敌机
				POSITION enemyPos, enemyTemp;
				for (enemyPos = enemyList.GetHeadPosition(); (enemyTemp = enemyPos) != NULL;)
				{
					enemy = (CEnemy *)enemyList.GetNext(enemyPos);
					//将爆炸对象添加到爆炸链表中
					CExplosion *explosion = new CExplosion((enemy->GetPoint().x + ENEMY_WIDTH / 2), (enemy->GetPoint().y + ENEMY_HEIGHT / 2));
					explosionList.AddTail(explosion);
					PlaySound((LPCTSTR)IDR_WAV_DAZHAO, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);

					//删除敌机
					enemyList.RemoveAt(enemyTemp);
					delete  enemy;
					//增加得分
					passScore++;
				}//for
			if(isBoss==TRUE) {
				//将爆炸对象添加到爆炸链表中
				CExplosion *explosion = new CExplosion(boss->GetPoint().x + BOSS_WIDTH / 2, boss->GetPoint().y + BOSS_HEIGHT / 2);
				explosionList.AddTail(explosion);
				PlaySound((LPCTSTR)IDR_WAV_DAZHAO, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
				bossBlood -= 50;
				if (bossBlood <= 0) {
					//boss死，过关
					//过关的标志变量
					delete boss;
					boss = NULL;
					//过关的标志变量
					isPause = TRUE;
					CMyPlane* temp = myplane;
					myplane = new CMyPlane(FALSE);
					delete  temp;
					temp = NULL;
					isPass = 1;
					isBoss = FALSE;
				}
			}
			 //清空敌机炮弹
			POSITION ballPos, ballTemp;
			for (ballPos = ballList.GetHeadPosition(); (ballTemp = ballPos) != NULL;)
			{
				ball = (CBall *)ballList.GetNext(ballPos);
				//删除敌机炮弹
				ballList.RemoveAt(ballTemp);
				delete  ball;
			}//for
		}
	}//if
	else if (myplane != NULL&&GetKeyState('C') < 0 && isPause == 0) {
		//开启防护罩
		isProtect = TRUE;
		PlaySound((LPCTSTR)IDR_WAV_PROTECT, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
	}
	else if (myplane != NULL&&GetKeyState('V') < 0 && isPause == 0) {
		//战机升级
		isUpdate = TRUE;
		myplane->SetIsUpdate(isUpdate);
		PlaySound((LPCTSTR)IDR_WAV_UPDATE, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
	}
	else if (GetKeyState('Y') < 0 ) {
		if (isStop == FALSE) {
			//无敌模式开关
			if (test == FALSE)
				test = TRUE;
			else test = FALSE;
		}
		else {
			isStop = FALSE;
			Restart();
		}
	}
	else if (GetKeyState('N') < 0) {
		if (isStop!=FALSE) {
			MyDialog dialog;
			dialog.DoModal(); 
		}
	}
	//按空格进入游戏
	if (isStarted == FALSE && (GetKeyState(VK_SPACE) < 0)) {
		isStarted = TRUE;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

//鼠标移动监听，控制战机位置
void CPlaneWarView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (myplane!=NULL && isPause == 0 ) {
		//绘制新游戏对象
		myplane->SetPoint(point.x,point.y);
	}
	CView::OnMouseMove(nFlags, point);
}

//按下鼠标左键监听
void CPlaneWarView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (myplane!=NULL&&isPause == 0 && bossLoaded == TRUE)
	{
		CBomb *Bomb1 = new CBomb(myplane->GetPoint().x, myplane->GetPoint().y, 1, isUpdate);
		PlaySound((LPCTSTR)IDR_WAV_BOMB, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		bombList.AddTail(Bomb1);
		CBomb *Bomb2 = new CBomb(myplane->GetPoint().x + BOMB_DISTANCE, myplane->GetPoint().y, 1, isUpdate);
		bombList.AddTail(Bomb2);
		//子弹自动飞行，在Timer中绘制
	}
	if (isStarted == FALSE) {
		isStarted = TRUE;
	}

	CView::OnLButtonDown(nFlags, point);
}

//鼠标右键监听
void CPlaneWarView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	/*ClientToScreen(&point);
	OnContextMenu(this, point);*/

	/*myplane = new CMyPlane();
	myplane->Draw(GetDC(), TRUE);*/
}

//生命周期

//窗口销毁
void CPlaneWarView::OnDestroy()
{
	// 销毁指针资源
	if (myplane != NULL)
		delete myplane;
	if (boss != NULL)
		delete boss;
	/*if (enemy != NULL)
		delete enemy;
	if (bomb != NULL)
		delete bomb;
	if (ball != NULL)
		delete ball;
	if (explosion != NULL)
		delete explosion;*/

		//释放内存资源
	scene.ReleaseScene();

	CView::OnDestroy();
}

//游戏重新开始
void CPlaneWarView::Restart()
{
	// TODO: 在此处添加游戏重新开始初始化参数
	//战机重新加载
	myplane = new CMyPlane(FALSE);

	//清空敌机链表
	if (enemyList.GetCount() > 0)
		enemyList.RemoveAll();
	//清空战机链表
	if (meList.GetCount() > 0)
		meList.RemoveAll();
	//清空战机子弹链表
	if (bombList.GetCount() > 0)
		bombList.RemoveAll();
	//清空敌机炸弹链表
	if (ballList.GetCount() > 0)
		ballList.RemoveAll();
	//清空爆炸链表
	if (explosionList.GetCount() > 0)
		explosionList.RemoveAll();
	//清空血包列表
	if (bloodList.GetCount() > 0)
		bloodList.RemoveAll();

	//参数重新初始化
	myLife = DEFAULT_LIFE;
	lifeNum = DEFAULT_LIFE_COUNT;
	myScore = 0;
	passScore = 0;
	passNum = DEFAULT_PASS;
	isPass = 0;
	isPause = 0;
	lifeCount = 1;
	magicCount = 0;
	bloodExist = FALSE;
	bossBlood = BOSS_LIFE;
	isBoss = FALSE;
	bossLoaded = TRUE;
	isProtect = FALSE;
	isUpdate = FALSE;
	test = FALSE;
	boss = NULL;
	//isStarted = FALSE;
	SetMyTimer();
}

//游戏暂停
void CPlaneWarView::Pause()
{
	// TODO: 在此处添加游戏暂停操作
	isPause = TRUE;
	Sleep(1000);
}

// 生命值归零，游戏结束
void CPlaneWarView::gameOver(CDC* pDC,CDC& cdc,CBitmap* cacheBitmap)
{
	//结束游戏界面
	//释放计时器
	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
	//KillTimer(4);
	KillTimer(5);
	//计算最后得分
	myScore += passScore;
	//播放游戏结束音乐
	PlaySound((LPCTSTR)IDR_WAV_GAMEOVER, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
	//清屏
	CBitmap* tCache = cacheBitmap;
	cacheBitmap = new CBitmap;
	cacheBitmap->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	cdc.SelectObject(cacheBitmap);
	//内存中承载临时图像的缓冲位图
	delete tCache;
	isStop = FLAG_STOP;
}

//设置计时器
void CPlaneWarView::SetMyTimer()
{
	SetTimer(1, 17, NULL);//刷新界面定时器
	SetTimer(2, 400-passNum*30, NULL);//产生敌机定时器
	SetTimer(3, 2000- passNum * 100, NULL);//产生敌机炮弹频率
	
	SetTimer(5, 2000, NULL);//控制魔法值变化频率
}

//void CPlaneWarView::OnClose()
//{
//	isPause = TRUE;
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	KillTimer(1);
//	KillTimer(2);
//	KillTimer(3);
//	KillTimer(4);
//	KillTimer(5 );
//	CView::OnClose();
//}
