#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NOPAIR 1
#define DOUBLE 2
#define STRAIGHT 3
#define TRIPLE 4

//세팅: 칩 50개, 최대 10회

void print_game_status(int round, int user_chips, int com_chips);
void card_shuffle(int* shared_card1, int* shared_card2, int* user_card, int* computer_card);
void print_card_info(int shared_card1, int shared_card2, int user_card, int computer_card);
int is_valid_num(int a, int b, int num);
int user_turn(int user_chips, int* user_betting_chips, int betted_chips, int turn);
int calc_hand(int card, int shard_card1, int shard_card2);
int computer_turn(int user_hand, int com_chips, int* com_betting_chips, int betted_chips, int turn);
int calc_winner(int shared_card1, int shared_card2, int user_card, int computer_card);

int main()
{
	srand(time(NULL));

	//변수선언
	int round = 1;
	int user_chips = 50;
	int	com_chips = 50;

	int shared_card1;
	int shared_card2;
	int user_card;
	int computer_card;

	int user_hand;
	int user_betting_chips;
	int com_betting_chips;

	int past_winner = 0; //이전 결과, 초기화는 컴퓨터가 이긴 걸로
	int turn_count;

	int game_result; 

	while (1)
	{
		system("cls"); // 리눅스 또는 맥의 경우 system("clear");
		
		if (round > 10 || user_chips == 0 || com_chips == 0)
			break;
		print_game_status(round, user_chips, com_chips);

		card_shuffle(&shared_card1, &shared_card2, &user_card, &computer_card);
		print_card_info(shared_card1, shared_card2, user_card, computer_card);

		user_betting_chips = 1;
		com_betting_chips = 1;
		game_result = -1;
		if (user_chips > 1 && com_chips > 1)
		{
			printf("┏━━━━━━━━━━┓\n");
			printf("┃ Betting  ┃\n");
			printf("┗━━━━━━━━━━┛\n");

			printf("┏━━━━━━━━━━━━━━━━━━━━┓\n");
			printf("┃ User: 1  ┃ Com: 1  ┃\n");
			printf("┗━━━━━━━━━━━━━━━━━━━━┛\n");

			turn_count = 1;

			while (1)
			{
				int betting_result;
				if ((turn_count + past_winner) % 2 == 1)
					betting_result = user_turn(user_chips, &user_betting_chips, com_betting_chips, turn_count);
				else
				{
					user_hand = calc_hand(user_card, shared_card1, shared_card2);
					betting_result = computer_turn(user_hand, com_chips, &com_betting_chips, user_betting_chips, turn_count);
				}
				printf("┏━━━━━━━━━━━━━━━━━━━━━━┓\n");
				printf("┃ User: %2d  ┃ Com: %2d  ┃\n", user_betting_chips, com_betting_chips);
				printf("┗━━━━━━━━━━━━━━━━━━━━━━┛\n");

				if (betting_result == 0)
				{
					game_result = calc_winner(shared_card1, shared_card2, user_card, computer_card);
				}
				else if (betting_result == -1)
				{
					game_result = 1 - (turn_count + past_winner) % 2;
				}
				turn_count++;
				//결과출력
				if (game_result == -1)
					continue;

				printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
				printf("Betting Finished\n");
				break;
			}//while
		}//if문 끝
		
		//카드 결과 출력
		printf("┏━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━┓\n");
		printf("┃ Computer ┃  Shared Cards       ┃  User    ┃\n");
		printf("┣━━━━━━━━━━╋━━━━━━━━━━┳━━━━━━━━━━╋━━━━━━━━━━┫\n");
		printf("┃ ┏━━━━━━┓ ┃ ┏━━━━━━┓ ┃ ┏━━━━━━┓ ┃ ┏━━━━━━┓ ┃\n");
		printf("┃ ┃♠    ┃ ┃ ┃♠    ┃ ┃ ┃♠    ┃ ┃ ┃♠    ┃ ┃\n");
		printf("┃ ┃      ┃ ┃ ┃      ┃ ┃ ┃      ┃ ┃ ┃      ┃ ┃\n");
		printf("┃ ┃  %2d  ┃ ┃ ┃  %2d  ┃ ┃ ┃  %2d  ┃ ┃ ┃  %2d  ┃ ┃\n", computer_card, shared_card1, shared_card2, user_card);
		printf("┃ ┃      ┃ ┃ ┃      ┃ ┃ ┃      ┃ ┃ ┃      ┃ ┃\n");
		printf("┃ ┃    ♠┃ ┃ ┃    ♠┃ ┃ ┃    ♠┃ ┃ ┃    ♠┃ ┃\n");
		printf("┃ ┗━━━━━━┛ ┃ ┗━━━━━━┛ ┃ ┗━━━━━━┛ ┃ ┗━━━━━━┛ ┃\n");
		printf("┗━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━┛━━━━━━━━━━┛\n");

		if (game_result == 1)
		{
			user_chips += com_betting_chips;
			com_chips -= com_betting_chips;

			printf("┏━━━━━━━━━━━┓\n");
			printf("┃ User win! ┃\n");
			printf("┗━━━━━━━━━━━┛\n");

			printf("┏━━━━━━━━━┳━━━━┓\n");
			printf("┃ User    ┃ %2d ┃\n", user_chips);
			printf("┃ Computer┃ %2d ┃\n", com_chips);
			printf("┗━━━━━━━━━┻━━━━┛\n");
		}
		else if (game_result == 0)
		{
			user_chips -= user_betting_chips;
			com_chips += user_betting_chips;

			printf("┏━━━━━━━━━━━━━━━┓\n");
			printf("┃ Computer win! ┃\n");
			printf("┗━━━━━━━━━━━━━━━┛\n");

			printf("┏━━━━━━━━━┳━━━━┓\n");
			printf("┃ User    ┃ %2d ┃\n", user_chips);
			printf("┃ Computer┃ %2d ┃\n", com_chips);
			printf("┗━━━━━━━━━┻━━━━┛\n");
		}

		past_winner = game_result;

		round++;

		int proceed;
		printf("Proceed or Not? [Go: 1, End: -1]: ");
		scanf("%d", &proceed);
		if (proceed == -1)
			break;
	}//while문 종료-게임종료

	//게임결과 출력
	printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

	printf("Number of Games: %d\n", round-1);
	printf("Chips remaining:\n");
	printf("┏━━━━━━━━━┳━━━━┓\n");
	printf("┃ User    ┃ %2d ┃\n", user_chips);
	printf("┃ Computer┃ %2d ┃\n", com_chips);
	printf("┗━━━━━━━━━┻━━━━┛\n");

	if (user_chips >= com_chips)
		printf("User win!\n");

	else
		printf("Computer win!\n");
}

