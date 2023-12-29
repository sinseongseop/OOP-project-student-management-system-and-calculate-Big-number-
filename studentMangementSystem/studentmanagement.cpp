#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm> 

using namespace std;

class Student {
private:
	string name;
	string id;
	string department;
	string birth_year;
	string telephone;
	string admission_year;

public:
	Student() {};
	Student(string name, string id, string birth_year, string department, string tel);
	void set_student_information(string name, string id, string birth_year, string department, string telephone); // 학생의 모든 정보를 설정하는 함수
	void print_information(); // 학생의 모든정보를 출력하는 함수
	string get_name();
	string get_id();
	string get_department();
	string get_birth_year();
	string get_telephone();
	string get_admission_year();
};

class StudentDB {
private:
	Student student[300];
	fstream myfile;
	string file_name;

	bool is_fileopen = false;
	int count_student = 0; //학생수
	int sort_option = 1; //정렬 옵션
	int  now_sorting_criterion = 0; //지금 정렬되어 있는 기준 ( 0:정렬 안됨, 1: 이름 , 2: ID, 3: 입학연도, 4: 학부)

	void update_file(string name, string id, string department, string birth_year, string telephone); // 추가된 학생 정보 파일에 적기
	void search_by_name();
	void search_by_id();
	void search_by_admission_yaer();
	void search_by_department();
	void print_all();
	static bool name_compare(Student a, Student b); //sort()를 위한 정렬 기준 비교 함수들
	static bool id_compare(Student a, Student b);
	static bool admission_year_compare(Student a, Student b);
	static bool department_compare(Student a, Student b);


	string display_sorting_option = "- Sorting Option-\n 1. Sort by Name\n 2. Sort by Student ID\n 3. Sort by Admission Year\n 4. Sort by Department name\n";
	string display_search_option = "- Search\n 1. Search by name\n 2. Search by student ID (10 numbers)\n 3. Search by admission year (4 numbers)\n 4. Search by department name\n 5. List All\n";
	string selection_explain = "Select the number corresponding to the feature you desire.\n";
	string num_error_message = "You have entered an incorrect number. Please enter again.\n";

public:
	StudentDB() {};

	void do_sort(); // 정렬하는 함수
	void open_file();
	void close_file();
	void read_file();
	void select_sort_option();
	void search_studentinfo();
	void add_studentinfo();
	void set_filename(string file_name);
};

class Student_Management {
private:
	StudentDB studentdb;

	string choice_window = " 1. Insertion \n 2. Search \n 3. Sorting Option \n 4. Exit \n";
	string selection_explain = "Select the number corresponding to the feature you desire.\n";
	string num_error_message = "You have entered an incorrect number. Please enter again.\n";
	string finish_message = "Thank you for using my program. Closing the program.\n";

public:
	Student_Management() {};

	void run();
	void set_filename(string file_name);
};

Student::Student(string name, string id, string department, string birth_year, string telephone) {
	this->name = name;
	this->id = id;
	this->department = department;
	this->birth_year = birth_year;
	this->telephone = telephone;
	this->admission_year = id.substr(0, 4);
}

void Student::set_student_information(string name, string id, string department, string birth_year, string telephone) { // 학생 정보 입력 함수
	this->name = name;
	this->id = id;
	this->department = department;
	this->birth_year = birth_year;
	this->telephone = telephone;
	this->admission_year = id.substr(0, 4);
}

string Student::get_name() {
	return name;
}

string Student::get_birth_year() {
	return birth_year;
}

string Student::get_id() {
	return id;
}

string Student::get_department() {
	return department;
}

string Student::get_telephone() {
	return telephone;
}

string Student::get_admission_year() {
	return admission_year;
}

void Student::print_information() {
	cout << std::left << setw(17) << name;
	cout << std::left << setw(12) << id;
	cout << std::left << setw(24) << department;
	cout << std::left << setw(12) << birth_year;
	cout << std::left << setw(14) << telephone << "\n";
}


void StudentDB::open_file() {
	if (is_fileopen == false) {
		// cout << file_name << "\n"; //Debuging 용( 파일 이름 확인)
		myfile.open(file_name);
		if (!(myfile.is_open())) { // 파일 열기 실패한 경우 (파일이 존재 하지 않는 경우-> 새롭게 파일 만들어 줌)
			ofstream savefile(file_name);
			if (savefile.is_open()) {
				savefile.close(); //단순 빈 txt 파일 만들기 용도
				myfile.open(file_name); // 작업용 txt 파일 다시 열기
				if (!(myfile.is_open())) {
					cout << "error: Failed to open the file.(" << file_name << ") Terminating the program urgently by some reason \n";
					exit(1);
				}
				else {
					is_fileopen = true;
				}
			}
			else {
				cout << "error: Failed to open the file.(" << file_name << ") Terminating the program urgently by incorrect file name or access denied \n";
				exit(1);
			}
		}
		else {
			is_fileopen = true;
		}
	}
	else {
		cout << "already file is opened (file name :" << file_name << ")\n";
	}
}

