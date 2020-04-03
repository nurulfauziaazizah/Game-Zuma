#include <iostream> //Untuk cin dan cout, fungsi biasa
#include <cstdlib> //Untuk alokasi memori
#include <conio.h> //Untuk fungsi getch
#include <ctime> //Untuk merandomize rand()
#include <sstream> //Untuk konversi string to int
#include <windows.h> //Untuk penggunaan fungsi system
#include <fstream> //Untuk file eksternal
using namespace std;

typedef char infotype;
typedef struct ElementStruct* address;

typedef struct ElementStruct {
	infotype Value;
	address Previous;
	address Next;
} Element;

struct List {
	address First;
	address Last;
	int Total;
};

struct StructHighScore {
	string Name;
	int Score;
};

//Variabel global untuk permainan zuma
int JumlahJalur = 15, BanyakBolaKeluar = 5, Pointer = BanyakBolaKeluar - 1, JarakPointer = 2, BallVariant = 3;
int Score = 0, Phase = 1;
char Jalur, Command, Jenis[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'}, CurrentBall;

//Variabel global untuk penyimpanan highscores
char _HighScore[] = "./res/HighScore.savedata";
StructHighScore HighScore[10];
short int HighScoreLastIndex = -1;

//Deklarasi awal fungsi & prosedur

//Fungsi tampilan
void PrintZUMA ();
void PrintTransition ();
void PrintControl ();
void PrintStarsWithEndline (int Width, int Height);
string ReturnSomeTab (int HowMuch);
void PrintHowToPlay ();

//Fungsi game
int Game (int HowMuchBall);
void EndlessModeLevelChange (int Score);
void HilangkanTigaBolaDiAwalList (List *L, int Index);
void HilangkanTigaBola (List *L, int Index, int BombStreak);

//Fungsi primitif double linked list berindex
void CreateEmpty (List *L);
bool IsEmpty (List L);
bool IsOneElement(List L);
address Allocate ();
void Deallocate (address Element);
address Access (List *L, int Index);
bool InsertAfter (List *L, int Index, infotype Value);
void DeleteAfter (List *L, int Index);
void LinearPrint (List L, int ElementFromFirst);



//TAMPILAN
void PrintZUMA () {
	PrintStarsWithEndline(116, 6);
	cout<<ReturnSomeTab(38)<<" ______   __    __   __     __   ___"<<ReturnSomeTab(38)<<endl;
	cout<<ReturnSomeTab(38)<<"/__   /  |  |  |  | |  \\   /  | |   \\"<<ReturnSomeTab(38)<<endl;
	cout<<ReturnSomeTab(38)<<"  /  /   |  |__|  | |   \\_/   | | |\\ \\"<<ReturnSomeTab(38)<<endl;
	cout<<ReturnSomeTab(38)<<" /  /__  |________| |         | |_____\\"<<ReturnSomeTab(38)<<endl;
	cout<<ReturnSomeTab(38)<<"/______| |________| |__|\\_/|__| |_____|"<<ReturnSomeTab(38)<<endl;
	cout<<ReturnSomeTab(34)<<"A  L  T  E  R  N  A  T  I  V  E    S  P  A  C  E"<<endl;
}

void PrintControl () {
	//Print tombol yang digunakan dalam permainan
	PrintStarsWithEndline(116, 8);
	cout<<ReturnSomeTab(37)<<"[W]"<<endl;
	cout<<ReturnSomeTab(25)<<"Control: [A][S][D] to Move                     [SPACEBAR] to Select/Shoot"<<endl;
}

void PrintTransition () {
	//Print transisi kresek-kresek
	system("CLS");
	for(int i=0; i<10; i++){
		PrintStarsWithEndline(116, 32);
		Sleep(0.01);
		system("CLS");
	}
}

void PrintStarsWithEndline (int Width, int Height) {
	//Print kekosongan dengan aksen bintang dengan endl
	for(int i=0; i<Height; i++){
		cout<<ReturnSomeTab(Width)<<endl;
	}
}

string ReturnSomeTab (int HowMuch) {
	//Mengembalikan string dengan isi spasi dengan aksen bintang tanpa endl
	string Tabs = "";
	short int Star, Limit = rand() % 8;
	
	for(int i=0; i<HowMuch; i++){
		Star = rand() % 500;
		if(Star < 10 && Limit <= 0){
			Tabs += "*";
			Limit = 8;
		}else{
			Tabs += " ";
			Limit--;
		}
	}
	return Tabs;
}

void PrintHighScores () {
	//Menampilkan highscores
	PrintStarsWithEndline(116, 4);
	cout<<ReturnSomeTab(55)<<"HIGHSCORES"<<ReturnSomeTab(54);
	PrintStarsWithEndline(116, 2);
	
	//Fetch highscores
	if(HighScoreLastIndex == -1){
		cout<<ReturnSomeTab(50)<<"No highscore for now~"<<ReturnSomeTab(50)<<endl;
	}else{
		int NameMaxLength;
		for(int i=0; i<=HighScoreLastIndex; i++){
			if(i==0) NameMaxLength = HighScore[i].Name.length();
			if(NameMaxLength < HighScore[i].Name.length()){
				NameMaxLength = HighScore[i].Name.length();
			}
		}
		
		for(int i=0; i<=HighScoreLastIndex; i++){
			cout<<ReturnSomeTab(50)<<HighScore[i].Name;
			for(int j=0; j<(NameMaxLength - HighScore[i].Name.length() + 2); j++){
				cout<<" ";
			}
			cout<<HighScore[i].Score<<endl;
		}
	}
	
	PrintStarsWithEndline(116, 2);
	cout<<ReturnSomeTab(50)<<"PRESS ANY KEY TO BACK"<<ReturnSomeTab(54);
	PrintStarsWithEndline(116, 16-HighScoreLastIndex);
	getch();
	PrintTransition();
}

void PrintHowToPlay () {
	//Print cara bermain
	PrintStarsWithEndline(116, 4);
	cout<<ReturnSomeTab(52)<<"HOW TO PLAY"<<ReturnSomeTab(54);
	PrintStarsWithEndline(116, 2);
	cout<<ReturnSomeTab(30)<<"In the main menu, select PLAY to play endless Zuma game."<<ReturnSomeTab(30)<<endl;
	cout<<ReturnSomeTab(30)<<"There will be one lane of balls, a pointer (shooter), and"<<ReturnSomeTab(30)<<endl;
	cout<<ReturnSomeTab(30)<<"a hole (represented by XXX in the end of lane). You can"<<ReturnSomeTab(30)<<endl;
	cout<<ReturnSomeTab(30)<<"detonate the ball by shoot at two same balls or more. You"<<ReturnSomeTab(30)<<endl;
	cout<<ReturnSomeTab(30)<<"can detonate up to three kind of balls in one shoot. You"<<ReturnSomeTab(30)<<endl;
	cout<<ReturnSomeTab(30)<<"will be a winner if the remaining ball reach zero point,"<<ReturnSomeTab(30)<<endl;
	cout<<ReturnSomeTab(30)<<"and you will be a loser if the balls touch the hole (XXX)."<<ReturnSomeTab(30)<<endl;
	PrintStarsWithEndline(116, 2);
	cout<<ReturnSomeTab(47)<<"PRESS ANY KEY TO BACK"<<ReturnSomeTab(56);
	PrintStarsWithEndline(116, 8);
	getch();
	PrintTransition();
}



int Game (int HowMuchBall) {
	//membuat list baru
	List L;
	CreateEmpty(&L);
	
	//resetting game
	JumlahJalur = 15;
	BanyakBolaKeluar = 5;
	Pointer = BanyakBolaKeluar - 1;
	JarakPointer = 2;
	BallVariant = 3;
	Score = 0;
	Phase = 1;
	
	srand(time(0));
	bool IsEndlessMode = (HowMuchBall == 999);
	if(IsEndlessMode) EndlessModeLevelChange(0);
	
	//Mengisi List dengan random char
	for(int i=0;i<HowMuchBall;i++){
		if(i>120){
			InsertAfter(&L, L.Total, Jenis[rand() % 5]);
		}else if(i>20){
			InsertAfter(&L, L.Total, Jenis[rand() % 4]);
		}else{
			InsertAfter(&L, L.Total, Jenis[rand() % BallVariant]);
		}
	}
	
	//Merefresh fungsi rand thd waktu dan Mengacak nilai CurrentBall
	srand(time(NULL));
	CurrentBall = Jenis[rand() % BallVariant];
	
	do {
		system("CLS");
		
		//Jika bola keluar 0, maka setidaknya keluar 1 bola
		if(BanyakBolaKeluar <= 0 && L.Total >= 0){
			BanyakBolaKeluar = 1;
		}else if(L.Total < BanyakBolaKeluar){
			BanyakBolaKeluar = L.Total;
		}
		
		//Jika pointer tidak tampil di layar, maka pointer direset ke tempat semula
		if(Pointer > BanyakBolaKeluar - 1 || Pointer < -1){
			Pointer = BanyakBolaKeluar - 1;
		}
		
		//menampilkan header
		cout<<ReturnSomeTab(28)<<"Remaining Ball(s): "<<L.Total<<"    Score: "<<Score<<"    Phase: "<<Phase<<endl;
		cout<<ReturnSomeTab(28)<<"================================================================="<<endl<<endl;
		
		//menampilkan bola ke layar sebanyak BanyakBolaKeluar
		if(Phase < 7){
			PrintStarsWithEndline(116, 6);
		}else{
			PrintStarsWithEndline(116, 2);
		}
		
		cout<<ReturnSomeTab(43-JumlahJalur);
		LinearPrint(L, BanyakBolaKeluar);
		
		//menampilkan blank untuk jalur, dan XXX untuk lubang
		for(int i=JumlahJalur;i>BanyakBolaKeluar;i--){
			cout<<" ___";
		}
		cout<<" XXX"<<ReturnSomeTab(43-JumlahJalur);
		
		//membuat jarak jalur zuma ke pointer
		PrintStarsWithEndline(116, JarakPointer);
		
		//membuat pointer arah (^)
		cout<<ReturnSomeTab(43-JumlahJalur);
		for(int i=BanyakBolaKeluar-1;i>=-1;i--){
			if(i!=Pointer){
				cout<<"    ";
			}else{
				cout<<"^ ";
			}
		}
		cout<<endl;
		
		//membuat pointer bola yang akan ditembak
		cout<<ReturnSomeTab(43-JumlahJalur);
		for(int i=BanyakBolaKeluar-1;i>=-1;i--){
			if(i!=Pointer){
				cout<<"    ";
			}else{
				cout<<CurrentBall<<" ";
			}
		}
		
		//mendapatkan char/tombol yang ditekan
		Command = getch();
		switch (Command) {
			case 'A':
			case 'a':
				//pointer bergerak ke kiri
				if(Pointer != BanyakBolaKeluar - 1){
					Pointer++;
				}
				break;
			case 'D':
			case 'd':
				//pointer bergerak ke kanan
				if(Pointer != -1){
					Pointer--;
				}
				break;
			case ' ':
				//menembakkan bola
				InsertAfter(&L, Pointer, CurrentBall);
				BanyakBolaKeluar += 1;
				
				//mengecek apakah 3 bola sama di dekat pointer, jika iya akan dihapus
				if(Pointer<0){
					HilangkanTigaBolaDiAwalList(&L, Pointer);          
				}else{
					HilangkanTigaBola(&L, Pointer, 0);//masih ada yg ngikut kehapus
				}
				
				if(IsEndlessMode) EndlessModeLevelChange(Score);
				
				//mengacak bola yang akan keluar
				CurrentBall = Jenis[rand() % BallVariant];
				
				//memudahkan player, jika hanya ada 1 bola (di list juga 1) maka yg keluar adalah bola yg ada di list
				if(L.Total == 2){
					CurrentBall = L.First->Next->Value;
				}else if(L.Total == 1){
					CurrentBall = L.First->Value;
				}
				break;
		}
		
		
		//diulang jika BanyakBolaKeluar lebih kecil dari JumlahJalur
		//dan Total Bola di dalam List bukan 0
	} while(BanyakBolaKeluar <= JumlahJalur && L.Total > 0);
	
	//menampilkan pesan menang/kalah
	string Name;
	PrintTransition();
	PrintStarsWithEndline(116, 12);
	
	//menampilkan pesan setelah game selesai
	if(BanyakBolaKeluar > JumlahJalur){
		cout<<ReturnSomeTab(54)<<"YOU LOSE!"<<endl;
	}else if(L.Total < 1){
		cout<<ReturnSomeTab(54)<<"YOU WIN!"<<endl;
	}
	cout<<ReturnSomeTab(54)<<"Your Score: "<<Score<<endl<<endl;
	cout<<ReturnSomeTab(49)<<"Input your name: ";
	getline(cin, Name);
	
	
	//menambahkan ke highscores
	if(HighScoreLastIndex == -1){
		HighScore[0].Score = Score;
		HighScore[0].Name = Name;
		HighScoreLastIndex  = 0;
	}else{
		for(int i=0; i<10; i++){
			if(HighScore[i].Score <= Score){
				//if(HighScoreLastIndex != i){
					for(int j=HighScoreLastIndex; j>=i; j--){
						if(j != 9){
							HighScore[j+1].Score = HighScore[j].Score;
							HighScore[j+1].Name = HighScore[j].Name;
						}
					}
				//}
				
				HighScore[i].Score = Score;
				HighScore[i].Name = Name;
				if(HighScoreLastIndex != 9)	HighScoreLastIndex++;
				break;
			}
		}
		
	}
	
	//menyimpan highscores ke file eksternal
	ofstream SaveHighScore;
	SaveHighScore.open(_HighScore, ofstream::trunc);
	for(int i=-1; i<=HighScoreLastIndex; i++){
		if(i==-1){
			SaveHighScore<<HighScoreLastIndex<<endl;
		}else if(i==HighScoreLastIndex){
			SaveHighScore<<HighScore[i].Name<<endl;
			SaveHighScore<<HighScore[i].Score;
		}else{
			SaveHighScore<<HighScore[i].Name<<endl;
			SaveHighScore<<HighScore[i].Score<<endl;
		}	
	}
	SaveHighScore.close();
	
	//menghapus isi dari list setelah selesai digunakan
	for(int i=0; i<=L.Total; i++){
		DeleteAfter(&L, -1);
	}
}

void EndlessModeLevelChange (int Score){
	//Untuk mengubah phase sesuai skor yang didapatkan
	if(Score == 0){
		Score = 0;
		Phase = 1;
		JumlahJalur = 15;
		JarakPointer = 2;
		BallVariant = 3;
	}else if(Score >= 206000){
		Phase = 999;
		JumlahJalur = 8;
	}else if(Score >= 204800){
		Phase = 7;
		JarakPointer = 20;
	}else if(Score >= 102400){
		Phase = 6;
		JumlahJalur = 10;
		JarakPointer = 16;
	}else if(Score >= 51200){
		Phase = 5;
		JumlahJalur = 12;
	}else if(Score >= 25600){
		Phase = 4;
		JumlahJalur = 15;
		JarakPointer = 12;
		BallVariant = 5;
	}else if(Score >= 6400){
		Phase = 3;
		JumlahJalur = 13;
		JarakPointer = 8;
	}else if(Score >= 1600){
		Phase = 2;
		JumlahJalur = 14;
		JarakPointer = 4;
		BallVariant = 4;
	}
}

void HilangkanTigaBolaDiAwalList (List *L, int Index){
	//Untuk menghilangkan jenis bola yang sama minimal 3 bola berurutan, digunakan pada Pointer = -1
	address CurrentElement = Access(L, Index + 1);
	address LeftElement = CurrentElement->Next;
	
	char Bola = CurrentElement->Value;
	
	int Left = 0;
	bool StopLeft;
	while(LeftElement != NULL && !StopLeft && !((Left + Index + 1) > (BanyakBolaKeluar-2))){
		if(LeftElement->Value == Bola){
			Left++;
		}else{
			StopLeft = true;
		}
		LeftElement = LeftElement->Next;
	}
	
	if(Left >= 2){
		for(int i=0;i<=Left;i++){
			DeleteAfter(L, -1);
			BanyakBolaKeluar--;
		}
		if(!((*L).Total <= BanyakBolaKeluar + 1)) BanyakBolaKeluar++;
		Score += 100;
	}
}

void HilangkanTigaBola (List *L, int Index, int BombStreak){
	//Fungsi rekursif, Untuk menghilangkan jenis bola yang sama minimal 3 bola berurutan, digunakan pada Pointer != -1
	address CurrentElement = Access(L, Index);
	address LeftElement = CurrentElement->Next;
	address RightElement = CurrentElement;
	
	
	if(LeftElement == NULL){
		if(BombStreak > 0 && BanyakBolaKeluar + BombStreak <= (*L).Total){
			BanyakBolaKeluar += BombStreak;
		}
		return;
	} 
	char Bola = CurrentElement->Next->Value;
	
	int Left = 0 - BombStreak, Right = -1 + BombStreak;
	bool StopLeft = false, StopRight = false;
	while(LeftElement != NULL && !StopLeft && !((Left + Index) > (BanyakBolaKeluar-2-BombStreak))){
		if(LeftElement->Value == Bola){
			Left++;
		}else{
			StopLeft = true;
		}
		LeftElement = LeftElement->Next;
	}
	
	while(RightElement != NULL && !StopRight){
		if(RightElement->Value == Bola){
			Right++;
		}else{
			StopRight = true;
		}
		RightElement = RightElement->Previous;
	}
	
	int Streak = Left + Right + 1, DeleteAfterIndex = Pointer - Right - 1;
	if(Streak >=3){
		for(int i=0;i<Streak;i++){
			DeleteAfter(L, DeleteAfterIndex);
			BanyakBolaKeluar--;
		}

		Score += (BombStreak + 1)*100;
		if(DeleteAfterIndex >= 0){
			HilangkanTigaBola(L, DeleteAfterIndex, BombStreak + 1);
		}
	}else{
		if(BombStreak > 0 && BanyakBolaKeluar + BombStreak <= (*L).Total){
			BanyakBolaKeluar += BombStreak;
		}
	}
}

int StringToInt (string Text) {
	//mengubah string ke int
	int Num;
	istringstream ISStream(Text);
	ISStream>>Num;
	return Num;
}

void CreateEmpty (List *L){
	//mengosongkan list
	(*L).First = NULL;
	(*L).Last = NULL;
	(*L).Total = -1;
}

bool IsEmpty (List L){
	//mengecek apakah list kosong
	return (L.Total == -1);
}

bool IsOneElement(List L){
	//mengecek apakah list berisi satu elemen saja
	return (L.Total == 0);
}

address Allocate (){
	//mengalokasi Element di memory sebesar Element
	address NewElement = (Element*) malloc (sizeof(Element));
	NewElement->Next = NULL;
	NewElement->Previous = NULL;
	return NewElement;
}

void Deallocate (address Element){
	//menghapus Element dalam memory
	if(Element != NULL){
		free(Element);
	}
}

address Access (List *L, int Index){
	//mengakses Element pada List dengan Indeks, misal akses index ke 1 = Access(&L, 1);
	address AnElement = NULL;
	if(!IsEmpty(*L) && Index <= (*L).Total && Index >= 0){
		int Half = (*L).Total;
		Half /= 2;
		int Current;
	
		if(Index < Half){
			AnElement = (*L).First;
			Current = 0;
			while(Index != Current){
				AnElement = AnElement->Next;
				Current++;
			}
		}else{
			AnElement = (*L).Last;
			Current = (*L).Total;
			while(Index != Current){
				AnElement = AnElement->Previous;
				Current--;
			}
		}
	}
	
	return AnElement;
}

bool InsertAfter (List *L, int Index, infotype Value){
	//Menyelipkan Element baru ke dalam list setelah Index tertentu
	address NewElement = Allocate();
	bool Success = true;
	
	if(IsEmpty(*L)){
		(*L).First = NewElement;
		(*L).Last = NewElement;
	}else{
		if(Index == -1){
			(*L).First->Previous = NewElement;
			NewElement->Next = (*L).First;
			(*L).First = NewElement;
		}else if(Index == (*L).Total){
			(*L).Last->Next = NewElement;
			NewElement->Previous = (*L).Last;
			(*L).Last = NewElement;
		}else if(Index > -1 && Index < (*L).Total){
			address ElementIndex = Access(L, Index);
			NewElement->Previous = ElementIndex;
			NewElement->Next = ElementIndex->Next;
			ElementIndex->Next->Previous = NewElement;
			ElementIndex->Next = NewElement;
		}else{
			Success = false;
		}
	}
	
	if(Success){
		int TempTotal = (*L).Total;
		(*L).Total = TempTotal + 1;
		NewElement->Value = Value;
	}
	
	return Success;
}

void DeleteAfter (List *L, int Index){
	//Menghapus Element dari list setelah Index tertentu
	address ToBeDeleted = NULL;
	if(Index < (*L).Total && Index >= -1){
		if(!IsEmpty(*L) && IsOneElement(*L)){
			ToBeDeleted = (*L).First;
			CreateEmpty(L);
			Deallocate(ToBeDeleted);
		}else if(!IsEmpty(*L)){
			if(Index == -1){
				ToBeDeleted = (*L).First;
				(*L).First = (*L).First->Next;
				(*L).First->Previous = NULL;
			}else if(Index == (*L).Total - 1){
				ToBeDeleted = (*L).Last;
				(*L).Last = (*L).Last->Previous;
				(*L).Last->Next = NULL;
			}else if(Index > -1 && Index < (*L).Total - 1){
				address ElementIndex = Access(L, Index);
				ToBeDeleted = ElementIndex->Next;
				ElementIndex->Next = ElementIndex->Next->Next;
				ElementIndex->Next->Previous = ElementIndex;
			}
			
			if(ToBeDeleted != NULL){
				int TempTotal = (*L).Total;
				(*L).Total = TempTotal - 1;
				Deallocate(ToBeDeleted);
			}
		}
	}
	
}

void LinearPrint (List L, int ElementFromFirst){
	//Print isi dalam list sejumlah ElementFromFirst dari awal list
	int Current = L.Total;
	address CurrentElement = L.Last;
	
	while(Current != -1 && CurrentElement != NULL){
		if(Current < ElementFromFirst){
			cout<<" ("<<CurrentElement->Value<<")";
		}
		CurrentElement = CurrentElement->Previous;
		Current--;
	}
	
}
