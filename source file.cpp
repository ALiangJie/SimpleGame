#include <graphics.h>//easyxͼ���
#include <conio.h>
#include <windows.h>
#include <time.h>
#include<stdio.h>
#include <stdlib.h>
#pragma comment(lib,"winmm.lib")//������Ƶ

#define  WIDTH 512//�궨�廭���ߴ��
#define  HIGH 256//�궨�廭���ߴ��
#define R_H 94//���������
#define R_W 68//���������
int T = 2000;//ͼƬ��ʾ��ʱ

//�����ϰ���ṹ��
struct barrier
{
	int id;//�����ϰ�������,�����ϰ���bar��Ϊ0�������ϰ��1�������2����ʳ�˻���3����������
	int begin_i;
	int pass_i;
	int end_i;
	int img_num;
	int i;
};

//����ȫ�ֱ���
IMAGE bk;	    // ���廭��
IMAGE img_bg;	// ���屳��
IMAGE img_run;  //�����ܶ�ͼ
IMAGE img_run1; //�ܶ�����ͼ
IMAGE img_jump; //������Ծͼ
IMAGE img_jump1;//��Ծ����ͼ
IMAGE img_slide;//�����»�ͼ
IMAGE img_slide1;//�»�����ͼ
IMAGE img_trap;//����ͼ
IMAGE img_trap1;//��������ͼ
IMAGE img_dowmPlant;//ʳ�˻�ͼ
IMAGE img_dowmPlant1;//ʳ�˻�����ͼ
IMAGE img_upPlant;//������ͼ
IMAGE img_upPlant1;//����������ͼ
IMAGE img_trapDeath; //�����ش�����ͼ
IMAGE img_trapDeath1;//�����ش���������ͼ
IMAGE img_plantDeath;//������������ͼ
IMAGE img_plantDeath1;//����������������ͼ
IMAGE img_deathbk;//������ ��Ϸ��������
int img_bg_i;//����ͼƬ�ָ���
int M_X;//����xλ��
int M_Y;//����yλ��
int begin_i, end_i, pass_i;//begin_iΪ������ͼƬ�ϵĳ�ʼ���أ�end_iΪ��ͼƬ�ϵĵ�ǰ���أ�pass_iΪͨ��ͼƬ����
int img_num1;//��ʾ��img_num1���ܶ�ͼƬ
int img_num2;//��ʾ��img_num2����ԾͼƬ
int img_num3;//��ʾ��img_num3���»�ͼƬ
int move;//moveΪ0�����ܣ�1��������2����
barrier barr;//�ϰ���
int gameStatus; // ��Ϸ״̬��0Ϊ��ʼ�˵����棬1Ϊ������Ϸ��2Ϊ������Ϸ����
int judge; //�����ж� 1Ϊ����2Ϊ����
int score; // �÷�
int scoreNum;//�жϱ���ѭ��������ѭ��һ��Ϊ2048

// ����͸���Ȼ�ͼ
void drawAlpha(IMAGE* dstimg, int x, int y, IMAGE* srcimg);

//������
void change(int a, int b, IMAGE flower);

//�˵�������
void startMenu();

//��ʾ�÷����
void showScore();

//���ݳ�ʼ��
void startup();

//��Ϸ�������³�ʼ��
void restartup();

// ��ʱ����
void delay(unsigned int xms);

//��
void runImg(int x, int y);

//��
void jumpImg(int w, int h, int i, int j);

//��
void slideImg(int X, int Y, int w, int h, int i, int j);

//�ϰ���
void barrierImg(int id, int x, int i, int j, int w, int h);

//����������
void trapDeathImg(int y, int i, int j, int w, int h);

//��ֲ������
void plantDeathImg(int i, int j, int w, int h);

//������ͼƬ��ʾ
void run(int i);

//������ͼƬ��ʾ
void jump(int i);

//���ﻬͼƬ��ʾ
void slide(int i);

//�ϰ���ͼƬ��ʾ
void isBarrier(int i);

//����������ͼƬ��ʾ
void trapDeath();

//��ֲ������ͼƬ��ʾ
void plantDeath();

//��ʾ������Ϸ��������
void showdeathbk();

//�ж�����
void die();

//��ʾ����
void show();

//���û��޹ص�����
void updateWithoutInput();

//���û��йص�����
void updateWithInput();

//��Ϸ����
void gameover();

//������
void main()
{
	startup();//��Ϸ��ʼ��
	while (1)
	{
		show();//��ʾ����
		updateWithoutInput();//���û������޹�
		updateWithInput();//���û������й�
	}
	gameover();//��Ϸ����
}

//���û��йص�����
void updateWithInput()
{
	char ch;
	if (kbhit())
	{
		ch = getch();
		if (ch == 'w')
		{
			mciSendString("close smusic", NULL, 0, NULL);
			mciSendString("open res\\1159.wav alias smusic", NULL, 0, NULL);//��������
			mciSendString("play smusic ", NULL, 0, NULL);//������һ��
			move = 1;
			begin_i = img_bg_i;//��ȡ����ʱ��img_bg_i
		}

		if (ch == 's')
		{
			mciSendString("close smusic", NULL, 0, NULL);
			mciSendString("open res\\8.wav alias smusic", NULL, 0, NULL);//�򿪻�����
			mciSendString("play smusic ", NULL, 0, NULL);//������һ��
			move = 2;
			begin_i = img_bg_i;//��ȡ����ʱ��img_bg_i
		}
	}
}

