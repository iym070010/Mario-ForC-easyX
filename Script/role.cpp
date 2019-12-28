#include "inertia.h"
#include"graphics.h"
#include"mydefine.h"
#include"math.h"
#include"inertia.h"
#include "role.h"
#pragma comment(lib,"Winmm.lib")   //windows API 音乐

//

role::role(int world)
{
	myHero.x = X0;	//初始化主角数据
	myHero.y = X0;
	myHero.xx = myHero.x;
	myHero.yy = myHero.y;
	myHero.x0 = 0;
	myHero.vX = 0;
	myHero.vY = 0;
	myHero.turn = 1;
	myHero.direction.x = 0;
	myHero.direction.y = 0;
	myHero.isFly = true;
	myHero.isShoot = false;
	myHero.died = false;
	myHero.ending = false;
	myHero.passed = false;
	myScene = 0;	//初始化参数
	hero_iframe = 1;
	enemy_iframe = 1;
	shootButtonDown = false;
	shootTimeInterval = 0;
	score = 0;
	loadimage(&img_hero, "res\\role.bmp");
	loadimage(&img_enemy, "res\\ani.bmp");
	loadimage(&img_showBomb, "res\\ani.bmp");
	loadimage(&img_bullet, "res\\ani.bmp");
	for (int i = 0; i<ENEMY_TOTE; i++)	//初始化敌人
	{
		myEnemy[i].x = 0;
		myEnemy[i].y = 0;
		myEnemy[i].turn = 0;
	}
	for (int j = 0; j<BOMB_NUMBER; j++)	//初始化子弹爆炸
	{
		bombs[j].x = 0;
		bombs[j].y = 0;
		bomb_iframe[j] = 0;
	}
	for (int k = 0; k<BULLET_NUMBER; k++)	//初始化子弹
	{
		bullets[k].x = 0;
		bullets[k].y = 0;
		bullets[k].turn = 0;
		bullet_iframe[k] = 0;
	}

	createEnemy(world);

	mciSendString("open res\\死亡1.mp3 alias music_died", NULL, 0, NULL);	//音乐导入
	mciSendString("open res\\跳.mp3 alias music_jump", NULL, 0, NULL);
	mciSendString("open res\\金币.mp3 alias music_coin", NULL, 0, NULL);
	mciSendString("open res\\踩敌人.mp3 alias music_tread", NULL, 0, NULL);
	mciSendString("open res\\吃到武器.mp3 alias music_getWeapon", NULL, 0, NULL);
	mciSendString("open res\\子弹.mp3 alias music_bullet", NULL, 0, NULL);
	mciSendString("open res\\子弹撞墙.mp3 alias music_boom", NULL, 0, NULL);
	mciSendString("open res\\子弹打到敌人.mp3 alias music_boom2", NULL, 0, NULL);
}

role::~role(void)
{

}

void role::createEnemy(int world)	//与scene中的createFood等一样
{
	if (world == 1)
	{
		Enemy emy[] = { { 3,8,1 },{ 18,7,-1 },{ 25,7,1 },{ 28,8,-1 },{ 33,8,-1 },{ 39,8,1 },{ 68,3,1 },{ 66,8,-1 },{ 81,6,1 },{ 92,6,1 } };
		int i = 0;
		while (i <= sizeof(emy) / sizeof(emy[0]))
		{
			myEnemy[i].x = emy[i].x*WIDTH;
			myEnemy[i].y = emy[i].y*HEIGHT;
			myEnemy[i].turn = emy[i].turn;
			i++;
		}
	}
	else if (world == 2)
	{
		Enemy emy[] = { { 18,8,1 },{ 24,5,1 },{ 28,5,-1 },{ 44,9,1 },{ 97,6,1 },{ 99,4,1 } };
		int i = 0;
		while (i <= sizeof(emy) / sizeof(emy[0]))
		{
			myEnemy[i].x = emy[i].x*WIDTH;
			myEnemy[i].y = emy[i].y*HEIGHT;
			myEnemy[i].turn = emy[i].turn;
			i++;
		}
	}
	else if (world == 3)
	{
		Enemy emy[] = { { 25,3,-1 },{ 31,3,1 },{ 39,3,-1 },{ 50,3,-1 },{ 68,3,1 },{ 66,8,-1 },{ 86,8,1 } };
		int i = 0;
		while (i <= sizeof(emy) / sizeof(emy[0]))
		{
			myEnemy[i].x = emy[i].x*WIDTH;
			myEnemy[i].y = emy[i].y*HEIGHT;
			myEnemy[i].turn = emy[i].turn;
			i++;
		}
	}
}

