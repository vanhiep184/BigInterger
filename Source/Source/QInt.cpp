#include "QInt.h"


//Tạo dãy bit ban đầu = 0
QInt::QInt()
{
	for (int i = 0; i < 128; i++) {
		arrBits[i] = '0';
	}
	arrBits[128] = '\0';
}

QInt::~QInt()
{
}
//Rút ngắn chuỗi, bỏ bớt số 0 thừa ở đầu dãy bit
char* QInt::value()
{
	return CharProcessing(arrBits);
}
//Copy chuỗi dữ liệu vào class lưu bởi arrBits
void QInt::CreateData(const char* data) {
	strcpy(arrBits, data);
}
//Các hàm quá tải toán tử =,+,-,*,/,&,|,^,~
QInt* QInt::operator=(QInt* num1) {

	CreateData(num1->arrBits); // sao chép chuỗi số
	return this;
}
QInt* QInt::operator+(QInt &num1) {
	QInt *res = new QInt();
	CanBangQInt(num1);
	int n = (int)strlen(arrBits);
	int dem = 0; // lưu biến nhớ
	int demres = 127;
	for (int i = n - 1; i >= 0; i--) // cộng từ phải sang trái
	{
		if (arrBits[i] == '0'&&num1.arrBits[i] == '0') // 0 + 0
		{
			if (dem == 0) res->arrBits[demres] = '0';
			if (dem == 1) {
				res->arrBits[demres] = '1';
				dem = 0;
			}
		}
		if ((arrBits[i] == '1'&&num1.arrBits[i] == '0') || (arrBits[i] == '0'&&num1.arrBits[i] == '1')) // 1 + 0 hoặc 0 + 1
		{
			if (dem == 0) res->arrBits[demres] = '1';
			if (dem == 1) res->arrBits[demres] = '0';
		}
		if (arrBits[i] == '1'&&num1.arrBits[i] == '1') // 1 + 1
		{
			if (dem == 0) res->arrBits[demres] = '0';
			if (dem == 1) res->arrBits[demres] = '1';
			dem = 1;
		}
		demres--;
	}
	if (dem == 1 && demres != 0) res->arrBits[demres] = '1'; // thêm 1 vào sau cùng khi nhớ vẫn là 1
	return res;
}
QInt* QInt::operator-(QInt &num1) {
	QInt* res = new QInt();
	CanBangQInt(num1); // can bang do dai
	int n = (int)strlen(arrBits);
	QInt num1Bu2;
	strcpy(num1Bu2.arrBits, QInt::Bu2(ToVector(num1.arrBits))); //lấy dạng bù 2 của số sau
	res = *this + num1Bu2; // A - B = A + bù 2 của B
	return res;
}
QInt* QInt::operator*(QInt &num1) {
	QInt* res = new QInt();
	int soam = 0;
	int n = (int)strlen(num1.arrBits);
	int n1 = (int)strlen(this->arrBits);
	// nếu có số nào âm thì chuyển sang dương
	// nếu 2 số khác dấu thì kết quả sẽ âm lưu soam = 1
	if (n1 == 128 && this->arrBits[0] == '1'&&n == 128 && num1.arrBits[0] == '1')
	{
		this->ChuyenSangDuong();
		num1.ChuyenSangDuong();
	}
	else if (n1 == 128 && this->arrBits[0] == '1' && ((n == 128 && num1.arrBits[0] == '0') || n < 128))
	{
		this->ChuyenSangDuong();
		soam = 1;
	}
	else if (n == 128 && num1.arrBits[0] == '1' && ((n1 == 128 && this->arrBits[0] == '0') || n < 128))
	{
		num1.ChuyenSangDuong();
		soam = 1;
	}
	n = (int)strlen(num1.arrBits);
	for (int i = 0; i < n; i++) // thuc hien nhan theo tung dong tu phai sang trai
	{
		if (i != 0) // lần 1 không dịch
		{
			*this << 1; // dịch trái 1 lần
		}
		if (num1.arrBits[n - i - 1] == '1') // nếu bit của số sau là 1 thì cộng số đầu sau khi dịch vào kết quả
		{
			*res = *res + *this;
		}
		this->value(); // rút gọn các số 0 phía trước
	}
	if (soam == 1) strcpy(res->arrBits, QInt::Bu2(ToVector(res->arrBits))); // nếu kết quả âm thì đổi về dạng bù 2
	return res;
}
QInt* QInt::operator/(QInt &num1) {
	// áp dụng Restoring Division Algorithm
	QInt A; // A = 0
	QInt num1full;
	int soam = 0;
	QInt* thisfull = new QInt();
	int n = (int)strlen(num1.arrBits);
	int n1 = (int)strlen(this->arrBits);
	// tạo 1 số mới cùng giá trị nhưng thêm 0 vào cho đủ độ dài 128 bit
	for (int i = 0; i < n; i++) 
	{
		num1full.arrBits[128 - n + i] = num1.arrBits[i]; // gán vào cuối chuỗi mới
	}
	for (int i = 0; i < n1; i++)
	{
		thisfull->arrBits[128 - n1 + i] = this->arrBits[i];
	}
	// nếu có số nào âm thì chuyển sang dương
	// nếu 2 số khác dấu thì kết quả sẽ âm lưu soam = 1
	if (thisfull->arrBits[0] == '1'&&num1full.arrBits[0] == '1') // cả 2 đều âm
	{
		thisfull->ChuyenSangDuong();
		num1full.ChuyenSangDuong();
	}
	else if (thisfull->arrBits[0] == '1'&&num1full.arrBits[0] == '0') // số đầu âm
	{
		thisfull->ChuyenSangDuong();
		soam = 1;
	}
	else if (thisfull->arrBits[0] == '0'&&num1full.arrBits[0] == '1') // số sau âm
	{
		num1full.ChuyenSangDuong();
		soam = 1;
	}
	char temp;
	int count = 128;
	do
	{
		// dịch trái [A,thisfull]
		temp = thisfull->arrBits[0]; // lưu lại bit đầu của thisfull sau khi dich trái 1 lần sẽ là bit cuối của A;
		for (int i = 0; i < strlen(A.arrBits) - 1; i++)
		{
			A.arrBits[i] = A.arrBits[i + 1];
		}
		for (int i = 0; i < strlen(thisfull->arrBits) - 1; i++)
		{
			thisfull->arrBits[i] = thisfull->arrBits[i + 1];
		}
		thisfull->arrBits[127] = '0';
		A.arrBits[127] = temp;
		A = A - num1full;
		if (A.arrBits[0] == '1') //nếu A < 0
		{

			thisfull->arrBits[127] = '0'; 
			A = A + num1full; // hoàn lại A
		}
		else if (A.arrBits[0] == '0') // nếu A > 0
		{
			thisfull->arrBits[127] = '1';
		}
		count--;
	} while (count != 0);
	if (soam == 1) strcpy(thisfull->arrBits, QInt::Bu2(ToVector(thisfull->arrBits))); // nếu kết quả là số âm thì đưa về dạng bù 2
	return thisfull;
}
QInt* QInt::operator&(QInt &num1)
{
	QInt *res = new QInt();
	CanBangQInt(num1);
	int n = (int)strlen(num1.arrBits);
	for (int i = 0; i < n; i++) // duyệt từ trái sang phải
	{
		if (arrBits[i] == '1' && num1.arrBits[i] == '1') // 1 & 1 = 1
			res->arrBits[128 - n + i] = '1';
	}
	res->value();
	return res;
}
QInt* QInt::operator|(QInt &num1)
{
	QInt* res = new QInt;
	CanBangQInt(num1);
	int n = (int)strlen(num1.arrBits);
	for (int i = 0; i < n; i++) // duyệt từ trái sang phải
	{
		if (arrBits[i] == '1' || num1.arrBits[i] == '1') // khi có ít nhất 1 phần tử là 1 thì cho kết quả là 1
			res->arrBits[128 - n + i] = '1';
	}
	res->value();
	return res;
}
QInt* QInt::operator^(QInt &num1) {
	QInt* res = new QInt();
	CanBangQInt(num1);
	int n = (int)strlen(num1.arrBits);
	for (int i = 0; i < n; i++) // duyệt từ trái sang phải
	{
		// nếu 2 bit khác nhau thì kết quả là 1
		if ((arrBits[i] == '0' && num1.arrBits[i] == '1') || (arrBits[i] == '1' && num1.arrBits[i] == '0'))
			res->arrBits[128 - n + i] = '1';
	}
	res->value();
	return res;
}
QInt* QInt::operator~() {
	QInt thisfull;
	int n = (int)strlen(arrBits);
	// tạo 1 số mới cùng giá trị nhưng thêm 0 vào cho đủ độ dài 128 bit
	for (int i = 0; i < n; i++)
	{
		thisfull.arrBits[128 - n + i] = this->arrBits[i];
	}
	for (int i = 0; i < 128; i++)
	{
		// đảo bit
		if (thisfull.arrBits[i] == '0')
			thisfull.arrBits[i] = '1';
		else
			thisfull.arrBits[i] = '0';
	}
	*this = thisfull;
	return this;
}
//Cân bằng 2 chuỗi để có độ dài bằng nhau bằng cách thêm 0 vào chuỗi ngắn hơn
void QInt::CanBangQInt(QInt &num1)
{
	vector<char>b1, b2;
	b1 = ToVector(arrBits);
	b2 = ToVector(num1.arrBits);
	CanBang(b1, b2);
	strcpy(arrBits, ToChar(b1));
	strcpy(num1.arrBits, ToChar(b2));
}
//Chuyển sang dương
QInt* QInt::ChuyenSangDuong() {
	QInt one;
	one.arrBits[127] = '1'; // lay so am truyen vao tru 1 
	*this = *this - one;
	~*this; // dao bit
	return this;
}
//Dich phai
QInt* QInt::operator>>(int count) {
	int len = (int)strlen(arrBits);

	if (len <= 0 || len > 128 || count <= 0) {
		return false;
	}
	else if (len == 128) {
		// Dich bit so hoc  
		//Kiểm tra bit dấu( bit trái nhất có bằng 1 hay không)
		while (count > 0) {
			//Chuyển các bit sang phải
			for (int i = 0; i < len; i++) {
				arrBits[len - 1 - i] = arrBits[len - 2 - i];
			}
			arrBits[0] = '1';
			count--;
		}
		return this;
	}
	else {
		//Nếu dich bit với số lượng bit cần dich lớn hơn 128 bit cần lưu trữ
			//Vòng lặp số lần dich bit
		while (count > 0) {
			//Chuyển các bit sang phải
			for (int i = 0; i < len; i++) {
				arrBits[len - 1 - i] = arrBits[len - 2 - i];
			}
			arrBits[0] = '0';
			count--;
		}

		return this;
	}
	return false;
}
//Dich Trái
QInt* QInt::operator<<(int count) {
	int len = (int)strlen(arrBits);
	if (len <= 0 || len > 128 || count <= 0) {
		return false;
	}
	else if (len == 128) {
		while (count > 0) {
			for (int i = 0; i < len; i++) {
				arrBits[i] = arrBits[i + 1];
			}
			arrBits[len] = '0';
			count--;
		}
		return this;
	}
	else {

		//Tao biến đếm số lần dịch
		//Dich đén khi nào đủ số lần cần dich
		//Nếu trong lúc dịch mà số lượng bit dich vừa đủ 128 bit 
		// thì phải bỏ bít trái nhất và tiếp tục dịch
		int temp = 0;
		while (temp < count) {
			//Nếu dich bit với số lượng bit cần dich lớn hơn 128 bit cần lưu trữ
			if (len == 128) {
				for (int i = 0; i < 128; i++) {
					arrBits[i] = arrBits[i + 1];
				}
				//Them bit trái nhất bằng 0
				arrBits[len - 1] = '0';
			}
			else {
				arrBits[len] = '0';
				arrBits[++len] = '\0';
			}
			temp++;
		}
		return this;
	}
	return false;
}
//Xoay phải
QInt* QInt::ror() {
	int len = (int)strlen(arrBits);
	if (len <= 0 || len > 128) {
		return false;
	}
	else if (len == 128) {
		char temp = arrBits[len - 1];
		for (int i = len - 1; i > 0; i--) {
			arrBits[i] = arrBits[i - 1];
		}
		arrBits[0] = temp;
		return this;
	}
	else {
		//Nếu phần tử cuối bằng 0 thì giống với dịch phải
		if (arrBits[len - 1] == '0') {
			*this >> 1;
			return this;
		}
		//Nếu bit cuối phải nhất bằng 1 thì tạo mảng 128 phần tử
		else {
			//Dich sang phải 1 bit
			*this >> 1;
			//Tạo vùng nhớ 128 bit để lưu bít phải nhất vừa dich lên trên đầu
			for (int i = 0; i <= len; i++) {
				arrBits[128 - i] = arrBits[len - i];
				arrBits[len - i] = '0';
			}
			//Lưu bit phải nhất thành bit trái nhất
			arrBits[0] = '1';
			return this;
		}

	}
	return false;
}
//Xoay Trái
QInt* QInt::rol() {
	int len = (int)strlen(arrBits);
	if (len <= 0 || len > 128) {
		return this;
	}
	else if (len == 128) {
		char temp = arrBits[0];
		for (int i = 0; i < len - 1; i++) {
			arrBits[i] = arrBits[i + 1];
		}
		arrBits[len - 1] = temp;
		return this;
	}
	//Nếu độ dài bít nhỏ hơn 128 bit thì gióng như dịch trái 1 bit
	else {
		*this << 1;
		return this;
	}

}
//Đọc file ghi vào DATA
void Filein(string fileName, Data & _Data) {
	fstream fin;
	fin.open(fileName, ios_base::in);


	//Tạo các biến để lưu dữ liệu
	vector<string> str;
	int INTtemp;
	string STRtemp;

	//Đọc file ghi dữ liệu vào các biến vừa tạo
	while (!fin.eof()) {
		if (fin >> INTtemp) {
			_Data.p.push_back(INTtemp);
		}
		else
			break;
		//Chuỗi tạm
		fin >> STRtemp;
		str.push_back(STRtemp);

		fin >> STRtemp;
		_Data.str1.push_back(STRtemp);
		//Xử lý lưu trữ dử liểu hợp lý
		//Nếu biến vừa đọc là các kí tự =, +,...
		//Thì không phải là chuyển hệ cơ số nên p1 = -1 ( không khả dụng )
		//Thêm 2 chuỗi cần thực hiện tính toán vào str1 (đang lưu ở chuỗi tạm) và str2 ( tiếp tục đọc trong file)
		if (STRtemp == "=" || STRtemp == "+" || STRtemp == "-" || STRtemp == "*" || STRtemp == "/" ||
			STRtemp == ">>" || STRtemp == "<<" || STRtemp == "&" || STRtemp == "|" || STRtemp == "^") {
			_Data.p1.push_back(-1);
			_Data._operator.push_back(STRtemp);
			_Data.str1[_Data.str1.size() - 1] = str[str.size() - 1];
			fin >> STRtemp;
			_Data.str2.push_back(STRtemp);
		}
		//Ngược lại nếu chuỗi tạm đọc được là các phép tính ror,rol,~,=
		//Thì cũng không phải là chuyển hệ cơ số nên p1 = -1 ( không khả dụng )
		//Lưu chuỗi tam đó vào operator
		//Giá trị chuỗi str2 không có 
		else if (str[str.size() - 1] == "ror" || str[str.size() - 1] == "rol" || str[str.size() - 1] == "~" || str[str.size() - 1] == "=") {
			_Data.p1.push_back(-1);
			_Data._operator.push_back(str[str.size() - 1]);
			_Data.str2.push_back(valueNULL);
		}
		//Ngược lại nếu chuỗi tạm khác các trường hợp trên thì là chuyển hệ cơ số
		//Lưu kết quả chuỗi tạm vào p1
		//opertor không khả dụng
		//str2 không khả dụng
		else {

			_Data.p1.push_back(stoi(str[str.size() - 1]));
			_Data._operator.push_back(valueNULL);
			_Data.str2.push_back(valueNULL);
		}

	}
	//Tính số phép toán cần thực hiện
	_Data.LineLength = (int)_Data.p.size();
	fin.close();
}
//Xử lý dữ liệu vừa đọc được
void DataProcessing(Data & _Data) {
	for (int i = 0; i < _Data.LineLength; i++) {
		QInt BigInt;
		BigInt.CreateData(_Data.str1[i].c_str());
		//Nếu p1 == -1 thì thực hiện các phép toán  +,-,*,/,...
		if (_Data.p1[i] == -1) {

			//Chuyển dãy số cần thực hiện sang nhị phân
			if (_Data.p[i] == 10) {
				BigInt.He10Sang2();
			}
			else if (_Data.p[i] == 16) {
				BigInt.He16Sang2();
			}
			QInt BigInt1;
			//Thực hiện các phép toán
			if (_Data._operator[i] == "=" || _Data._operator[i] == "+" || _Data._operator[i] == "-" || _Data._operator[i] == "*" || _Data._operator[i] == "/" ||
				_Data._operator[i] == "&" || _Data._operator[i] == "|" || _Data._operator[i] == "^")
			{
				BigInt1.CreateData(_Data.str2[i].c_str());
				if (_Data.p[i] == 10) {

					BigInt1.He10Sang2();
				}
				else if (_Data.p[i] == 16) {
					BigInt1.He16Sang2();
				}
			}
			if (_Data._operator[i] == "=") {
				//Tạo dữ liệu cho số để gán
				if (_Data.str2[i] != valueNULL) {
					BigInt1.CreateData(_Data.str2[i].c_str());
					if (_Data.p[i] == 10) {

						BigInt1.He10Sang2();
					}
					else if (_Data.p[i] == 16) {
						BigInt1.He16Sang2();
					}
				}
				/*else{
					BigInt1.CreateData(_Data.str1[i].c_str());
				}*/
				BigInt = BigInt1;
			}
			else if (_Data._operator[i] == "+") {

				BigInt = BigInt + BigInt1;
			}
			else if (_Data._operator[i] == "-") {
				BigInt = BigInt - BigInt1;
			}
			else if (_Data._operator[i] == "*") {
				BigInt = BigInt * BigInt1;
			}
			else if (_Data._operator[i] == "/") {
				BigInt = BigInt / BigInt1;
			}
			else if (_Data._operator[i] == "&") {
				BigInt = BigInt & BigInt1;
			}
			else if (_Data._operator[i] == "|") {
				BigInt = BigInt | BigInt1;
			}
			else if (_Data._operator[i] == "^") {
				BigInt = BigInt ^ BigInt1;
			}
			else if (_Data._operator[i] == "~") {
				~BigInt;
			}
			else if (_Data._operator[i] == "<<") {
				int count = stoi(_Data.str2[i]);
				BigInt << count;
			}
			else if (_Data._operator[i] == ">>") {
				int count = stoi(_Data.str2[i]);
				BigInt >> count;
			}
			else if (_Data._operator[i] == "rol") {
				BigInt.rol();
			}
			else if (_Data._operator[i] == "ror") {
				BigInt.ror();
			}

			//Chuyển dạy số về hệ số ban đầu
			if (_Data.p[i] == 10) {
				BigInt.He2Sang10();
			}
			else if (_Data.p[i] == 16) {
				BigInt.He2Sang16();
			}
		}
		//Nếu p1 != -1 thì thực hiện chuyển hệ số 
		else {

			if (_Data.p[i] == 10 && _Data.p1[i] == 2)
			{
				BigInt.He10Sang2();
			}
			else if (_Data.p[i] == 10 && _Data.p1[i] == 16)
			{
				BigInt.He10Sang16();
			}
			else if (_Data.p[i] == 2 && _Data.p1[i] == 10)
			{
				BigInt.He2Sang10();
			}
			else if (_Data.p[i] == 2 && _Data.p1[i] == 16)
			{
				BigInt.He2Sang16();
			}
			else if (_Data.p[i] == 16 && _Data.p1[i] == 10)
			{
				BigInt.He16Sang10();
			}
			else if (_Data.p[i] == 16 && _Data.p1[i] == 2)
			{
				BigInt.He16Sang2();
			}
			//*/
		}
		_Data.Result.push_back(BigInt.value());
	}

}
//Ghi kết quả vừa đọc được vào file output
void Fileout(string fileName, Data _Data) {
	fstream fout;
	fout.open(fileName, ios_base::out);
	for (int i = 0; i < _Data.LineLength; i++) {
		fout << _Data.Result[i] << endl;
	}
	fout.close();
}

