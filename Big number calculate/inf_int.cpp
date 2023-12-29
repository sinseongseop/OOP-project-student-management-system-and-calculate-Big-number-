#define _CRT_SECURE_NO_WARNINGS

#include "inf_int.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

inf_int::inf_int()
{
	this->digits = new char[2];	// 동적할당
	this->digits[0] = '0';		// default 값 0 설정
	this->digits[1] = '\0';
	this->length = 1;
	this->thesign = true;
}

inf_int::inf_int(int n) {
	char buf[100] = {0};

	if (n < 0) {		// 음수 처리
		this->thesign = false;
		n = -n;
	}
	else {
		this->thesign = true;
	}

	int i = 0;
	while (n > 0) {			// 숫자를 문자열로 변환하는 과정
		buf[i] = n % 10 + '0';

		n /= 10;
		i++;
	}

	if (i == 0) {	// 숫자의 절댓값이 0일 경우
		new (this) inf_int();	// 생성자 재호출...gcc에서 컴파일에러가 있다고 함. inf_int()의 경우 별개의 인스턴스가 생성됨. 
	}
	else {
		buf[i] = '\0';

		this->digits = new char[i + 1];
		this->length = i;
		strcpy(this->digits, buf);
	}
}

inf_int::inf_int(const char* str)
{
	// "100"이 들어왔다면 내부 표현에 맞게 "001"로 변환
	// ex) "-1053" -> thesign=false, digits="3501", len=4

	char buf[300] = {0};
	int num_length = (int)strlen(str);
	int num_position = 0; // 몇번째 자리수를 가르키고 있는지 (자리수-1)= num_position으로 생각
	
	if (str[0] == '-') { 	// 부호 처리 
		this->thesign = false;
	}
	else {
		this->thesign = true;
	}
	
	for (int i = num_length-1; i > 0; i--) { // 수를 뒤집어서 저장하는 작업
		buf[num_position] = str[i];
		num_position++;
	}
	
	if (this->thesign == true) {
		buf[num_position] = str[0];
		num_position++;
	}
	else {
		num_length -= 1; // '-'가 차지했던 1칸 길이 제외
	}

	buf[num_position] = '\0';
	this->digits = new char[num_position + 1];
	this->length = num_length;
	strcpy(this->digits, buf);
}

inf_int::inf_int(const inf_int& a) {
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;
}

inf_int::~inf_int() {
	delete digits;		// 메모리 할당 해제
}

inf_int& inf_int::operator=(const inf_int& a)
{
	if (this->digits) {
		delete this->digits;		// 이미 문자열이 있을 경우 제거.
	}
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;

	return *this;
}

bool operator==(const inf_int& a, const inf_int& b)
{
	// we assume 0 is always positive.
	if ((strcmp(a.digits, b.digits) == 0) && a.thesign == b.thesign)	// 부호가 같고, 절댓값이 일치해야함.
		return true;
	return false;
}

bool operator!=(const inf_int& a, const inf_int& b)
{
	return !operator==(a, b);
}

bool operator>(const inf_int& a, const inf_int& b)
{
	if (a.thesign != b.thesign) { // 부호가 다를 경우, a가 양수일 경우 b는 음수, a가 음수일 경우 b는 양수이기에 a의 부호진리값을 반환하면 됨
		return a.thesign;
	
	}else {  
		bool result = true;
		if (a.length > b.length) { // 길이가 긴게 큰 값 (우선 둘다 양수라고 가정)
			result = true;
		}
		else if (a.length < b.length) {
			result = false;
		}
		else {
			for (int i = a.length - 1; i >= 0; i--) { // 길이가 같은 경우 가장 높은 자리 수 부터 비교
				if (a.digits[i] > b.digits[i]) {
					result = true;
					break;
				}else if( a.digits[i]< b.digits[i]) {
					result = false;
					break;
				}
			}
		}

		if (a == b) {
			result = false;
		}

		// 둘 다 양수일 경우 절댓값 비교한 것을 그대로 return
		// 둘 다 음수일 경우 & 같지 않은 경우 반전하여 return

		if ((a.thesign == false) and a!=b) {
			result = not result;
		}
		return result;
	}
}

bool operator<(const inf_int& a, const inf_int& b)
{
	if (operator>(a, b) || operator==(a, b)) {
		return false;
	}
	else {
		return true;
	}
}

inf_int operator+(const inf_int& a, const inf_int& b)
{
	inf_int c;
	unsigned int i;

	if (a.thesign == b.thesign) {	// 이항의 부호가 같을 경우 + 연산자로 연산
		for (i = 0; i < a.length; i++) {
			c.Add(a.digits[i], i + 1);
		}
		for (i = 0; i < b.length; i++) {
			c.Add(b.digits[i], i + 1);
		}

		c.thesign = a.thesign;

		return c;
	}
	else {	// 이항의 부호가 다를 경우 - 연산자로 연산
		c = b;
		c.thesign = a.thesign;

		return a - c;
	}
}