void role::setBomb(int x, int y)	//碰到墙体、最大距离、敌人时爆炸 传入xy
{
	for (int i = 0; i<BOMB_NUMBER; i++)
	{
		if (bombs[i].x == 0 && bombs[i].y == 0)
		{
			bombs[i].x = x;
			bombs[i].y = y;
			bomb_iframe[i] = 1;
			break;
		}
	}
}

void role::setBullet(int x, int y)
{
	for (int i = 0; i<BULLET_NUMBER; i++)
	{
		if (bullets[i].x == 0 && bullets[i].y == 0)
		{
			bullets[i].x = x;
			bullets[i].y = y;
			bullets[i].turn = myHero.turn;
			bullet_iframe[i] = 1;
			break;
		}
	}
}

void role::bullteFlying(Bullet *p, scene *myScene)	//子弹	//改良：最大距离
{
	if (p->x == 0 && p->y == 0)
		return;
	if (p->x>XSIZE || p->x<-WIDTH)
	{
		p->x = 0;
		p->y = 0;
		p->turn = 0;
		return;
	}
	Enemy *emy = touchEnemy(p->x, p->y, myEnemy);
	Map *map = touchMap(p->x, p->y, myScene);
	if (emy != NULL || map != NULL|| p->x>MAX_DISTANCE)//如果子弹打到敌人或者墙或者达到最大距离
	{
		if (emy != NULL)
		{
			mciSendString("play music_boom2 from 0", NULL, 0, NULL);
			score += 5;
		}
		else
			mciSendString("play music_boom from 0", NULL, 0, NULL);

		//记下爆炸点的坐标
		int x, y;
		if (emy != NULL)
		{
			x = emy->x;
			y = emy->y;
			emy->x = 0;
			emy->y = 0;
			emy->turn = 0;
			p->x = 0;
			p->y = 0;
			p->turn = 0;
		}
		else
		{
			x = (-(int)myHero.x0 + p->x) / WIDTH*WIDTH;
			if (p->turn == -1)
				x = (-(int)myHero.x0 + p->x + WIDTH) / WIDTH*WIDTH;
			y = p->y;
			p->x = 0;
			p->y = 0;
			p->turn = 0;

		}
		setBomb(x, y);
	}
	else
		p->x += LEHGTH_INTERVAL_BULLET*p->turn;	//子弹飞行
}

bool role::isTouch(POINT *p1, POINT *p2)
{
	for (int i = 0; i<4; i++)//如果主角4个顶点有一个顶点在地图内，则判断为接触到地图
	{
		if (p1[i].x >= p2[0].x&&p1[i].y >= p2[0].y&&p1[i].x <= p2[1].x&&p1[i].y <= p2[1].y)
		{
			return true;
		}
	}
	return false;
}

Map *role::touchMap(int x, int y, scene *myScene)	//碰撞
{
	//总体思路是判断主角的四个顶点是否至少有一个在地图内，有，则判断为主角碰撞到地图
	int xmap = (int)myHero.x0;	//图片坐标
	POINT r[4];//主角4个顶点的坐标（比主角小一圈）
	r[0].x = -xmap + x + 1;
	r[0].y = y + 1;
	r[1].x = -xmap + x + WIDTH - 1;
	r[1].y = y + 1;
	r[2].x = -xmap + x + 1;
	r[2].y = y + HEIGHT - 1;
	r[3].x = -xmap + x + WIDTH - 1;
	r[3].y = y + HEIGHT - 1;
	int i = 0;

	//设定碰撞框与碰撞
	while (myScene->getMap()[i].id>0 && myScene->getMap()[i].id<11 && i<MAP_NUMBER)//条件：地图存在且为前10种地图(地形)	//后面的id是景色
	{
		POINT m[2];//地图左上角顶点和右下角顶点的坐标

		m[0].x = myScene->getMap()[i].x*WIDTH;
		m[0].y = myScene->getMap()[i].y*HEIGHT;

		if (myScene->getMap()[i].id == 10)//id为10的地图要大些
		{
			m[1].x = myScene->getMap()[i].x*WIDTH + myScene->getMap()[i].xAmount * 2 * WIDTH;
			m[1].y = myScene->getMap()[i].y*HEIGHT + myScene->getMap()[i].yAmount * 2 * HEIGHT;
		}
		else
		{
			m[1].x = myScene->getMap()[i].x*WIDTH + myScene->getMap()[i].xAmount*WIDTH;
			m[1].y = myScene->getMap()[i].y*HEIGHT + myScene->getMap()[i].yAmount*HEIGHT;
		}
		if (isTouch(r, m))//如果两个矩形相交，则碰撞
			return &(myScene->getMap()[i]);
		i++;
	}
	return NULL;
}