//���û��޹ص�����
void updateWithoutInput()
{
	die();//��������
	if (img_bg_i < 2048)//����ѭ��һ�Σ����ؾ���3200
		score = scoreNum * 2048 + img_bg_i;
}

// ����͸���Ȼ�ͼ
void drawAlpha(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
{
	if (dstimg == NULL)
	{
		return;
	}
	// ������ʼ��
	DWORD *dst = GetImageBuffer(dstimg);
	DWORD *src = GetImageBuffer(srcimg);
	int src_width = srcimg->getwidth();
	int src_height = srcimg->getheight();
	int dst_width = dstimg->getwidth();
	int dst_height = dstimg->getheight();

	// ʵ��͸����ͼ  ���Ż�
	for (int iy = 0; iy < src_height; iy++)
	{
		for (int ix = 0; ix < src_width; ix++)
		{
			int srcX = ix + iy * src_width;
			int sa = ((src[srcX] & 0xff000000) >> 24);
			int sr = ((src[srcX] & 0xff0000) >> 16);
			int sg = ((src[srcX] & 0xff00) >> 8);
			int sb = src[srcX] & 0xff;
			if (x + ix >= 0 && x + ix < dst_width
				&& y + iy >= 0 && y + iy < dst_height)
			{
				int dstX = (x + ix) + (y + iy) * dst_width;
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				dst[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)
					| (sb * sa / 255 + db * (255 - sa) / 255);
			}
		}
	}
}

//������
void change(int a, int b, IMAGE flower)
{
	drawAlpha(&bk, a, b, &flower);
}

//�˵�������
void startMenu()
{
	initgraph(480, 320);//������С
	MOUSEMSG m;
	IMAGE main1, main2, main3, main4, main5, main6, main7, main8, main9, main10, main11,main12;
	loadimage(&main1, "res\\main2.png");// �˵�����
	loadimage(&main2, "res\\button_more02.png");//�˵���more
	loadimage(&main3, "res\\button_difficuty02.png");//�˵���difficul
	loadimage(&main4, "res\\button_play02.png");///�˵���play
	loadimage(&main5, "res\\button_back01.png");//�˵���back
	loadimage(&main6, "res\\morebackgound.png");//more����
	loadimage(&main7, "res\\diffultybackgoud.png");//difficult����
	loadimage(&main8, "res\\gameover_stage1.png");//�Ѷ�ѡ��1
	loadimage(&main9, "res\\gameover_stage2.png");//�Ѷ�ѡ��2
	loadimage(&main10, "res\\gameover_stage3.png");//�Ѷ�ѡ��3
	loadimage(&main11, "res\\gameover_stage4.png");//�Ѷ�ѡ��4
	loadimage(&main12, "res\\diffcultbackgound.png");//difficult����
									  //��Ϸ��ʼ��ӭ���� ,�������� 
	//��ʼҳ����
	int i;
	BeginBatchDraw();//��ͷ������ͼƬ
	for (i = 1; i <= 9; i++)
	{
		char tmp[128];//����һ���ַ�����
		IMAGE bk;//����ͼ��
		sprintf_s(tmp, "res\\%d.png", i + 1);//sprintf����������͵��ַ���������ͼƬ������1,2,3....
		loadimage(&bk, tmp);//����ͼƬ��Դ
		putimage(0, 0, &bk);//������ͼƬ
		mciSendString("open res\\WE080.wav alias kaitoumusic", NULL, 0, NULL);//�򿪿�ͷ����
		mciSendString("play kaitoumusic ", NULL, 0, NULL);//������һ��
		FlushBatchDraw();//ˢ�»���
		Sleep(70);//����ͼƬ�����ٶ�
	}
	Sleep(2000);
	//���ڷ�����Ծ 
loop1:
	{
		putimage(0, 0, &main1);
		while (true)
		{
			m = GetMouseMsg();
			//ѡ��diffulet
			if (m.x > 287 && m.x < 408 && m.y>211 && m.y < 243)
			{
				change(287, 211, main3);
				FlushBatchDraw();
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					//mciSendString("play turn from 0",NULL,0,NULL);
				loop2:
					{
						putimage(0, 0, &main12);
						FlushBatchDraw();
						m = GetMouseMsg();
						if (m.x > 352 && m.x < 473 && m.y>262 && m.y < 294)
						{
						loop7:
							{
								change(352, 262, main5);
								FlushBatchDraw();
								if (m.uMsg == WM_LBUTTONDOWN) 
								{
									mciSendString("play turn from 0", NULL, 0, NULL); 
									goto loop1;
								}
								m = GetMouseMsg();
								if (m.x > 352 && m.x < 473 && m.y>262 && m.y < 294)
								{
									goto loop7;
								}
								else goto loop2;
							}
						}
						else goto loop2;
					}
				}
			}
			//ѡ��more
			if (m.x > 287 && m.x < 408 && m.y>150 && m.y < 182)
			{
				change(287, 150, main2);
				FlushBatchDraw();
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					mciSendString("play turn from 0", NULL, 0, NULL);
				loop3:
					{
						putimage(0, 0, &main6);
						FlushBatchDraw();
						m = GetMouseMsg();
						if (m.x > 352 && m.x < 473 && m.y>262 && m.y < 294)
						{
						loop6:
							{
								change(352, 262, main5);
								FlushBatchDraw();
								if (m.uMsg == WM_LBUTTONDOWN)
								{
									mciSendString("play turn from 0", NULL, 0, NULL);
									goto loop1;
								}
								m = GetMouseMsg();
								if (m.x > 352 && m.x < 473 && m.y>262 && m.y < 294)
								{
									goto loop6;
								}
								else goto loop3;
							}
						}
						else goto loop3;
					}
				}
			}
			//ѡ��play 
			if (m.x > 287 && m.x < 408 && m.y>90 && m.y < 122)//��Ϸ��ʼ��λ��
			{
				change(287, 90, main4);//��ʼ��Ϸ���仯
				FlushBatchDraw();
				if (m.uMsg == WM_LBUTTONDOWN)//���
				{
					mciSendString("play turn from 0", NULL, 0, NULL);
				loop4:
					{
						putimage(0, 0, &main7);//��Ϸ�Ѷ�ѡ�����
						FlushBatchDraw();
						m = GetMouseMsg();
						if (m.x > 352 && m.x < 473 && m.y>262 && m.y < 294)//���ؼ�λ��
						{
						loop8:
							{
								change(352, 262, main5);//back���仯
								FlushBatchDraw();
								m = GetMouseMsg();
								if (m.uMsg == WM_LBUTTONDOWN)//���
								{
									mciSendString("play turn from 0", NULL, 0, NULL);
									goto loop1;//����������
								}
								if (m.x > 352 && m.x < 473 && m.y>262 && m.y < 294)//���ؼ�λ��
								{
									goto loop8;//back���仯
								}
								else goto loop4;//����������Ϸ�Ѷ�ѡ�����
							}
						}
						//ѡ��1������ 1
						if (m.x > 270 && m.x < 421 && m.y>49 && m.y < 72)//�Ѷ�һ��λ��
						{
						loop9:
							{
								change(250, 29, main8);//�Ѷ�һ���仯
								FlushBatchDraw();
								m = GetMouseMsg();
								if (m.uMsg == WM_LBUTTONDOWN)//���
								{
									mciSendString("stop interfacemusic", NULL, 0, NULL);//������
									mciSendString("close interfacemusic", NULL, 0, NULL);
									closegraph();//�رյ�ǰͼ��
									T = 2000;
									initgraph(WIDTH, HIGH);
									gameStatus = 1;
									return;
								}
								if (m.x > 270 && m.x < 421 && m.y>49 && m.y < 72)//��굽���λ�ý�����ת
									goto loop9;
								else goto loop4;//������ת���Ѷ�ѡ�����
							}
						}
						//ѡ��2ģʽ������  2
						if (m.x > 270 && m.x < 421 && m.y>106 && m.y < 129)
						{
						loop10:
							{
								change(250, 86, main9);
								FlushBatchDraw();
								m = GetMouseMsg();
								if (m.uMsg == WM_LBUTTONDOWN) {
									mciSendString("stop interfacemusic", NULL, 0, NULL);
									mciSendString("close interfacemusic", NULL, 0, NULL);
									closegraph();
									T = 1500;
									initgraph(WIDTH, HIGH);
									gameStatus = 1;
									return;
								}
								if (m.x > 270 && m.x < 421 && m.y>106 && m.y < 129)
									goto loop10;
								else goto loop4;
							}
						}
						//ѡ��3ģʽ������3
						if (m.x > 270 && m.x < 421 && m.y>165 && m.y < 188)
						{
						loop11:
							{
								change(150, 145, main10);
								FlushBatchDraw();
								m = GetMouseMsg();
								if (m.uMsg == WM_LBUTTONDOWN) {
									mciSendString("stop interfacemusic", NULL, 0, NULL);
									mciSendString("close interfacemusic", NULL, 0, NULL);
									T = 500;
									initgraph(WIDTH, HIGH);
									gameStatus = 1;
									return;
								}
								if (m.x > 270 && m.x < 421 && m.y>165 && m.y < 188)
									goto loop11;
								else goto loop4;
							}
						}
						//ѡ��4ģʽ������4
						if (m.x > 270 && m.x < 421 && m.y>222 && m.y < 245)
						{
						loop12:
							{
								change(250, 202, main11);
								FlushBatchDraw();
								m = GetMouseMsg();
								if (m.uMsg == WM_LBUTTONDOWN) {
									mciSendString("stop interfacemusic", NULL, 0, NULL);
									mciSendString("close interfacemusic", NULL, 0, NULL);
									closegraph();
									T = 100;
									initgraph(WIDTH, HIGH);
									gameStatus = 1;
									return;
								}
								if (m.x > 270 && m.x < 421 && m.y>222 && m.y < 245)
									goto loop12;
								else goto loop4;
							}

						}
						if ((m.x > 352 && m.x < 473 && m.y>262 && m.y < 294 || m.x > 270 && m.x < 421 && m.y>49 && m.y < 72 && m.uMsg == WM_LBUTTONDOWN || m.x > 270 && m.x < 421 && m.y>106 && m.y < 129 && m.uMsg == WM_LBUTTONDOWN || m.x > 270 && m.x < 421 && m.y>165 && m.y < 188 && m.uMsg == WM_LBUTTONDOWN || m.x > 270 && m.x < 421 && m.y>222 && m.y < 245 && m.uMsg == WM_LBUTTONDOWN) == 0)
							goto loop4;//��Щ����λ����ת���Ѷ�ѡ��˵�
					}
				}
			}
			//ѡ���˳���Ϸ
			if (m.x > 287 && m.x < 408 && m.y>271 && m.y < 303)//�˳���Ϸ��λ��
			{
			loop5: {
				change(287, 271, main5);//�˳���Ϸ�����仯
				FlushBatchDraw();
				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONDOWN)//���
				{
					mciSendString("stop interfacemusic", NULL, 0, NULL);
					mciSendString("close interfacemusic", NULL, 0, NULL);
					closegraph();
					exit(0);
				}
				if (m.x > 287 && m.x < 408 && m.y>271 && m.y < 303)
					goto loop5;
				else goto loop1;//��ѡ���˳���������������
				}
			}
			if ((m.x > 287 && m.x < 408 && m.y>90 && m.y < 122 || m.x > 287 && m.x < 408 && m.y>150 && m.y < 182 || m.x > 287 && m.x < 408 && m.y>211 && m.y < 243 || m.x > 287 && m.x < 408 && m.y>271 && m.y < 303) == 0)
			{
				putimage(0, 0, &main1);//������ͼƬ
				FlushBatchDraw();
			}
		}
		EndBatchDraw();
	}
}

