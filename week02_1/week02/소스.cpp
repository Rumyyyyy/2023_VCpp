#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
	printf("202218016\n��Ƹ�����\nApplication");
	char i;

	printf("���ĺ��� �Է����ּ���. (p, f, j)\n");
	while (1)
	{
		printf("command> ");
		scanf(" %c", &i);
		if (i == 'p') {
			printf("���� ������ �̳� ���ϴ� �л��̴�.");
			break;
		}
		else if (i == 'f') {
			printf("���� ������ �ʹ� ��̴�.");
			break;
		}
		else if (i == 'j') {
			printf("������ ���� �� ���ּ���.");
			break;
		}
		else {
			printf("�ٽ� �Է��ϼ���.(p, f, j)\n");
			continue;
		}
	}
}