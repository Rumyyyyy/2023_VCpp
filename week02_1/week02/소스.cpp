#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
	printf("��Ƹ��� �ູ�� VC++");
	char i;

	printf("���ĺ��� �Է����ּ���. (p, f, j)\n");
	while (1)
	{
		printf("command> ");
		scanf(" %c", &i);
		if (i == 'q') {
			printf("qwer �ູ��");
			break;
		}
		else if (i == 'w') {
			printf("���� �ʹ� ����");
			break;
		}
		else if (i == 'e') {
			printf("�������� ������ 3���");
			break;
		}
		else if (i == 'r') {
			printf("�� �ູ�ؿ� ������ �Ф�");
			break;
		}
		else {
			printf("�ٽ� �Է��ϼ���.(q, w, e, r)\n");
			scanf("%c", &i);
			continue;
		}
	}
}