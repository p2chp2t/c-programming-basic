//개발 환경: Visual Studio 2019
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"


//[3.플레이리스트 명령어 구현]
//A. show 명령어 구현
void show_fn(LIST playlist)
{
	printf("                                      PLAYLIST\n");
	printf("====================================================================================\n");
	printf(" No.          Title                  Artist             Volume          Preference\n");
	printf("------------------------------------------------------------------------------------\n");
	if (playlist.count == 0) //플레이리스트에 음악이 하나도 없을 때
		printf("Empty playlist!\n"); //[에러 메시지 #4]
	else
	{
		NODE* pNode = playlist.head;
		for (int i = 0; i < playlist.count; i++)
		{
			printf("#%d  |%14s     |    %14s     |      %4.2f MB   |      %4.2f\n", i + 1, pNode->data.title,
				pNode->data.artist, pNode->data.volume, pNode->data.preference);
			pNode = pNode->next;
		}
	}
	printf("------------------------------------------------------------------------------------\n");
	printf("Total number of songs: %d\n", playlist.count);
	printf("Storage: %5.2f MB\n", playlist.storage);
	printf("====================================================================================\n");
	printf("\n");
	return;
}

//B. show_favorites 명령어 구현
void show_favorites_fn(LIST playlist)
{
	printf("상위 몇 개의 음악을 출력할까요? >> ");
	int top_n; //출력할 음악의 개수를 입력받는 변수
	scanf("%d", &top_n);

	float storage = 0; //용량 저장하는 변수

	if (top_n > playlist.count) //추출하고자 하는 음악 수가 전체 음악 수보다 많을 때
		printf("추출하고자 하는 음악의 수가 전체 음악의 수보다 많습니다.\n"); //[에러 메시지 #5]
	else if (top_n < 1) //추출하고자 하는 음악 수가 1 미만일 때
		printf("추출하고자 하는 음악의 수는 1 이상이어야 합니다.\n"); //[에러 메시지 #6]
	else
	{
		bubbleSort(&playlist, "pre"); //선호도 순서대로 bubble sort
		//상위 top_n개의 음악 정보 출력
		printf("                                     FAVORITES\n");
		printf("====================================================================================\n");
		printf(" No.          Title                  Artist             Volume          Preference\n");
		printf("------------------------------------------------------------------------------------\n");
		NODE* pNode = playlist.head;
		for (int i = 0; i < top_n; i++)
		{
			printf("#%d  |%14s     |    %14s     |      %4.2f MB   |      %4.2f\n", i + 1, pNode->data.title,
				pNode->data.artist, pNode->data.volume, pNode->data.preference);
			storage += pNode->data.volume;
			pNode = pNode->next;
		}
	}
	printf("------------------------------------------------------------------------------------\n");
	printf("Total number of songs: %d\n", top_n);
	printf("Storage: %5.2f MB\n", storage);
	printf("====================================================================================\n");
	printf("\n");
	bubbleSort(&playlist, "alp"); //다시 사전 순서대로 정렬해 원래 상태로 되돌리기
	return;
}

//선호도 순과 사전 순에 따라 playlist를 sort하여 정렬하는 함수
void bubbleSort(LIST* playlist, char pre_alp[5])
{
	NODE* pNode;
	NODE temp;
	for (int current = 0; current < playlist->count - 1; current++)
	{
		pNode = playlist->head;
		for (int walker = 0; walker < playlist->count - current - 1; walker++)
		{
			if (strcmp(pre_alp, "pre") == 0) //선호도
			{
				if (pNode->data.preference < pNode->next->data.preference)
				{
					strcpy(temp.data.artist, pNode->data.artist);
					strcpy(temp.data.title, pNode->data.title);
					temp.data.volume = pNode->data.volume;
					temp.data.preference = pNode->data.preference; //temp=pNode

					strcpy(pNode->data.artist, pNode->next->data.artist);
					strcpy(pNode->data.title, pNode->next->data.title);
					pNode->data.volume = pNode->next->data.volume;
					pNode->data.preference = pNode->next->data.preference; //pNode=pNode->next

					strcpy(pNode->next->data.artist, temp.data.artist);
					strcpy(pNode->next->data.title, temp.data.title);
					pNode->next->data.volume = temp.data.volume;
					pNode->next->data.preference = temp.data.preference; //pNode->next=temp
				}
			}
			if (strcmp(pre_alp, "alp") == 0) //사전순서
			{
				if (strcmp(pNode->data.title, pNode->next->data.title) > 0)
				{
					strcpy(temp.data.artist, pNode->data.artist);
					strcpy(temp.data.title, pNode->data.title);
					temp.data.volume = pNode->data.volume;
					temp.data.preference = pNode->data.preference; //temp=pNode

					strcpy(pNode->data.artist, pNode->next->data.artist);
					strcpy(pNode->data.title, pNode->next->data.title);
					pNode->data.volume = pNode->next->data.volume;
					pNode->data.preference = pNode->next->data.preference; //pNode=pNode->next

					strcpy(pNode->next->data.artist, temp.data.artist);
					strcpy(pNode->next->data.title, temp.data.title);
					pNode->next->data.volume = temp.data.volume;
					pNode->next->data.preference = temp.data.preference; //pNode->next=temp
				}
			}
			pNode = pNode->next;
		}
	}
	return;
}