//Xử lý chuỗi bỏ số 0 bị thừa ở đầu chuỗi
char* CharProcessing(char* Data) {
	int len = (int)strlen(Data);
	int i = 0;
	while (i != len) {
		if (Data[0] == '0' && Data[1] != '\0') {
			for (int j = 0; j < len; j++) {
				Data[j] = Data[j + 1];
			}
		}
		else {
			break;
		}
		i++;
	}
	return Data;
}



// Cau a b c

//Chuyen char sang vector
vector<char> QInt::ToVector(const char* a)
{
	vector <char> b;
	for (int i = 0; i < strlen(a); i++)
	{
		b.push_back(a[i] - 48);
	}
	return b;
}

//Chueyn vector sang char
char* QInt::ToChar(vector <char> KetQua)
{
	char* res = new char[128];
	for (int i = 0; i < KetQua.size(); i++)
	{
		res[i] = KetQua[i] + 48;
	}
	res[KetQua.size()] = '\0';
	return res;
}


//Can bang so chu so cua 2 bien
void QInt::CanBang(vector<char> &b1, vector<char> &b2)
{
	while (b1.size() != b2.size())
	{
		if (b1.size() < b2.size())
		{
			b1.insert(b1.begin(), '0' - 48);
		}
		if (b1.size() > b2.size())
		{
			b2.insert(b2.begin(), '0' - 48);
		}
	}
}



