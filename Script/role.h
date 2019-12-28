#pragma once
#include"graphics.h"
#include"mydefine.h"

const int ENEMY_TOTE = 30;
const int BOMB_NUMBER = 5;
const int BULLET_NUMBER = 30;
const int BULLET_INTERVAL = WIDTH;
const int LEHGTH_INTERVAL_BULLET = 4;//数值越大，子弹的飞行速度越快 
const double TIME_INTERVAL_BULLET = 0.2;//数值越大，要求子弹发射的间隔时间越长

const int MAX_DISTANCE = 480; //子弹作用最大距离

class scene;//超前说明
			//因为map类中包括role类的数据成员，使用这种方式不至于死循环
			//只适用于用来定义类指针或者类应用 
			//不能来定义类对象变量或者函数的变量名

struct Hero	//主角 
{
	int x;//主角的当前坐标（在屏幕上的坐标）
	int y;
	double xx;//主角的坐标（double型）
	double yy;
	double x0;//原点坐标	//用来做顶点判断
	double vX;//速度
	double vY;
	int turn;//主角运动的方向	//1左-1右
	POINT direction;
	bool isFly;//主角是否在空中
	bool isShoot;//主角是否可以射击
	bool died;//主角是否死亡
	bool ending;//主角是否马上就要过关	//进入自动走的范围内
	bool passed;//主角是否过关
};
struct Enemy
{
	int x;
	int y;
	int turn;//敌人的运动方向
};
struct Bullet
{
	int x;
	int y;
	int turn;
};
#ifndef _MAP
#define _MAP
struct Map//地图的结构体
{
	int x;
	int y;
	int id;
	int xAmount;//横向地图块的个数
	int yAmount;//纵向地图块的个数
	double u;//摩擦因数
};
#endif

class role
{
private:
	Hero myHero;//定义一个主角
	scene *myScene;
	Enemy myEnemy[ENEMY_TOTE];//定义若干敌人
	POINT bombs[BOMB_NUMBER];//定义若干爆炸点
	Bullet bullets[BULLET_NUMBER];//定义若干子弹
	IMAGE img_hero;
	IMAGE img_enemy;
	IMAGE img_showBomb;
	IMAGE img_bullet;
	bool shootButtonDown;//用于记录射击键是否按下
	double shootTimeInterval;	//发射间隔
	int hero_iframe;//用于控制主角、敌人和爆炸的加载帧（加载第几副精灵图，这样就能让精灵看上去动起来了）**
	double enemy_iframe;
	double bomb_iframe[BOMB_NUMBER];
	double bullet_iframe[BULLET_NUMBER];	//根据iframe就是选择精灵用第几张图来做主图
	int score;
private:
	Map *touchMap(int x, int y, scene *myScene);//指针做参数后，效率明显提高
	POINT *touchCoins(int x, int y, scene *myScene);
	POINT *touchFood(int x, int y, scene *myScene);
	Enemy *touchEnemy(int x, int y, Enemy *emy);
	bool isTouch(POINT *p1, POINT *p2);
	void setBomb(int x, int y);
	void setBullet(int x, int y);
	void bullteFlying(Bullet *p, scene *myScene);
public:
	role(int world);
	~role(void);
	Hero *getHero() { return &myHero; }
	bool isDied() { return myHero.died; }
	bool isPassed() { return myHero.passed; }
	void createEnemy(int world);
	void show();
	void action(int KEY, scene *myScene);
	int getScore() { return score; }
};