#pragma once

#ifndef MYDEFINE
#define MYDIFINE

#define XSIZE 512//屏幕大小
#define YSIZE 384
#define WIDTH 32//素材图片的尺寸
#define HEIGHT 32
#define X0 2*WIDTH//主角的起始坐标
#define Y0 3*HEIGHT
#define TIME 0.01//Sleep(Time*1000) 
#define STEP 10//主角走一步相距的像素个数									    //Test
#define ENEMY_STEP 1//敌人走一步相距的像素个数									//Test
#define K_MAP_BG 5//地图每移动5个像素点背景就移动1个像素点。及map/bg=5			//
#define XLEFT 0//主角水平运动的左界限											//
#define XRIGHT WIDTH*6//主角水平运动的右界限									//
#define REAL_HEIGHT 3.5//主角跳跃时，给人的感觉像跳跃了现实世界中的REAL_HEIGHT米//
#define UNREAL_HEIGHT (3*HEIGHT+5)//主角跳跃的最大高度（y轴的像素个数）
#define G 30.0//重力加速度（加速度大有利于使主角跳跃时间缩短）
#define V_MAX 8.0//主角前进的最大速度（和人类的最大速度差不多）
#define A_ROLE 20.0//主角前进的加速度
#define T1 0.5//主角加速到最大速度所用的时间
#define T2 1.2
#define T3 1.5
#define F TIME*0.3;//控制景物刷新的频率
#define LIFE 5;//主角有LIFE条生命

//按键的宏定义	方便判断语句的使用
#define	CMD_LEFT 1
#define	CMD_RIGHT 2	
#define	CMD_UP 4
#define CMD_DOWN 8
#define CMD_SHOOT 16
#define CMD_ESC 32
//虚拟按键的宏定义	方便判断语句的使用
#define VIR_RETURN 64
#define VIR_RESTART 128
#define VIR_HOME 256

#endif
