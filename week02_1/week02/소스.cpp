#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
	printf("202218016\n김아름님의\nApplication");
	char i;

	printf("알파벳을 입력해주세요. (p, f, j)\n");
	while (1)
	{
		printf("command> ");
		scanf(" %c", &i);
		if (i == 'p') {
			printf("나는 과제를 겁나 잘하는 학생이다.");
			break;
		}
		else if (i == 'f') {
			printf("나는 과제가 너무 즐겁다.");
			break;
		}
		else if (i == 'j') {
			printf("교수님 과제 더 내주세요.");
			break;
		}
		else {
			printf("다시 입력하세요.(p, f, j)\n");
			continue;
		}
	}
}