//C. delete 명령어 구현
void delete_fn(LIST* playlist)
{
	printf("삭제할 음악의 타이틀을 입력해주세요. >> ");
	char delete[15]; //삭제할 음악의 title을 입력받는 변수
	int check = 0;
	scanf("%s", delete);

	NODE* pPre = NULL;
	NODE* pNext = playlist->head;
	for (int i = 0; i < playlist->count; i++) { //플레이리스트에 삭제하고자 하는 음악이 있는지 확인
		if (strcmp(pNext->data.title, delete) == 0)  //같은 음악이 있는 경우
		{
			printf("====================================================================================\n");
			printf(" No.          Title                  Artist             Volume          Preference\n");
			printf("------------------------------------------------------------------------------------\n");
			printf("#%d  |%14s     |    %14s     |      %4.2f MB   |      %4.2f\n", i + 1, pNext->data.title,
				pNext->data.artist, pNext->data.volume, pNext->data.preference);
			printf("------------------------------------------------------------------------------------\n");
			break;
		}
		else if (i == playlist->count - 1) //for문의 마지막을 돌 때
		{
			printf("플레이리스트에 해당 음악(%s)이 없습니다.\n", delete); //[에러 메시지 #7]
			check = 1; //플레이리스트에 삭제하고자 하는 음악이 없다는 신호
		}
		pPre = pNext;
		pNext = pNext->next;
	}
	if (check == 1) //플레이리스트에 삭제하고자 하는 음악이 없는 경우
		return;

	//플레이리스트에서 음악 삭제
	(playlist->count)--;
	playlist->storage -= pNext->data.volume;
	NODE* temp;
	if (playlist->head == pNext) // 첫 번째 노드 삭제 하는 경우
	{
		playlist->head = pNext->next;
		free(pNext); //할당해제
	}
	else     //나머지(중간 노드, 마지막 노드)의 경우
	{
		for (temp = playlist->head; temp->next != NULL; temp = temp->next)
		{
			if (temp->next == pNext)
				break;
		}
		if (temp->next == pNext)
		{
			temp->next = pNext->next;
			free(pNext); //할당해제
		}
	}
	printf("위 음악이 삭제되었습니다.\n");
	printf("\n");
	return;
}

//D. add 명령어 구현
void add_fn(LIST* playlist)
{
	SONG new; //새로 받을 노래
	printf("추가할 음악의 타이틀을 입력해주세요. >> ");
	scanf("%s", new.title);
	printf("추가할 음악의 아티스트를 입력해주세요. >> ");
	scanf("%s", new.artist);
	printf("추가할 음악의 용량을 입력해주세요. >> ");
	scanf("%f", &new.volume);
	printf("추가할 음악의 선호도를 입력해주세요. >> ");
	scanf("%f", &new.preference); //새로 받을 노래 정보 입력 받기

	NODE* node = (NODE*)malloc(sizeof(NODE)); //노드 동적할당
	strcpy(node->data.title, new.title);
	strcpy(node->data.artist, new.artist);
	node->data.volume = new.volume;
	node->data.preference = new.preference;

	if ((playlist->storage + node->data.volume) <= 50)
	{
		NODE* pPre = NULL;
		NODE* pNext = playlist->head; //pPre와 pNext 사이에 node가 들어간다
		for (int i = 0; i < playlist->count; i++) { //node가 들어갈 자리를 찾는 반복문
			if (strcmp(pNext->data.title, node->data.title) > 0)
			{
				break; //node가 삽입될 위치 찾으면 반복문 탈출
			}
			if (strcmp(pNext->data.title, node->data.title) == 0 && strcmp(pNext->data.artist, node->data.artist) == 0) //같은 음악이 있는 경우
			{
				printf("해당 음악이 이미 플레이리스트 내에 존재합니다.\n"); //[에러 메시지 #8]
				return;
			}
			pPre = pNext;
			pNext = pNext->next;
		}
		if (pPre == NULL) { //리스트의 처음 또는 빈 리스트에 삽입되는 경우
			node->next = pNext;
			playlist->head = node;
		}
		else { //리스트의 중간 또는 끝에 삽입되는 경우
			node->next = pNext;
			pPre->next = node;
		}
		playlist->storage += node->data.volume; //플레이리스트 용량 업데이트
		playlist->count++; //플레이리스트 count 업데이트
	}
	else //용량(50)을 초과하여 플레이리스트에 추가되지 않는 경우
	{
		printf("용량 초과! 음악(%s)은 추가되지 않았습니다.\n", node->data.title); //[에러 메시지 #2]
	}
	printf("\n");
	return;
}
