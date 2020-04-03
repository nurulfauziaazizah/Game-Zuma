#include "ZumaLibrary.cpp"

short int PointerMenu = 0;

bool Menu ();
bool PlayMenu ();

int main (){
	//IsExit untuk memerintahkan program untuk exit/close jika bernilai true
	bool IsExit = false;


	//Kode untuk memuat highscore
	ifstream LoadHighScore;
	LoadHighScore.open(_HighScore);
	string TempLoad;
	if(!(LoadHighScore.fail())){
		getline(LoadHighScore, TempLoad);
		HighScoreLastIndex = StringToInt(TempLoad);
		for(int i=0; i<=HighScoreLastIndex; i++){
			getline(LoadHighScore, HighScore[i].Name);
			getline(LoadHighScore, TempLoad);
			HighScore[i].Score = StringToInt(TempLoad);
		}
	}
	LoadHighScore.close();


	//Kode untuk looping menu awal sampai IsExit bernilai true
	PrintTransition();
	do{
		IsExit = Menu();
	}while(!IsExit);

	return 0;
}

bool Menu () {
	//Kamus
	string pilihan;
	bool IsExit = false, IsBack = false;
	short int Space = 54;
	string MenuArray[] = {"PLAY", "HIGHSCORES", "HOW TO PLAY", "EXIT"};
	char Command;

	system("CLS");


	//Print judul program
	PrintZUMA();


	//Print menu utama
    cout<<"\n\n\n";
    for(int i=0; i<4; i++){
    	cout<<ReturnSomeTab(Space);
    	if(PointerMenu == i) cout<<"> ";
    	cout<<MenuArray[i]<<ReturnSomeTab(114 - Space - MenuArray[i].length())<<endl;
	}


	//Print tombol yang digunakan dalam permainan
	PrintControl();


	//Mendapatkan tombol yang ditekan pengguna
	Command = getch();
	switch(Command){
		case 'w':
		case 'W':
		case 'a':
		case 'A':
			PointerMenu--;
			if(PointerMenu < 0) PointerMenu += 4;
			break;
		case 's':
		case 'S':
		case 'd':
		case 'D':
			PointerMenu++;
			if(PointerMenu > 3) PointerMenu -= 4;
			break;
		case ' ':
			switch(PointerMenu){
				case 0:
					//Memulai game
					PrintTransition();
					Game(999);
					break;
				case 1:
					//Menampilkan highscores
					PrintTransition();
					PrintHighScores();
					break;
				case 2:
					//Menampilkan cara bermain
					PrintTransition();
					PrintHowToPlay();
					break;
				case 3:
					//Inisialisasi IsExit = true agar program keluar
					IsExit = true;
					break;
			}

			//Mereset pointer agar tetap berada di paling atas
			PointerMenu = 0;
	}

    return IsExit;
}
