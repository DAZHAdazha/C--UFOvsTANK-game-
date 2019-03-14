#include"acllib.h"
#include<stdio.h>
#include<stdlib.h> 
#include<time.h>
#include<string.h> 
#include<math.h>
static int score=0;
static int shellCount=0; 
static int gameOver=0;
static int timeCount=0;
static int powerful=1;//前2.1秒无敌 
static int x=500,y=450;//你的初始坐标  
static int clearCount=1;//大招数量 
char buf[10];
int highScore=0;
char name[20];
char temp;
FILE *fp;
typedef struct node//运用链表来表示子弹 
{
	int X;
	int Y;
	struct node *next; 
} Node,*LinkList;
LinkList L;
void ListInsert(int x,int y) //增加子弹 
{
	LinkList p,s;
	p=L;
	s=(LinkList)malloc(sizeof(Node));
	s->X=x;
	s->Y=y;
	s->next=p->next;
	p->next=s;
}
void ListDelete()//检测子弹是否超出屏幕，是则删除 
{
	LinkList p;
	p=L->next;
	while(p&&p->next)
	{
		if(p->Y<=0)
		{
			LinkList q;
			q=p->next;
			p->next=q->next;
			free(q);
		}
		p=p->next;
	}
}
struct Enemy{
	int true;
	int X;
	int Y;
	int random;
	int count;
	int shelltrue;
	int shellX;
	int shellY;
	int deathTime; 
	int on;
};
struct Enemy enemy[8];
struct ITEM{
	int X;
	int Y;
	int true;
	int random;
	int count;
}item;
struct ITEM item={
	.true=1,
	.count=0,
};

