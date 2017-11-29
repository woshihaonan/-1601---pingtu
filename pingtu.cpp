#include<stdio.h>

#include<graphics.h>

#include<conio.h>

#include<time.h>


#define Width  800

#define Height 600


/*����ͼƬ�ṹ��*/
typedef struct{
	IMAGE image;
	int position;									//ͼƬƴ��ʱ������
	int x,y;										//ͼƬԭʼ����
}Picture;

/*���ӽṹ��*/
typedef struct{
	Picture	picture;
	int x,y;
	int isFilled;		//�жϸ����Ƿ���ͼƬ
}Grid;


Picture picture[12];
int grid_width=100;						//���ӿ��
int boardstart_x=0,boardstart_y=400;					//������ʼ����
int filled_x=100,filled_y=50;						//������������ʼ����
Grid boardgrid[2][6];						//�������
int row=3,column=4;						//����������	
Grid fillgrid[3][4];							//��������
int isOver=0;							//�ж���Ϸ�Ƿ����
Picture temp;							//������ʱͼƬ					

long time1,assist=0;						//ʱ�估��������										

void start_up();										//��ʼ�� ��ͼƬ����ֲ�����������
void loadPicture();										//����ͼƬ
void drawBoard();										//���ƻ�������
void drawFilled();										//���ƴ��������
void updateWithInput();									//���û��йصĸ���
void updateWithoutInput();								//���û��޹صĸ���
int isWin();											//�ж�ͼƬ�Ƿ�ƴ��
void gameOver();										//��Ϸ����������

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
			sprintf(s,"ͼƬ/ԭͼ_0%d.gif",i+1);
		else
			sprintf(s,"ͼƬ/ԭͼ_%d.gif",i+1);
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
	loadimage(&tempImage,"ͼƬ/ԭͼ.jpg");
	putimage(580,150,&tempImage);


	temp.x=-1,temp.y=-1;										//��ʱͼƬ��ʼ��ֵ
	getimage(&temp.image,0,50,100,100);

	srand(time(NULL));
	int temp[]={0,1,2,3,4,5,6,7,8,9,10,11};
	
	int i,j;
	for(i=0;i<12;i++){
		int number=rand()%12;
		int temp1=temp[number];
		temp[number]=temp[i];
		temp[i]=temp1;
	}											//�����������
	
	for(i=0;i<2;i++){
		for(j=0;j<6;j++){
			int index=temp[i*6+j];
			picture[index].x=boardstart_x+j*grid_width;
			picture[index].y=boardstart_y+i*grid_width;
			
			boardgrid[i][j].picture=picture[index];					//�������ͼƬ
			boardgrid[i][j].isFilled=1;						//���ӱ����

			putimage(boardstart_x+j*grid_width,boardstart_y+i*grid_width,
				&picture[index].image);					//�����ʾͼƬ
		}
	}

	for(i=0;i<row;i++){
		for(j=0;j<column;j++)
			fillgrid[i][j].isFilled=0;					//��ʼ������䷽��
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
	int filledEnd_y=filled_y+row*grid_width;					//���������߽��յ�����

	if(MouseHit()){
		MOUSEMSG m=GetMouseMsg();

		if(m.y>boardstart_y&&m.x<6*grid_width&&m.uMsg==WM_LBUTTONDOWN){
			int board_x=m.x/grid_width;
			int board_y=m.y/grid_width-4;
			if(boardgrid[board_y][board_x].isFilled){				//�жϻ������Ƿ���ͼƬ

				if(temp.x>=0&&temp.y>=0){
					int y=temp.y/grid_width-4;
					int x=temp.x/grid_width;
					putimage(temp.x,temp.y,&temp.image);
					boardgrid[y][x].isFilled=1;		//ԭ�����ڵķ������(�޸�bug)
				}
				
				temp=boardgrid[board_y][board_x].picture;		//����ͼƬ������ʱͼƬ

			boardgrid[board_y][board_x].isFilled=0;
			clearrectangle(boardstart_x+board_x*grid_width,boardstart_y+board_y*grid_width,
			boardstart_x+(board_x+1)*grid_width,boardstart_y+(board_y+1)*grid_width);	//���ԭ������
			
				/*���ϱ߿�*/
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
				boardgrid[y][x].isFilled=1;		//ԭ�����ڵķ������
			}					//������ӱ���� ԭͼ�ص�ԭλ��
			
			putimage(filled_x+fill_x*grid_width,filled_y+fill_y*grid_width,&temp.image);	//���ͼƬ
			fillgrid[fill_y][fill_x].picture=temp;									//����ͼƬ

			fillgrid[fill_y][fill_x].isFilled=1;
			
			temp.x=-1;
			temp.y=-1;
			getimage(&temp.image,0,50,100,100);				//��ֹ���ͬһ��ͼ

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
				if(fillgrid[i][j].picture.position!=i*column+j)		//�ж�λ���Ƿ����Ӧ
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
	settextstyle(20,0,_T("����"));
	char str[20];
	sprintf(str,"����ʱ %3d��",time1);
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
	settextstyle(50,0,_T("����"));

	if(isWin()){
	
		outtextxy(x,y,"��ϲ��ͨ��!");
	}
	else{
		outtextxy(x,y,"Game Over");
	}

	getch();
	closegraph();
}