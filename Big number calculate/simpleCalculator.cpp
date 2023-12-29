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
	string tokens[3]; // (정수)(공백)(연산자)(공백)(정수) 꼴 입력
	cout << "Input: ";
	cin >> tokens[0];
	
	if (tokens[0] == "0") { // 0 이면 계산 종료
		finish = true;
		return;
	}

	cin>> tokens[1] >> tokens[2];

	inf_int first((tokens[0].c_str())); // c_str() 함수는 string 객체를 char* 형태로 바꿔줌
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