//��ʾ�÷����
void showScore()
{
	settextcolor(RED);//������ɫ
	settextstyle(30, 0, _T("����"));//��������
	outtextxy(WIDTH*0.4, HIGH*0.5, _T("�÷֣�"));//��ʾλ��
	TCHAR s[500];
	_stprintf(s, _T("%d"), score / 10);
	outtextxy(WIDTH*0.6, HIGH*0.5, s);
}

//���ݳ�ʼ��
void startup()
{
	initgraph(WIDTH, HIGH);
	BeginBatchDraw();//��ͷ����
	loadimage(&img_bg, _T("res\\back1.png"));//����ͼƬ
	loadimage(&img_run, _T("res\\run.png"));//�ܲ�ͼƬ
	loadimage(&img_run1, _T("res\\run1.png"));
	loadimage(&img_jump, _T("res\\jump.png"));//��ͼƬ
	loadimage(&img_jump1, _T("res\\jump1.png"));
	loadimage(&img_slide, _T("res\\slide.png"));//��ͼƬ
	loadimage(&img_slide1, _T("res\\slide1.png"));
	loadimage(&img_trap, _T("res\\fire.png"));//����
	loadimage(&img_trap1, _T("res\\fire1.png"));
	loadimage(&img_dowmPlant, _T("res\\plantGreen.png"));//ʳ�˻�
	loadimage(&img_dowmPlant1, _T("res\\plantGreen1.png"));
	loadimage(&img_upPlant, _T("res\\ivyAnim1.jpg"));//������
	loadimage(&img_upPlant1, _T("res\\ivyAnim1_1.jpg"));
	loadimage(&img_trapDeath, _T("res\\deathTrapRunInto.png"));//����������
	loadimage(&img_trapDeath1, _T("res\\deathTrapRunInto1.png"));
	loadimage(&img_plantDeath, _T("res\\plantDeath.png"));//��ֲ������
	loadimage(&img_plantDeath1, _T("res\\plantDeath1.png"));
	loadimage(&img_deathbk, _T("res\\death.jpg"));//��������
	mciSendString("open res\\0441.mp3 alias bkmusic", NULL, 0, NULL);//�򿪱�������
	mciSendString("play bkmusic repeat", NULL, 0, NULL);//ѭ������
	img_bg_i = 0;//��ʼ��Ϊ0
	M_X = 100;//���ﵱǰxλ��
	M_Y = 110;//���ﵱǰyλ��
	begin_i = 0;//��ʼ����
	barr.begin_i = 512;//�ϰ���
	img_num1 = 0;//��ʼ���� ʾ��0���ܶ�ͼƬ
	img_num2 = 0;//��ʼ����ʾ��0����ԾͼƬ
	img_num3 = 0;//��ʼ����ʾ��0���»�ͼƬ
	barr.img_num = 0;//��ʼ����ʾ��0���ϰ���ͼƬ
	score = 0;//�÷�
	gameStatus = 0;//��ʼ�˵�����
	scoreNum = 0;
	srand((unsigned)time(NULL));//��ʱ��
	BeginBatchDraw();//��ͷ����
	while (gameStatus == 0)
		startMenu(); // ��ʼ�˵�����
}

