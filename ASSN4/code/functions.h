//개발 환경: Visual Studio 2019
#pragma once
//구조체 정의
typedef struct {
	char title[15]; //음악 제목
	char artist[15]; //아티스트 이름
	float volume; //음악의 용량(MB), 0 ~ 50 사이의 실수
	float preference; //음악의 선호도, 0 ~ 10 사이의 실수
}SONG;

typedef struct node{ //노드 선언
	SONG data;
	struct node* next;
}NODE;

typedef struct {
	int count; //플레이리스트 내의 음악 개수
	float storage; //플레이스트에 추가된 음악들의 총 용량
	NODE* head;
}LIST;

//함수 선언
void show_fn(LIST playlist); //플레이리스트 내의 음악 목록을 보여줌
void show_favorites_fn(LIST playlist); //플레이리스트 내 선호도가 높은 음악 목록을 보여줌
void add_fn(LIST* playlist); //새로운 음악을 플레이리스트에 추가
void bubbleSort(LIST* playlist, char pre_alp[5]); 
void delete_fn(LIST* playlist); //선택한 음악을 플레이리스트에서 삭제

