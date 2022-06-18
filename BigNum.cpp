#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include <stdbool.h>
#include <string.h>
#include <type_traits> 
#include <string>
#include <bitset>

using namespace std;

typedef unsigned char BASE;
typedef unsigned short int DBASE;
#define BASE_SIZE (sizeof(BASE)*8)

class BigNum {
	int len, maxlen;
	BASE* coef;
public:
	BigNum(); // конструктор 1
	BigNum(int size, bool flag = false); // конструктор 2
	BigNum(const BigNum&); // конструктор копирования

	~BigNum() { // деструктор
		if (coef) delete[] coef; coef = NULL;
	}

	void NormalLen() {
		for (len = maxlen; coef[len - 1] == 0 && len > 0; len--);
		if (len == 0) len = 1;
	}

	BigNum& operator = (const BigNum&); // оператор присвоения
	bool operator == (const BigNum&);
	bool operator != (const BigNum&);
	bool operator < (const BigNum&);
	bool operator <= (const BigNum&);
	bool operator > (const BigNum&);
	bool operator >= (const BigNum&);
	
	friend istream& operator >> (istream&, BigNum&); // ввод 16 СС
	friend ostream& operator << (ostream&, BigNum&); // вывод 16 СС
	
	BigNum operator + (const BigNum&);
	BigNum& operator +=(const BigNum&);
	BigNum operator + (const BASE);
	BigNum& operator +=(const BASE);

	BigNum operator -(const BigNum&);
	BigNum& operator -=(const BigNum&);
	BigNum operator - (const BASE);
	BigNum& operator -=(const BASE);

	BigNum operator *(const BASE);
	BigNum& operator *=(const BASE);
	BigNum operator *(const BigNum&);
	BigNum& operator *=(const BigNum&);

	BigNum operator /(const BASE);
	BigNum& operator /=(const BASE);
	BigNum operator /(const BigNum&);
	BigNum& operator /=(const BigNum&);

	BigNum operator %(const BASE);
	BigNum& operator %=(const BASE);
	BigNum operator %(const BigNum&);
	BigNum& operator %=(const BigNum&);

	void Input();
	void Output();
};

BigNum::BigNum() {
	maxlen = 1;
	len = 1;
	coef = new BASE[maxlen];
	coef[0] = 0;
}

BigNum::BigNum(int size, bool flag) {
	if (size < 1)
		exit(0);

	maxlen = size;
	len = size;
	coef = new BASE[maxlen];

	if (flag) {
		for (int i = 0; i < maxlen; i++) {
			coef[i] = (rand() << 16) + rand();
		}
		for (int i = maxlen - 1; i > 0 && coef[i] == 0; i++) {
			len = len - 1;
		}
	}
	else {
		len = 1;
		for (int i = 0; i < maxlen; i++) {
			coef[i] = 0;
		}
	}
}

BigNum::BigNum(const BigNum& Big) {
	if (!Big.coef || Big.maxlen < 1 || Big.len < 1)
		exit(0);
	
	maxlen = Big.maxlen;
	len = Big.len;
	coef = new BASE[maxlen];
	for (int i = 0; i < maxlen; i++) {
		coef[i] = Big.coef[i];
	}
}

BigNum& BigNum::operator=(const BigNum& Big) {
	if (this == &Big)
		return *this;
	if (Big.maxlen < 1 || Big.len < 1 || !Big.coef) 
		exit(0);

	delete[] coef;
	maxlen = Big.maxlen;
	len = Big.len;
	coef = new BASE[maxlen];
	for (int i = 0; i < maxlen; i++) {
		coef[i] = Big.coef[i];
	}
	return *this;
}

bool BigNum::operator == (const BigNum& Big) {
	if (len != Big.len) return false;
	for (int i = len - 1; i >= 0; i--) {
		if (coef[i] != Big.coef[i]) return false;
	}
	return true;
}

bool BigNum::operator != (const BigNum& Big) {
	if (len != Big.len) return true;
	for (int i = len - 1; i >= 0; i--) {
		if (coef[i] != Big.coef[i]) return true;
	}
	return false;
}