void drawMe(void)
{
	beginPaint();
	clearDevice();//清屏 
	if(x+40>1000)//判断是否超出屏幕 
	{
		x=960;
	}
	if(y+120>600)
	{
		y=480;
	}
	if(x-40<0)
	{
		x=40;
	}
	if(y<0)
	{
		y=0;
	} 
	if(powerful)
	{
	setPenStyle(PEN_STYLE_DASHDOT);
	setBrushColor(WHITE);
	ellipse(x-80,y-20,x+80,y+140);
	}
	setPenStyle(PEN_STYLE_SOLID);
	line(x,y,x-5,y+20);//画 
	line(x,y,x+5,y+20);
	line(x-5,y+20,x+5,y+20);
	line(x-5,y+20,x-10,y+45);
	line(x+5,y+20,x+10,y+45);
	line(x-10,y+45,x-25,y+53);
	line(x+10,y+45,x+25,y+53);
	line(x-25,y+53,x-12,y+53);
	line(x+25,y+53,x+12,y+53);
	line(x-12,y+53,x-12,y+63);
	line(x+12,y+53,x+12,y+63);
	line(x-12,y+63,x-40,y+80);
	line(x+12,y+63,x+40,y+80);
	line(x-40,y+80,x-40,y+95);
	line(x+40,y+80,x+40,y+95);
	line(x-40,y+95,x-8,y+80);
	line(x+40,y+95,x+8,y+80);
	line(x-8,y+80,x-8,y+90);
	line(x+8,y+80,x+8,y+90);
	line(x-8,y+90,x-30,y+100);
	line(x+8,y+90,x+30,y+100);
	line(x-30,y+100,x-20,y+110);
	line(x+30,y+100,x+20,y+110);
	line(x-20,y+110,x+21,y+110);
	setBrushColor(WHITE);
	ellipse(x-3,y+25,x+3,y+45);
	line(x-7,y+50,x+7,y+50);
	line(x-7,y+50,x,y+62);
	line(x+7,y+50,x-1,y+62);
	line(x-3,y+50,x-3,y+57);
	line(x+3,y+50,x+3,y+57);
	line(x,y+65,x,y+85);
	roundrect(x-13,y+95,x-3,y+120,5,5);
	roundrect(x+13,y+95,x+3,y+120,5,5);
	setPenStyle(PEN_STYLE_DOT);
	line(x-15,y+125,x-25,y+135);
	line(x-7,y+125,x-13,y+135);
	line(x,y+125,x,y+135);
	line(x+15,y+125,x+25,y+135);
	line(x+7,y+125,x+13,y+135);
	endPaint();
}
void drawEnemy(void) 
{
	beginPaint();
	setBrushColor(WHITE);//改变颜色，因此敌人为空心 
	setPenStyle(PEN_STYLE_SOLID);
	int i=0;
	while(i<8)//8个敌人 
	{
		if(enemy[i].true==2)//若该敌人不存在则随机位置 
		{
			srand((enemy[i].X+enemy[i].Y+x+y+i)*time(NULL));
				enemy[i].true=1;
				enemy[i].X=rand()%1000;
				enemy[i].Y=rand()%450;
				enemy[i].random=rand()%4;
		}
			if(enemy[i].true==1)
			{
				arc(enemy[i].X-20,enemy[i].Y,enemy[i].X+90,enemy[i].Y+50,enemy[i].X-20,enemy[i].Y-4,enemy[i].X+90,enemy[i].Y-5);
				arc(enemy[i].X-3,enemy[i].Y-10,enemy[i].X+72,enemy[i].Y+30,enemy[i].X-3,enemy[i].Y+20,enemy[i].X+72,enemy[i].Y+20);
				arc(enemy[i].X,enemy[i].Y+40,enemy[i].X+70,enemy[i].Y-20,enemy[i].X+70,enemy[i].Y+20,enemy[i].X,enemy[i].Y+20);
				ellipse(enemy[i].X-5,enemy[i].Y+25,enemy[i].X+10,enemy[i].Y+40);
				ellipse(enemy[i].X+25,enemy[i].Y+32,enemy[i].X+40,enemy[i].Y+47);
				ellipse(enemy[i].X+57,enemy[i].Y+27,enemy[i].X+72,enemy[i].Y+42);
				if(enemy[i].count==10)//改变方向 
				{
				enemy[i].random=rand()%4;
				enemy[i].count=0;
				}
				if(enemy[i].X+90>1000)//判断是否超出屏幕 
				{
				enemy[i].X=910;
				}
				if(enemy[i].X-20<0)
				{
				enemy[i].X=20;
				}
				if(enemy[i].Y+60>600)
				{
				enemy[i].Y=540;
				}
				if(enemy[i].Y-25<0)
				{
				enemy[i].Y=25;
				}
				if(enemy[i].random==0)//移动 
				{
					enemy[i].X-=5;
				}
				else if(enemy[i].random==1)
				{
					enemy[i].X+=5;
				}
				else if(enemy[i].random==2)
				{
					enemy[i].Y-=5;
				}
				else if(enemy[i].random==3)
				{
					enemy[i].Y+=5;
				}
				enemy[i].count++;//计数器加一 
		}
		i++;
	}
		endPaint();
}
void shell(void)//画子弹 
{
	beginPaint();
	setPenStyle(PEN_STYLE_SOLID);
	setBrushColor(WHITE);
	LinkList p;
	p=L;
	while(p)
	{
		rectangle(p->X,p->Y,p->X+4,p->Y+10);
		p->Y-=10;
		p=p->next;
	}
	endPaint();
}
void enemyShell(void)
{
	beginPaint();
	setPenStyle(PEN_STYLE_SOLID);
	setBrushColor(BLACK);//改变面颜色 
	int i=0;
	while(i<8)
	{
		if(enemy[i].true==0)
		{
			i++;
			continue;
		}
		if(enemy[i].shelltrue==1&&enemy[i].shellY+10>600)//判断是否超出屏幕 
		{
		enemy[i].shelltrue=0;
		}
		if(enemy[i].shelltrue==0)
		{
			enemy[i].shellX=enemy[i].X+44;
			enemy[i].shellY=enemy[i].Y+80;
			enemy[i].shelltrue=1;
		}
		rectangle(enemy[i].shellX,enemy[i].shellY,enemy[i].shellX+4,enemy[i].shellY+10);
		enemy[i].shellY+=7;
		if(enemy[i].shellX<x)
		{
			enemy[i].shellX++;
		} 
		if(enemy[i].shellX>x)
		{
			enemy[i].shellX--;
		}
		i++;
	}
	endPaint();
}
void drawItem(void)
{
	beginPaint();
	srand((x+y)+time(NULL));
	if(item.true==0)
	{
		item.count++;
	}
	if(item.true==0&&item.count==200)
		{
			item.X=rand()%1000;
			item.Y=rand()%600;
			item.count=0;
			item.true=1;
		}
	if(item.true==1)
	{
		item.random=rand()%4;
		if(item.X-10<0)
		{
			item.X=10;
		}
		if(item.Y<0)
		{
			item.Y=0;
		}
		if(item.X+50>1000)
		{
			item.X=950;
		}
		if(item.Y+20>600)
		{
			item.Y=580;
		}
		line(item.X,item.Y,item.X+40,item.Y);
		line(item.X+40,item.Y,item.X+50,item.Y+10);
		line(item.X,item.Y,item.X-10,item.Y+10);
		line(item.X+50,item.Y+10,item.X+40,item.Y+20);
		line(item.X-10,item.Y+10,item.X,item.Y+20);
		line(item.X,item.Y+20,item.X+41,item.Y+20);
		line(item.X+25,item.Y+5,item.X+25,item.Y+2);
		line(item.X+25,item.Y+2,item.X+15,item.Y+2);
		line(item.X+15,item.Y+2,item.X+15,item.Y+10);
		line(item.X+15,item.Y+10,item.X+25,item.Y+10);
		line(item.X+25,item.Y+10,item.X+25,item.Y+18);
		line(item.X+25,item.Y+18,item.X+15,item.Y+18);
		line(item.X+15,item.Y+18,item.X+15,item.Y+15);
		if(item.random==0)//移动 
		{
			item.X-=5;
		}
		else if(item.random==1)
		{
			item.X+=5;
		}
		else if(item.random==2)
		{
			item.Y-=5;
		}
		else if(item.random==3)
		{
			item.Y+=5;
		}
	}	
	endPaint();
} 
void hit(void)
{
		int j=0;
		while(j<8)
		{
			LinkList p;
			p=L->next;
			while(p&&p->next&&enemy[j].true==1)
			{	
				if(p->X+2>enemy[j].X-20&&
				p->X+2<enemy[j].X+90&&
				p->Y+5<enemy[j].Y+60&&
				p->Y+5>enemy[j].Y-25)
				//判断自己子弹是否集中敌人 
				{
					enemy[j].true=0;
					LinkList q;
					q=p->next;
					p->next=q->next;
					free(q);
					score++;
				}
				p=p->next;
			}
			j++;
		}
	if(powerful==0)//判断敌人是否撞到自己 
	{
		int j=0;
		while(j<8)
		{
				if((enemy[j].X-20>=x-20&&enemy[j].X-20<=x+20&&enemy[j].Y-20>=y+20&&enemy[j].Y-20<=y+90)||
				(enemy[j].X-20>=x-20&&enemy[j].X-20<=x+20&&enemy[j].Y+47>=y+20&&enemy[j].Y+47<=y+90)||
				(enemy[j].X+90>=x-20&&enemy[j].X+90<=x+20&&enemy[j].Y-20>=y+20&&enemy[j].Y-20<=y+90)||
				(enemy[j].X+90>=x-20&&enemy[j].X+90<=x+20&&enemy[j].Y+47>=y+20&&enemy[j].Y+47<=y+90)||
				(x-20>=enemy[j].X-20&&x-20<=enemy[j].X+90&&y+20>=enemy[j].Y-20&&y+20<=enemy[j].Y+47)||
				(x-20>=enemy[j].X-20&&x-20<=enemy[j].X+90&&y+90>=enemy[j].Y-20&&y+90<=enemy[j].Y+47)||
				(x+20>=enemy[j].X-20&&x+20<=enemy[j].X+90&&y+20>=enemy[j].Y-20&&y+20<=enemy[j].Y+47)||
				(x+20>=enemy[j].X-20&&x+20<=enemy[j].X+90&&y+90>=enemy[j].Y-20&&y+90<=enemy[j].Y+47))
				{
					printf("you are dead.\n");
					printf("你阵亡了。\n");
					Sleep(500);
					printf("Your score is %d.\n",score);
					printf("你的得分是%d。\n",score);
					Sleep(500);
					if(score>highScore)
					{
						fopen("highScore.txt","w");
						printf("congraduation! you get the highest score,please enter your name!\n");
						printf("恭喜！你成为了最高分的得主，请输入你的名字来保存（请输入英文字母）。\n"); 
						scanf("%s",name);
						fprintf(fp,"%d\n%s",score,name);
						fclose(fp);
					}
					gameOver=1;
					break;
				}
			if(enemy[j].shellX+2>x-30&&
			enemy[j].shellX<x+30&&
			enemy[j].shellY+5>y+20&&
			enemy[j].shellY+5<y+70&&gameOver==0)//判断敌人子弹是否击中自己 
			{
				printf("you are dead.\n");
				printf("你阵亡了。\n");
				Sleep(500);
				printf("Your score is %d.\n",score);
				printf("你的得分是%d。\n",score);
				Sleep(500);
				if(score>highScore)
				{
					fopen("highScore.txt","w");
					printf("congraduation! you get the highest score,please enter your name!\n");
					printf("恭喜！你成为了最高分的得主，请输入你的名字来保存（请输入英文字母）。\n");
					scanf("%s",name);
					printf("it's saved,bye!\n");
					printf("储存成功，再见！\n"); 
					fprintf(fp,"%d\n%s",score,name);
					fclose(fp);
				}
				gameOver=1;
				break;
			}
			j++;
		} 
	}
	if(x+40>item.X+25&&
		x-40<item.X+25&&
		y<item.Y+10&&
		y+100>item.Y+10&&item.true==1) 
		{
			item.true=0;
			int i=0;
			while(i<8)
			{
				enemy[i].true=0;
				i++;	
			} 
			score+=8;
		} 
}
void dead(void)
{
	int i=0;
	while(i<8)
	{
		if(enemy[i].true==0&&enemy[i].deathTime==0)
		{
			enemy[i].deathTime=1;
		}
		if(enemy[i].on==1)
		{
			beginPaint();
			setBrushColor(WHITE);
			arc(enemy[i].X-20,enemy[i].Y,enemy[i].X+90,enemy[i].Y+50,enemy[i].X-20,enemy[i].Y-4,enemy[i].X+90,enemy[i].Y-5);
			arc(enemy[i].X-3,enemy[i].Y-10,enemy[i].X+72,enemy[i].Y+30,enemy[i].X-3,enemy[i].Y+20,enemy[i].X+72,enemy[i].Y+20);
			arc(enemy[i].X,enemy[i].Y+40,enemy[i].X+70,enemy[i].Y-20,enemy[i].X+70,enemy[i].Y+20,enemy[i].X,enemy[i].Y+20);
			ellipse(enemy[i].X-5,enemy[i].Y+25,enemy[i].X+10,enemy[i].Y+40);
			ellipse(enemy[i].X+25,enemy[i].Y+32,enemy[i].X+40,enemy[i].Y+47);
			ellipse(enemy[i].X+57,enemy[i].Y+27,enemy[i].X+72,enemy[i].Y+42);
			endPaint();
		}
		i++;
	}
}
void timerListener(int id)//重要的回调函数 
{ 
		if(powerful)//前2.1秒无敌 
		{
			if(timeCount<=70)
			{
			timeCount++;
			}
			else
			{
			powerful=0;
			}
		}
		drawMe();
		shell();
		drawEnemy();
		enemyShell();
		drawItem();
		ListDelete();
		hit();
		dead();
		int i=0;
		while(i<8)
		{
			if(enemy[i].deathTime>0&&enemy[i].deathTime<10)
			{
				enemy[i].deathTime++;
			}
			else if(enemy[i].deathTime>=10&&enemy[i].deathTime<=20)
			{
				enemy[i].deathTime++;
				enemy[i].on=1;
			}
			else if(enemy[i].deathTime>20&&enemy[i].deathTime<=30)
			{
				enemy[i].deathTime++;
				enemy[i].on=0;
			}
			else if(enemy[i].deathTime>30&&enemy[i].deathTime<=40)
			{
				enemy[i].deathTime++;
				enemy[i].on=1;
			}
			else if(enemy[i].deathTime>40)
			{
				enemy[i].on=0;
				enemy[i].deathTime=0;
				enemy[i].true=2;
			}
			i++;
		}
		if(gameOver) 
		{
		cancelTimer(0);
		}
}