//��Ϸ�������³�ʼ��
void restartup()
{
	img_bg_i = 0;//��ʼ��Ϊ0
	M_X = 100;
	M_Y = 110;//�������ﵱǰλ��
	begin_i = 0;
	barr.begin_i = 512;
	img_num1 = 0;//��ʼ����ʾ��0���ܶ�ͼƬ
	img_num2 = 0;//��ʼ����ʾ��0����ԾͼƬ
	img_num3 = 0;//��ʼ����ʾ��0���»�ͼƬ
	barr.img_num = 0;//��ʼ����ʾ��0���ϰ���ͼƬ
	score = 0;//�÷�Ϊ0
	gameStatus = 0;//��ʼ�ڲ˵�����
	scoreNum = 0;//����ѭ��������ʼΪ0
}

// ��ʱ����
void delay(unsigned int xms) // xms������Ҫ��ʱ�ĺ�����
{
	unsigned int x, y;
	for (x = xms; x > 0; x--)
		for (y = 1000; y > 0; y--);
}

//��
void runImg(int x, int y)
{
	putimage(M_X, M_Y, R_W, R_H, &img_run1, x, y, NOTSRCERASE);
	putimage(M_X, M_Y, R_W, R_H, &img_run, x, y, SRCINVERT);
	FlushBatchDraw();
}

