#include "scene.h"
#include"graphics.h"
#include"math.h"
#include"role.h"
#include"mydefine.h"	

scene::scene(int world)
{
	loadimage(&img_bg, "res\\mapsky.bmp", XSIZE, 4 * YSIZE);	//加载图片
	loadimage(&img_map, "res\\map.bmp");
	loadimage(&img_scenery, "res\\scenery.bmp");
	loadimage(&img_coin, "res\\ani.bmp");
	loadimage(&img_food, "res\\ani.bmp");
	xBg = 0;
	yBg = 0;
	xMap = 0;
	yMap = 0;
	scenery_iframe = 1;	//帧数设置
	coin_iframe = 1;
	food_iframe = 1;
	for (int i = 0; i<MAP_NUMBER; i++)	//map初始化
	{
		map[i].x = 0;
		map[i].y = 0;
		map[i].id = 0;
		map[i].u = 0;
		map[i].xAmount = 0;
		map[i].yAmount = 0;
	}
	for (int j = 0; j<COINS_NUMBER; j++)	//coin初始化
	{
		coins[j].x = 0;
		coins[j].y = 0;
	}
	for (int k = 0; k<SCORE_NUMBER; k++)	//score初始化
	{
		score[k].x = 0;
		score[k].y = 0;
		score_iframe[k] = 0;
	}
	for (int l = 0; l<FOOD_NUMBER; l++)		//food初始化
	{
		food[l].x = 0;
		food[l].y = 0;
	}
	this->world = world;	//根据关卡绘制场景
	createCoin();
	createFood();
	createMap(world);
}
scene::~scene(void)
{}

bool scene::isEnding(int distance)
{
	//目前所在关卡是否结束	根据所走的长度决定
	switch (world)
	{
	case 1:
		if (distance>94 * WIDTH)
			return true;
		return false;
	case 2:
		if (distance>104 * WIDTH)
			return true;
		return false;
	case 3:
		if (distance>94 * WIDTH)
			return true;
		return false;
	}
	return false;
}