void StudentDB::close_file() { //안전한 파일 close를 위한 함수
	if (is_fileopen == true) {
		myfile.close();
		is_fileopen = false;
	}
}

void StudentDB::read_file() { //txt 파일에서 메모리로 DB정보를 복사하는 함수
	string line;
	while (getline(myfile, line)) { //한줄씩 읽어서 EOF 가 나올때 까지
		if (line == "") {
			continue;
		}
		//cout << line<<"\n"; //Debuging 확인용(파일 읽기)
		string tokens[5]; //"name,id,dept,birth,tel 순으로 저장"
		int start = 0, end;
		int count_token = 0;		//string::npos는 -1과 동일		
		while ((end = line.find('/', start)) != string::npos) {  //입력된 문자에서 '/' 기준으로 토큰을 분리하는 작업 (파이썬의 split() 역할)
			tokens[count_token] = line.substr(start, end - start);
			start = end + 1;
			count_token += 1;
		}
		tokens[count_token] = line.substr(start); //마지막 토큰은 따로 또 추가
		student[count_student].set_student_information(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4]); //DB에 정보 저장
		count_student += 1; // 학생수 증가 반영
	}
	close_file(); //EOF 도달 초기화를 위해 파일을 껐다 다시 킴
	open_file();
}

void StudentDB::set_filename(string file_name) { //파일 이름 설정 함수
	this->file_name = file_name;
}

void StudentDB::update_file(string name, string id, string department, string birth_year, string telephone) { //txt 파일에 새로운 정보를 update 하는 함수
	string information = name + "/" + id + "/" + department + "/" + birth_year + "/" + telephone + "\n";
	//cout << information << "\n"; //디버깅 확인용
	myfile.seekp(0, ios::end);//맨 끝에 추가
	myfile << information;

}

bool StudentDB::name_compare(Student a, Student b) {
	return a.get_name() < b.get_name();
}

bool StudentDB::id_compare(Student a, Student b) {
	return a.get_id() < b.get_id();
}

bool StudentDB::admission_year_compare(Student a, Student b) {
	return a.get_admission_year() < b.get_admission_year();
}

bool StudentDB::department_compare(Student a, Student b) {
	return a.get_department() < b.get_department();
}

void StudentDB::do_sort() { // 정렬 수행하는 함수
	switch (sort_option)
	{
	case 1: // 이름으로 정렬
		sort(student, student + count_student, name_compare);
		break;
	case 2: // ID로 정렬
		sort(student, student + count_student, id_compare);
		break;
	case 3: // 입학 연도로 정렬
		sort(student, student + count_student, admission_year_compare);
		break;
	case 4: // 학부로 정렬
		sort(student, student + count_student, department_compare);
		break;
	}
}

void StudentDB::search_studentinfo() { // 학생 정보 검색용 함수
	if (now_sorting_criterion != sort_option) { // 정렬기준이 원하는 정렬과 다른 경우 정렬
		do_sort();
		now_sorting_criterion = sort_option;
	}
	int selection_num = 0;
	cout << display_search_option;
	cin >> selection_num;
	if (selection_num < 1 or selection_num>5) { //선택 error 인 경우
		cout << num_error_message;
		cin >> selection_num;
	}
	cout << endl;

	switch (selection_num)
	{
	case 1: // 이름으로 검색
		search_by_name();
		break;
	case 2: // ID로 검색
		search_by_id();
		break;
	case 3: // 입학 연도로 검색
		search_by_admission_yaer();
		break;
	case 4: // 학부로 검색
		search_by_department();
		break;
	case 5: //모든 리스트 출력
		print_all();
		break;
	}
}

void StudentDB::search_by_name() {
	string name;
	cout << "name keyword?: ";
	cin >> name;
	cout << "Name             StudentID   Dept                    Birth Year  Tel\n";
	for (int i = 0; i < count_student; i++) {
		if (name == student[i].get_name()) {
			student[i].print_information();
		}
	}
	cout << "\n";
}

void StudentDB::search_by_id() {
	string id;
	cout << "id(10 numbers) keyword?: ";
	cin >> id;
	cout << "Name             StudentID   Dept                    Birth Year  Tel\n";
	for (int i = 0; i < count_student; i++) {
		if (id == student[i].get_id()) {
			student[i].print_information();
		}
	}
	cout << "\n";
}

void StudentDB::search_by_admission_yaer() {
	string year;
	cout << " admission year (4 numbers) keyword?: ";
	cin >> year;
	cout << "Name             StudentID   Dept                    Birth Year  Tel\n";
	for (int i = 0; i < count_student; i++) {
		if (year == student[i].get_admission_year()) {
			student[i].print_information();
		}
	}
	cout << "\n";
}