POINT *role::touchCoins(int x, int y, scene *myScene)
{
	//与碰撞地形的思路是一样的
	//总体思路是判断主角的四个顶点是否至少有一个在硬币内，有，则判断为主角碰吃到硬币
	int xmap = (int)myHero.x0;
	POINT r[4];//主角4个顶点的坐标（比主角小一圈）
	r[0].x = -xmap + x + 1;
	r[0].y = y + 1;
	r[1].x = -xmap + x + WIDTH - 1;
	r[1].y = y + 1;
	r[2].x = -xmap + x + 1;
	r[2].y = y + HEIGHT - 1;
	r[3].x = -xmap + x + WIDTH - 1;
	r[3].y = y + HEIGHT - 1;
	int i = 0;
	while (i<COINS_NUMBER)	//条件：存在硬币
	{
		POINT m[2];//硬币左上角顶点和右下角顶点的坐标

		m[0].x = myScene->getCoins()[i].x*WIDTH;
		m[0].y = myScene->getCoins()[i].y*HEIGHT;
		m[1].x = m[0].x + WIDTH;
		m[1].y = m[0].y + HEIGHT;
		if (isTouch(r, m))//如果两个矩形相交，则碰撞
			return &(myScene->getCoins()[i]);
		i++;
	}
	return NULL;
}

POINT *role::touchFood(int x, int y, scene *myScene)
{
	//总体思路是判断主角的四个顶点是否至少有一个在食物内，有，则判断为主角碰吃到食物
	int xmap = (int)myHero.x0;
	POINT r[4];//主角4个顶点的坐标（比主角小一圈）
	r[0].x = -xmap + x + 1;
	r[0].y = y + 1;
	r[1].x = -xmap + x + WIDTH - 1;
	r[1].y = y + 1;
	r[2].x = -xmap + x + 1;
	r[2].y = y + HEIGHT - 1;
	r[3].x = -xmap + x + WIDTH - 1;
	r[3].y = y + HEIGHT - 1;
	int i = 0;
	while (i<FOOD_NUMBER)	//条件：存在食物
	{
		POINT m[2];//食物左上角顶点和右下角顶点的坐标

		m[0].x = myScene->getFood()[i].x;
		m[0].y = myScene->getFood()[i].y;
		m[1].x = m[0].x + 3 * WIDTH / 2 + 4;
		m[1].y = m[0].y + 4 * HEIGHT / 5;
		if (isTouch(r, m))//如果两个矩形相交，则碰撞
			return &(myScene->getFood()[i]);
		i++;
	}
	return NULL;
}

Enemy *role::touchEnemy(int x, int y, Enemy *emy)
{
	//与之前碰撞思路一样
	//总体思路是判断主角的四个顶点是否至少有一个在地图内，有，则判断为主角碰撞到敌人
	int xmap = (int)myHero.x0;
	POINT r[4];//主角4个顶点的坐标（比主角小一圈）
	r[0].x = -xmap + x + 1;
	r[0].y = y + 1;
	r[1].x = -xmap + x + WIDTH - 1;
	r[1].y = y + 1;
	r[2].x = -xmap + x + 1;
	r[2].y = y + HEIGHT - 1;
	r[3].x = -xmap + x + WIDTH - 1;
	r[3].y = y + HEIGHT - 1;
	int i = 0;
	while (i<ENEMY_TOTE)//条件：存在敌人
	{
		if (emy[i].turn != 0)
		{
			POINT m[2];//敌人左上角顶点和右下角顶点的坐标

			m[0].x = emy[i].x;
			m[0].y = emy[i].y;
			m[1].x = m[0].x + WIDTH;
			m[1].y = m[0].y + HEIGHT;
			if (isTouch(r, m))//如果两个矩形相交，则碰撞
			{
				return &emy[i];
			}
		}
		i++;
	}
	return NULL;
}