//��
void jumpImg(int w, int h, int i, int j)
{
	putimage(M_X, M_Y, w, h, &img_jump1, i, j, NOTSRCERASE);
	putimage(M_X, M_Y, w, h, &img_jump, i, j, SRCINVERT);
	FlushBatchDraw();
}

//��
void slideImg(int X, int Y, int w, int h, int i, int j)
{
	putimage(X, Y, w, h, &img_slide1, i, j, NOTSRCERASE);
	putimage(X, Y, w, h, &img_slide, i, j, SRCINVERT);
	FlushBatchDraw();
}

//�ϰ���
void barrierImg(int id, int x, int i, int j, int w, int h)//ͼƬ�е�x��y��λ�ã�ͼƬ�߶�
{
	if (id == 1)//����
	{
		putimage(x, 145, w, h, &img_trap1, i, j, NOTSRCERASE);//ͼƬ�߶ȣ�ͼƬ��x��y��λ��
		putimage(x, 145, w, h, &img_trap, i, j, SRCINVERT);
	}
	else if (id == 2)//ʳ�˻�
	{
		putimage(x, 110, w, h, &img_dowmPlant1, i, j, NOTSRCERASE);
		putimage(x, 110, w, h, &img_dowmPlant, i, j, SRCINVERT);
	}
	else if (id == 3)//������
	{
		putimage(x, -165, w, h, &img_upPlant1, i, j, NOTSRCERASE);
		putimage(x, -165, w, h, &img_upPlant, i, j, SRCINVERT);
	}
}

//����������
void trapDeathImg(int y, int i, int j, int w, int h)//����λ�ã�������ͼƬ��x��yλ�ã�������x�߶�y��
{
	putimage(M_X, y - y + 110, w, h, &img_trapDeath1, i, j, NOTSRCERASE);
	putimage(M_X, y - y + 110, w, h, &img_trapDeath, i, j, SRCINVERT);
}

//��ֲ������
void plantDeathImg(int i, int j, int w, int h)//������ͼƬ��x��yλ�ã�������x�߶�y
{
	putimage(M_X + 20, 110, w, h, &img_plantDeath1, i, j, NOTSRCERASE);
	putimage(M_X + 20, 110, w, h, &img_plantDeath, i, j, SRCINVERT);
}

//������ͼƬ��ʾ
void run(int i)
{
	end_i = i;//��ǰ����
	pass_i = end_i - begin_i;//��ǰ����-��ʼ����
	if (pass_i == 8)
	{
		img_num1++;//��ʾ��һ���ܶ�ͼƬ������ͼƬ
		begin_i = end_i;
	}
	if (img_num1 == 9)//������9ʱ����ʾ��0��
		img_num1 = 0;

	switch (img_num1)
	{
	case 0:runImg(R_W * 1, 0); break;//���ﶯ����ͼƬ�е�x��yλ��
	case 1:runImg(R_W * 2, 0); break;
	case 2:runImg(R_W * 3, 0); break;
	case 3:runImg(R_W * 4, 0); break;
	case 4:runImg(R_W * 5, 0); break;
	case 5:runImg(R_W * 6, 0); break;
	case 6:runImg(R_W * 7, 0); break;
	case 7:runImg(R_W * 8, 0); break;
	case 8:runImg(R_W * 9, 0); break;
	case 9:runImg(R_W * 10,0); break;
	}
}