void StudentDB::search_by_department() {
	string department_name;
	cout << " department name keyword?: ";
	cin >> department_name;
	cout << "Name             StudentID   Dept                    Birth Year  Tel\n";
	for (int i = 0; i < count_student; i++) {
		if (department_name == student[i].get_department()) {
			student[i].print_information();
		}
	}
	cout << "\n";
}

void StudentDB::print_all() {
	cout << "Name             StudentID   Dept                    Birth Year  Tel\n";
	for (int i = 0; i < count_student; i++) {
		student[i].print_information();
	}
	cout << "\n";
}


void StudentDB::add_studentinfo() {

	string info[5]; //Name, ID,department,Year,tel 순으로 저장

	cout << "Name ? : ";
	cin >> info[0];
	while (info[0] == "-" or info[0].length() >= 15) {
		if (info[0] == "-") {
			cout << "Name should not be blank. enter your name again.\n";
		}
		else {
			cout << "Name has up to 15 (English) characters\n";
		}
		cout << "Name ? : ";
		cin >> info[0];
	}

	while (true) {
		bool error_flag = false;
		cout << "Student ID (10 digits)? : ";
		cin >> info[1];
		if (info[1].length() != 10) {
			cout << "Student ID should be exactly 10 digits where first 4 digits represent admission year.\n";
			continue;
		}
		for (int i = 0; i < count_student; i++) {
			if (info[1] == student[i].get_id()) { // 같은 id가 있는 지 확인하는 작업
				cout << "Error : Same id is already inserted. Enter a different id \n";
				error_flag = true;
				break;
			}
		}
		if (!error_flag) { //문제 없으면 다음 입력으로
			break;
		}
	}

	cout << "Department(no information, please enter '-') ? : ";
	cin >> info[2];
	if (info[2] == "") {
		info[2] = "-";
	}

	cout << "Birth Year(4 digits)(no information, please enter '-') ? : ";
	cin >> info[3];
	while (!(info[3].length() == 4 or info[3] == "-")) {
		cout << "Birth Year” should be exactly 4 digits or empty\n";
		cout << "Birth Year(4 digits)(no information, please enter '-') ? : ";
		cin >> info[3];
	}

	if (info[3] == "") {
		info[3] = "-";
	}

	cout << "Tel ?(no information, please enter '-') :";
	cin >> info[4];
	while (info[4].length() > 12) {
		cout << "Tel has up to 12 digits.\n";
		cout << "Tel ?(no information, please enter '-') : ";
		cin >> info[4];
	}
	if (info[4] == "") {
		info[4] = "-";
	}
	cout << "\n";
	student[count_student].set_student_information(info[0], info[1], info[2], info[3], info[4]); // DB 정보 업데이트
	now_sorting_criterion = 0; // 맨끝에 새로운 정보를 업데이트 했으므로 정렬이 안된 상태가 됨
	update_file(info[0], info[1], info[2], info[3], info[4]); //파일에 정보 업데이트
	count_student += 1; // 학생수 증가 반영
}


void StudentDB::select_sort_option() { //정렬 옵션 선택 함수
	cout << display_sorting_option;
	int select_num = 0;
	cin >> select_num;
	while (select_num < 1 or select_num>4) {
		cout << num_error_message;
		cin >> select_num;
	}
	cout << "\n";
	sort_option = select_num; // sort 기준 업데이트
}

void Student_Management::set_filename(string file_name) {
	studentdb.set_filename(file_name);
}

void Student_Management::run() {
	studentdb.open_file(); //파일 열기
	studentdb.read_file(); // 파일에서 내용을 읽어와 DB에 정보 복사

	int selection_num = 0;
	while (true) {
		cout << selection_explain;
		cout << choice_window; //선택창 display
		cin >> selection_num;
		if (selection_num < 1 or selection_num>4) { //선택 error 인 경우
			cout << num_error_message;
			cin >> selection_num;
		}
		cout << "\n";

		switch (selection_num)
		{
		case 1: //학생 정보 추가
			studentdb.add_studentinfo();
			break;
		case 2: // 학생 정보 검색
			studentdb.search_studentinfo();
			break;
		case 3: // 정렬 기준 설정
			studentdb.select_sort_option();
			break;
		case 4: // 종료
			studentdb.close_file(); //파일 닫기
			cout << finish_message;
			return;
			break;
		}
	}
}

int main(int argc, char* argv[]) {
	string file_name;

	if (argc == 1) { // 파일 이름이 입력되지 않은 경우
		cout << "Please enter the filename to be used as the database\n";
		cin >> file_name;
		cout << "\n";
	}
	else {
		file_name = string(argv[1]);
	}
	static Student_Management st_magement;
	st_magement.set_filename(file_name); //파일 이름 전달
	st_magement.run(); // 학생정보관리시스템 시작
	return 0;
}