void role::show()	//可以封装更多函数为了方便
{
	//下面代码用于控制主角的步伐，使主角给人的感觉更像在走路
	//改变主角加载图片的第几帧
	int rolePos = -(int)myHero.x0 + myHero.x;//主角的位置
	if (rolePos / STEP % 2 == 0 && rolePos / STEP % 4 != 0)
		hero_iframe = 2;
	else if (rolePos / STEP % 4 == 0)
		hero_iframe = 1;
	//控制主角的方向
	if (myHero.direction.x == 1)
		myHero.turn = 1;
	else if (myHero.direction.x == -1)
		myHero.turn = -1;
	if (myHero.died == true)	//绘制死亡图像
	{
		putimage(myHero.x, myHero.y, WIDTH, HEIGHT, &img_hero, 2 * WIDTH, HEIGHT, SRCAND);
		putimage(myHero.x, myHero.y, WIDTH, HEIGHT, &img_hero, 2 * WIDTH, 0, SRCPAINT);
	}
	else
	{	//绘制走路图像
		if (myHero.turn == 1)
		{
			putimage(myHero.x, myHero.y, WIDTH, HEIGHT, &img_hero, (hero_iframe - 1)*WIDTH, HEIGHT, SRCAND);
			putimage(myHero.x, myHero.y, WIDTH, HEIGHT, &img_hero, (hero_iframe - 1)*WIDTH, 0, SRCPAINT);

		}
		else
		{
			putimage(myHero.x, myHero.y, WIDTH, HEIGHT, &img_hero, (hero_iframe - 1)*WIDTH + 3 * WIDTH, HEIGHT, SRCAND);
			putimage(myHero.x, myHero.y, WIDTH, HEIGHT, &img_hero, (hero_iframe - 1)*WIDTH + 3 * WIDTH, 0, SRCPAINT);
		}
	}
	//下面代码用于控制敌人的步伐，使敌人给人的感觉更像在走路
	//改变敌人加载图片的第几帧
	enemy_iframe += TIME * 5;
	if ((int)enemy_iframe == 3)
		enemy_iframe = 1;
	int i = 0;
	while (i<ENEMY_TOTE)
	{
		if (myEnemy[i].turn != 0)
		{
			putimage((int)myHero.x0 + myEnemy[i].x, myEnemy[i].y, WIDTH, HEIGHT, &img_enemy, ((int)enemy_iframe - 1)*WIDTH, HEIGHT, SRCAND);
			putimage((int)myHero.x0 + myEnemy[i].x, myEnemy[i].y, WIDTH, HEIGHT, &img_enemy, ((int)enemy_iframe - 1)*WIDTH, 0, SRCPAINT);
		}
		i++;
	}
	//下面代码用于控制爆炸效果
	for (int j = 0; j<BOMB_NUMBER; j++)
	{
		if (bombs[j].x != 0 && bombs[j].y != 0)
		{
			bomb_iframe[j] += TIME * 10;
			if ((int)bomb_iframe[j] == 5)
			{
				bomb_iframe[j] = 1;
				bombs[j].x = 0;
				bombs[j].y = 0;
			}
			else
			{
				putimage((int)myHero.x0 + bombs[j].x - WIDTH / 2, bombs[j].y - HEIGHT / 2, 2 * WIDTH, 2 * HEIGHT, &img_showBomb, ((int)bomb_iframe[j] - 1) * 2 * WIDTH, 6 * HEIGHT, SRCAND);
				putimage((int)myHero.x0 + bombs[j].x - WIDTH / 2, bombs[j].y - HEIGHT / 2, 2 * WIDTH, 2 * HEIGHT, &img_showBomb, ((int)bomb_iframe[j] - 1) * 2 * WIDTH, 4 * HEIGHT, SRCPAINT);
			}
		}
	}
	//下面代码用于控制子弹的显示
	for (int k = 0; k<BULLET_NUMBER; k++)
	{
		if (bullets[k].x != 0 && bullets[k].y != 0)
		{
			bullet_iframe[k] += TIME * 10;
			if ((int)bullet_iframe[k] == 3)
			{
				bullet_iframe[k] = 1;
			}
			else
			{
				putimage(bullets[k].x, bullets[k].y, WIDTH, HEIGHT, &img_bullet, ((int)bullet_iframe[k] - 1)*WIDTH, 3 * HEIGHT, SRCAND);
				putimage(bullets[k].x, bullets[k].y, WIDTH, HEIGHT, &img_bullet, ((int)bullet_iframe[k] - 1)*WIDTH, 2 * HEIGHT, SRCPAINT);
			}
			bullteFlying(&bullets[k], myScene);
		}
	}
}

