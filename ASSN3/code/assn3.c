//개발 환경: Visual Studio 2019

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>

void set_vt_mode() {
	DWORD l_mode;
	GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &l_mode);
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), l_mode | 0x0004 | 0x0008);
}

void set_color(int code) {
	printf("\033[%dm", code);
}
void reset_color() {
	printf("\033[0m");
}

void generate_ladder(int** board, int num_line, int par, int height);
void save_ladder(char filename[], int** board, int par, int height, int num_line);
void free_ladder(int** board, int board_row);

int** load_ladder(char filename[], int* board_row, int* board_col);
void print_ladder(int** board, int** ladder, int board_row, int board_col);
void move_ladder(int** board, int board_row, int board_col, int** ladder, int* x, int* y, int start);

int main()
{
	srand(time(NULL));
	int** ladder_board = NULL;

	int menu; //메뉴 입력 변수
	int par, height, num_line; //참여 인원수, 사다리 높이, 가로줄 개수
	char filename[20]; //파일이름

	while (1)
	{
		//[0. 메뉴 출력]
		printf("[사다리 게임]\n");
		printf("================================\n");
		printf("1. 사다리 보드 생성\n");
		printf("2. 사다리 타기 시작\n");
		printf("3. 종료\n");
		printf("================================\n");
		printf("선택: ");
		scanf("%d", &menu);
		printf("\n");
		//[1. 사다리 게임을 위한 판 생성]
		if (menu == 1)
		{
			//참여 인원수, 사다리 높이, 가로줄 개수, 파일이름 입력 받기
			printf("참여 인원수: ");
			scanf("%d", &par);
			printf("사다리 높이: ");
			scanf("%d", &height);
			printf("가로줄 개수: ");
			scanf("%d", &num_line);
			printf("파일이름: ");
			scanf("%s", filename);

			int board_row, board_col;
			board_col = par * 2 - 1;
			board_row = height;
			
			//1) 게임 판 생성 - 2차원 동적 할당
			ladder_board = (int**)calloc(board_row, sizeof(int*));
			for (int i = 0; i < board_row; i++)
				ladder_board[i] = (int*)calloc(board_col, sizeof(int));

			//2) 가로 줄 채우기
			generate_ladder(ladder_board, num_line, par, height);
			
			//3) 파일에 저장
			save_ladder(filename, ladder_board, par, height, num_line);

			//파일 저장 후 할당 해제
			free_ladder(ladder_board, board_row);
			printf("\n");
		} //menu==1인 경우 fin
		//[2. 사다리 타기 게임 시작]
		if (menu == 2)
		{
			printf("파일 이름: ");
			scanf("%s", filename);
			system("cls");
			int board_row;
			int board_col;
			ladder_board = load_ladder(filename, &board_row, &board_col);
			if (ladder_board== NULL)
			{
				printf("Cannot open file.\n");
				continue;
			}
			else
			{
				int** ladder; //경로 저장하는 배열 ladder 동적할당
				ladder = (int**)calloc(board_row, sizeof(int*));
				for (int i = 0; i < board_row; i++)
					ladder[i] = (int*)calloc(board_col, sizeof(int));
				print_ladder(ladder_board, ladder, board_row, board_col); //사다리 출력
				while (1) 
				{
					//1) 출발지 선택
					int start;
					printf(">> ");
					scanf("%d", &start); //출발지 입력 받기
					//0 입력 받은 경우
					if (start == 0)
						break;
					char dummy;
					scanf("%c", &dummy); //엔터키 누를 때마다 멈춤
					system("cls");
					//2) 선 긋기
					if (start != 0 && start != -1)
					{
						int lad_row = board_row - 1;
						int lad_col = 2 * (start - 1); //현재 위치
						ladder[lad_row][lad_col] = start; //초기 위치 설정
						while (1)
						{
							if (lad_row == 0) 
								break;
							printf("%2d %d\n", lad_row, lad_col);
							print_ladder(ladder_board, ladder, board_row, board_col);
							move_ladder(ladder_board, board_row, board_col, ladder, &lad_col, &lad_row, start);
							scanf("%c", &dummy); //엔터키
							system("cls");
						}
						printf("%2d %d\n", lad_row, lad_col);
						print_ladder(ladder_board, ladder, board_row, board_col);
						printf("\n");
						printf("result: %d -> %c\n", start, 'A' + lad_col / 2);
						printf("\n");
					}//start가 0 또는 -1이 아닌 경우
					//start가 -1인 경우
					if (start == -1)
					{
						//도착지 값 저장하는 배열 동적할당
						char* result = (char*)calloc((board_col + 1) / 2, sizeof(char));

						for (int i = 1; i <= (board_col + 1) / 2; i++)
						{
							int lad_row = board_row - 1;
							int lad_col = 2 * (i - 1); //현재 위치
							ladder[lad_row][lad_col] = i; //초기 위치
							while (1)
							{
								if (lad_row == 0)
									break;
								move_ladder(ladder_board, board_row, board_col, ladder, &lad_col, &lad_row, i);
							}
						}
						for (int i = 0; i < (board_col + 1) / 2; i++) {
							result[ladder[0][2*i] - 1] = 'A' + i;
						}
						printf("0 %d\n", (result[(board_col + 1) / 2 - 1] - 'A') * 2);
						print_ladder(ladder_board, ladder, board_row, board_col);
						printf("\n");
						//최종 결과 출력
						for (int i = 0; i < (board_col + 1) / 2; i++)
						{
							printf("%d -> %c\n", i + 1, result[i]);
						}
						printf("\n");
						free(result); //result 배열 할당 해제
					}
				}
				free_ladder(ladder, board_row);
				system("cls");
			}
			free_ladder(ladder_board, board_row);
		} //menu 2인 경우 fin
		//[3. 종료]
		if (menu == 3)
			break;
	}
	return 0;
}

