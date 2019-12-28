#include "control.h"
#include"graphics.h"
#include"conio.h"
#include"mydefine.h"
#include "stdio.h"
extern int world;	//声明全局变量

control::control(void) //构造
{
	key = 0;
}   
control::~control(void) //析构
{}  

int control::GetCommand() //键盘中转 返回key
{
	int c = 0;
	
	//由于接收的信号不知一个，所以需要异步输入
	//异步输入函数：利用Windows API中的GetAsyncKeyState函数

	if (GetAsyncKeyState('A') & 0x8000)
		c |= CMD_LEFT;
	if (GetAsyncKeyState('D') & 0x8000)
		c |= CMD_RIGHT;
	if ((GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('K') & 0x8000))
		c |= CMD_UP;
	if (GetAsyncKeyState('S') & 0x8000)
		c |= CMD_DOWN;
	if (GetAsyncKeyState('J') & 0x8000)
		c |= CMD_SHOOT;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		c |= CMD_ESC;
	return c;
}
void control::pauseClick()
{

	//绘制UI界面
	BeginBatchDraw();

	//确定页面顶点
	POINT points[8] = { { XSIZE / 2 - 45,YSIZE / 3 },{ XSIZE / 2 + 45,YSIZE / 3 },{ XSIZE / 2 + 45,YSIZE / 3 + 120 },{ XSIZE / 2 - 45,YSIZE / 3 + 120 } };
	setfillcolor(GREEN);
	fillpolygon(points, 4); //绘制多边形
	setbkmode(TRANSPARENT); //设置背景模式-透明
	
	//绘制Text
	settextstyle(15, 0, "华纹琥珀");

	//darwtext Windows自带的矩形文本 可以用outtextxy()代替的 由于可以使用RECT来储存文本矩形的参数 所有用起来更方便一些
	//RECT：rect这个对象是用来存储成对出现的参数，比如，一个矩形框的左上角坐标、宽度和高度
	//DT_SINGLELINE :单行显示
	//DT_CENTER:居中显示
	//DT_VCENTER : 垂直居中显示

	RECT r2 = { XSIZE / 2 - 45,YSIZE / 3,XSIZE / 2 + 45,YSIZE / 3 + 30 };	//回到游戏
	rectangle(XSIZE / 2 - 45, YSIZE / 3, XSIZE / 2 + 45, YSIZE / 3 + 30);
	drawtext("滚回游戏", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r3 = { XSIZE / 2 - 45,YSIZE / 3 + 30,XSIZE / 2 + 45,YSIZE / 3 + 60 };	//重新开始
	rectangle(XSIZE / 2 - 45, YSIZE / 3 + 30, XSIZE / 2 + 45, YSIZE / 3 + 60);
	drawtext("再来一次", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r4 = { XSIZE / 2 - 45,YSIZE / 3 + 60,XSIZE / 2 + 45,YSIZE / 3 + 90 };	//主菜单
	rectangle(XSIZE / 2 - 45, YSIZE / 3 + 60, XSIZE / 2 + 45, YSIZE / 3 + 90);
	drawtext("打包回家", &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	RECT r5 = { XSIZE / 2 - 45,YSIZE / 3 + 90,XSIZE / 2 + 45,YSIZE / 3 + 120 };	//主菜单
	rectangle(XSIZE / 2 - 45, YSIZE / 3 + 90, XSIZE / 2 + 45, YSIZE / 3 + 120);
	drawtext("求你存档", &r5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//结束绘制
	FlushBatchDraw();

	FlushMouseMsgBuffer();//清空鼠标消息队列，否则会出错 //因为延时的存在，所以需要这样正确处理鼠标消息

	//绘制暂停时候的UI交互
	while (true)
	{
		BeginBatchDraw();
		MOUSEMSG m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN: //点击选中
			EndBatchDraw();
			if (m.x>XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 && m.y<YSIZE / 3 + 30)//如果选择“回到游戏”，则返回VIR_RETURN
			{
				key = VIR_RETURN;
				return;
			}
			else if (m.x>XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 + 30 && m.y<YSIZE / 3 + 60)//如果选择“重新开始”，则返回VIR_RESTART
			{
				key = VIR_RESTART;
				return;
			}
			else if (m.x>XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 + 60 && m.y<YSIZE / 3 + 90)//如果选择“主菜单”，则返回VIR_HOME
			{
				key = VIR_HOME;
				return;
			}
			else if (m.x>XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 + 90 && m.y<YSIZE / 3 + 120)//如果选择“主菜单”，则返回VIR_HOME
			{
				FILE *fp=NULL;
				fp = fopen(".\\gameRecord.dat", "w");
				fprintf_s(fp, "%d",world );
				fclose(fp);
				key = VIR_RETURN;
				return;
			}

		case WM_MOUSEMOVE: //移动高亮
			RECT r;
			int i;
			for (i = 0; i<4; i++)	//相当于重新绘制一遍 用不同的颜色突出
			{
				//当鼠标移动到该区域时
				if (m.x>XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 + i * 30 && m.y<YSIZE / 3 + 30 + i * 30)
				{
					r.left = XSIZE / 2 - 45;
					r.top = YSIZE / 3 + i * 30;
					r.right = XSIZE / 2 + 45;
					r.bottom = YSIZE / 3 + 30 + i * 30;
					POINT points[8] = { { r.left,r.top },{ r.right,r.top },{ r.right,r.bottom },{ r.left,r.bottom } };
					setfillcolor(BLUE);
					fillpolygon(points, 4);
					setbkmode(TRANSPARENT);
					switch (i)
					{
					case 0:
						drawtext("Return", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						break;
					case 1:
						drawtext("start again", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						break;
					case 2:
						drawtext("The menu", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						break;
					case 3:
						drawtext("Write Dat", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						break;
					}
				}
				else
				{
					//当鼠标移出该区域时
					if (getpixel(XSIZE / 2 - 45 + 1, YSIZE / 3 + i * 30 + 1) == BLUE) //获取点的颜色
					{
						r.left = XSIZE / 2 - 45;
						r.top = YSIZE / 3 + i * 30;
						r.right = XSIZE / 2 + 45;
						r.bottom = YSIZE / 3 + 30 + i * 30;
						POINT points[8] = { { r.left,r.top },{ r.right,r.top },{ r.right,r.bottom },{ r.left,r.bottom } };
						setfillcolor(GREEN);
						fillpolygon(points, 4);
						setbkmode(TRANSPARENT);
						switch (i)
						{
						case 0:
							drawtext("滚回游戏", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 1:
							drawtext("再来一次", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 2:
							drawtext("打包回家", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 3:
							drawtext("求你存档", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						}
					}
					FlushBatchDraw();
				}
			}
		}
	}
}

int control::getKey() //获取键盘
{
	if (_kbhit())
	{
		key = GetCommand();
	}
	if (key&CMD_ESC) //当按Esc键时弹出暂停菜单
	{
		pauseClick();
	}
	return key;
}
void control::gameStart() //进入时的开始界面
{
	cleardevice(); //清屏
	settextstyle(40, 0, "Gill Sans");
	RECT r1 = { 0, 0, XSIZE, YSIZE / 3 };
	drawtext("超级马里奥v1.0", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//主界面(同时也是界面1)
	settextstyle(20, 0, "Agency FB");
	RECT r2 = { XSIZE / 2 - 45,YSIZE / 3,XSIZE / 2 + 45,YSIZE / 3 + 30 }; rectangle(XSIZE / 2 - 45, YSIZE / 3, XSIZE / 2 + 45, YSIZE / 3 + 30);
	drawtext("开始", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT r3 = { XSIZE / 2 - 45,YSIZE / 3 + 30,XSIZE / 2 + 45,YSIZE / 3 + 60 }; rectangle(XSIZE / 2 - 45, YSIZE / 3 + 30, XSIZE / 2 + 45, YSIZE / 3 + 60);
	drawtext("介绍", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT r4 = { XSIZE / 2 - 45,YSIZE / 3 + 60,XSIZE / 2 + 45,YSIZE / 3 + 90 }; rectangle(XSIZE / 2 - 45, YSIZE / 3 + 60, XSIZE / 2 + 45, YSIZE / 3 + 90);
	drawtext("指导", &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT r5 = { XSIZE / 2 - 45,YSIZE / 3 + 90,XSIZE / 2 + 45,YSIZE / 3 + 120 }; rectangle(XSIZE / 2 - 45, YSIZE / 3 + 90, XSIZE / 2 + 45, YSIZE / 3 + 120);
	drawtext("退出", &r5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r6 = { XSIZE / 2 - 45,YSIZE / 3 + 120,XSIZE / 2 + 45,YSIZE / 3 + 150 }; rectangle(XSIZE / 2 - 45, YSIZE / 3 + 120, XSIZE / 2 + 45, YSIZE / 3 + 150);
	drawtext("读档", &r6, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	bool _HOME = true, _INTRODUCTION = false, _OPERATION = false,_READ=false; //true表示处于当前页面 UI界面三个

	MOUSEMSG m;
	while (_HOME) //当在界面1时的UI交互
	{
		BeginBatchDraw();
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN: //点击选择
			EndBatchDraw();
			if (m.x>XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 && m.y<YSIZE / 3 + 30 && _HOME == 1 && _INTRODUCTION == 0 && _OPERATION == 0)//如果选择“开始游戏”
			{
				_HOME = false;
				break;
			}
			else if (m.x>XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 + 30 && m.y<YSIZE / 3 + 60 && _HOME == 1 && _OPERATION == 0)//如果选择“游戏介绍”
			{
				_INTRODUCTION = true;	//界面2的绘制
				cleardevice();
				rectangle(50, 50, 213, 220);
				outtextxy(52, 52, "game introduction：");
				outtextxy(52, 82, "This is super Mario");
				outtextxy(52, 102, "Actually its name is CX'adventure");
				outtextxy(52, 132, "developers：");
				outtextxy(52, 152, "YM LMX");
				outtextxy(52, 172, "ZBW LHS");
				RECT R1 = { XSIZE - 46,YSIZE - 26,XSIZE - 2,YSIZE - 2 }; rectangle(XSIZE - 46, YSIZE - 26, XSIZE - 2, YSIZE - 2);
				drawtext("返回", &R1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				break;
			}
			else if (m.x>XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 + 60 && m.y<YSIZE / 3 + 90 && _HOME == 1 && _INTRODUCTION == 0)//如果选择“操作说明”
			{
				_OPERATION = true;	//界面3的绘制
				cleardevice();
				rectangle(50, 50, 213, 220);
				outtextxy(52, 52, "Introduction：");
				outtextxy(52, 72, "move LEFT：A键");
				outtextxy(52, 92, "move RIGHT：D键");
				outtextxy(52, 112, "Fire：J键");
				outtextxy(52, 132, "Jump：W键/K键");
				outtextxy(52, 152, "Pause：Esc键");
				RECT R2 = { XSIZE - 46,YSIZE - 26,XSIZE - 2,YSIZE - 2 }; rectangle(XSIZE - 46, YSIZE - 26, XSIZE - 2, YSIZE - 2);
				drawtext("返回", &R2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				break;
			}
			else if (m.x>XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 + 90 && m.y<YSIZE / 3 + 120 && _HOME == 1 && _INTRODUCTION == 0 && _OPERATION == 0)//如果选择“退出游戏”
				exit(0);
			else if (m.x>XSIZE - 46 && m.x<XSIZE - 3 && m.y>YSIZE - 26 && m.y<YSIZE - 3 && (_INTRODUCTION == 1 || _OPERATION == 1))//如果选择“返回”
			{
				cleardevice();
				_HOME = false, _INTRODUCTION = false, _OPERATION = false;
				gameStart();
			}
			else if (m.x>XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 + 120 && m.y<YSIZE / 3 + 150 && _HOME == 1 && _INTRODUCTION == 0 && _OPERATION == 0) //如果选择"读取存档"
			{
				//读档在此加入
				FILE *fp;
				fp = fopen(".\\gameRecord.dat","r");
				fscanf(fp,"%d",&world);
				fclose(fp);
				_HOME = false;
				break;
			}
			else
				break;
		case WM_MOUSEMOVE:	//高亮显示
			RECT r;
			if (_INTRODUCTION == 1 || _OPERATION == 1)
			{
				if (m.x>XSIZE - 46 && m.x<XSIZE - 3 && m.y>YSIZE - 26 && m.y<YSIZE - 3)
				{
					r.left = XSIZE - 46;
					r.top = YSIZE - 26;
					r.right = XSIZE - 2;
					r.bottom = YSIZE - 2;
					POINT points[8] = { { r.left,r.top },{ r.right,r.top },{ r.right,r.bottom },{ r.left,r.bottom } };
					setfillcolor(RED);
					fillpolygon(points, 4);
					setbkmode(TRANSPARENT);
					drawtext("Return", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				else
				{
					if (getpixel(XSIZE - 46 + 1, YSIZE - 26 + 1) == RED)
					{
						r.left = XSIZE - 46;
						r.top = YSIZE - 26;
						r.right = XSIZE - 2;
						r.bottom = YSIZE - 2;
						POINT points[8] = { { r.left,r.top },{ r.right,r.top },{ r.right,r.bottom },{ r.left,r.bottom } };
						setfillcolor(BLACK);
						fillpolygon(points, 4);
						setbkmode(TRANSPARENT);
						drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
				}
			}
			else
			{
				for (int i = 0; i<5; i++)
				{
					if (m.x>XSIZE / 2 - 45 && m.x<XSIZE / 2 + 45 && m.y>YSIZE / 3 + i * 30 && m.y<YSIZE / 3 + 30 + i * 30)
					{
						r.left = XSIZE / 2 - 45;
						r.top = YSIZE / 3 + i * 30;
						r.right = XSIZE / 2 + 45;
						r.bottom = YSIZE / 3 + 30 + i * 30;
						POINT points[8] = { { r.left,r.top },{ r.right,r.top },{ r.right,r.bottom },{ r.left,r.bottom } };
						setfillcolor(RED);
						fillpolygon(points, 4);
						setbkmode(TRANSPARENT);
						switch (i)
						{
						case 0:
							drawtext("Start game", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 1:
							drawtext("Introduction", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 2:
							drawtext("directions", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 3:
							drawtext("Exit", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 4:
							drawtext("Read", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						}
					}
					else
					{
						if (getpixel(XSIZE / 2 - 45 + 1, YSIZE / 3 + i * 30 + 1) == RED)
						{
							r.left = XSIZE / 2 - 45;
							r.top = YSIZE / 3 + i * 30;
							r.right = XSIZE / 2 + 45;
							r.bottom = YSIZE / 3 + 30 + i * 30;
							POINT points[4] = { { r.left,r.top },{ r.right,r.top },{ r.right,r.bottom },{ r.left,r.bottom } };
							setfillcolor(BLACK);
							fillpolygon(points, 4);
							setbkmode(TRANSPARENT);
							switch (i)
							{
							case 0:
								drawtext("开始", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 1:
								drawtext("介绍", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 2:
								drawtext("指导", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 3:
								drawtext("退出", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 4:
								drawtext("读档", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							}
						}
					}
				}
			}
			FlushBatchDraw();
			break;
		default:
			break;
		}
	}
}
void control::showScore(int score) //显示分数
{
	settextstyle(0, 0, "Cooper");
	char s1[20] = "SCORE:  ";
	char s2[5];
	_itoa_s(score, s2, 10);	//功能：将任意类型的数字转换为字符串。在<stdlib.h>中与之有相反功能的函数是atoi。sprintf同样功能 
	strcat_s(s1, s2);
	setbkmode(TRANSPARENT); //文本UI的最上显示 layout
	outtextxy(10, 10, s1);  //左边显示
	setbkmode(OPAQUE);
}
void control::showLevel(int level) //显示关卡
{
	settextstyle(0, 0, "Cooper");
	char s1[20] = "LEVEL:  ";
	char s2[2];
	_itoa_s(level, s2, 10);	//同理
	strcat_s(s1, s2);	
	setbkmode(TRANSPARENT);	//右边显示
	outtextxy(XSIZE - 90, 10, s1);
	setbkmode(OPAQUE);
}
void control::showDied(int life) //主角死亡时的动画 告诉生命值
{
	settextstyle(0, 0, "Goudy Stout");
	cleardevice();
	char s1[24] = "You 还 can 扑街:  ";
	char s2[2];
	_itoa_s(life, s2, 10);
	outtextxy(XSIZE / 2 - 43, YSIZE / 3, s1);
	outtextxy(XSIZE / 2, YSIZE / 2 - 20, s2);
	Sleep(2000);
}
void control::showGameOver() //游戏结束时的动画
{
	IMAGE img;
	loadimage(&img, "res\\home.bmp", XSIZE, 5 * YSIZE);
	putimage(0, -YSIZE, &img);
	Sleep(6500);
}
void control::showPassed(int world) //通过某一关的画面
{
	cleardevice();
	char s1[20] = "LEVEL:  ";
	char s2[2];
	_itoa_s(world, s2, 10);
	outtextxy(XSIZE / 2 - 20, YSIZE / 3, s1);
	outtextxy(XSIZE / 2, YSIZE / 2 - 20, s2);
	Sleep(2000);
}
void control::showPassedAll() //通关所有的动画
{
	IMAGE img;
	loadimage(&img, "res\\home.bmp", XSIZE, 5 * YSIZE);
	putimage(0, -3 * YSIZE, &img);
	Sleep(7800);
}