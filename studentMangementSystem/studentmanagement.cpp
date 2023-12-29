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
	void set_student_information(string name, string id, string birth_year, string department, string telephone); // �л��� ��� ������ �����ϴ� �Լ�
	void print_information(); // �л��� ��������� ����ϴ� �Լ�
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
	int count_student = 0; //�л���
	int sort_option = 1; //���� �ɼ�
	int  now_sorting_criterion = 0; //���� ���ĵǾ� �ִ� ���� ( 0:���� �ȵ�, 1: �̸� , 2: ID, 3: ���п���, 4: �к�)

	void update_file(string name, string id, string department, string birth_year, string telephone); // �߰��� �л� ���� ���Ͽ� ����
	void search_by_name();
	void search_by_id();
	void search_by_admission_yaer();
	void search_by_department();
	void print_all();
	static bool name_compare(Student a, Student b); //sort()�� ���� ���� ���� �� �Լ���
	static bool id_compare(Student a, Student b);
	static bool admission_year_compare(Student a, Student b);
	static bool department_compare(Student a, Student b);


	string display_sorting_option = "- Sorting Option-\n 1. Sort by Name\n 2. Sort by Student ID\n 3. Sort by Admission Year\n 4. Sort by Department name\n";
	string display_search_option = "- Search\n 1. Search by name\n 2. Search by student ID (10 numbers)\n 3. Search by admission year (4 numbers)\n 4. Search by department name\n 5. List All\n";
	string selection_explain = "Select the number corresponding to the feature you desire.\n";
	string num_error_message = "You have entered an incorrect number. Please enter again.\n";

public:
	StudentDB() {};

	void do_sort(); // �����ϴ� �Լ�
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