//������ͼƬ��ʾ
void jump(int i)
{
	end_i = i;
	pass_i = end_i - begin_i;
	if (pass_i == 20)
	{
		img_num2++;							//��ʾ��һ����ԾͼƬ
		if (img_num2 < 8 && img_num2>0)		//������ʾ��ԾͼƬ��λ��
			M_Y -= 20;
		else if (img_num2 > 8 && img_num2 < 16)
			M_Y += 20;

		begin_i = end_i;
	}

	switch (img_num2)
	{
	case 0:jumpImg(68, 94, 68, 0); break;//����Ŀ�͸ߣ�������ͼƬ�е�x,yλ��
	case 1:jumpImg(68, 94, 136, 0); break;
	case 2:jumpImg(68, 94, 204, 0); break;
	case 3:jumpImg(68, 94, 272, 0); break;
	case 4:jumpImg(68, 94, 340, 0); break;
	case 5:jumpImg(68, 94, 408, 0); break;
	case 6:jumpImg(68, 94, 408, 0); break;
	case 7:jumpImg(68, 94, 340, 0); break;
	case 8:jumpImg(68, 94, 272, 0); break;
	case 9:jumpImg(68, 94, 204, 0); break;
	case 10:jumpImg(68, 94,136, 0); break;
	case 11:jumpImg(68, 94, 68, 0); break;
	case 12:jumpImg(68, 94, 68, 0); break;
	case 13:jumpImg(68, 94, 68, 0); break;
	case 14:jumpImg(68, 94, 68, 0); break;
	case 15:jumpImg(68, 94, 68, 0); break;
	}
}

//���ﻬͼƬ��ʾ
void slide(int i)
{
	end_i = i;
	pass_i = end_i - begin_i;
	if (pass_i == 20)
	{
		img_num3++;//��ʾ��һ���»�ͼƬ
		begin_i = end_i;
	}

	switch (img_num3)
	{

	case 0:slideImg(M_X, 110, 68, 94, 0, 0); break;
	case 1:slideImg(M_X, 110, 45, 94, 68, 0); break;
	case 2:slideImg(M_X, 125, 80, 94, 112, 0); break;
	case 3:slideImg(M_X, 125, 80, 94, 192, 0); break;
	case 4:slideImg(M_X, 125, 80, 94, 272, 0); break;
	case 5:slideImg(M_X, 125, 80, 94, 112, 0); break;
	case 6:slideImg(M_X, 125, 80, 94, 192, 0); break;
	case 7:slideImg(M_X, 125, 80, 94, 272, 0); break;
	case 8:slideImg(M_X, 125, 80, 94, 192, 0); break;
	case 9:slideImg(M_X, 125, 80, 94, 272, 0); break;
	case 10:slideImg(M_X, 110, 68, 94, 0, 0); break;
	case 11:slideImg(M_X, 110, 45, 94, 68, 0); break;

	}
}

//�ϰ���ͼƬ��ʾ
void isBarrier(int i)
{
	int imgCount;//��ͬ�ϰ���ͼƬ������

	if (i % 512 == 0)
	{
		barr.id = rand() % 3 + 1;//�������������ϰ���
		barr.img_num = 0;
	}

	if (barr.id == 1)//����
		imgCount = 7;//7��
	else if (barr.id == 2)//ʳ�˻�
		imgCount = 6;//6��
	else if (barr.id == 3)//������
		imgCount = 6;//6��

	barr.end_i = i;
	barr.pass_i = barr.end_i - barr.begin_i;
	if (barr.pass_i == -20)
	{
		barr.img_num++;//��ʾ��һ��ͼƬ
		barr.begin_i = barr.end_i;
	}
	if (barr.img_num == imgCount)//ѭ����ʾ
		barr.img_num = 0;

	if (barr.id == 1)
	{
		switch (barr.img_num)
		{
		case 0:barrierImg(barr.id, i, 58 * 2, 0, 58, 82); break;//ͼƬ�е�x��y��λ�ã�ͼƬ�߶�
		case 1:barrierImg(barr.id, i, 58 * 3, 0, 58, 82); break;
		case 2:barrierImg(barr.id, i, 58 * 4, 0, 58, 82); break;
		case 3:barrierImg(barr.id, i, 58 * 5, 0, 58, 82); break;
		case 4:barrierImg(barr.id, i, 58 * 6, 0, 58, 82); break;
		case 5:barrierImg(barr.id, i, 58 * 7, 0, 58, 82); break;
		case 6:barrierImg(barr.id, i, 58 * 8, 0, 58, 82); break;
		}
	}
	else if (barr.id == 2)
	{
		switch (barr.img_num)
		{
		case 0:barrierImg(barr.id, i, 94 * 1, 0, 94, 100); break;//ͼƬ�е�x��y��λ�ã�ͼƬ�߶�
		case 1:barrierImg(barr.id, i, 94 * 2, 0, 94, 100); break;
		case 2:barrierImg(barr.id, i, 94 * 3, 0, 94, 100); break;
		case 3:barrierImg(barr.id, i, 94 * 4, 0, 94, 100); break;
		case 4:barrierImg(barr.id, i, 94 * 5, 0, 94, 100); break;
		case 5:barrierImg(barr.id, i, 94 * 6, 0, 94, 100); break;
		}
	}
	else
	{
		switch (barr.img_num)
		{
		case 0:barrierImg(barr.id, i, 0, 25, 265, 310); break;
		case 1:barrierImg(barr.id, i, 265, 25, 265, 310); break;
		case 2:barrierImg(barr.id, i, 2 * 265, 25, 265, 310); break;
		case 3:barrierImg(barr.id, i, 0, 372, 265, 315); break;
		case 4:barrierImg(barr.id, i, 265, 372, 265, 315); break;
		case 5:barrierImg(barr.id, i, 2 * 265, 372, 265, 315); break;
		}
	}
}