bool BigNum::operator < (const BigNum& Big) {
	if (len < Big.len) return true;
	if (len > Big.len) return false;
	for (int i = len - 1; i >= 0; i--) {
		if (coef[i] < Big.coef[i]) return true;
		else
			if (coef[i] > Big.coef[i]) return false;
	}
	return false;
}

bool BigNum::operator <= (const BigNum& Big) {
	if (len < Big.len) return true;
	if (len > Big.len) return false;
	for (int i = len - 1; i >= 0; i--) {
		if (coef[i] < Big.coef[i]) return true;
		else
			if (coef[i] > Big.coef[i]) return false;
	}
	return true;
}

bool BigNum::operator > (const BigNum& Big) {
	if (len > Big.len) return true;
	if (len < Big.len) return false;
	for (int i = len - 1; i >= 0; i--) {
		if (coef[i] > Big.coef[i]) return true;
		else
			if (coef[i] < Big.coef[i]) return false;
	}
	return false;
}

bool BigNum::operator >= (const BigNum& Big) {
	if (len > Big.len) return true;
	if (len < Big.len) return false;
	for (int i = len - 1; i >= 0; i--) {
		if (coef[i] > Big.coef[i]) return true;
		else
			if (coef[i] < Big.coef[i]) return false;
	}
	return true;
}

ostream& operator<<(ostream& out, BigNum& Big) {
	if (!Big.coef) return out;

	for (int j = Big.len - 1; j >= 0; j--) {
		cout.width(BASE_SIZE / 4);
		cout.fill('0');
		cout << hex << (unsigned int)Big.coef[j];
	}
	return out;
}

istream& operator >> (istream& in, BigNum& Big) {
	char* s = new char[1000];
	cout << "Enter 16CC:";
	cin >> s;

	int size = (strlen(s) - 1) / (BASE_SIZE / 4) + 1;

	BigNum Big1(size);
	Big1.len = size;

	int j = 0, k = 0;
	unsigned int tmp;
	bool flag = true;

	for (int i = strlen(s) - 1; i >= 0; i--) {
		flag = false;
		if ('0' <= s[i] && s[i] <= '9') {
			tmp = s[i] - '0';
			flag = true;
		}
		if ('a' <= s[i] && s[i] <= 'f') {
			tmp = s[i] - 'a' + 10;
			flag = true;
		}
		if ('A' <= s[i] && s[i] <= 'F') {
			tmp = s[i] - 'A' + 10;
			flag = true;
		}
		if (!flag) {
			puts("Error");
			exit(0);
		}

		Big1.coef[j] |= tmp << (k * 4);
		k++;
		if (k >= BASE_SIZE / 4) {
			k = 0;
			j++;
		}
	}

	Big1.NormalLen();

	Big = Big1;
	delete[] s;
	return in;
}

BigNum BigNum::operator + (const BigNum& Big) {
	int min, max;
	BASE* MaxBig;
	DBASE tmp;
	bool k = 0;
	
	if (len > Big.len) {
		min = Big.len;
		max = len + 1; 
		MaxBig = coef;
	}
	else { 
		min = len; 
		max = Big.len + 1;
		MaxBig = Big.coef;
	}
	
	BigNum SUM(max, 0);
	
	for (int i = 0; i < min; i++) {
		tmp = (DBASE)Big.coef[i] + (DBASE)coef[i] + k;
		SUM.coef[i] = (BASE)tmp;
		k = (bool)(tmp >> BASE_SIZE);
	}
	
	for (int i = min; i < max - 1; i++) {
		tmp = (DBASE)MaxBig[i] + k;
		SUM.coef[i] = (BASE)tmp;
		k = (bool)(tmp >> BASE_SIZE);
	}
	
	SUM.coef[max - 1] = k;
	SUM.NormalLen();
	return SUM;
}

BigNum& BigNum::operator +=(const BigNum& Big) {
	*this = *this + Big;
	return *this;
}

BigNum BigNum::operator + (const BASE Number) {
	DBASE tmp;
	bool k = 0;
	int size = maxlen;

	if (len + 1 > maxlen) size = len + 1;
	BigNum SUM(size, 0);

	tmp = (DBASE)coef[0] + (DBASE)Number;
	k = (bool)(tmp >> BASE_SIZE);
	SUM.coef[0] = tmp;
	
	for (int i = 1; i < len; i++) {
		tmp = coef[i] + k;
		k = bool(tmp >> BASE_SIZE);
		SUM.coef[i] = tmp;
	}

	SUM.coef[len] = k;
	SUM.NormalLen();
	return SUM;
}