inf_int operator-(const inf_int& a, const inf_int& b)
{
	inf_int c; //같은 경우 default 값(0) 그대로 반환 해주면 됨.
	if (a.thesign == b.thesign) { // 부호가 같을 때 절댓값이 큰 수에서 절댓값이 작은 수를 빼준 후 부호 결정. 
		if (a > b) {
			for (unsigned int i = 0; i < a.length; i++) {
				c.Add(a.digits[i], i + 1);
			}
			for (unsigned int i = 0; i < b.length; i++) {
				c.Sub(b.digits[i], i + 1);
			}
			
			c.thesign = a.thesign;
		}

		else if(a<b) { // a-b= -(b-a) 와 동일함.
			for (unsigned int i = 0; i < b.length; i++) {
				c.Add(b.digits[i], i + 1);
			}
			for (unsigned int i = 0; i < a.length; i++) {
				c.Sub(a.digits[i], i + 1);
			}
			c.thesign = not a.thesign;
		}
		
		return c;
	}
	else {	// 이항의 부호가 다른 경우 + 연산자로 연산
		c = b;
		c.thesign = a.thesign;

		return a + c;
	
	}
}

inf_int operator*(const inf_int& a, const inf_int& b)
{
	inf_int c;
	if (a == 0 or b == 0) { // 0을 곱하면 항상 0
		return c;
	}

	for (unsigned int i = 0; i < a.length; i++) {//각 자리수 끼리의 곱의 덧셈을 해주는 것
		for (unsigned int j = 0; j < b.length; j++) {
			int mul = (a.digits[i]-'0') * (b.digits[j]-'0'); // mul은 0~81의 어떤 정수가 나옴. 
			if (mul >= 10) {
				c.Add(mul / 10 + '0', i + j + 2);
			}
			c.Add(mul % 10 + '0', i + j + 1);
		}
	}
	if (a.thesign == b.thesign) { // 부호가 동일하면 양수 아니면 음수임
		c.thesign = true;
	}
	else {
		c.thesign = false;
	}
	return c;
}


ostream& operator<<(ostream& out, const inf_int& a)
{
	int i;

	if (a.thesign == false) {
		out << '-';
	}
	for (i = a.length - 1; i >= 0; i--) {
		out << a.digits[i];
	}
	return out;
}

void inf_int::Add(const char num, const unsigned int index)	// a의 index 자리수에 n을 더한다. 0<=n<=9, ex) a가 391일때, a.Add(2, 2)의 결과는 411
{
	if (this->length < index) {
		char* temp = (char*)realloc(this->digits, index + 1); // 메모리 크기 재할당

		if (temp == NULL) {		// 할당 실패 예외처리
			cout << "Memory reallocation failed, the program will terminate." << endl;
			exit(0);
		}
		else {
			this->digits = temp;
		}

		this->length = index;					// 길이 지정
		this->digits[this->length] = '\0';	// 널문자 삽입
	}

	if (this->digits[index - 1] < '0') {	// 연산 전에 '0'보다 작은 아스키값인 경우 0으로 채움. 쓰여지지 않았던 새로운 자리수일 경우 발생
		this->digits[index - 1] = '0';
	}

	this->digits[index - 1] += num - '0';	// 값 연산


	if (this->digits[index - 1] > '9') {	// 자리올림이 발생할 경우
		this->digits[index - 1] -= 10;	// 현재 자릿수에서 (아스키값) 10을 빼고
		Add('1', index + 1);			// 윗자리에 1을 더한다
	}
}

void inf_int::Sub(const char num, const unsigned int index) // a의 index 자리수에 n을 뺀다. 0<=n<=9, ex) a가 391일때, a.Add(2, 2)의 결과는 371
{
	if (this->length < index) {
		char* temp = (char*)realloc(this->digits, index + 1); // 메모리 크기 재할당

		if (this->digits == NULL) {		// 할당 실패 예외처리
			cout << "Memory reallocation failed, the program will terminate." << endl;
			exit(0);
		}
		else {
			this->digits = temp;
		}

		this->length = index;					// 길이 지정
		this->digits[this->length] = '\0';	// 널문자 삽입
	}

	if (this->digits[index - 1] < '0') {	// 연산 전에 '0'보다 작은 아스키값인 경우 0으로 채움. 쓰여지지 않았던 새로운 자리수일 경우 발생
		this->digits[index - 1] = '0';
	}

	this->digits[index - 1] -= (num - '0');	// 값 연산

	if (this->digits[index - 1] < '0') {	// 자리수 내림이 발생할 경우
		this->digits[index - 1] += 10;	// 현재 자릿수에서 (아스키값) 10을 더하고
		Sub('1', index + 1);			// 윗자리에 1을 뺀다.
	}
	while (this->length > 1 and this->digits[this->length-1]=='0') { // 뺄셈으로 자리수가 변동되는 걸 반영 ex) 100-99=1 이 되어 길이가 1이됨.
		this->digits[this->length - 1] = '\0';
		this ->length -= 1;
	}

}