//����������ͼƬ��ʾ
void trapDeath()
{
	int num = 0;//��ʾ��num��ͼƬ
	mciSendString("close swumusic", NULL, 0, NULL);
	mciSendString("open res\\4.wav alias swumusic", NULL, 0, NULL);//����������
	mciSendString("play swumusic ", NULL, 0, NULL);//������һ��
	while (num < 8)
	{
		//�����ϰ����ǣ�Ϊ��ͼƬ��������ʾ
		if (img_bg_i < 3 * WIDTH)
			putimage(0, 0, WIDTH, HIGH, &img_bg, img_bg_i, 0);
		else if (img_bg_i >= 3 * WIDTH&&img_bg_i < 4 * WIDTH)//����ѭ��
		{
			putimage(0, 0, 4 * WIDTH - img_bg_i, HIGH, &img_bg, img_bg_i, 0);
			putimage(4 * WIDTH - img_bg_i, 0, img_bg_i - 3 * WIDTH, HIGH, &img_bg, 0, 0);
		}
		isBarrier(barr.i);

		switch (num)
		{
		case 0:trapDeathImg(0, 76 * 1, 0, 76, 92); break;//����λ�ã�������ͼƬ��x��yλ�ã�������x�߶�y��
		case 1:trapDeathImg(0, 76 * 2, 0, 76, 92); break;
		case 2:trapDeathImg(0, 76 * 3, 0, 76, 92); break;
		case 3:trapDeathImg(0, 76 * 4, 0, 76, 92); break;
		case 4:trapDeathImg(0, 76 * 5, 0, 76, 92); break;
		case 5:trapDeathImg(0, 76 * 6, 0, 76, 92); break;
		case 6:trapDeathImg(0, 76 * 7, 0, 76, 92); break;
		case 7:trapDeathImg(0, 0, 92, 76, 92); break;
		}
		FlushBatchDraw();
		num++;
		Sleep(150);
	}
}

//��ֲ������ͼƬ��ʾ
void plantDeath()
{
	int num = 0;//��ʾ��num��ͼƬ
	mciSendString("close sumusic", NULL, 0, NULL);
	mciSendString("open res\\1158.wav alias sumusic", NULL, 0, NULL);//����������
	mciSendString("play sumusic ", NULL, 0, NULL);//������һ��
	while (num < 6)
	{
		//�����ϰ����ǣ�Ϊ��ͼƬ��������ʾ
		if (img_bg_i < 3 * WIDTH)
			putimage(0, 0, WIDTH, HIGH, &img_bg, img_bg_i, 0);
		else if (img_bg_i >= 3 * WIDTH&&img_bg_i < 4 * WIDTH)//����ѭ��
		{
			putimage(0, 0, 4 * WIDTH - img_bg_i, HIGH, &img_bg, img_bg_i, 0);
			putimage(4 * WIDTH - img_bg_i, 0, img_bg_i - 3 * WIDTH, HIGH, &img_bg, 0, 0);
		}
		isBarrier(barr.i);

		switch (num)
		{
		case 0:plantDeathImg(100, 0, 100, 94); break;//������ͼƬ��x��yλ�ã�������x�߶�y��
		case 1:plantDeathImg(200, 0, 100, 94); break;
		case 2:plantDeathImg(300, 0, 100, 94); break;
		case 3:plantDeathImg(400, 0, 100, 94); break;
		case 4:plantDeathImg(500, 0, 100, 94); break;
		case 5:plantDeathImg(600, 0, 100, 94); break;
		}
		FlushBatchDraw();
		num++;
		Sleep(150);
	}
}

