작성자:sinseongseop
사용언어: C++

OS: windows 10
compiler: visual studio 2022 17.7.4 version 
used file name: file1.txt

using x64 

source file: studentmanagement.cpp


How to run?

1.Debug(디버그)-> debugging properties(디버그 속성)-> configuration properties(구성속성) 
->  debugging(디버깅) -> command-line arguments(명령인수)

Enter the file name next to the command-line arguments
('명령인수'옆 칸에 파일 이름을 적습니다.)

*To check if it's x64 or x86 (x64로 디버깅하는 지 x86으로 디버깅하는 지 확인)
* should enter file1.txt (확장명까지 붙여야 합니다.)

2.Debug(디버그) ->Start without debugging ('디버그 하지 않고 시작' 을 누릅니다.)

3.The program is running (프로그램이 실행됩니다.)



program explanation

This program support inserting/searching/displaying(sorted) student information. 
After the program is terminated, the inserted student information should be maintained. 