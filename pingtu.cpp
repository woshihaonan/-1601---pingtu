#include<stdio.h>

#include<graphics.h>

#include<conio.h>

#include<time.h>


#define Width  800

#define Height 600


/*构造图片结构体*/
typedef struct{
	IMAGE image;
	int position;									//图片拼好时的坐标
	int x,y;										//图片原始坐标
}Picture;

/*格子结构体*/
typedef struct{
	Picture	picture;
	int x,y;
	int isFilled;		//判断格子是否有图片
}Grid;


Picture picture[12];
int grid_width=100;						//格子宽度
int boardstart_x=0,boardstart_y=400;					//画板起始坐标
int filled_x=100,filled_y=50;						//待填充的区域起始坐标
Grid boardgrid[2][6];						//画板格子
int row=3,column=4;						//行数和列数	
Grid fillgrid[3][4];							//待填充格子
int isOver=0;							//判断游戏是否结束
Picture temp;							//保存临时图片					

long time1,assist=0;						//时间及辅助变量										

void start_up();										//初始化 将图片随机分布到画板区域
void loadPicture();										//加载图片
void drawBoard();										//绘制画板区域
void drawFilled();										//绘制待填充区域
void updateWithInput();									//与用户有关的更新
void updateWithoutInput();								//与用户无关的更新
int isWin();											//判断图片是否拼好
void gameOver();										//游戏结束处理函数