//��ʾ������Ϸ��������
void showdeathbk()
{
	mciSendString("stop  bkmusic", NULL, 0, NULL);//ֹͣ��������
	//mciSendString("close swmusic", NULL, 0, NULL);//�ر��������֣�����
	mciSendString("open res\\2.wav alias swmusic", NULL, 0, NULL);//��������������
	mciSendString("play swmusic ", NULL, 0, NULL);//������������
	putimage(0, 0, &img_deathbk);//��Ϸ��������
	setbkmode(TRANSPARENT);//������ʾ͸����ʹ�÷���ʾ
	showScore();//��ʾ�÷����
	//��ʾ��Ϸ��������
	FlushBatchDraw();
	Sleep(2);
	MOUSEMSG m;		// ���������Ϣ
	while (MouseHit())  //����������ڼ�⵱ǰ�Ƿ��������Ϣ
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 330 && m.x <= 450 && m.y >= 70 && m.y <= 100)//�������NEW GAME�����¿�ʼ��Ϸ
		{
			restartup();//���ݳ�ʼ��
			mciSendString("stop  swmusic", NULL, 0, NULL);//ֹͣ������������
			mciSendString("play  bkmusic repeat", NULL, 0, NULL);//���ű�������
			gameStatus = 1;//��Ϸ״̬���1
		}
		else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 330 && m.x <= 450 && m.y >= 190 && m.y <= 220)//�������MENU��������Ϸ�������
		{
			restartup();//���ݳ�ʼ��
			mciSendString("stop  swmusic", NULL, 0, NULL);//ֹͣ������������
			mciSendString("play  bkmusic repeat", NULL, 0, NULL);//���ű�������
			gameStatus = 0;//��Ϸ״̬���0���˵�����
			while (gameStatus == 0)
			{
				startMenu();//�������˵�
			}
		}
		else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 330 && m.x <= 450 && m.y >= 300 && m.y <= 330)//�������EXIT���˳�����
		{
			restartup();//��ʼ������
			exit(0);//�˳���Ϸ
			closegraph();
		}
	}
	FlushBatchDraw();

}

//�ж�����
void die()
{
	//����������ϰ����ǻ����ڻ����������ܺͻ���������
	if (barr.id == 1)
	{
		if (move == 0 || move == 2)
			if (M_X + 30 >= barr.i && M_X <= barr.i + 88)
			{
				trapDeath();
				gameStatus = 2;
			}
	}
	//����������ϰ�����ֲ���ֲ�����ܺͻ���������
	else if (barr.id == 2)
	{
		if (move == 0 || move == 2)
			if (M_X + 20 >= barr.i && M_X <= barr.i + 78)
			{
				plantDeath();
				gameStatus = 2;
			}
	}
	//����������ϰ��������������������������ܺ�����������
	else
	{
		if (move == 0 || move == 1)
			if (M_X + 20 >= barr.i && M_X <= barr.i + 256)
			{
				plantDeath();
				gameStatus = 2;
			}
	}
}

//��ʾ����
void show()//��ʾ
{
	while (gameStatus == 0)//��Ϸ״̬Ϊ0����ʾ�˵�����
	{
		startMenu();
	}
	while (gameStatus == 2)//��Ϸ״̬Ϊ2����ʾ������������
	{
		showdeathbk();
	}
	//��������
	if (img_bg_i < 3 * WIDTH)
	{
		putimage(0, 0, WIDTH, HIGH, &img_bg, img_bg_i, 0);

		if (img_bg_i < WIDTH)//���һ��0-512��λ��
			barr.i = WIDTH - img_bg_i;
		else if (img_bg_i >= WIDTH && img_bg_i < 2 * WIDTH)
			barr.i = 2 * WIDTH - img_bg_i;
		else if (img_bg_i >= 2 * WIDTH&&img_bg_i < 3 * WIDTH)
			barr.i = 3 * WIDTH - img_bg_i;

		isBarrier(barr.i);

		//�ж�ִ�ж���
		if (move == 0)
			run(img_bg_i);
		else if (move == 1)
		{
			jump(img_bg_i);
			if (img_num2 == 15)
			{
				move = 0;//һ����������Ծ����
				img_num2 = 0;
				img_num1 = 0;//�����ʼ��
			}
		}
		else if (move == 2)
		{
			slide(img_bg_i);
			if (img_num3 == 11)
			{
				move = 0;//һ���������»�����
				img_num3 = 0;
				img_num1 = 0;//�����ʼ��
			}
		}
		FlushBatchDraw();
	}
	else if (img_bg_i >= 3 * WIDTH&&img_bg_i < 4 * WIDTH)//����ѭ��
	{
		putimage(0, 0, 4 * WIDTH - img_bg_i, HIGH, &img_bg, img_bg_i, 0);
		putimage(4 * WIDTH - img_bg_i, 0, img_bg_i - 3 * WIDTH, HIGH, &img_bg, 0, 0);

		barr.i = 4 * WIDTH - img_bg_i;//���һ��0-512��λ��
		isBarrier(barr.i);

		if (move == 0)
			run(img_bg_i);
		else if (move == 1)
		{
			jump(img_bg_i);
			if (img_num2 == 15)
			{
				move = 0;
				img_num2 = 0;
				img_num1 = 0;
			}
		}
		else if (move == 2)
		{
			slide(img_bg_i);
			if (img_num3 == 12)
			{
				move = 0;//һ���������»�����
				img_num3 = 0;
				img_num1 = 0;//�����ʼ��
			}
		}
		FlushBatchDraw();
	}

	img_bg_i++;//�ָ������ƣ�ͼƬ�ƶ�
	if (img_bg_i == 4 * WIDTH)
	{
		img_bg_i = 0;
		begin_i = 0;
	}

	if (img_bg_i%WIDTH == 0)
		barr.begin_i = 800;
	delay(T);
}

//��Ϸ����
void gameover()
{
	EndBatchDraw();
	getch();
	closegraph();
}
