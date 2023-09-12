#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
	printf("김아름의 행복한 VC++");
	char i;

	printf("알파벳을 입력해주세요. (p, f, j)\n");
	while (1)
	{
		printf("command> ");
		scanf(" %c", &i);
		if (i == 'q') {
			printf("qwer 행복해");
			break;
		}
		else if (i == 'w') {
			printf("과제 너무 좋다");
			break;
		}
		else if (i == 'e') {
			printf("다음부턴 과제량 3배다");
			break;
		}
		else if (i == 'r') {
			printf("안 행복해요 교수님 ㅠㅠ");
			break;
		}
		else {
			printf("다시 입력하세요.(q, w, e, r)\n");
			scanf("%c", &i);
			continue;
		}
	}
}