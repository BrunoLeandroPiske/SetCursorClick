/*
	Project Developec by Bruno 20/08/2016
*/

#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>

using namespace std;

void Menu();
void CaptureMousePosition();
POINT GetMousePosition();
void SetWindowsFormName();
void SetMousePosition(POINT& mp);
void LeftClick();
void RunAutomation();
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam);
void ReadFile();
void SaveFile();

boolean sair = false;
POINT mouse;
string windowsname;

int main()
{
	ReadFile();
	while (!sair) {
		Menu();
	}
	system("cls");
	cout << "Sair do programa..." << endl;
	system("pause");
    return 0;
}

void Menu() {
	system("cls");
	cout << "-----> Auto click automation <-----" << endl;
	cout << "1 - Capturar Ponto de click" << endl;
	cout << "2 - Setar nome janela de disparo de click" << endl;
	cout << "3 - Iniciar Programa" << endl;
	cout << "4 - Sair" << endl;
	char option;
	cin >> option;
	switch (option) {
	case '1':
		CaptureMousePosition();
		SaveFile();
		break;
	case '2':
		SetWindowsFormName();
		SaveFile();
		break;
	case '3':
		RunAutomation();
		break;
	case '4':
		sair = true;
		break;
	default:
		break;
	}
}

void CaptureMousePosition() {
	system("cls");
	cout << "Posicione o cursor onde o click devera ser realizado " << endl;
	system("pause");
	mouse = GetMousePosition();
	cout << "Posicao capturada: " << endl;
	cout << "X : " << mouse.x << endl;
	cout << "Y : " << mouse.y << endl;
	system("pause");
}

POINT GetMousePosition() {
	static POINT m;
	POINT mouse;
	GetCursorPos(&mouse);
	m.x = mouse.x;
	m.y = mouse.y;
	return m;
}

void SetWindowsFormName() {
	system("cls");
	cout << "Digite o nome da Janela que ira disparar o evento de click: " << endl;
	cin >> windowsname;
	system("pause");
}

void SetMousePosition(POINT& mp) {
	long fScreenWidth = GetSystemMetrics(SM_CXSCREEN) - 1;
	long fScreenHeight = GetSystemMetrics(SM_CYSCREEN) - 1;

	float fx = mp.x * (65535.0f / fScreenWidth);
	float fy = mp.y * (65535.0f / fScreenHeight);

	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;

	Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

	Input.mi.dx = (long)fx;
	Input.mi.dy = (long)fy;

	SendInput(1, &Input, sizeof(INPUT));
}

void LeftClick() {
	INPUT Input = { 0 };

	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &Input, sizeof(INPUT));

	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &Input, sizeof(INPUT));
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam) {

	int length = ::GetWindowTextLength(hwnd);
	if (0 == length) return TRUE;

	TCHAR* buffer;
	buffer = new TCHAR[length + 1];
	memset(buffer, 0, (length + 1) * sizeof(TCHAR));

	GetWindowText(hwnd, buffer, length + 1);
	wstring pr_windowTitle(&buffer[0]); //convert to wstring
	string windowTitle(pr_windowTitle.begin(), pr_windowTitle.end()); //and convert to string.
	delete[] buffer;

	if (windowTitle.find(windowsname) != string::npos) {
		cout << "Iniciando automacao de click";
		SetMousePosition(mouse);
		LeftClick();
		system("cls");
		cout << "Rodando . . ." << endl;
	}

	return TRUE;
}

void RunAutomation() {
	system("cls");
	cout << "-----> Auto click automation <-----" << endl;
	cout << "Rodando . . ." << endl;
	while (true) {
		EnumWindows(EnumWindowsProc, NULL);
		Sleep(1000);
	}
}

void SaveFile() {
	ofstream file;
	file.open("C://RobotConf.txt");
	file << "#Configuracao de posicao do cursor e click" << endl;
	file << "X = " << mouse.x << endl;
	file << "Y = " << mouse.y << endl;
	file << "N = " << windowsname << endl;
	file.close();
}

void ReadFile() {
	ifstream file;
	string line;
	string value;

	size_t length;
	char buffer[30];

	file.open("C://RobotConf.txt");
	if (file.is_open()) {
		while (getline(file, line)) {
			if (line[1] != '#') {
				switch (line[0]) {
				case 'X':
					value = line.substr(4, (sizeof(line) - 4));
					mouse.x = atol(value.c_str());
					break;
				case 'Y':
					value = line.substr(4, (sizeof(line) - 4));
					mouse.y = atol(value.c_str());
					break;
				case 'N':
					value = line.substr(4, (sizeof(line) - 4));
					windowsname = value;
					break;
				default:
					break;
				}
			}

		}
	}
}