void Student::set_student_information(string name, string id, string department, string birth_year, string telephone) { // �л� ���� �Է� �Լ�
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
		// cout << file_name << "\n"; //Debuging ��( ���� �̸� Ȯ��)
		myfile.open(file_name);
		if (!(myfile.is_open())) { // ���� ���� ������ ��� (������ ���� ���� �ʴ� ���-> ���Ӱ� ���� ����� ��)
			ofstream savefile(file_name);
			if (savefile.is_open()) {
				savefile.close(); //�ܼ� �� txt ���� ����� �뵵
				myfile.open(file_name); // �۾��� txt ���� �ٽ� ����
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

void StudentDB::close_file() { //������ ���� close�� ���� �Լ�
	if (is_fileopen == true) {
		myfile.close();
		is_fileopen = false;
	}
}

void StudentDB::read_file() { //txt ���Ͽ��� �޸𸮷� DB������ �����ϴ� �Լ�
	string line;
	while (getline(myfile, line)) { //���پ� �о EOF �� ���ö� ����
		if (line == "") {
			continue;
		}
		//cout << line<<"\n"; //Debuging Ȯ�ο�(���� �б�)
		string tokens[5]; //"name,id,dept,birth,tel ������ ����"
		int start = 0, end;
		int count_token = 0;		//string::npos�� -1�� ����		
		while ((end = line.find('/', start)) != string::npos) {  //�Էµ� ���ڿ��� '/' �������� ��ū�� �и��ϴ� �۾� (���̽��� split() ����)
			tokens[count_token] = line.substr(start, end - start);
			start = end + 1;
			count_token += 1;
		}
		tokens[count_token] = line.substr(start); //������ ��ū�� ���� �� �߰�
		student[count_student].set_student_information(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4]); //DB�� ���� ����
		count_student += 1; // �л��� ���� �ݿ�
	}
	close_file(); //EOF ���� �ʱ�ȭ�� ���� ������ ���� �ٽ� Ŵ
	open_file();
}

void StudentDB::set_filename(string file_name) { //���� �̸� ���� �Լ�
	this->file_name = file_name;
}

void StudentDB::update_file(string name, string id, string department, string birth_year, string telephone) { //txt ���Ͽ� ���ο� ������ update �ϴ� �Լ�
	string information = name + "/" + id + "/" + department + "/" + birth_year + "/" + telephone + "\n";
	//cout << information << "\n"; //����� Ȯ�ο�
	myfile.seekp(0, ios::end);//�� ���� �߰�
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

void StudentDB::do_sort() { // ���� �����ϴ� �Լ�
	switch (sort_option)
	{
	case 1: // �̸����� ����
		sort(student, student + count_student, name_compare);
		break;
	case 2: // ID�� ����
		sort(student, student + count_student, id_compare);
		break;
	case 3: // ���� ������ ����
		sort(student, student + count_student, admission_year_compare);
		break;
	case 4: // �кη� ����
		sort(student, student + count_student, department_compare);
		break;
	}
}

void StudentDB::search_studentinfo() { // �л� ���� �˻��� �Լ�
	if (now_sorting_criterion != sort_option) { // ���ı����� ���ϴ� ���İ� �ٸ� ��� ����
		do_sort();
		now_sorting_criterion = sort_option;
	}
	int selection_num = 0;
	cout << display_search_option;
	cin >> selection_num;
	if (selection_num < 1 or selection_num>5) { //���� error �� ���
		cout << num_error_message;
		cin >> selection_num;
	}
	cout << endl;

	switch (selection_num)
	{
	case 1: // �̸����� �˻�
		search_by_name();
		break;
	case 2: // ID�� �˻�
		search_by_id();
		break;
	case 3: // ���� ������ �˻�
		search_by_admission_yaer();
		break;
	case 4: // �кη� �˻�
		search_by_department();
		break;
	case 5: //��� ����Ʈ ���
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

	string info[5]; //Name, ID,department,Year,tel ������ ����

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
			if (info[1] == student[i].get_id()) { // ���� id�� �ִ� �� Ȯ���ϴ� �۾�
				cout << "Error : Same id is already inserted. Enter a different id \n";
				error_flag = true;
				break;
			}
		}
		if (!error_flag) { //���� ������ ���� �Է�����
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
		cout << "Birth Year�� should be exactly 4 digits or empty\n";
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
	student[count_student].set_student_information(info[0], info[1], info[2], info[3], info[4]); // DB ���� ������Ʈ
	now_sorting_criterion = 0; // �ǳ��� ���ο� ������ ������Ʈ �����Ƿ� ������ �ȵ� ���°� ��
	update_file(info[0], info[1], info[2], info[3], info[4]); //���Ͽ� ���� ������Ʈ
	count_student += 1; // �л��� ���� �ݿ�
}


void StudentDB::select_sort_option() { //���� �ɼ� ���� �Լ�
	cout << display_sorting_option;
	int select_num = 0;
	cin >> select_num;
	while (select_num < 1 or select_num>4) {
		cout << num_error_message;
		cin >> select_num;
	}
	cout << "\n";
	sort_option = select_num; // sort ���� ������Ʈ
}

void Student_Management::set_filename(string file_name) {
	studentdb.set_filename(file_name);
}

void Student_Management::run() {
	studentdb.open_file(); //���� ����
	studentdb.read_file(); // ���Ͽ��� ������ �о�� DB�� ���� ����

	int selection_num = 0;
	while (true) {
		cout << selection_explain;
		cout << choice_window; //����â display
		cin >> selection_num;
		if (selection_num < 1 or selection_num>4) { //���� error �� ���
			cout << num_error_message;
			cin >> selection_num;
		}
		cout << "\n";

		switch (selection_num)
		{
		case 1: //�л� ���� �߰�
			studentdb.add_studentinfo();
			break;
		case 2: // �л� ���� �˻�
			studentdb.search_studentinfo();
			break;
		case 3: // ���� ���� ����
			studentdb.select_sort_option();
			break;
		case 4: // ����
			studentdb.close_file(); //���� �ݱ�
			cout << finish_message;
			return;
			break;
		}
	}
}

int main(int argc, char* argv[]) {
	string file_name;

	if (argc == 1) { // ���� �̸��� �Էµ��� ���� ���
		cout << "Please enter the filename to be used as the database\n";
		cin >> file_name;
		cout << "\n";
	}
	else {
		file_name = string(argv[1]);
	}
	static Student_Management st_magement;
	st_magement.set_filename(file_name); //���� �̸� ����
	st_magement.run(); // �л����������ý��� ����
	return 0;
}