//So sanh so luong chu so 2 bien
bool  QInt::SoSanh(vector<char> b1, vector<char>b2)
{
	CanBang(b1, b2);
	for (int i = 0; i < b1.size(); i++)
	{
		if (b1[i] >= b2[i]) return true;
		else if (b1[i] < b2[i])return false;
	}
	return false;
}

//Cong 2 so he 10
char * QInt::CongHaiSo(char* a1, char* a2)
{
	vector<char>b1, b2;
	b1 = ToVector(a1);
	b2 = ToVector(a2);

	CanBang(b1, b2);

	vector <char> KetQua;
	int dem = 0;
	for (int i = (int)b1.size() - 1; i >= 0; i--)
	{

		if ((b1[i] + b2[i] + dem) < 10)
		{
			KetQua.insert(KetQua.begin(), b1[i] + b2[i] + dem);
			dem = 0;
		}
		else
		{
			KetQua.insert(KetQua.begin(), (b1[i] + b2[i] + dem) % 10);
			dem = 1;
			if (i == 0)
			{
				KetQua.insert(KetQua.begin(), dem);
			}
		}
	}
	char* res = new char[128];
	strcpy(res, ToChar(KetQua));
	return res;
}



//tinh 2^n
char * QInt::HaiMuN(int n)
{
	char* KetQua = new char[2];
	KetQua[0] = '1';
	KetQua[1] = '\0';
	if (n == 0) return KetQua;
	for (int i = 0; i < n; i++)
	{
		KetQua = CongHaiSo(KetQua, KetQua);
	}
	return KetQua;
}