void print_game_status(int round, int user_chips, int com_chips)
{
	printf("┏━━━━━━━━━━━━━━━━━┓\n");
	printf("┃ %2d Game Starts! ┃\n", round);
	printf("┗━━━━━━━━━━━━━━━━━┛\n");

	printf("Chips remaining:\n");
	printf("┏━━━━━━━━━┳━━━━┓\n");
	printf("┃ User    ┃ %2d ┃\n", user_chips);
	printf("┃ Computer┃ %2d ┃\n", com_chips);
	printf("┗━━━━━━━━━┻━━━━┛\n");

	return;
}


void card_shuffle(int* shared_card1, int* shared_card2, int* user_card, int* computer_card)
{
	*shared_card1 = rand() % 10 + 1;
	*shared_card2 = rand() % 10 + 1;
	*user_card = rand() % 10 + 1;
	*computer_card = rand() % 10 + 1;
	return;
}

void print_card_info(int shared_card1, int shared_card2, int user_card, int computer_card)
{
	printf("┏━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━┓\n");
	printf("┃ Computer ┃  Shared Cards       ┃  User    ┃\n");
	printf("┣━━━━━━━━━━╋━━━━━━━━━━┳━━━━━━━━━━╋━━━━━━━━━━┫\n");
	printf("┃ ┏━━━━━━┓ ┃ ┏━━━━━━┓ ┃ ┏━━━━━━┓ ┃ ┏━━━━━━┓ ┃\n");
	printf("┃ ┃♠    ┃ ┃ ┃♠    ┃ ┃ ┃♠    ┃ ┃ ┃♠    ┃ ┃\n"); 
	printf("┃ ┃      ┃ ┃ ┃      ┃ ┃ ┃      ┃ ┃ ┃      ┃ ┃\n");
	printf("┃ ┃  %2d  ┃ ┃ ┃  %2d  ┃ ┃ ┃  %2d  ┃ ┃ ┃   ?  ┃ ┃\n", computer_card, shared_card1, shared_card2);
	printf("┃ ┃      ┃ ┃ ┃      ┃ ┃ ┃      ┃ ┃ ┃      ┃ ┃\n");
	printf("┃ ┃    ♠┃ ┃ ┃    ♠┃ ┃ ┃    ♠┃ ┃ ┃    ♠┃ ┃\n");
	printf("┃ ┗━━━━━━┛ ┃ ┗━━━━━━┛ ┃ ┗━━━━━━┛ ┃ ┗━━━━━━┛ ┃\n");
	printf("┗━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━┛━━━━━━━━━━┛\n");
	return;
}

