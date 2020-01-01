#include <iostream>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <windows.h>
#include <string>
using namespace std;
string yazpath = "sonuc.txt";
string path = "kelime.txt";
char ch = 39;
string trim(const string& str) // dosya içerisindeki iki kelime arasındaki boşluğu yok sayar.
{
	size_t first = str.find_first_not_of(' ');
	if (string::npos == first)
	{
		return str;
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}
int inline minimum(int x, int y, int z) {
	return (x <= y && x <= z) ? x : (y <= x && y <= z) ? y : z;
}

struct word { // Tek yönlü liste
	string data;
	word* next;
};
word* ekle(word* r, string x) { // Tek yönlü listeye ekleme fonksiyonu
	if (r == NULL) { // İlk eleman boşsa ilk eleman tanımlanır ve nextine NULL atanır.
		r = new word;
		r->data = x;
		r->next = NULL;
	}
	else // İlk eleman boş değilse ilk elaman yrd adlı node'a  aktarılır ve yrd node'unun nexti  boş değilse yrd nodu yrd nodunun nextine atanır. 
	{
		word* yrd;
		yrd = r;
		while (yrd->next != NULL)
			yrd = yrd->next;
		yrd->next = new word;
		yrd->next->data = x;
		yrd->next->next = NULL;
		yrd = NULL;
		delete yrd;
	}
	return r;
}


int levenshtein(string s1, string s2) { // İki kelime arasındaki harf değişiklik sayısını bulur ve bu sayıyı döndürür.
	int x, y, s1len, s2len, sonuc;
	s1len = s1.size(); // ilk kelimenin uzunluğu
	s2len = s2.size(); // ikinci kelimenin uzunluğu
	int** matrix = new int* [s2len + 1];
	for (int i = 0; i < s2len + 1; i++)
		matrix[i] = new int[s1len + 1];
	matrix[0][0] = 0;
	for (x = 1; x <= s2len; x++)
		matrix[x][0] = matrix[x - 1][0] + 1;
	for (y = 1; y <= s1len; y++)
		matrix[0][y] = matrix[0][y - 1] + 1;
	for (x = 1; x <= s2len; x++)
		for (y = 1; y <= s1len; y++)
			matrix[x][y] = minimum(matrix[x - 1][y] + 1, matrix[x][y - 1] + 1, matrix[x - 1][y - 1] + (s1[y - 1] == s2[x - 1] ? 0 : 1));


	sonuc = matrix[s2len][s1len];



	return sonuc;
}


string tam_arama(word* root, string kelime) { // Tam arama

	while (root != NULL) // ilk node boş değilse döngü başlar
	{
		ofstream dosyaYaz(yazpath, ios::app); // Dosyaya yazma oluşturulur.
		string aranan = root->data; // aranan kelime root'un datasına aktarılır.
		for (int i = 0; i < aranan.length(); i++) // aranan kelimenin uzunluğu kadar for döndürülür.
		{
			aranan[i] = tolower(aranan[i]); // aranan kelimenin tüm karakterleri küçük harfe çevrilir.
		}
		for (int i = 0; i < kelime.length(); i++) //  dosya içerisinden gelen kelimenin boyu kadar for döndürülür.
		{
			kelime[i] = tolower(kelime[i]); // gelen kelimenin tüm karakterleri küçük harfe çevirilir.
		}
		if (aranan == kelime) // Aranan kelime dosyadan gelen kelimeye eşitse
		{
			cout << "Aradiginiz kelime: " << root->data << endl; // aranan kelime ekrana yazdırılır.
			dosyaYaz << "Aradiginiz kelime: " << root->data << endl; // aranan kelime dosyaya yazdırılıyor.
			dosyaYaz.close(); // dosya kapatılıyor.
			return "bulundu";
		}
		root = root->next;
	}
	return "Kelime yok";
}

void yakin_arama(word* r, string kelime) {
	string check[999];
	int sayac = 0, kontrol = 0;
	ofstream dosyaYaz(yazpath, ios::app);
	int size = kelime.size();
	int kriter = (size <= 5) ? 2 : 3;
	while (r != NULL)
	{
		string aranan = r->data;
		for (int i = 0; i < aranan.length(); i++)
		{
			aranan[i] = tolower(aranan[i]);
		}
		int mesafe = levenshtein(kelime, r->data);
		if (kelime.length() <= 20 && mesafe <= 3)
		{
			kontrol = 0;
			for (int i = 0; i < sayac + 1; i++)
			{
				if (check[i] == aranan)
					kontrol++;
			}
			if (kontrol == 0)
			{
				cout << "Bunu mu demek istediniz?\n\n" << r->data << "\n\n";
				check[sayac] = aranan;
				dosyaYaz << "Bunu mu demek istediniz? " << r->data << endl;
				sayac++;
			}
		}
		else if (kelime.length() > 20 && mesafe <= 6)
		{
			cout << "Bunu mu demek istediniz?\n\n" << r->data << "\n\n";
			dosyaYaz << "Bunu mu demek istediniz? " << r->data << endl;
		}
		r = r->next;

	}
	dosyaYaz.close();
}



int main()
{
adim1:
	cout << ".txt(1) .html(2) .xml(3) .rtf(4)" << endl;
	string aranan;
	int Secim;
	cin >> Secim;
	if (Secim == 1)
	{
		path = "kelime.txt";
		yazpath = "sonuc.txt";
		word* root;
		root = NULL;
		auto start = std::chrono::system_clock::now();
		ifstream file(path);
		ofstream dosyaYaz("sonuc.txt", ios::app);
		char karakter;
		string kelime;
		while (file.get(karakter)) {

			if (karakter != ',' && karakter != '?' && karakter != '.' && karakter != '-' && karakter != '!' && karakter != '"' && karakter != ';' && karakter != ':' && karakter != '(' && karakter != ')' && karakter != '[' && karakter != ']' && karakter != ch)
				kelime = kelime + karakter;
			if (karakter == ' ')
			{
				root = ekle(root, trim(kelime));
				kelime = "";

			}
		}
		cout << "aramak istediginiz kelimeyi giriniz" << endl;
		cin >> aranan;
		yakin_arama(root, aranan); // kelıme buraya girilcek merhaba yerıne 
		string sonuc = tam_arama(root, aranan);
		cout << sonuc;
		auto end = chrono::system_clock::now();
		chrono::duration<double> diff = end - start;
		cout << "  gecen zaman : " << diff.count() << "s" << endl;
		dosyaYaz.close();
		file.close();
		system("sonuc.txt");
		system("pause");
		goto adim1;
	}
	if (Secim == 2)
	{
		path = "kelime.html";
		yazpath = "sonuc.html";
		word* root;
		root = NULL;
		auto start = std::chrono::system_clock::now();
		ifstream file(path);
		ofstream dosyaYaz("sonuc.html", ios::app);
		char karakter;
		string kelime;
		while (file.get(karakter)) {

			if (karakter != ',' && karakter != '?' && karakter != '.' && karakter != '-' && karakter != '!' && karakter != '"' && karakter != ';' && karakter != ':' && karakter != '(' && karakter != ')' && karakter != '[' && karakter != ']' && karakter != ch)
				kelime = kelime + karakter;
			if (karakter == ' ')
			{
				root = ekle(root, trim(kelime));
				kelime = "";

			}
		}
		cout << "aramak istediginiz kelimeyi giriniz" << endl;
		cin >> aranan;
		yakin_arama(root, aranan); // kelıme buraya girilcek merhaba yerıne 
		string sonuc = tam_arama(root, aranan);
		cout << sonuc;
		auto end = chrono::system_clock::now();
		chrono::duration<double> diff = end - start;
		cout << "  gecen zaman : " << diff.count() << "s" << endl;
		dosyaYaz.close();
		file.close();
		system("sonuc.html");
		system("pause");
		goto adim1;
	}
	if (Secim == 3)
	{
		path = "kelime.xml";
		yazpath = "sonuc.xml";
		word* root;
		root = NULL;
		auto start = std::chrono::system_clock::now();
		ifstream file(path);
		ofstream dosyaYaz("sonuc.xml", ios::app);
		char karakter;
		string kelime;
		while (file.get(karakter)) {

			if (karakter != ',' && karakter != '?' && karakter != '.' && karakter != '-' && karakter != '!' && karakter != '"' && karakter != ';' && karakter != ':' && karakter != '(' && karakter != ')' && karakter != '[' && karakter != ']' && karakter != ch)
				kelime = kelime + karakter;
			if (karakter == ' ')
			{
				root = ekle(root, trim(kelime));
				kelime = "";

			}
		}
		cout << "aramak istediginiz kelimeyi giriniz" << endl;
		cin >> aranan;
		yakin_arama(root, aranan); // kelıme buraya girilcek merhaba yerıne 
		string sonuc = tam_arama(root, aranan);
		cout << sonuc;
		auto end = chrono::system_clock::now();
		chrono::duration<double> diff = end - start;
		cout << "  gecen zaman : " << diff.count() << "s" << endl;
		dosyaYaz.close();
		file.close();
		system("sonuc.xml");
		system("pause");
		goto adim1;
	}
	if (Secim == 4)
	{
		path = "kelime.rtf";
		yazpath = "sonuc.rtf";
		word* root;
		root = NULL;
		auto start = std::chrono::system_clock::now();
		ifstream file(path);
		char karakter;
		string kelime;
		while (file.get(karakter)) {

			if (karakter != ',' && karakter != '?' && karakter != '.' && karakter != '-' && karakter != '!' && karakter != '"' && karakter != ';' && karakter != ':' && karakter != '(' && karakter != ')' && karakter != '[' && karakter != ']' && karakter != ch)
				kelime = kelime + karakter;
			if (karakter == ' ')
			{
				root = ekle(root, trim(kelime));
				kelime = "";

			}
		}
		cout << "aramak istediginiz kelimeyi giriniz" << endl;
		cin >> aranan;
		yakin_arama(root, aranan); // kelıme buraya girilcek merhaba yerıne 
		string sonuc = tam_arama(root, aranan);
		cout << sonuc;
		auto end = chrono::system_clock::now();
		chrono::duration<double> diff = end - start;
		cout << "  gecen zaman : " << diff.count() << "s" << endl;
		file.close();
		system("sonuc.rtf");
		system("pause");
		goto adim1;
	}
	else
	{
		cout << "lutfen gecerli bir sayi giriniz" << endl;
		system("pause");
		goto adim1;
	}
	system("pause");

}


/* 
Veri Yapilari dersi projesi
Grup sırası 10

172113037 Mert Ozden vy11
172113001 Ahmet Tahir Sezis vy11
172113041 Muhammed Talha Köse vy11
172113073 Orkan Sahinoglu vy12

*/