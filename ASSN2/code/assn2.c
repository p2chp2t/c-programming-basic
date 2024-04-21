//개발환경: Visual Studio 2019

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <Windows.h> //GetConsoleMode() 등의 사용을 위해 포함
#define SIZE 75

int set_vt_mode()
{
	DWORD l_mode;
	GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &l_mode);
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), l_mode | 0x0004 | 0x0008);
}
void set_color_rgb(int r, int g, int b)
{
	printf("\033[38;2;%d;%d;%dm", r, g, b);
}
void reset_color()
{
	printf("\033[0m");
}
int load_image(const char* filename, int image_rgb[][SIZE][SIZE], float image_hsv[][SIZE][SIZE], int* width, int* height);
void save_image(int image_rgb[][SIZE][SIZE], int width, int height);
void print_image(int image_rgb[][SIZE][SIZE], int width, int height);
void print_histogram(float image_hsv[][SIZE][SIZE], int width, int height);
void change_color(float image_hsv[][SIZE][SIZE], int width, int height, int source, int target);

float MAX(float a, float b, float c);
float MIN(float a, float b, float c);

void rgb_hsv(int r, int g, int b, float* h, float* s, float* v);
void hsv_rgb(float h, float s, float v, int* r, int* g, int* b);

int main()
{
	set_vt_mode();
	//변수 및 배열 선언
	char filename[SIZE]; //최대 74자 까지 저장 가능
	FILE* fp;
	int image_rgb[3][SIZE][SIZE];
	float image_hsv[3][SIZE][SIZE];
	int width;
	int height;

	printf("Enter input filename: ");
	scanf("%s", filename); //공백 전 까지만 읽음
	
	//파일이 유효한지 확인하는 변수 선언
	int image_load = load_image(filename, image_rgb, image_hsv, &width, &height);

	if (image_load == 0)
	{
		printf("File not found: %s\n", filename);
		return 0;
	}
	else
	{	
		int menu = 0; //메뉴를 입력받는 변수

		while (menu != 5)
		{
			//메뉴화면 실행
			printf("=======================\n");
			printf("  IMAGE COLOR CHANGER  \n");
			printf("=======================\n");
			printf("  1. Image Histogram\n");
			printf("  2. Change Color\n");
			printf("  3. Print Image\n");
			printf("  4. Save Image\n");
			printf("  5. Exit\n");
			printf("Loaded file: %s\n", filename);
			while (1)
			{
				printf("Choose menu number >> ");
				scanf("%d", &menu);
				if (menu >= 1 && menu <= 5)
					break; //올바른 메뉴를 고르면 메뉴를 받는 반복문 탈출
				else
					printf("Wrong input!\n");
			}

			if (menu == 3) //3. Print Image를 선택한 경우
			{
				print_image(image_rgb, width, height);
			}
			else if (menu == 1) //1. Image Histogram을 선택한 경우
			{
				print_histogram(image_hsv, width, height);
			}
			else if (menu == 2) //2. Change Color를 선택한 경우
			{
				print_histogram(image_hsv, width, height);
				int target, source;
				while (1)
				{
					printf("Choose source color >> ");
					scanf("%d", &source);
					if (source >= 0 && source <= 11)
						break;
					else
						printf("Wrong input!\n");
				}
				while (1)
				{
					printf("Choose target color >> ");
					scanf("%d", &target);
					if (target >= 0 && target <= 11)
						break;
					else
						printf("Wrong input!\n");
				}
				change_color(image_hsv, width, height, source, target);
				for (int y = 0; y < height; y++)
				{
					for (int x = 0; x < width; x++)
					{
						hsv_rgb(image_hsv[0][x][y], image_hsv[1][x][y], image_hsv[2][x][y], &image_rgb[0][x][y],
							&image_rgb[1][x][y], &image_rgb[2][x][y]); //바뀐 색조를 이용해 rgb도 업데이트
					}
				}
			}
			else if (menu == 4)//4. Save Image를 선택한 경우
			{
				save_image(image_rgb, width, height);
			}
			else if (menu == 5) //5. Exit
				break;
		}
		return 0;
	}

}

int load_image(const char* filename, int image_rgb[][SIZE][SIZE], float image_hsv[][SIZE][SIZE], 
	int* width, int* height)
//입력 받은 파일 이름을 통해 이미지를 읽고 이미지의 정보를 각 색공간에 맞는 배열에 저장
//이미지의 너비와 높이를 전달받은 포인터에 알맞게 전달
{
	FILE* fp;
	fp = fopen(filename, "r"); //입력 받은 파일을 읽기 모드로 열기
	if (fp == NULL)
		return 0;
	char ppm[3]; //p3
	int num; //255
	fscanf(fp, "%s %d %d %d", ppm, width, height, &num);
	for (int y = 0; y < *height; y++) //세로축
	{
		for (int x = 0; x < *width; x++) //가로축
		{
			for (int i = 0; i < 3; i++) //각각 r, g, b
			{
				fscanf(fp, "%d", &image_rgb[i][x][y]);
			}
			rgb_hsv(image_rgb[0][x][y], image_rgb[1][x][y], image_rgb[2][x][y], 
				&image_hsv[0][x][y], &image_hsv[1][x][y], &image_hsv[2][x][y]);
		}
	}
	fclose(fp);
	return 1;
}