//Chuyen sang dang bu 2
char * QInt::Bu2(vector<char> b)
{
	//Chuyen qua Bu 1
	for (int i = 0; i < b.size(); i++)
	{
		b[i] = (b[i] + 1) % 2;
	}
	//Chuyen qua bu 2
	int nho = 0;//Bien nho
	int i = (int)b.size() - 1;
	do
	{
		b[i] = (b[i] + 1) % 2;
		if (b[i] == 0) nho = 1;
		else nho = 0;
		if (i == 0 && b[i] == 0)
		{
			b.insert(b.begin(), '1' - 48);
			break;
		}
		i--;
	} while (nho == 1);
	while (b.size() < 128)
	{
		b.insert(b.begin(), '1' - 48);
	}
	return ToChar(b);
}

//chuyen bu 2 tu he 2 sang he 10
QInt * QInt::Bu22Sang10()
{
	vector <char> KetQua;
	KetQua = ToVector(arrBits);
	int nho = 0;//Bien nho
	int i = (int)KetQua.size() - 1;
	do
	{
		KetQua[i] = (KetQua[i] + 1) % 2;
		if (KetQua[i] == 1) nho = 1;
		else nho = 0;
		i--;
	} while (nho == 1);

	//Chuyen qua Bu 1
	for (int i = 0; i < KetQua.size(); i++)
	{
		KetQua[i] = (KetQua[i] + 1) % 2;
	}
	strcpy(arrBits, ToChar(KetQua));
	QInt::He2Sang10();
	for (int i = (int)strlen(arrBits); i >= 0; i--)
	{
		arrBits[i + 1] = arrBits[i];
	}
	arrBits[0] = '-';
	return this;
}



