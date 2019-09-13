#include"QInt.h"


int main(int argc, char* argv[]) {

	Data _Data;
	if (argc == 3) {
		//Đọc dữ liệu
		Filein(argv[1], _Data);
		cout << "Read from file: " << argv[1] << " is done." << endl;
		//Xử lý dữ liệu
		DataProcessing(_Data);
		cout << "Data Processing is done." << endl;
		//Ghi file từ dữ liệu vừa đọc
		Fileout(argv[2], _Data);
		cout << "Write in file is done." << endl;

	}
	else {
		cout << "ERROR!! Du lieu dau vao khong dung !!!" << endl;
		system("pause");
		return 0;
	}
	system("pause");

	return 0;
}
