#include "inf_int.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

class calculator {
private:
	bool finish = false;
	inline void calculate();
public:
	void run();
};

void calculator::run() {
	while (not finish) {
		calculate();
	}
}

 inline void calculator::calculate() {
	string tokens[3]; // (����)(����)(������)(����)(����) �� �Է�
	cout << "Input: ";
	cin >> tokens[0];
	
	if (tokens[0] == "0") { // 0 �̸� ��� ����
		finish = true;
		return;
	}

	cin>> tokens[1] >> tokens[2];

	inf_int first((tokens[0].c_str())); // c_str() �Լ��� string ��ü�� char* ���·� �ٲ���
	inf_int second((tokens[2].c_str()));
	
	cout << "Output: ";

	if (tokens[1] == "+") {
		cout << first + second << endl;
	}else if(tokens[1] == "-") {
		cout << first - second << endl;
	}
	else if (tokens[1] == "*") {
		cout << first * second << endl;
	}
}

int main()
{
	calculator cal;
	cal.run();
	return 0;
}
