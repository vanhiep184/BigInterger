#define _CRT_SECURE_NO_WARNINGS
#ifndef __QINT_H__
#define __QINT_H__



#include<iostream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;




#define valueNULL "-1"

struct Data
{
	//Độ dài của dữ liệu
	int LineLength = 0;
	// Chuyển từ hệ p sang p1 lưu hệ cần chuyển 
	vector<int> p;
	vector<int> p1;
	//Lưu phép toán cần thực hiện
	vector<string> _operator;
	//Lưu chuỗi dữ liệu đầu tiên
	vector<string> str1;
	//Lưu chuỗi dữ liệu thứ 2 (nếu có) trong TH thưc hiện các phép +,-, *, /.
	vector<string> str2;
	//Lưu kết quả tương ứng với từng phép toán
	vector<string> Result;
};



class QInt
{
private:
	char arrBits[129]; // (1) dùng mảng tĩnh 128 byte 
public:
	QInt();
	~QInt();
	char* value();
	void CreateData(const char* data);
	void CanBangQInt(QInt &num1);
	QInt* ChuyenSangDuong();
	QInt* operator=(QInt* num1);
	QInt* operator+(QInt &num1);
	QInt* operator-(QInt &num1);
	QInt* operator*(QInt &num1);
	QInt* operator/(QInt &num1);
    QInt* operator&(QInt &num1);
	QInt* operator|(QInt &num1);
	QInt* operator^(QInt &num1);
	QInt* operator~();
	QInt* operator>>(int count);
	QInt* operator<<(int count);

	QInt* rol();
	QInt* ror();

	vector<char> ToVector(const char * a);
	char* ToChar(vector <char> KetQua);
	void CanBang(vector<char>& b1, vector<char>& b2);
	bool SoSanh(vector<char> b1, vector<char> b2);
	char * CongHaiSo(char * a1, char * a2);
	char * HaiMuN(int n);
	char * Bu2(vector<char> b);

	QInt * Bu22Sang10();
	QInt * He10Sang2();
	QInt * He2Sang10();
	QInt * He10Sang16();
	QInt * He16Sang10();
	QInt * He2Sang16();
	QInt * He16Sang2();

	void ThemHe16Tu2(vector<char>& b, int k);
	void ThemHe2Tu16(vector<char>& b, int k);


};


void Filein(string input, Data & _Data);
void DataProcessing(Data & _Data);
void Fileout(string input, Data _Data);
char* CharProcessing(char* Data);





#endif // !__QINT_H__
