#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#define ESC 27

int main() {
	char command;
	printf("��Ƹ��� �ູ�� VC++\n");
	Sleep(3000);
	system("cls");
	

	printf("���ĺ��� �Է����ּ���. (q, w, e, r)\n");
	while (1)
	{
		printf("\ncommand>");
		command = _getch();
		if (command == ESC) {
			printf("���α׷��� ����Ǿ����ϴ�.");
			break;
		}
		else if (command == 'q') {
			printf("qwer �ູ��");
		}
		else if (command == 'w') {
			printf("���� �ʹ� ����");
		}
		else if (command == 'e') {
			printf("�������� ������ 3���");
		}
		else if (command == 'r') {
			printf("�� �ູ�ؿ� ������ �Ф�");
		}
		else {
			printf("�ٽ� �Է��ϼ���.(q, w, e, r)\n");
		}
	}
}