int is_valid_num(int a, int b, int num)
{
	if (num >= a && num <= b)
		return 1;
	else return 0;
}

int user_turn(int user_chips, int* user_betting_chips, int betted_chips, int turn)
{
	int num;
	while (1)
	{
		printf("USER ┃ [Call: 1 | Raise: 2 | Fold: 3]: ");
		scanf("%d", &num);
		if (is_valid_num(1, 3, num) && !(num == 1 && turn == 1))
			break;
		printf("USER ┃ [Invalid input]\n");
	}
	int raise_chips;
	switch (num)
	{
	case 1: //call
		*user_betting_chips = betted_chips;
		if (*user_betting_chips > user_chips) 
			*user_betting_chips = user_chips;
		return 0;
	case 2: //raise
		printf("USER ┃ [Input number of chips for Raise]: ");
		scanf("%d", &raise_chips);
		*user_betting_chips = betted_chips + raise_chips;
		return raise_chips;
	default: //fold
		return -1;
	}
}

int calc_hand(int card, int shared_card1, int shared_card2)
{
	int a, b, c; //a>=b>=c
	int temp;
	a = card, b = shared_card1, c = shared_card2;
	
	if (b >= a && b >= c) //b가 가장 큰 경우
	{
		temp = a;
		a = b;
		b = temp;
	}
	else if (c >= a && c >= b) //c가 가장 큰 경우
	{
		temp = a;
		a = c;
		c = a;
	}

	//제일 큰 수 구했으니 나머지 두개 비교해서 더 큰 것 b에 저장
	if (c >= b)
	{
		temp = b;
		b = c;
		c = temp;
	}//수 정렬 끝(a>=b>=c)

	//triple
	if (a == b && b == c)
		return TRIPLE;
	//straight
	else if (a - b == 1 && b - c == 1)
		return STRAIGHT;
	//double
	else if (card == shared_card1 || card == shared_card2)
		return DOUBLE;
	//no pair
	else
		return NOPAIR;
}

int computer_turn(int user_hand, int com_chips, int* com_betting_chips, int betted_chips, int turn)
{
	if (turn == 1)
	{
		*com_betting_chips = 2;
		printf("COM ┃ Raise, +1\n");
		return 1; //raise 1
	}

	int rate = 0;
	if (user_hand == DOUBLE || user_hand == STRAIGHT || user_hand == TRIPLE)
	{
		rate = rand() % 100 + 1;
		if (rate <= 70)
		{
			printf("COM ┃ Fold\n");
			return -1; //fold
		}
		
		else
		{
			*com_betting_chips = betted_chips;
			if (com_chips < *com_betting_chips)
			{
				*com_betting_chips = com_chips;
			}
			printf("COM ┃ Call\n");
			return 0; //call
		}
			
	}
	else
	{
		rate = rand() % 100 + 1;
		if (rate <= 50)
		{
			*com_betting_chips = betted_chips;
			if (com_chips < *com_betting_chips)
			{
				*com_betting_chips = com_chips;
			}
			printf("COM ┃ Call\n");
			return 0; //call
		}
		else
		{
			int raise_chips = rand() % 5 + 1;
			*com_betting_chips = betted_chips + raise_chips;
			if (com_chips < *com_betting_chips)
			{
				*com_betting_chips = com_chips;
				printf("COM ┃ Call\n");
				return 0;
			}
			printf("COM ┃ Raise, +%d\n",raise_chips);
			return *com_betting_chips;
		}
	}
}

int calc_winner(int shared_card1, int shared_card2, int user_card, int computer_card)
{
	int a = calc_hand(user_card, shared_card1, shared_card2);
	int b = calc_hand(computer_card, shared_card1, shared_card2);

	if (a > b)
		return 1; //user 승
	else if (a == b) //같은 hand
	{
		if (user_card >= computer_card)
			return 1; //user 승
		else
			return 0; //computer 승
	}
	else
		return 0; //computer 승
}

