#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

struct student
{
	std::string name;
	std::string secondName;
	std::string lastName;
	int group;
	double money;
	int marks[4];
};

student* createStudent() {
	student* newStudent = new student;
	std::cout << "Enter name: ";
	std::cin >> newStudent->name;
	std::cout << "Enter surname: ";
	std::cin >> newStudent->secondName;
	std::cout << "Enter patronymic: ";
	std::cin >> newStudent->lastName;
	std::cout << "Enter group number: ";
	std::cin >> newStudent->group;
	std::cout << "Enter amount of money: ";
	std::cin >> newStudent->money;
	for (int i = 0; i < 4; i++) {
		std::cout << "Enter mark #" << (i + 1) << ": ";
		std::cin >> newStudent->marks[i];
	}
	return newStudent;
}

void printStudent(student newStudent) 
{
	std::cout << "name: ";
	std::cout << newStudent.name << std::endl;
	std::cout << "surname: ";
	std::cout << newStudent.secondName << std::endl;
	std::cout << "patronymic: ";
	std::cout << newStudent.lastName << std::endl;
	std::cout << "group number: ";
	std::cout << newStudent.group << std::endl;
	std::cout << "amount of money: ";
	std::cout << newStudent.money << std::endl;
	for (int i = 0; i < 4; i++) {
		std::cout << "mark #" << (i + 1) << ": ";
		std::cout << newStudent.marks[i] << " ";
	}
	std::cout << std::endl;
}


int main() {
	WSADATA wsaData;    //для винды нужно
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "Winsock initialization error" << std::endl;
		return 1;
	}

	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET) {
		std::cerr << "Socket creation error" << std::endl;
		return 1;
	}

	sockaddr_in server_addr{};            
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(client_socket, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
		std::cerr << "Server connection error" << std::endl;
		return 1;
	}

	int request = 0;

	while (true) {
		std::cout << "Enter request number (1-3), or -1 to quit: ";
		std::cin >> request;

		if (request == -1) {
			break; // 1-отобразить задание, 2 - добавить нового, 3 - перезаписать
 		}

		send(client_socket, (char*)&request, sizeof(int), 0);

		switch (request) {
		case 1:
		{
			int studentCount = 0;
			recv(client_socket, (char*)&studentCount, sizeof(int), 0);
			for (int i = 0; i < studentCount; i++) {
				student* receivedStudent = new student;
				recv(client_socket, (char*)receivedStudent, sizeof(student), 0);
				printStudent(*receivedStudent);
			}
			break;
		}

		case 2:
		{
			student* newStudent = createStudent();
			send(client_socket, (char*)newStudent, sizeof(student), 0);
			break;
		}

		case 3:
		{
			int studentCount = 0;
			recv(client_socket, (char*)&studentCount, sizeof(int), 0);
			for (int i = 0; i < studentCount; i++) {
				student* receivedStudent = new student;
				recv(client_socket, (char*)receivedStudent, sizeof(student), 0);
				std::cout << i << ") " << std::endl;
				printStudent(*receivedStudent);
				std::cout << "-------------------------------" << std::endl;

			}
			int indexToReplace; /* Index of the student to replace */;
			std::cout << "enter Index of the student to replace: ";
			std::cin >> indexToReplace;
			send(client_socket, (char*)&indexToReplace, sizeof(int), 0);
			student* newStudent = createStudent();
			send(client_socket, (char*)newStudent, sizeof(student), 0);
			break;
		}

		default:
			std::cerr << "Unknown request" << std::endl;
			break;
		}
	}

	closesocket(client_socket);
	WSACleanup();

	return 0;
}
