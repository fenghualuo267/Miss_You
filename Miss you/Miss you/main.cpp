#include<stdio.h>
#include<graphics.h>//֮ǰҪ�Ȱ�װEasyX,��Ȼ����������ᱨ��
#include<conio.h>
#include<math.h>
#include<time.h>//�����
#include<mmsystem.h>//��ý���豸�ӿڵ�ͷ�ļ�
#pragma comment(lib,"winmm.lib")//Media control interface
//����ָ������ڹ��������"winmm.lib"�Ĺ���һ�������߱�����Ҫ����winmm��

#define NUM 13 //��������Ļ��,������13������
void Welcome();//����������н���
void init(int i);//��ʼ��ÿ��С�̻�������ֵ:����x,y����ֵ,�Ƿ����
void Load();//����ͼƬ
void Choose(DWORD &t1);//ѡ��Ҫ���͵��̻���
void shoot();//�����̻���
void Show(DWORD*);		// �����̻�
void Style(DWORD& st1);
void write(DWORD&);
void clear(DWORD*);
struct Jet
{
	int x, y;//�̻���������
	int hx, hy;//��ߵ������
	int height;//��:y-hy
	bool isshot;//�Ƿ���
	DWORD t1, t2, dt;//DWORD���޷��ŵ�,�൱��unsigned long ,
	//��t1,t2,dt�����ٶ�
	IMAGE Img[2];//���ͼƬ������
	byte n : 1;//�±�	λ��:/:ָһ��λ�����n
	//λ��:(bit-field)����λΪ��λ������ṹ��(��������)�еĳ�Ա������ռ�Ŀռ�

}jet[NUM];//����ṹ������
struct Fire
{
	int r;//��ǰ��ը�뾶
	int max_r;//��ը���ľ����Ե���뾶
	int x, y;//��ը�����ڴ��ڵ�����
	int cen_x,cen_y;//��ը�������ͼƬ���Ͻǵ�����
	int width;//ͼƬ�Ŀ��
	int height;//ͼƬ�ĸ߶�
	int xy[240][240];//����ͼƬ���ص�
	bool isshow;//�Ƿ���ʾ
	bool isdraw;//�Ƿ񻭳�
	DWORD t1, t2, dt;//�����ٶ�
}fire[NUM];
int main()
{
	initgraph(1000, 600);//��ʼ��һ������
	srand((unsigned int)time(NULL));		//Ϊ�˵õ���һ���������
	
	Welcome();
	Load();
	DWORD t1 = timeGetTime();			// ɸѡ�̻���ʱ
	DWORD st1 = timeGetTime();			// ���Ż�����ʱ
	DWORD tm = timeGetTime();			//���岥�ż�ʱ
	DWORD tn;
	DWORD TP;
	DWORD TC;

	DWORD* pMem = GetImageBuffer();		// ��ȡ�����Դ�ָ��,��ÿ�����ص���в���
	
	for (int i = 0; i < NUM; i++)
	{
		init(i);//��ʼ��ÿ��С�̻����������

	}
	
	Load();								// ���̻�ͼƬ��Ϣ���ؽ���Ӧ�ṹ��
	IMAGE img;
	loadimage(&img, "./file/����ͼƬ.jpg", 150, 266);
	BeginBatchDraw();					// ��ʼ������ͼ
	while (!_kbhit())
	{
		Sleep(10);
		clear(pMem);
		TC = timeGetTime();
		if (TC - t1 > 0)
		{
			
			putimage(850, 334, &img);
		}
		Choose(t1);
		
		shoot();
		Show(pMem);
		Style(st1);
		FlushBatchDraw();	// ��ʾǰ������л�ͼ����
	}
	getchar();

	return 0;
}
void Welcome()//��������һ��ʼ����Ľ���
{
	//�����ı�(����)��ɫ
	mciSendString("open ./file/С����-С����.mp3 alias music ", 0, 0, 0);//aliasȡ����
	mciSendString("play music repeat", 0, 0, 0);
	mciSendString("open ./file/¼��1.mp3 alias music1 ", 0, 0, 0);//aliasȡ����
	mciSendString("play music1", 0, 0, 0);
	settextcolor(YELLOW);
	for (int i = 0; i < 50; i++)
	{
		int x = 200 + int(180 * sin(2 * 3.1415926*i / 60));
		//500ָԲ��x����,180ָ�뾶
		int y = 100 + int(180 * cos(2 * 3.1415926*i / 60));
		//300ָԲ��x����,180ָ�뾶
		cleardevice();
		settextstyle(i, 0, "���Ĳ���");//�м�Ϊ0ʱ���Ŀ�Ϊ����Ӧ

		outtextxy(x, y-80, "��ʵ�ҵ�����һֱ������");//��ָ����λ������ı�
		outtextxy(x+100, y+100, "�������˵:");
		Sleep(25);
	}
	Sleep(1000);
	mciSendString("open ./file/¼��2.mp3 alias music2 ", 0, 0, 0);//aliasȡ����
	mciSendString("play music2", 0, 0, 0);
	cleardevice();
	settextstyle(25, 0, "����");
	outtextxy(300, 150, "ԭ��������������ס������");
	Sleep(3000);
	outtextxy(300, 200, "ԭ�����ǺͰ�������������ô��");
	Sleep(3000);
	outtextxy(300, 250, "��Ϊ�ҶԿ�����ľ���");
	Sleep(3000);
	outtextxy(300, 300, "�������ܵ���");
	Sleep(2000);
	outtextxy(300, 350, "һĻĻ������");
	Sleep(2000);
	outtextxy(300, 400, "һ�㲻Ⱦ������.");
	Sleep(1000);
	outtextxy(550, 450, "---С����");

}
//����ͼƬ
void Load()
{
/****�����̻������ص�ͼƬ****/

	IMAGE fImg, gImg;
	loadimage(&fImg,"./file/flower.jpg",3120,240);
	for (int i = 0; i < NUM; i++)
	{
		SetWorkingImage(&fImg);
		getimage(&gImg, i * 240, 0, 240, 240);
		SetWorkingImage(&gImg);
		for (int a = 0; a < 240; a++)
		{
			for (int b = 0; b < 240; b++)
			{
				fire[i].xy[a][b] = getpixel(a, b);//��ȡ�����ɫ
			}
		}

	}
	IMAGE jetImg;//���ͼƬ	
	loadimage(&jetImg,"./file/shoot.jpg",200,50);//����ͼƬ,��ͼƬ���浽jetImage��������ȥ
	//[2]���ͼƬ
	for (int i = 0; i < NUM; i++)
	{
		SetWorkingImage(&jetImg);//���õ�ǰ���豸,��ǰ�豸��ΪjetImg
		int n = rand() % 5;
		getimage(&jet[i].Img[0],n*20,0,20,50);//�õ����ǰ�ɫͼƬ
		//getimage()�ӵ�ǰ�豸��ȡͼƬ
		//getimage()����1ָ���õ��Ĳ�ֳ���ͼƬ��ŵ�λ��
		//����2,3ָ�õ���ͼƬ��ԭͼƬ���Ͻǵ�λ��
		//����4,5ָ�����µĵ���ͼƬ�Ŀ�͸�		��������
		getimage(&jet[i].Img[1], (n+5) * 20, 0, 20, 50);//�õ���������ͼƬ
	}
	SetWorkingImage(NULL);//�ؽ���ǰ�豸����Ϊ����

}//�����̻�СͼƬ
//��ʼ��,Ϊ������һЩ��ǰ��׼��
void init(int i)
{
	// �ֱ�Ϊ���̻����ĵ�ͼƬ��Ե����Զ���롢�̻����ĵ�ͼƬ���Ͻǵľ��� (x��y) ��������
	int r[13] = { 120,120,155,123,130,147,138,138,130,135,140,132,155 };
	int x[13] = { 120,120,110,117,110,93,102,102,110,105,100,108,110 };
	int y[13] = { 120,120,85,118,120,103,105,110,110,120,120,104,85 };
	//��������Կ��������������ͼƬ��Ծ���
/**** ��ʼ���̻� *****/
	fire[i].x = 0;//�̻����ĵ�����
	fire[i].y = 0;
	fire[i].width = 240;
	fire[i].height = 240;
	fire[i].max_r = r[i];//���뾶
	fire[i].cen_x = x[i];//��ը�������ͼƬ���Ͻ�����
	fire[i].cen_y = y[i];
	fire[i].isshow = false;
	fire[i].isdraw = false;
	fire[i].dt = 5;//����ʱ����
	fire[i].t1 = timeGetTime();
	fire[i].r = 0;//���㿪ʼ����
/**** ��ʼ���̻��� *****/
	jet[i].x = 0;
	jet[i].y = 0;
	jet[i].hx = 0;
	jet[i].hy = 0;
	jet[i].height = 0;
	jet[i].t1 = GetTickCount();//��ȡϵͳʱ��
	jet[i].dt = rand() % 5;//�����ٶ�ʱ����
	jet[i].n = 0;
	jet[i].isshot = false;
}//�̻��ͷ�Ч��ͼ
void Choose(DWORD &t1)//ѡ���̻�������
{
	DWORD t2 = timeGetTime();//��ȡϵͳʱ��;
	if (t2 - t1 > 100)		//ÿ100ms����һ���̻���
	{
		
		int n = rand() % 20;//��Ϊ�������������ͬ,����ֻ�ܲ���Ψһ���̻���
		if (n < 13 && jet[n].isshot == false&&fire[n].isshow==false)
		{
			/***�����̻���,Ԥ������***/
			jet[n].x = rand() % 1000;
			jet[n].y = rand() % 50 + 550;
			jet[n].hx = jet[n].x;
			jet[n].hy = rand() % 300;
			jet[n].height = jet[n].y - jet[n].hy;
			jet[n].isshot = true;
			putimage(jet[n].x, jet[n].y, &jet[n].Img[jet[n].n], SRCINVERT);
			/**** ����ÿ���̻��������� *****/
			char c1[50], c2[30], c3[30];
			sprintf_s(c1, "open ./file/shoot.mp3 alias s%d", n);
			sprintf_s(c2, "play s%d", n);
			sprintf_s(c3, "close n%d", n);

			mciSendString(c3, 0, 0, 0);
			mciSendString(c1, 0, 0, 0);
			mciSendString(c2, 0, 0, 0);
		}
		t1 = t2;
	}
}
void shoot()
{
	/**** �̻��������� *****/	
	for (int i = 0; i < NUM; i++)
	{
		jet[i].t2 = timeGetTime();
		if (jet[i].t2 - jet[i].t1 >= jet[i].dt&&jet[i].isshot==true)
		{
			putimage(jet[i].x, jet[i].y, &jet[i].Img[jet[i].n],SRCINVERT);//SRCINVERT��֤ͼƬ��͸����
			if (jet[i].y >= jet[i].hy)
			{
				jet[i].n++;
				jet[i].y -= 5;
			}
			putimage(jet[i].x, jet[i].y, &jet[i].Img[jet[i].n], SRCINVERT);
			/**** �������߶ȵ� 3 / 4������ *****/
			if ((jet[i].y - jet[i].hy) * 4 < jet[i].height)
				jet[i].dt = rand() % 4 + 10;
			/**** ���������߶� *****/
			if (jet[i].y <= jet[i].hy)
			{
				// ���ű�ը��
				char c1[50], c2[30], c3[30];
				sprintf_s(c1, "open ./file/bomb.wav alias n%d", i);
				sprintf_s(c2, "play n%d", i);
				sprintf_s(c3, "close s%d", i);

				mciSendString(c3, 0, 0, 0);
				mciSendString(c1, 0, 0, 0);
				mciSendString(c2, 0, 0, 0);
				putimage(jet[i].x, jet[i].y, &jet[i].Img[jet[i].n], SRCINVERT);	// �����̻���
				fire[i].x = jet[i].hx + 10;											// ���̻����м䱬ը
				fire[i].y = jet[i].hy;												// ����ߵ�����
				fire[i].isshow = true;					// ��ʼ����
				jet[i].isshot = false;					// ֹͣ����
			}
				jet[i].t1 = jet[i].t2;
			//����һ���̻�������һ��,����һ���̻�������һ��
		}
	}
}
void Show(DWORD* pMem)
{
	//�̻����׶�����ʱ����,������������Ч��
	int drt[16] = { 5, 5, 5, 5, 5, 6, 25, 25, 25, 25, 55, 55, 55, 55, 55 };
	for (int i = 0; i < NUM; i++)
	{
		fire[i].t2 = timeGetTime();
		// ���ӱ�ը�뾶�������̻�������ʱ����������Ч��
		if (fire[i].t2 - fire[i].t1 > fire[i].dt && fire[i].isshow == true)
		{

			if (fire[i].r < fire[i].max_r)
			{
				fire[i].r++;
				fire[i].dt = drt[fire[i].r / 10];
				fire[i].isdraw = true;
			}
			if (fire[i].r >= fire[i].max_r - 1)
			{
				fire[i].isdraw = false;
				init(i);//���³�ʼ��,�����Ϳ���ѭ����������
			}
			fire[i].t1 = fire[i].t2;
			// ����ú��ڻ��ɱ�ը�����ݵ�ǰ��ը�뾶���̻�����ɫֵ�ӽ���ɫ�Ĳ������
			if (fire[i].isdraw)
			{//�ο�:
				for (double a = 0; a <= 6.28; a += 0.01)
				{
					int x1 = (int)(fire[i].cen_x + fire[i].r * cos(a));				// �����ͼƬ���Ͻǵ�����
					int y1 = (int)(fire[i].cen_y - fire[i].r * sin(a));

					if (x1 > 0 && x1 < fire[i].width && y1 > 0 && y1 < fire[i].height)	// ֻ���ͼƬ�ڵ����ص�
					{	//ʹ���γ��̻�Ч��
						int b = fire[i].xy[x1][y1] & 0xff;
						int g = (fire[i].xy[x1][y1] >> 8) & 0xff;
						int r = (fire[i].xy[x1][y1] >> 16);

						// �̻����ص��ڴ����ϵ�����
						int xx = (int)(fire[i].x + fire[i].r * cos(a));
						int yy = (int)(fire[i].y - fire[i].r * sin(a));

						// �ϰ������ص㲻�������ֹԽ��
						if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < 1000 && yy > 0 && yy < 600)
							pMem[yy * 1000 + xx] = BGR(fire[i].xy[x1][y1]);	// �Դ���������̻�
					}
				}
				fire[i].isdraw = false;
			}
		}
	}
}
/****��ʾ����****/
void Style(DWORD& st1)
{//�ο�:
	
	DWORD st2 = timeGetTime();//ͨ������ʱ������ʾ�����̻�ʱ��
	if (st2 - st1 >15000)		// һ�׸��ʱ��
	{
		// ��������
		double x[13] = {45.4,45.7,34.0, 56.7,25.9,65.5,34.6,56.5,40.9,50.6,38.4,52.8, 64.8 };
		double y[13] = {12.5, 35.9,5.0,5.0,13.8,13.8,28.4,28.4, 6.6,6.6,31.6,31.6,10.7 };
		for (int i = 0; i < NUM; i++)
		{
			/**** ���ɷֲ��̻��� ***/
			jet[i].x = (int)(x[i] * 10);
			jet[i].y = (int)((y[i] + 75) * 10);
			jet[i].hx = (int)(jet[i].x);
			jet[i].hy = (int)(y[i] * 10);
			jet[i].height = (int)(jet[i].y - jet[i].hy);
			jet[i].isshot = true;
			jet[i].dt = 7;
			putimage(jet[i].x, jet[i].y, &jet[i].Img[jet[i].n], SRCINVERT);	// ��ʾ�̻���

																			/**** �����̻����� ***/
			fire[i].x = (int)(jet[i].x + 10);
			fire[i].y =(int)( jet[i].hy);
			fire[i].isshow = false;
			fire[i].r = 0;

			/**** ���ŷ������� ***/
			char c1[50], c2[30], c3[30];
			sprintf_s(c1, "open ./file/shoot.mp3 alias s%d", i);
			sprintf_s(c2, "play s%d", i);
			sprintf_s(c3, "close n%d", i);

			mciSendString(c3, 0, 0, 0);
			mciSendString(c1, 0, 0, 0);
			mciSendString(c2, 0, 0, 0);
		}
		st1 = st2;
	}
}
void clear(DWORD* pMem)
{
	 pMem = GetImageBuffer();		// ��ȡ�����Դ�ָ��,��ÿ�����ص���в���
	// ���ѡ�� 4000 �����ص����
	for (int clr = 0; clr < 1000; clr++)
	{
		for (int j = 0; j < 2; j++)
		{
			int px1 = rand() % 1000;
			int py1 = rand() % 600;

			if (py1 < 599)				// ��ֹԽ��
				pMem[py1 * 1000 + px1] = pMem[py1 * 1000 + px1 + 1] = BLACK;	// ���Դ渳ֵ�������ص�
		}
	}//�����ص���в���
}