BigNum& BigNum::operator +=(const BASE Number) {
	*this = *this + Number;
	return *this;
}

BigNum BigNum::operator - (const BigNum& Big) {
	if (*this < Big) {
		cout << '\n' << "First value smaller than second" << '\n';
		exit(0);
	}

	bool k = 0;
	DBASE tmp;

	BigNum DIFF(maxlen, 0);

	for (int i = 0; i < Big.len; i++) {
		tmp = ((DBASE)1 << BASE_SIZE) | coef[i];
		tmp = tmp - Big.coef[i] - k;
		DIFF.coef[i] = (BASE)tmp;
		k = !(tmp >> BASE_SIZE);
	}

	for (int i = Big.len; i < maxlen; i++) {
		tmp = ((DBASE)1 << BASE_SIZE) | coef[i];
		tmp -= k;
		DIFF.coef[i] = (BASE)tmp;
		k = !(tmp >> BASE_SIZE);
	}

	DIFF.NormalLen();
	return DIFF;
}

BigNum& BigNum::operator -=(const BigNum& Big) {
	*this = *this - Big;
	return *this;
}

BigNum BigNum::operator - (const BASE Number) {
	if (*this < Number) {
		throw invalid_argument("first value should be bigger than second to subtract");
	}
	DBASE tmp;
	bool k = 0;
	BigNum A(*this);
	tmp = A.coef[0] + ((DBASE)1 << BASE_SIZE) - Number;
	A.coef[0] = (BASE)tmp;
	k = !((tmp) >> BASE_SIZE);
	for (int i = A.len; k && i <= A.len; i++) {
		tmp = A.coef[i] + ((DBASE)1 << BASE_SIZE) - k;
		A.coef[i] = (BASE)tmp;
		k = !((tmp) >> BASE_SIZE);
	}
	A.NormalLen();
	return A;
}

BigNum& BigNum::operator -= (const BASE Number) {
	*this = *this - Number;
	return *this;
}

BigNum BigNum::operator *(BASE Number) {
	DBASE tmp;
    BASE k = 0;

	BigNum MUL(maxlen + 1, 0);

	for (int i = 0; i < len; i++) {
		tmp = (DBASE)coef[i] * Number + k;
		k = (BASE)(tmp >> BASE_SIZE);
		MUL.coef[i] = (BASE)tmp;
	}

	MUL.coef[len] = k;
	MUL.NormalLen();
	return MUL;
}

BigNum& BigNum::operator*= (const BASE Number) {
	*this = *this * Number;
	return *this;
}

BigNum BigNum::operator*(const BigNum& Big) {
	DBASE tmp; 
	BASE k;
	BigNum MUL(len + Big.len, 0);

	for (int i = 0; i < len; i++) {
		if (coef[i] != 0) {
			k = 0;
			for (int j = 0; j < Big.len; j++) {
				tmp = (DBASE)coef[i] * (DBASE)Big.coef[j] + (DBASE)MUL.coef[i + j] + (DBASE)k;
				MUL.coef[i + j] = (BASE)tmp;
				k = (BASE)(tmp >> BASE_SIZE);
			}
			MUL.coef[i + Big.len] = k;
		}
	}

	MUL.NormalLen();
	return MUL;
}

BigNum& BigNum::operator *=(const BigNum& Big) {
	*this = *this * Big;
	return *this;
}

BigNum BigNum::operator /(const BASE Number) {
	DBASE tmp;
	BASE r = 0;
	BigNum DIV(len, 0);
	for (int i = len - 1; i >= 0; i--) {
		tmp = ((DBASE)r << BASE_SIZE) + (DBASE)coef[i];
		DIV.coef[i] = (BASE)(tmp / (DBASE)Number);
		r = (BASE)(tmp % (DBASE)Number);
	}

	DIV.NormalLen();
	return DIV;
}