void keylistener(int key,int event)
{
	//printf("%d %d\n",key,event);
	//key←37↑38→39↓40 event按下0 抬起1 space 32 shift 16 a 65 w 87 d 68 s 83
	switch(key)
	{
	case 37:
		x-=20;
		break;
	case 38:
		y-=20;
		break;
	case 39:
		x+=20;
		break;
	case 40: 
		y+=20;
		break;
	case 32:
		ListInsert(x,y);
		break;
	case 16:
		if(clearCount>0)
		{
			clearCount--;
			score+=8;
			int i=0;
			while(i<8)
			{
				enemy[i].true=0;
				i++;
			}
		}
		break; 
	case 65:
		x-=100;
		break;
	case 87:
		y-=100;
		break;
	case 68:
		x+=100;
		break;
	case 83:
		y+=100;
		break;
	default:
		break;
	} 
}
int Setup()//setUp函数只是程序初始化，它的结束才是程序的开始，进入消息循环 
{

	initWindow("test",800,0,1000,600);
	initConsole();
	L=(LinkList)malloc(sizeof(Node));
	L->next=NULL;
	int j=0;
	while(j<8)
	{
		enemy[j].deathTime=0;
		enemy[j].true=2;
		enemy[j].on=0;
		j++;
	}
	srand((x+y)+time(NULL));
	item.X=rand()%1000;
	item.Y=rand()%600;
	fp=fopen("highScore.txt","r");
	fgets(buf,10,fp);
	int len=strlen(buf)-1;
	int i=0;
	while(len>0)
	{
		highScore+=(buf[i]-'0')*pow(10,len-1);
		len--;
		i++;
	}
	fclose(fp);
	printf("slay the more enemy as you can, you will be dead if you hit the enemy and their bullets\n") ;
	printf("尽可能消灭多的敌人，如果被敌人子弹击中或者撞到敌人便会死亡。\n"); 
	Sleep(500);
	printf("use arrow up,down,letf,right to move slowly,space to fire.\n"); 
	printf("上下左右来移动，空格开火。\n");
	Sleep(500);
	printf("if you want to be more thrilled, you may use w a s d to control,but perhaps the combination is the best choice\n");
	printf("如果你想来点刺激的那就用wasd来控制，但我建议还是两者结合为妙。\n"); 
	Sleep(500);
	printf("and you luckily have once powerful skill, if you press shift you will kill all the enemies,or you may get the item to do it again\n");
	printf("幸运的是你会拥有一个清屏的大招，按shift就可以消灭所有敌人，吃到道具也会又相同效果。\n"); 
	registerKeyboardEvent(keylistener); 
	registerTimerEvent(timerListener);
	startTimer(0,30);
	setTextSize(20);
	return 0;
} 