void scene::createMap(int world)	//生成不同的地板 id就是其地板样式
{
	if (world == 1)
	{
		//编辑地图
		Map m[] = { { 0,9,1,15,3 },{ 18,8,1,9,4 },{ 27,9,1,37,3 },{ 67,9,1,4,3 },{ 74,9,1,1,3 },{ 78,9,1,28,3 }, //id:1
		{ 10,6,2,4,1 },{ 40,4,2,3,1 },{ 62,6,2,1,1 },{ 65,4,2,5,1 },	//id:2
		{ 36,7,10,1,1 },{ 45,7,10,1,1 },{ 80,7,10,1,1 },{ 90,7,10,1,1 },	//id:10
		{ 19,6,11,1,1 },{ 23,6,11,1,1 },{ 32,7,11,1,1 },{ 49,7,11,1,1 },{ 53,7,11,1,1 },{ 57,7,11,1,1 },{ 93,7,11,1,1 },	//id:11
		{ 101,7,12,1,1 },	//id:12
		{ 15,10,13,1,1 },{ 64,10,13,1,1 },{ 71,10,13,1,1 },{ 75,10,13,1,1 }	//id:13
		};
		int i = 0;
		while (m[i].id>0 && m[i].id<15 && i<MAP_NUMBER)//(???存在问题)	//不同的地板设置不同的摩擦力
		{
			switch (m[i].id)
			{
			case 1:
			case 3:
			case 4:
			case 5:
			case 6:
				m[i].u = (V_MAX / T1) / G;
				break;
			case 2:
				m[i].u = (V_MAX / T2) / G;
				break;
			default:
				m[i].u = (V_MAX / T3) / G;
				break;
			}
			map[i] = m[i];	//此处赋值
			i++;
		}
	}
	else if (world == 2)
	{
		//编辑地图
		Map m[] = { { 18,9,2,4,1 },{ 23,6,2,6,1 },{ 25,3,2,3,1 },{ 31,10,2,2,1 },{ 39,7,2,1,1 },{ 39,4,2,1,1 },{ 96,7,2,2,1 },{ 99,5,2,2,1 },{ 102,3,2,12,1 },
		{ 0,10,4,15,2 },{ 38,10,4,10,2 },{ 50,10,4,1,2 },{ 53,10,4,1,2 },{ 56,10,4,1,2 },{ 61,10,4,10,2 },{ 75,10,4,3,2 },{ 82,10,4,11,2 },
		{ 67,9,8,1,1 },{ 68,8,8,1,2 },{ 69,7,8,1,3 },{ 70,6,8,1,4 },{ 75,6,8,1,4, },{ 76,7,8,1,3 },{ 77,8,8,1,2 },
		{ 42,8,11,1,1 },
		{ 111,1,12,1,1 },
		{ 4,8,14,1,1 },{ 9,8,14,1,1 },{ 83,8,14,1,1 },{ 89,8,14,1,1 },
		};
		int i = 0;
		while (m[i].id>0 && m[i].id<15 && i<MAP_NUMBER)//???存在问题
		{
			switch (m[i].id)
			{
			case 1:
			case 3:
			case 4:
			case 5:
			case 6:
				m[i].u = (V_MAX / T1) / G;
				break;
			case 2:
				m[i].u = (V_MAX / T2) / G;
				break;
			default:
				m[i].u = (V_MAX / T3) / G;
				break;
			}
			map[i] = m[i];
			i++;
		}
	}
	else if (world == 3)
	{
		//编辑地图
		Map m[] = { { 0,0,5,104,2 },{ 0,9,5,15,3 },{ 18,8,5,9,4 },{ 27,9,5,37,3 },{ 67,9,5,4,3 },{ 74,9,5,1,3 },{ 78,9,5,28,3 },
		{ 10,6,2,4,1 },{ 18,4,2,44,1 },{ 62,6,2,1,1 },{ 65,4,2,5,1 },
		{ 36,7,10,1,1 },{ 45,7,10,1,1 },{ 80,7,10,1,1 },{ 90,7,10,1,1 },
		{ 19,6,11,1,1 },{ 23,6,11,1,1 },{ 32,7,11,1,1 },{ 49,7,11,1,1 },{ 53,7,11,1,1 },{ 57,7,11,1,1 },{ 93,7,11,1,1 },
		{ 101,7,12,1,1 },
		{ 15,10,13,1,1 },{ 64,10,13,1,1 },{ 71,10,13,1,1 },{ 75,10,13,1,1 }
		};
		int i = 0;
		while (m[i].id>0 && m[i].id<15 && i<MAP_NUMBER)//???存在问题
		{
			switch (m[i].id)
			{
			case 1:
			case 3:
			case 4:
			case 5:
			case 6:
				m[i].u = (V_MAX / T1) / G;
				break;
			case 2:
				m[i].u = (V_MAX / T2) / G;
				break;
			default:
				m[i].u = (V_MAX / T3) / G;
				break;
			}
			map[i] = m[i];
			i++;
		}
	}
}
void scene::createCoin()
{
	if (world == 1)
	{	
		//改进：自动随机生成
		POINT p[] = { { 10,5 },{ 11,5 },{ 12,5 },{ 13,5 },{ 66,3 },{ 67,3 },{ 68,3 },{ 69,3 },{ 74,5 },{ 74,6 },{ 74,7 },{ 74,8 },
		{ 82,7 },{ 83,8 },{ 84,7 },{ 85,8 },{ 86,7 },{ 87,8 },{ 88,7 },{ 89,8 }
		};
		int i = 0;
		while (i <= sizeof(p) / sizeof(p[0]))
		{
			coins[i].x = p[i].x;
			coins[i].y = p[i].y;
			i++;
		}
	}
	else if (world == 2)
	{
		POINT p[] = { { 25,2 },{ 26,2 },{ 27,2 },{ 32,4 },{ 32,5 },{ 32,6 },{ 32,7 },{ 32,8 },{ 50,6 },{ 53,6 },{ 56,6 } };
		int i = 0;
		while (i <= sizeof(p) / sizeof(p[0]))
		{
			coins[i].x = p[i].x;
			coins[i].y = p[i].y;
			i++;
		}
	}
	else if (world == 3)
	{
		POINT p[] = { { 19,2 },{ 21,2 },{ 23,2 },{ 25,2 },{ 27,2 },{ 29,2 },{ 31,2 },{ 33,2 },{ 35,2 },{ 37,2 },{ 39,2 },{ 41,2 },{ 43,2 },{ 45,2 },{ 47,2 },{ 49,2 },{ 51,2 },{ 53,2 },{ 55,2 },{ 57,2 },{ 59,2 },
		{ 66,3 },{ 67,3 },{ 68,3 },{ 69,3 },{ 74,5 },{ 74,6 },{ 74,7 },{ 74,8 },
		{ 82,7 },{ 83,8 },{ 84,7 },{ 85,8 },{ 86,7 },{ 87,8 },{ 88,7 },{ 89,8 }
		};
		int i = 0;
		while (i <= sizeof(p) / sizeof(p[0]))
		{
			coins[i].x = p[i].x;
			coins[i].y = p[i].y;
			i++;
		}
	}
}
void scene::createFood()
{
	if (world == 1)
	{
		//目前只有发射子弹 因为蘑菇数量少 后续可以改进 长大 或其他功能
		POINT p[] = { { 41 * WIDTH,8 * HEIGHT + HEIGHT / 5 } };
		int i = 0;
		while (i <= sizeof(p) / sizeof(p[0]))
		{
			food[i].x = p[i].x;
			food[i].y = p[i].y;
			i++;
		}
	}
	else if (world == 2)
	{
		POINT p[] = { { 39 * WIDTH - WIDTH / 3,3 * HEIGHT + HEIGHT / 5 } };
		int i = 0;
		while (i <= sizeof(p) / sizeof(p[0]))
		{
			food[i].x = p[i].x;
			food[i].y = p[i].y;
			i++;
		}
	}
	else if (world == 3)
	{
		POINT p[] = { { 41 * WIDTH,5 * HEIGHT + HEIGHT / 5 } };
		int i = 0;
		while (i <= sizeof(p) / sizeof(p[0]))
		{
			food[i].x = p[i].x;
			food[i].y = p[i].y;
			i++;
		}
	}
}