BigNum& BigNum::operator/=(const BASE Number) {
	*this = *this / Number;
	return *this;
}

BigNum BigNum::operator/(const BigNum& Big) {
	if (Big.len == 1 && Big.coef[0] == 0) {
		cout << "Error";
		exit(0);
	}

	if (*this < Big) {
		BigNum q(1);
		return q;
	}

	if (Big.len == 1) return *this / Big.coef[0];
	
	if (*this == Big) {
		BigNum q(1);
		q.coef[0] = 1;
		return q;
	}

	int m = len - Big.len; // разность длин
	DBASE b = ((DBASE)1 << BASE_SIZE); // основание числа
	DBASE d = b / (DBASE)(Big.coef[Big.len - 1] + (BASE)1); // коэффициент нормализации
	BigNum q(m + 1); // частное
	q.len = m + 1;

	BigNum U = *this; // делимое
	BigNum V = Big; // делитель

	// D1
	// нормализация
	U = U * d; 
	V = V * d;
	if (U.len == len) { // если длина не поменялась
		if (U.maxlen == U.len) { // если нужно расширить размер делимого
			U.maxlen++;
			U.len = U.maxlen;
			delete[]U.coef;
			U.coef = new BASE[U.maxlen];
			for (int i = 0; i < len; i++) {
				U.coef[i] = coef[i];
			}
			U = U * d;
			U.coef[U.len - 1] = 0; // добавление нуля в начало
		}
		U.coef[len] = 0;// добавление нуля в начало
		U.len = len + 1;
	}

	// D2
	for (int j = m; j >= 0;j--) {
		// D3
		DBASE qq = (((DBASE)U.coef[j + Big.len] << BASE_SIZE) + U.coef[j + Big.len - 1]) / V.coef[Big.len - 1];
		DBASE rr = (((DBASE)U.coef[j + Big.len] << BASE_SIZE) + U.coef[j + Big.len - 1]) % V.coef[Big.len - 1];
		
		// коррекция догадок qq и rr
		if (qq == b || ((qq * V.coef[Big.len - 2]) > (b * rr + U.coef[j + Big.len - 2]))) {
			qq--;
			rr = rr + V.coef[Big.len - 1];
			if ((rr < b) && ((qq == b) || (qq * V.coef[Big.len - 2] > b * rr + U.coef[j + Big.len - 2]))) {
				qq--;
				rr = rr + V.coef[Big.len - 1];
			}
		}

		BigNum u(V.len + 1); // часть делимого
		u.len = V.len + 1;
		for (int i = 0; i < V.len + 1; i++) {
			u.coef[i] = U.coef[j + i]; // заносим в часть деломого старшие разряды делимого
		}
		// D6
		if (u < V * qq) qq--; // если делимое меньше произведения догадки на делитель то уменьшаем догадку в последний раз
		// D4
		u = u - V * qq; // выполняем вычитание от части делимого
		// D5
		q.coef[j] = qq; // заносим догадку в коэффициенты частного
		// D6
		for (int i = 0; i < V.len + 1; i++) { // в старший разряд исходного делимого заносим остаток от вычитания
			U.coef[j + i] = u.coef[i];
		}
		// D7
	}
	// D8
	q.NormalLen();
	return q;
}

BigNum& BigNum::operator/=(const BigNum& Big) {
	*this = *this / Big;
	return *this;
}

BigNum BigNum::operator%(const BASE Number) {
	DBASE tmp = 0;
	BASE r = 0;
	BigNum Rem(1, 0);
	for (int i = 0; i < len; i++) {
		tmp = (((DBASE)r << BASE_SIZE) + (DBASE)coef[len - 1 - i]);
		r = (BASE)(tmp % (DBASE)Number);
	}
	Rem.coef[0] = r;
	return Rem;
}

BigNum& BigNum::operator%=(const BASE Number) {
	*this = *this % Number;
	return *this;
}