//Chuyen he thap phan sang nhi phan
QInt * QInt::He10Sang2()
{
	//Kiem Tra so am
	int SoAm = 0;
	if (arrBits[0] == '-')
	{
		SoAm = 1;
		for (int i = 0; i <= strlen(arrBits); i++)
		{
			arrBits[i] = arrBits[i + 1];//xoa dau '-'
		}
	}
	while (arrBits[0] == '0' && arrBits[1] == '0')//neu 2 bit 0 dung dau thi xoa 1 bit
	{
		for (int i = 0; i <= strlen(arrBits); i++)
		{
			arrBits[i] = arrBits[i + 1];
		}
	}
	if (arrBits[0] == '0' && arrBits[1] == '\0') return this;//neu so truyen vao la 0 thi tra ve chinh no

	vector<char>b;
	b = ToVector(arrBits);

	vector<char> SoChia, KetQua;
	SoChia = b;
	do
	{
		vector<char> SoChia2;//So du sau moi lan chia
		for (int i = 0; i < SoChia.size(); i++)
		{
			if (i != SoChia.size() - 1)//khong phai so cuoi cung
			{
				if (SoChia[i] % 2 == 0)
				{
					SoChia2.push_back(SoChia[i] / 2);
				}
				else//neu chia khong het thi so tiep theo cong them 10
				{
					if (!((SoChia[i] / 2 == 0) && (i == 0)))
						SoChia2.push_back(SoChia[i] / 2);
					SoChia[i + 1] += 10;
				}
			}
			else//neu la so cuoi cung
			{
				KetQua.insert(KetQua.begin(), SoChia[i] % 2);//Them vao dau cua ket qua
				SoChia2.push_back(SoChia[i] / 2);
			}
		}
		SoChia = SoChia2;//So chia bang so du lan truoc
	} while (SoChia[0] != 0);
	if (SoAm == 1)
	{
		//return Bu2(KetQua);
		strcpy(arrBits, Bu2(KetQua));
		return this;
	}
	//return ToChar(KetQua);
	strcpy(arrBits, ToChar(KetQua));
	return this;
}

