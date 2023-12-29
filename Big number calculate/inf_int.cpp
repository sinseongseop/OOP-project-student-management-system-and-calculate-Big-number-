#define _CRT_SECURE_NO_WARNINGS

#include "inf_int.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

inf_int::inf_int()
{
	this->digits = new char[2];	// �����Ҵ�
	this->digits[0] = '0';		// default �� 0 ����
	this->digits[1] = '\0';
	this->length = 1;
	this->thesign = true;
}

inf_int::inf_int(int n) {
	char buf[100] = {0};

	if (n < 0) {		// ���� ó��
		this->thesign = false;
		n = -n;
	}
	else {
		this->thesign = true;
	}

	int i = 0;
	while (n > 0) {			// ���ڸ� ���ڿ��� ��ȯ�ϴ� ����
		buf[i] = n % 10 + '0';

		n /= 10;
		i++;
	}

	if (i == 0) {	// ������ ������ 0�� ���
		new (this) inf_int();	// ������ ��ȣ��...gcc���� �����Ͽ����� �ִٰ� ��. inf_int()�� ��� ������ �ν��Ͻ��� ������. 
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
	// "100"�� ���Դٸ� ���� ǥ���� �°� "001"�� ��ȯ
	// ex) "-1053" -> thesign=false, digits="3501", len=4

	char buf[300] = {0};
	int num_length = (int)strlen(str);
	int num_position = 0; // ���° �ڸ����� ����Ű�� �ִ��� (�ڸ���-1)= num_position���� ����
	
	if (str[0] == '-') { 	// ��ȣ ó�� 
		this->thesign = false;
	}
	else {
		this->thesign = true;
	}
	
	for (int i = num_length-1; i > 0; i--) { // ���� ����� �����ϴ� �۾�
		buf[num_position] = str[i];
		num_position++;
	}
	
	if (this->thesign == true) {
		buf[num_position] = str[0];
		num_position++;
	}
	else {
		num_length -= 1; // '-'�� �����ߴ� 1ĭ ���� ����
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
	delete digits;		// �޸� �Ҵ� ����
}

inf_int& inf_int::operator=(const inf_int& a)
{
	if (this->digits) {
		delete this->digits;		// �̹� ���ڿ��� ���� ��� ����.
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
	if ((strcmp(a.digits, b.digits) == 0) && a.thesign == b.thesign)	// ��ȣ�� ����, ������ ��ġ�ؾ���.
		return true;
	return false;
}

bool operator!=(const inf_int& a, const inf_int& b)
{
	return !operator==(a, b);
}

bool operator>(const inf_int& a, const inf_int& b)
{
	if (a.thesign != b.thesign) { // ��ȣ�� �ٸ� ���, a�� ����� ��� b�� ����, a�� ������ ��� b�� ����̱⿡ a�� ��ȣ�������� ��ȯ�ϸ� ��
		return a.thesign;
	
	}else {  
		bool result = true;
		if (a.length > b.length) { // ���̰� ��� ū �� (�켱 �Ѵ� ������ ����)
			result = true;
		}
		else if (a.length < b.length) {
			result = false;
		}
		else {
			for (int i = a.length - 1; i >= 0; i--) { // ���̰� ���� ��� ���� ���� �ڸ� �� ���� ��
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

		// �� �� ����� ��� ���� ���� ���� �״�� return
		// �� �� ������ ��� & ���� ���� ��� �����Ͽ� return

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

	if (a.thesign == b.thesign) {	// ������ ��ȣ�� ���� ��� + �����ڷ� ����
		for (i = 0; i < a.length; i++) {
			c.Add(a.digits[i], i + 1);
		}
		for (i = 0; i < b.length; i++) {
			c.Add(b.digits[i], i + 1);
		}

		c.thesign = a.thesign;

		return c;
	}
	else {	// ������ ��ȣ�� �ٸ� ��� - �����ڷ� ����
		c = b;
		c.thesign = a.thesign;

		return a - c;
	}
}

inf_int operator-(const inf_int& a, const inf_int& b)
{
	inf_int c; //���� ��� default ��(0) �״�� ��ȯ ���ָ� ��.
	if (a.thesign == b.thesign) { // ��ȣ�� ���� �� ������ ū ������ ������ ���� ���� ���� �� ��ȣ ����. 
		if (a > b) {
			for (unsigned int i = 0; i < a.length; i++) {
				c.Add(a.digits[i], i + 1);
			}
			for (unsigned int i = 0; i < b.length; i++) {
				c.Sub(b.digits[i], i + 1);
			}
			
			c.thesign = a.thesign;
		}

		else if(a<b) { // a-b= -(b-a) �� ������.
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
	else {	// ������ ��ȣ�� �ٸ� ��� + �����ڷ� ����
		c = b;
		c.thesign = a.thesign;

		return a + c;
	
	}
}

inf_int operator*(const inf_int& a, const inf_int& b)
{
	inf_int c;
	if (a == 0 or b == 0) { // 0�� ���ϸ� �׻� 0
		return c;
	}

	for (unsigned int i = 0; i < a.length; i++) {//�� �ڸ��� ������ ���� ������ ���ִ� ��
		for (unsigned int j = 0; j < b.length; j++) {
			int mul = (a.digits[i]-'0') * (b.digits[j]-'0'); // mul�� 0~81�� � ������ ����. 
			if (mul >= 10) {
				c.Add(mul / 10 + '0', i + j + 2);
			}
			c.Add(mul % 10 + '0', i + j + 1);
		}
	}
	if (a.thesign == b.thesign) { // ��ȣ�� �����ϸ� ��� �ƴϸ� ������
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

void inf_int::Add(const char num, const unsigned int index)	// a�� index �ڸ����� n�� ���Ѵ�. 0<=n<=9, ex) a�� 391�϶�, a.Add(2, 2)�� ����� 411
{
	if (this->length < index) {
		char* temp = (char*)realloc(this->digits, index + 1); // �޸� ũ�� ���Ҵ�

		if (temp == NULL) {		// �Ҵ� ���� ����ó��
			cout << "Memory reallocation failed, the program will terminate." << endl;
			exit(0);
		}
		else {
			this->digits = temp;
		}

		this->length = index;					// ���� ����
		this->digits[this->length] = '\0';	// �ι��� ����
	}

	if (this->digits[index - 1] < '0') {	// ���� ���� '0'���� ���� �ƽ�Ű���� ��� 0���� ä��. �������� �ʾҴ� ���ο� �ڸ����� ��� �߻�
		this->digits[index - 1] = '0';
	}

	this->digits[index - 1] += num - '0';	// �� ����


	if (this->digits[index - 1] > '9') {	// �ڸ��ø��� �߻��� ���
		this->digits[index - 1] -= 10;	// ���� �ڸ������� (�ƽ�Ű��) 10�� ����
		Add('1', index + 1);			// ���ڸ��� 1�� ���Ѵ�
	}
}

void inf_int::Sub(const char num, const unsigned int index) // a�� index �ڸ����� n�� ����. 0<=n<=9, ex) a�� 391�϶�, a.Add(2, 2)�� ����� 371
{
	if (this->length < index) {
		char* temp = (char*)realloc(this->digits, index + 1); // �޸� ũ�� ���Ҵ�

		if (this->digits == NULL) {		// �Ҵ� ���� ����ó��
			cout << "Memory reallocation failed, the program will terminate." << endl;
			exit(0);
		}
		else {
			this->digits = temp;
		}

		this->length = index;					// ���� ����
		this->digits[this->length] = '\0';	// �ι��� ����
	}

	if (this->digits[index - 1] < '0') {	// ���� ���� '0'���� ���� �ƽ�Ű���� ��� 0���� ä��. �������� �ʾҴ� ���ο� �ڸ����� ��� �߻�
		this->digits[index - 1] = '0';
	}

	this->digits[index - 1] -= (num - '0');	// �� ����

	if (this->digits[index - 1] < '0') {	// �ڸ��� ������ �߻��� ���
		this->digits[index - 1] += 10;	// ���� �ڸ������� (�ƽ�Ű��) 10�� ���ϰ�
		Sub('1', index + 1);			// ���ڸ��� 1�� ����.
	}
	while (this->length > 1 and this->digits[this->length-1]=='0') { // �������� �ڸ����� �����Ǵ� �� �ݿ� ex) 100-99=1 �� �Ǿ� ���̰� 1�̵�.
		this->digits[this->length - 1] = '\0';
		this ->length -= 1;
	}

}