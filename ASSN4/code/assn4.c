//개발 환경: Visual Studio 2019
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

int main()
{
	//[1. 프로그램 시작 화면]
	char filename[30];
	printf("음악 리스트 파일 이름을 입력해주세요. ");
	printf(">> ");
	scanf("%s", filename);
	FILE* infile;
	infile = fopen(filename, "r");

	//존재하지 않는 파일 이름을 입력 받은 경우
	while (infile == NULL)
	{
		printf("유효하지 않은 파일입니다. 음악 리스트 파일을 다시 입력해주세요. "); //[에러 메시지 #1]
		printf(">> ");
		scanf("%s", filename);
		infile = fopen(filename, "r");
	}

	//[2. 플레이리스트(연결리스트 구조) 구현]
	LIST playlist;
	
	playlist.count = 0;
	playlist.storage = 0;
	playlist.head = NULL;

	SONG song;	//파일에서 읽은 음악 정보
	NODE* pList;
	NODE* pNew;
	
	while (fscanf(infile, "%s %s %f %f", song.title, song.artist, &song.volume, &song.preference) != EOF) //파일 읽어오기
	{
		NODE* node = (NODE*)malloc(sizeof(NODE));
		strcpy(node->data.title, song.title);
		strcpy(node->data.artist, song.artist);
		node->data.volume = song.volume;
		node->data.preference = song.preference;

		if ((playlist.storage+node->data.volume) <= 50)
		{
			NODE* pPre = NULL;
			NODE* pNext = playlist.head; //pPre와 pNext 사이에 node가 들어간다
			for (int i = 0; i < playlist.count; i++) { //node가 들어갈 자리를 찾는 반복문
				if (strcmp(pNext->data.title, node->data.title) > 0)
				{
					break; //node가 삽입될 위치 찾으면 반복문 탈출
				}
				pPre = pNext;
				pNext = pNext->next;
			}
			if (pPre == NULL) { //리스트의 처음 또는 빈 리스트에 삽입되는 경우
				node->next = pNext;
				playlist.head = node;
			}
			else { //리스트의 중간 또는 끝에 삽입되는 경우
				node->next = pNext;
				pPre->next = node;
			}
			(playlist.storage) += node->data.volume; //플레이리스트 용량 업데이트
			(playlist.count)++; //플레이리스트 음악 개수 업데이트
		}
		else
		{
			printf("용량 초과! 음악(%s)은 추가되지 않았습니다.\n", node->data.title); //[에러 메시지 #2]
		}
	} //한 줄씩 읽어 데이터 저장하기
	printf("\n");
	//[3. 플레이리스트 명령어 구현]
	char command[20]; //명령어 입력받는 변수
	while (1)
	{
		printf("명령어를 입력해주세요. >> "); //[명령 메시지]
		scanf("%s", command);
		if (strcmp(command, "add")==0) { //명령어 add
			add_fn(&playlist);
		}
		if (strcmp(command, "delete")==0) { //명령어 delete
			delete_fn(&playlist);
		}
		if (strcmp(command, "show")==0) { //명령어 show
			show_fn(playlist);
		}
		if (strcmp(command, "show_favorites") == 0) { //명령어 show_favorites
			show_favorites_fn(playlist);
		}
		if (strcmp(command, "exit") == 0) { //명령어 exit
			printf("저장할 파일명을 입력해주세요. >> ");

			char filename1[30]; //저장할 파일명 입력받는 변수
			scanf("%s", filename1);
			FILE* outfile;
			outfile = fopen(filename1, "w"); //파일 열기
			//현재까지 편집한 플레이리스트 출력
			NODE* pNode = playlist.head;
			for (int i = 0; i < playlist.count; i++)
			{
				fprintf(outfile, "%s\t%s\t%.2f\t%.2f\n", pNode->data.title,
					pNode->data.artist, pNode->data.volume, pNode->data.preference);
				pNode = pNode->next; //음악 정보 출력
			}
			//노드 해제
			NODE* pPre = NULL;
			NODE* temp = playlist.head;
			while (temp != NULL) {
				pPre = temp;
				temp = temp->next;
				free(pPre); //할당 해제
			}
			fclose(outfile); //파일 닫기
			return 0;
		}
		if (strcmp(command, "show") != 0 && strcmp(command, "show_favorites") != 0 && strcmp(command, "delete") != 0 
			&& strcmp(command, "add") != 0 && strcmp(command, "exit") != 0) //5가지 명령어가 아닌 명령어를 입력한 경우
		{
			printf("유효하지 않은 명령어입니다.\n"); //[에러 메시지 #3]
			printf("\n");
			continue;
		}
	}
}