//Chuyen he nhi phan qua thap phan
QInt * QInt::He2Sang10()
{
	vector <char> b;
	b = ToVector(arrBits);


	if (b.size() == 128)
	{
		if (b[0] == 1)
		{
			Bu22Sang10();//Chuyen he nhi phan dang bu 2 qua he thap phan
			return this;
		}
	}//*/

	int dem = 0;
	char* KetQua = new char[2];
	KetQua[0] = '0'; KetQua[1] = '\0';
	for (int i = (int)b.size() - 1; i >= 0; i--)
	{
		if (b[i] == 1)
		{
			KetQua = CongHaiSo(KetQua, HaiMuN(dem));//Ket qua bang ket qua cong voi 2^i
		}
		dem++;
	}

	strcpy(arrBits, KetQua);
	return this;
}

//Chuyen he thap phan sang thap luc phan
QInt * QInt::He10Sang16()
{
	QInt::He10Sang2();//Chuyen tu he thap phan sang he nhi phan
	QInt::He2Sang16();//Chuyen tu he nhi phan sang he thap luc phan
	return this;
}

//Chuyen he thap luc phan sang he thap phan
QInt * QInt::He16Sang10()
{
	QInt::He16Sang2();//chuyen tu he thap luc phan qua he nhi phan
	QInt::He2Sang10();//chuyen tu he nhi phan qua he thap phan
	return this;
}