void scene::setScorePos(int x, int y) //记住得分硬币位置 在下一帧绘制图片时就可以清空了
{
	for (int i = 0; i<SCORE_NUMBER; i++)
	{
		if (score[i].x == 0 && score[i].y == 0)
		{
			score[i].x = x;
			score[i].y = y;
			score_iframe[i] = 1;
			break;
		}
	}
}
void scene::setFood(int x, int y)	//为何存在两处设置food的位置
{
	for (int i = 0; i<FOOD_NUMBER; i++)
	{
		if (food[i].x == 0 && food[i].y == 0)
		{
			food[i].x = x;
			food[i].y = y;
			break;
		}
	}
}

void scene::action(role *myRole)	//背景的移动
{
	//由主角的坐标改变背景和地图的坐标，使主角有向前运动的感觉
	int xmap = xMap;
	xMap = (int)(myRole->getHero())->x0;//获取role里面的原点坐标;	//地图像摄像头一样跟随移动
	double xabs = (myRole->getHero())->vX;
	if ((myRole->getHero())->vX < 0)
		xabs = -xabs;
	double bgStep = xabs*TIME*UNREAL_HEIGHT / REAL_HEIGHT / K_MAP_BG;	//背景小幅度移动
	if ((myRole->getHero())->x == XRIGHT && (myRole->getHero())->vX>0 && xmap != xMap)//如果主角已经到达右边界限且正在向右运动
		xBg -= bgStep;//背景向后移动	//坐标参考系的问题	//所以此处是减
						//实际上角色一直在一个窗口里，如果需要实现角色向前移动，那么背景图片就要向后移动
}
void scene::show()
{
	//控制背景移动	讲之前移动确定好的点绘制出来	//所以此处是小于负
	if (xBg <= -img_bg.getwidth())
		xBg = 0;
	yBg = -(world - 1)*YSIZE;
	putimage((int)xBg, (int)yBg, &img_bg);
	putimage(img_bg.getwidth() + (int)xBg, (int)yBg, &img_bg);

	//控制地图移动	scenery和map里面的图片
	int i = 0;
	while (map[i].id>0 && i<MAP_NUMBER)//map[i]中有数据才参与运算
	{
		for (int j = 0; j<map[i].xAmount; j++)
			for (int k = 0; k<map[i].yAmount; k++)
			{
				int x = xMap + map[i].x*WIDTH + j*WIDTH;	//绘制在map中的坐标
				int y = yMap + map[i].y*HEIGHT + k*HEIGHT;
				int x0 = 0;									//绘制在图片中的坐标
				int y0 = (map[i].id - 1)*HEIGHT;
				if (map[i].id>9)	//特殊景色
				{
					if (map[i].id == 10)
						putimage(x, y, 2 * WIDTH, 2 * HEIGHT, &img_map, x0, y0);
					else
					{
						y0 = (map[i].id - 11) * 4 * HEIGHT;
						putimage(x, y, 3 * WIDTH, 2 * HEIGHT, &img_scenery, ((int)scenery_iframe - 1) * 3 * WIDTH, y0 + 2 * HEIGHT, SRCAND);
						putimage(x, y, 3 * WIDTH, 2 * HEIGHT, &img_scenery, ((int)scenery_iframe - 1) * 3 * WIDTH, y0, SRCPAINT);
						scenery_iframe += F;
						if (scenery_iframe >= 3)
							scenery_iframe = 1;	//摆动的树和流动的河流
					}
				}
				else
					putimage(x, y, WIDTH, HEIGHT, &img_map, x0, y0);
			}
		i++;
	}
	//控制硬币
	coin_iframe += TIME * 7;
	if ((int)coin_iframe == 5)		//用来控制闪烁动画
		coin_iframe = 1;
	int j = 0;
	while (j<COINS_NUMBER)
	{
		if (coins[j].x != 0)	//硬币的移动、绘制
		{
			int x = coins[j].x*WIDTH;
			int y = coins[j].y*HEIGHT;
			putimage(xMap + x, y, WIDTH, HEIGHT, &img_coin, ((int)coin_iframe - 1)*WIDTH, 9 * HEIGHT, SRCAND);
			putimage(xMap + x, y, WIDTH, HEIGHT, &img_coin, ((int)coin_iframe - 1)*WIDTH, 8 * HEIGHT, SRCPAINT);
		}
		j++;
	}
	//下面代码用于控制吃到硬币的效果	//特效
	for (int k = 0; k<SCORE_NUMBER; k++)
	{
		if (score[k].x != 0 && score[k].y != 0)
		{
			score_iframe[k] += TIME * 8;
			if ((int)score_iframe[k] == 5)
			{
				score_iframe[k] = 1;
				score[k].x = 0;
				score[k].y = 0;
			}
			else
			{
				int x = (int)xMap + score[k].x*WIDTH;
				int y = score[k].y*HEIGHT;
				putimage(x, y, WIDTH, HEIGHT, &img_coin, ((int)score_iframe[k] - 1)*WIDTH, 11 * HEIGHT, SRCAND);
				putimage(x, y, WIDTH, HEIGHT, &img_coin, ((int)score_iframe[k] - 1)*WIDTH, 10 * HEIGHT, SRCPAINT);
			}
		}
	}
	//控制食物
	food_iframe += TIME * 7;
	if ((int)food_iframe == 3)
		food_iframe = 1;
	int l = 0;	//L而不是1 下面的都是L而非1
	while (l<FOOD_NUMBER)
	{
		if (food[l].x != 0)
		{
			int x = food[l].x;
			int y = food[l].y;
			putimage(xMap + x, y, 3 * WIDTH / 2 + 4, HEIGHT * 4 / 5, &img_food, ((int)food_iframe - 1)*(3 * WIDTH / 2 + 4), 12 * HEIGHT + HEIGHT * 4 / 5, SRCAND);
			putimage(xMap + x, y, 3 * WIDTH / 2 + 4, HEIGHT * 4 / 5, &img_food, ((int)food_iframe - 1)*(3 * WIDTH / 2 + 4), 12 * HEIGHT, SRCPAINT);
		}
		l++;
	}
}