float MAX(float a, float b, float c) //최대값을 구하는 함수
{
	if (a >= b)
	{
		if (a >= c)
			return a;
		else
			return c;
	}
	else
	{
		if (b >= c)
			return b;
		else
			return c;
	}
}
float MIN(float a, float b, float c) //최소값을 구하는 함수
{
	if (a <= b)
	{
		if (a <= c)
			return a;
		else
			return c;
	}
	else
	{
		if (b <= c)
			return b;
		else
			return c;
	}
}

void rgb_hsv(int r, int g, int b, float* h, float* s, float* v)//rgb를 hsv로 변환하는 함수
{
	float rr = r / 255.0;
	float gg = g / 255.0;
	float bb = b / 255.0;

	float c_max = MAX(rr, gg, bb);
	float c_min = MIN(rr, gg, bb);
	float delta;
	delta = c_max - c_min;
	//H 값 변환
	if (delta == 0)
		*h = 0;
	else if (c_max == rr)
		*h = 60 * ((gg - bb) / delta);
	else if (c_max == gg)
		*h = 60 * ((bb - rr) / delta + 2);
	else if (c_max == bb)
		*h = 60 * ((rr - gg) / delta + 4);
	//S 값 변환
	if (c_max == 0)
		*s = 0;
	else
		*s = delta / c_max;
	//V 값 변환
	*v = c_max;
	//H<0인 경우
	if (*h < 0)
		*h += 360;
	return;
}

void hsv_rgb(float h, float s, float v, int* r, int* g, int* b) //hsv를 rgb로 변환하는 함수
{
	float c, x, m;
	c = v * s;
	x = c * (1 - fabs(fmod((h / 60), 2) - 1));
	m = v - c;
	float rr, gg, bb;

	if (h >= 0 && h < 60)
	{
		rr = c;
		gg = x;
		bb = 0;
	}
	else if (h < 120)
	{
		rr = x;
		gg = c;
		bb = 0;
	}
	else if (h < 180)
	{
		rr = 0;
		gg = c;
		bb = x;
	}
	else if (h < 240)
	{
		rr = 0;
		gg = x;
		bb = c;
	}
	else if (h < 300)
	{
		rr = x;
		gg = 0;
		bb = c;
	}
	else if (h < 360)
	{
		rr = c;
		gg = 0;
		bb = x;
	}
	*r = (rr + m) * 255;
	*g = (gg + m) * 255;
	*b = (bb + m) * 255;
	return;
}

void print_image(int image_rgb[][SIZE][SIZE], int width, int height)
//전달받은 이미지를 출력하는 함수
{
	for (int y = 0; y < height; y++) //세로축
	{
		for (int x = 0; x < width; x++) //가로축
		{
			set_color_rgb(image_rgb[0][x][y], image_rgb[1][x][y], image_rgb[2][x][y]);
			printf("■");
		}
		printf("\n");
	}
	reset_color();
}

void print_histogram(float image_hsv[][SIZE][SIZE], int width, int height)
//전달받은 이미지의 hsv 값을 통해 이미지 히스토그램을 출력하는 함수
{
	//계급 별 빈도수를 저장하는 배열 arr_h[12]
	int arr_h[12] = { 0 };
	int h;
	int r, g, b;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			h = image_hsv[0][x][y] / 30;
			if (image_hsv[1][x][y] == 0) //s(채도)가 0인 값은 무채색이라 빈도수 계산 x
				continue;
			arr_h[h] += 1;
		}
	}
	for (int i = 0; i < 12; i++) //계급 별 픽셀 출력
	{
		printf("[%2d]", i);
		hsv_rgb(i * 30, 1, 1, &r, &g, &b);
		set_color_rgb(r, g, b);
		for (int j = 0; j < arr_h[i] / 10; j++) //픽셀 개수는 빈도 값의 1/10 출력
		{
			printf("■");
		}
		reset_color();
		printf("%d\n", arr_h[i]);
	}
}

void change_color(float image_hsv[][SIZE][SIZE], int width, int height, int source, int target)
//전달받은 이미지의 hsv 값을 변경하여 이미지의 색조를 변경하는 함수
{
	int gap = target - source;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			image_hsv[0][x][y] += gap * 30;
			if (image_hsv[0][x][y] < 0) //h가 음수인 경우
				image_hsv[0][x][y] += 360;
			else if (image_hsv[0][x][y] >= 360) //h가 360보다 큰 경우
				image_hsv[0][x][y] -= 360;
		}
	}
}

void save_image(int image_rgb[][SIZE][SIZE], int width, int height)
{
	FILE* outfile;
	outfile = fopen("output.ppm", "w");

	fprintf(outfile, "P3 %d %d 255\n", width, height);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			fprintf(outfile, "%d %d %d ", image_rgb[0][x][y], image_rgb[1][x][y], image_rgb[2][x][y]);
		}
	}

	fclose(outfile);
	return;
}