void role::action(int KEY, scene *myScene)
{
	this->myScene = myScene;
	myHero.direction.x = 0;//=0表示主角不面向任意个方向
	myHero.direction.y = 0;
	double a = 0;//主角运动的加速度和摩擦力给予的加速度
	double a1 = 0;
	Map *map = NULL;
	if ((KEY&CMD_UP) && myHero.isFly == false && myHero.ending == false)//只有当主角没有在空中且主角没过关的时候允许按上升键
	{
		mciSendString("play music_jump from 0", NULL, 0, NULL);
		myHero.isFly = true;//表示主角在空中
		myHero.vY = -sqrt(2 * G*REAL_HEIGHT);	//y轴方向上的速度
	}
	if (myHero.isFly == true)
	{
		myHero.yy = myHero.yy - (-inertia::move(myHero.vY, TIME, G)*UNREAL_HEIGHT / REAL_HEIGHT);//move函数返回单位时间（T）内，主角上升或下降的高度
		myHero.y = (int)myHero.yy;
		map = touchMap(myHero.x, myHero.y + 1, myScene);
		if (map != NULL)
		{
			if (myHero.vY>0)//表示当主角接触地面时
				myHero.isFly = false;
			myHero.vY = 0;
			myHero.y = (myHero.y + HEIGHT / 2) / HEIGHT*HEIGHT;//巧用了“+HEIGHT/2”，表示主角的中心
			myHero.yy = myHero.y;
			myHero.direction.y += 1;
		}
		if (myHero.vY>0)
		{
			Enemy *emy = touchEnemy(myHero.x, myHero.y, myEnemy);
			if (emy != NULL)//如果主角踩到敌人 
			{
				mciSendString("play music_tread from 0", NULL, 0, NULL);
				score += 5;
				//记下爆炸点的坐标
				setBomb(emy->x, emy->y);
				emy->x = 0;
				emy->y = 0;
				emy->turn = 0;
			}
			if (myHero.y>YSIZE)
			{
				myHero.died = true;
				mciSendString("play music_died from 0", NULL, 0, NULL);
			}
		}
	}
	else
	{
		map = touchMap(myHero.x, myHero.y + 1, myScene);
		if (map == NULL)//如果主角没有碰到地图，及在空中
		{
			myHero.isFly = true;
		}
	}
	if (KEY&CMD_DOWN)	//按下时并不需要改变属性
	{
	}
	if (KEY&CMD_LEFT&&myHero.ending == false)
	{
		a -= A_ROLE;
		myHero.direction.x -= 1;
	}
	if (KEY&CMD_RIGHT || myHero.ending == true)
	{
		a += A_ROLE;
		myHero.direction.x += 1;
	}
	//以下代码处理主角水平方向上的运动			//+3-3调节惯性 摩擦力
	int k = 0;
	double Xabs = myHero.vX;
	if (myHero.vX < 0)
	{
		Xabs = -Xabs;
		k = Xabs / myHero.vX + 3;
	}
	else
		k = Xabs / myHero.vX - 3;
	if (myHero.vX*a <= 0 && myHero.vX != 0)
	{
		if (map == NULL)
			a1 = 0;
		else
			a1 = k*G*map->u;//由物理公式：a=mg*u/g得出
	}
	double tmp = myHero.vX;
	double H = inertia::move(myHero.vX, TIME, a + a1)*UNREAL_HEIGHT / REAL_HEIGHT;
	if (tmp*myHero.vX<0)
		myHero.vX = 0;
	myHero.xx = myHero.xx + H;
	myHero.x = (int)myHero.xx;

	if (myScene->isEnding(-(int)myHero.x0 + myHero.x))//如果主角马上过关
	{
		myHero.ending = true;
	}
	if (myHero.x>XSIZE)	//已经过关
		myHero.passed = true;
	if (touchMap(myHero.x, myHero.y, myScene) != NULL)	//碰到边界
	{
		if (myHero.x>XRIGHT)
			myHero.x = XRIGHT;
		else
			myHero.x = (int)(myHero.xx - H);
		myHero.xx = myHero.x;
		myHero.vX = 0;
		a = 0;
		a1 = 0;
	}
	if (Xabs>V_MAX)	//最大速度
		myHero.vX = myHero.vX / Xabs*V_MAX;
	if (KEY&CMD_ESC)	//并不需要改变属性
	{
	}
	if (KEY&CMD_SHOOT&&myHero.isShoot == true)
	{
		//x为子弹的横坐标
		int x = myHero.x + WIDTH / 2;
		if (myHero.turn == -1)
			x = myHero.x - WIDTH;

		//以下代码控制子弹的运动
		//当一直按住射击键时，子弹在一定时间内才能发射；否者可以随时发射子弹
		if (shootButtonDown == false)
		{
			mciSendString("play music_bullet from 0", NULL, 0, NULL);
			setBullet(x, myHero.y);
		}
		else
		{
			if (shootTimeInterval == 0)
			{
				mciSendString("play music_bullet from 0", NULL, 0, NULL);
				setBullet(x, myHero.y);
			}
			shootTimeInterval += TIME;
			if (shootTimeInterval>TIME_INTERVAL_BULLET)
				shootTimeInterval = 0;
		}
		shootButtonDown = true;
	}
	else
	{
		shootButtonDown = false;
		shootTimeInterval = 0.01;
	}
	//判断出主角是否超出左右界限
	if (myHero.x<XLEFT)
	{
		myHero.x = XLEFT;
		myHero.xx = myHero.x;
		myHero.vX = 0;
		a = 0;
	}
	else if (myHero.x>XRIGHT&&myHero.ending == false)
	{
		myHero.x0 -= (myHero.x - XRIGHT);
		myHero.x = XRIGHT;
		myHero.xx = myHero.x;
	}
	//以下程序控制敌人的运动
	int i = 0;
	if ((int)(enemy_iframe * 100) % 2 == 0)//控制敌人运动的刷新频率
	{
		while (i<ENEMY_TOTE)
		{
			if (myEnemy[i].turn != 0)
			{
				myEnemy[i].x += myEnemy[i].turn*ENEMY_STEP;
				int x1 = (int)myHero.x0 + myEnemy[i].x;
				int y1 = myEnemy[i].y;
				int x2 = (int)myHero.x0 + myEnemy[i].x + myEnemy[i].turn*WIDTH;
				int y2 = myEnemy[i].y + 1;
				if (touchMap(x1, y1, myScene) != NULL || touchMap(x2, y2, myScene) == NULL)//如果敌人碰到墙壁或者没有踩在陆地上，则敌人向相反方向运动
					myEnemy[i].turn *= -1;
			}
			i++;
		}
	}
	//以下代码判断主角是否吃到金币，若吃到则把金币的坐标记下
	POINT *p = touchCoins(myHero.x, myHero.y, myScene);
	if (p != NULL)
	{
		mciSendString("play music_coin from 0", NULL, 0, NULL);
		score += 10;
		myScene->setScorePos(p->x, p->y);
		p->x = 0;
		p->y = 0;
	}
	//以下代码判断主角是否吃到食物，若吃到则把食物的坐标记下
	POINT *q = touchFood(myHero.x, myHero.y, myScene);
	if (q != NULL)
	{
		mciSendString("play music_getWeapon from 0", NULL, 0, NULL);
		myScene->setScorePos(q->x, q->y);
		q->x = 0;
		q->y = 0;
		myHero.isShoot = true;
	}
	//以下代码判断主角是否撞到敌人
	Enemy *emy = touchEnemy(myHero.x, myHero.y, myEnemy);
	if (emy != NULL&&myHero.vY <= 0)//如果主角碰到但不踩到敌人
	{
		myHero.died = true;
		mciSendString("play music_died from 0", NULL, 0, NULL);
	}
}