int main(){
	initgraph(Width,Height);
	
	start_up();
	
	while(!isOver){
		updateWithInput();
		updateWithoutInput();
	}
	
	gameOver();
	return 0;
}
void loadPicture(){
	for(int i=0;i<12;i++){
		char s[20];
		if(i<9)
			sprintf(s,"图片/原图_0%d.gif",i+1);
		else
			sprintf(s,"图片/原图_%d.gif",i+1);
		loadimage(&picture[i].image,s);
		picture[i].position=i;
	}
}
void drawBoard(){
	setlinecolor(WHITE);
	int boardstart_x=0,boardstart_y=400;
	int i,j;
	for(i=0;i<2;i++){
		for(j=0;j<6;j++){
			rectangle(boardstart_x+j*grid_width,boardstart_y+i*grid_width,
				boardstart_x+(j+1)*grid_width,boardstart_y+(i+1)*grid_width);
		}
		
	}
}
void start_up(){
	loadPicture();
	drawBoard();
	drawFilled();

	time1=120;

	IMAGE tempImage;
	loadimage(&tempImage,"图片/原图.jpg");
	putimage(580,150,&tempImage);


	temp.x=-1,temp.y=-1;										//临时图片初始赋值
	getimage(&temp.image,0,50,100,100);

	srand(time(NULL));
	int temp[]={0,1,2,3,4,5,6,7,8,9,10,11};
	
	int i,j;
	for(i=0;i<12;i++){
		int number=rand()%12;
		int temp1=temp[number];
		temp[number]=temp[i];
		temp[i]=temp1;
	}											//随机打乱数组
	
	for(i=0;i<2;i++){
		for(j=0;j<6;j++){
			int index=temp[i*6+j];
			picture[index].x=boardstart_x+j*grid_width;
			picture[index].y=boardstart_y+i*grid_width;
			
			boardgrid[i][j].picture=picture[index];					//保存格子图片
			boardgrid[i][j].isFilled=1;						//格子被填充

			putimage(boardstart_x+j*grid_width,boardstart_y+i*grid_width,
				&picture[index].image);					//随机显示图片
		}
	}

	for(i=0;i<row;i++){
		for(j=0;j<column;j++)
			fillgrid[i][j].isFilled=0;					//初始化待填充方格
	}
}
void drawFilled(){
	
	setlinecolor(WHITE);
	int i,j;
	for(i=0;i<row;i++){
		for(j=0;j<column;j++){
			rectangle(filled_x+j*grid_width,filled_y+i*grid_width,
						filled_x+(j+1)*grid_width,filled_y+(i+1)*grid_width);
		}
	}
}
void updateWithInput(){
	
	int filledEnd_x=filled_x+column*grid_width;
	int filledEnd_y=filled_y+row*grid_width;					//待填充区域边界终点坐标

	if(MouseHit()){
		MOUSEMSG m=GetMouseMsg();

		if(m.y>boardstart_y&&m.x<6*grid_width&&m.uMsg==WM_LBUTTONDOWN){
			int board_x=m.x/grid_width;
			int board_y=m.y/grid_width-4;
			if(boardgrid[board_y][board_x].isFilled){				//判断画板里是否有图片

				if(temp.x>=0&&temp.y>=0){
					int y=temp.y/grid_width-4;
					int x=temp.x/grid_width;
					putimage(temp.x,temp.y,&temp.image);
					boardgrid[y][x].isFilled=1;		//原画板内的方格被填充(修复bug)
				}
				
				temp=boardgrid[board_y][board_x].picture;		//格子图片赋给临时图片

			boardgrid[board_y][board_x].isFilled=0;
			clearrectangle(boardstart_x+board_x*grid_width,boardstart_y+board_y*grid_width,
			boardstart_x+(board_x+1)*grid_width,boardstart_y+(board_y+1)*grid_width);	//清空原来区域
			
				/*补上边框*/
			rectangle(boardstart_x+board_x*grid_width,boardstart_y+board_y*grid_width,
			boardstart_x+(board_x+1)*grid_width,boardstart_y+(board_y+1)*grid_width);	
			}	
		}
		if(m.x>filled_x&&m.x<filledEnd_x&&m.y>filled_y&&m.y<filledEnd_y
			&&m.uMsg==WM_LBUTTONDOWN){
			int fill_x=(m.x-filled_x)/grid_width;
			int fill_y=(m.y-filled_y)/grid_width;

			if(fillgrid[fill_y][fill_x].isFilled){
				Picture p=fillgrid[fill_y][fill_x].picture;		
				putimage(p.x,p.y,&p.image);
				int y=(p.y-boardstart_y)/grid_width;
				int x=p.x/grid_width;
				boardgrid[y][x].isFilled=1;		//原画板内的方格被填充
			}					//如果格子被填充 原图回到原位置
			
			putimage(filled_x+fill_x*grid_width,filled_y+fill_y*grid_width,&temp.image);	//填充图片
			fillgrid[fill_y][fill_x].picture=temp;									//保存图片

			fillgrid[fill_y][fill_x].isFilled=1;
			
			temp.x=-1;
			temp.y=-1;
			getimage(&temp.image,0,50,100,100);				//防止填充同一张图

			drawFilled();
			
		}
	}
}
int isWin(){
	for(int i=0;i<row;i++){
		for(int j=0;j<column;j++){
			if(!fillgrid[i][j].isFilled)
				return 0;
			if(fillgrid[i][j].isFilled){
				if(fillgrid[i][j].picture.position!=i*column+j)		//判断位置是否相对应
				return 0;
			}
		
		}
	}
	return 1;
}
void updateWithoutInput(){
	if(isWin())
		isOver=1;

	settextcolor(YELLOW);
	settextstyle(20,0,_T("黑体"));
	char str[20];
	sprintf(str,"倒计时 %3d秒",time1);
	outtextxy(650,300,str);
	
	if(time1<=0)
		isOver=1;
	
	assist++;
	Sleep(5);

	if(assist%200==0)
		time1--;


}
void gameOver(){
	clearrectangle(boardstart_x,boardstart_y,Width,Height);

	int x=boardstart_x+200;
	int y=boardstart_y+30;
	
	settextcolor(RED);
	settextstyle(50,0,_T("黑体"));

	if(isWin()){
	
		outtextxy(x,y,"恭喜你通关!");
	}
	else{
		outtextxy(x,y,"Game Over");
	}

	getch();
	closegraph();
}