//Chuyen he nhi phan sang he thap luc phan
QInt * QInt::He2Sang16()
{
	if (arrBits[0] == '0' && arrBits[1] == '\0')return this;
	vector<char>b;
	b = ToVector(arrBits);
	while (b.size() % 4 != 0)
	{
		b.insert(b.begin(), 0);
	}
	vector<char> KetQua;

	for (int i = (int)b.size() - 1; i >= 0; i = i - 4)
	{
		int Nho = 0;
		for (int j = i - 3; j <= i; j++)
		{
			Nho = Nho * 10 + b[j];//lay 4 bit he nhi phan
		}
		ThemHe16Tu2(KetQua, Nho);//chuyen 4 bit thanh he thap luc phan
	}

	strcpy(arrBits, ToChar(KetQua));
	return this;
}

//Chuyen he thap luc phan sang he nhi phan
QInt* QInt::He16Sang2()
{
	vector<char>b;
	b = ToVector(arrBits);
	vector<char> KetQua;

	for (int i = (int)b.size() - 1; i >= 0; i--)
	{
		ThemHe2Tu16(KetQua, b[i]);//chuyen tung so cua he 16 sang he 2
	}

	strcpy(arrBits, ToChar(KetQua));
	return this;
}


void QInt::ThemHe16Tu2(vector<char>& b, int k)
{
	switch (k)
	{
	case 0: b.insert(b.begin(), 0); break;
	case 1: b.insert(b.begin(), 1); break;
	case 10: b.insert(b.begin(), 2); break;
	case 11: b.insert(b.begin(), 3); break;
	case 100: b.insert(b.begin(), 4); break;
	case 101: b.insert(b.begin(), 5); break;
	case 110: b.insert(b.begin(), 6); break;
	case 111: b.insert(b.begin(), 7); break;
	case 1000: b.insert(b.begin(), 8); break;
	case 1001: b.insert(b.begin(), 9); break;
	case 1010: b.insert(b.begin(), 'A' - 48); break;
	case 1011: b.insert(b.begin(), 'B' - 48); break;
	case 1100: b.insert(b.begin(), 'C' - 48); break;
	case 1101: b.insert(b.begin(), 'D' - 48); break;
	case 1110: b.insert(b.begin(), 'E' - 48); break;
	case 1111: b.insert(b.begin(), 'F' - 48); break;
	}
}

void QInt::ThemHe2Tu16(vector<char>& b, int k)
{
	switch (k)
	{
	case 0:
	{
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 0);
	}break;
	case 1:
	{
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 0);
	}break;
	case 2:
	{
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 0);
	}; break;
	case 3:
	{
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 0);
	}break;
	case 4:
	{
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 0);
	}; break;
	case 5:
	{
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 0);
	}; break;
	case 6:
	{
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 0);
	}; break;
	case 7:
	{
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 0);
	}; break;
	case 8:
	{
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 1);
	}; break;
	case 9:
	{
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 1);
	}; break;
	case 17:
	{
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 1);
	}; break;
	case 18:
	{
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 1);
	}; break;
	case 19:
	{
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 1);
	}; break;
	case 20:
	{
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 1);
	}; break;
	case 21:
	{
		b.insert(b.begin(), 0);
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 1);
	}; break;
	case 22:
	{
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 1);
		b.insert(b.begin(), 1);
	}; break;
	}
}