//사다리의 적절한 위치에 해당 개수만큼 가로줄을 랜덤하게 생성하여 긋는 함수
void generate_ladder(int** board, int num_line, int par, int height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < par; j++)
		{
			board[i][j * 2] = 1; //세로선 부분(사다리 놓이지 않는 부분)
		}
	}		
	//사다리 랜덤하게 생성
	int lad_row, lad_col; //사다리의 좌표
	int lad_n = 0; //사다리 개수
	while (lad_n < num_line) //num_line: 입력 받은 사다리 개수
	{
		lad_col = (rand() % (par - 1)) * 2 + 1;
		lad_row = rand() % (height - 2) + 1;
		//인접한 가로줄이거나 이전에 이미 놓인 사다리인 경우
		if (lad_col == 1) //제일 왼쪽 사다리
		{
			if (board[lad_row][3] == 1 || board[lad_row][1] == 1)
				continue;
		}
		else if (lad_col == (par - 1) * 2 - 1) //제일 오른쪽 사다리
		{
			if (board[lad_row][(par - 1) * 2 - 3] == 1 || board[lad_row][(par - 1) * 2 - 1] == 1)
				continue;
		}
		else
		{
			if (board[lad_row][lad_col + 2] == 1 || board[lad_row][lad_col - 2] == 1 || board[lad_row][lad_col] == 1)
				continue;
		}
		board[lad_row][lad_col] = 1;
		lad_n++;
	}
	return;
}

//매개변수로 전달받은 파일 이름으로 텍스트 파일을 만들고 생성한 사다리 정보를 저장하는 함수
void save_ladder(char filename[], int** board, int par, int height, int num_line)
{
	FILE* outfile;
	outfile = fopen(filename, "w");
	fprintf(outfile, "%d %d %d\n", par, height, num_line);
	for (int i = 1; i < height; i++)
	{
		for (int j = 1; j < par; j++)
		{
			if (board[i][j * 2 - 1] == 1)
				fprintf(outfile, "%d %d\n", i, j);
		}
	}
	fclose(outfile);
	return;
}

//동적 할당 받은 메모리를 할당 해제하는 함수
void free_ladder(int** board, int board_row)
{
	for (int i = 0; i < board_row; i++)
		free(board[i]);
	free(board);
	return;
}

//파일 열고 데이터 읽어 사다리 판 생성하는 함수로 동적 할당 받은 사다리 판 반환
int** load_ladder(char filename[], int* board_row, int* board_col)
{
	FILE* infile;
	infile = fopen(filename, "r");
	if (infile == NULL) //입력한 파일이 없는 경우, NULL 반환
		return NULL;

	//데이터를 읽어 동적 할당
	int par, height, num_line;
	fscanf(infile, "%d %d %d", &par, &height, &num_line);
	*board_col = par * 2 - 1;
	*board_row = height;
	int** board;
	board = (int**)calloc(*board_row, sizeof(int*));
	for (int i = 0; i < *board_row; i++)
		board[i] = (int*)calloc(*board_col, sizeof(int));
	//세로선에 값 1(True) 넣어주기
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < par; j++)
		{
			board[i][j * 2] = 1; //세로선 부분
		}
	}
	int lad_row, lad_col;
	while (fscanf(infile, "%d %d", &lad_row, &lad_col) != EOF)
	{
		board[lad_row][lad_col * 2 - 1] = 1;
	}
	fclose(infile);
	return board;
	free_ladder(board, *board_row);
}

//사다리 출력하는 함수
void print_ladder(int** board, int** ladder, int board_row, int board_col)
{
	//색 정보
	int color[5] = { 95,91,92,93,94 };
	int par = (board_col + 1) / 2;
	//인덱스 출력(A, B, ...)
	for (int n = 0; n < par; n++)
	{
		printf("  %c ", 'A' + n);
	}
	printf("\n");
	//사다리 출력
	for (int i = 0; i < board_row; i++)
	{
		printf("  ");
		for (int j = 0; j < board_col; j++)
		{
			if (ladder[i][j] !=0)
			{
				set_color(color[ladder[i][j] % 5]);
			}
			if (j % 2 == 0)
			{
				printf("+"); //세로선 출력
			}
			else
			{
				if (board[i][j] == 1)
					printf("---"); //가로선 출력
				else
					printf("   "); //가로선 없는 경우
			}
			reset_color();
		}
		printf("\n");
	}
	//1부터 참여 인원수까지 인덱스 출력
	for (int n = 0; n < par; n++)
	{
		printf(" %2d ", n+1);
	}
	printf("\n");
	return;
}

//사다리를 타는 함수 -> ladder에 사다리를 타면서 이동하는 대로 start 값을 넣어 경로 저장
void move_ladder(int** board, int board_row, int board_col,int** ladder, int*x,int*y,int start)
{
	static int x_old = 0, y_old = 0;
	if (*x > 0 && board[*y][*x - 1] == 1 && x_old != *x - 1) //왼쪽에 길이 놓인 경우
	{
		y_old = *y;
		x_old = *x;
		ladder[*y][*x - 1] = start;
		*x -= 1;
	}
	else if (*x < 2 * board_col - 2 && board[*y][*x + 1] == 1 && x_old != *x + 1) //오른쪽에 길이 놓인 경우
	{
		y_old = *y;
		x_old = *x;
		ladder[*y][*x + 1] = start;
		*x += 1;
	}
	else //위로 올라가는 경우
	{
		ladder[*y - 1][*x] = start;
		y_old = *y;
		x_old = *x;
		*y -= 1;
	}
	return;
}