BigNum BigNum::operator%(const BigNum& Big) {
	if (Big.len == 1 && Big.coef[0] == 0) {
		cout << "Error";
		exit(0);
	}

	if (*this < Big) return *this;

	if (Big.len == 1) return *this % Big.coef[0];

	if (*this == Big) {
		BigNum Rem(1);
		return Rem;
	}

	int m = len - Big.len; // разность длин
	DBASE b = ((DBASE)1 << BASE_SIZE); // основание числа
	DBASE d = b / (DBASE)(Big.coef[Big.len - 1] + (BASE)1); // коэффициент нормализации

	int k = 0;

	BigNum U = *this;
	BigNum V = Big;

	U = U * d; // нормализация
	V = V * d;

	if (U.len == len) {
		if (U.maxlen == U.len) {
			U.maxlen++;
			U.len = U.maxlen;
			delete[]U.coef;
			U.coef = new BASE[U.maxlen];
			for (int i = 0; i < len; i++) {
				U.coef[i] = coef[i];
			}
			U = U * d;
			U.coef[U.len - 1] = 0;
		}
		U.coef[len] = 0;
		U.len = len + 1;
	}

	for (int j = m; j >= 0; j--) {

		DBASE qq = (((DBASE)U.coef[j + Big.len] << BASE_SIZE) + U.coef[j + Big.len - 1]) / V.coef[Big.len - 1];
		DBASE rr = (((DBASE)U.coef[j + Big.len] << BASE_SIZE) + U.coef[j + Big.len - 1]) % V.coef[Big.len - 1];

		if (qq == b || ((qq * V.coef[Big.len - 2]) > (b * rr + U.coef[j + Big.len - 2]))) {
			qq--;
			rr = rr + V.coef[Big.len - 1];
			if ((rr < b) && ((qq == b) || (qq * V.coef[Big.len - 2] > b * rr + U.coef[j + Big.len - 2]))) {
				qq--;
				rr = rr + V.coef[Big.len - 1];
			}
		}

		BigNum u(V.len + 1);
		u.len = V.len + 1;
		for (int i = 0; i < V.len + 1; i++) {
			u.coef[i] = U.coef[j + i];
		}

		if (u < V * qq) qq--;

		u = u - V * qq;

		for (int i = 0; i < V.len + 1; i++) {
			U.coef[j + i] = u.coef[i];
		}

	}

	U = U / d;
	return U;
}

BigNum& BigNum::operator%=(const BigNum& Big) {
	*this = *this % Big;
	return *this;
}

void BigNum::Input(){
	string S;
	cout << "Enter 10CC: ";
	cin >> S;
	
	int LEN = (S.length() - 1) / (BASE_SIZE / 4) + 1;
	BigNum Big(LEN);
	Big.len = LEN;
	for (int i = 0; i < S.length(); i++) {
		char num = 0;
		if ((S[i] >= '0') && (S[i] <= '9')) {
			num = S[i] - '0';
			Big = Big * 10 + num;
		}
		else {
			cout << "Error";
			exit(0);
		}
	}
	*this = Big;
}

void BigNum::Output() {
	BigNum NumToOut = *this;
	string s = "";
	while(NumToOut.coef[0] !=0 || NumToOut.len > 1) {
		BigNum r = NumToOut % 10;
		s += (r.coef[0] + '0');
		NumToOut = NumToOut / 10;
	}
	reverse(s.begin(), s.end());
	cout << "Num10: " << s << '\n';
}

void test() {
	srand(time(NULL));
	int M = 1000;
	int T = 1000;
	BigNum A;
	BigNum B;
	BigNum C;
	BigNum D;
		do {
			int n = rand() % M + 1;
			BigNum E(n, 1);
			int m = rand() % M + 1;
			BigNum F(m, 1);
			A = E;
			B = F;
			C = A / B;
			D = A % B;
			cout << m << " " << n << " ";
			cout << "T: " << T << '\n';
		} while (A == C * B + D && A - D == C * B && D < B && --T);
	cout << T << '\n';
}

int main() {
/*
	BigNum BN1;
	BigNum BN2;
	BN1.Input();
	BN2.Input();
	BigNum BN3;
	//BN3 = BN1 + BN2;
	BN1.Output();
	*/
	/*
	BN3 = BN3 - BN2;
	BN3.Output();
	BN3 = BN1 * BN2;
	BN3.Output();
	BN3 = BN3 / BN2;
	BN3.Output();
	BN3 = BN3 % BN2;
	BN3.Output();
	*/
	test();
}