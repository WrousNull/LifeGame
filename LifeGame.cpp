#include<iostream> 
#include<cstdlib>
#include<conio.h>
#include<windows.h>
#include<fstream>
#include<string>
#include <ctime>
#include <sstream>
//����������������������������������������������������������������������������������������������������������������������������������������������#ʹ�õĿ�#����������������������������������������������������������������������������������������������������������������������������������������������������������������������
using namespace std;

unsigned int Hight;
unsigned int Width;

bool DontInit = false;		//�Ƿ�������ʼ��
class Cell
{
	private:
		int x;
		int y;
		bool life;
		int alive_amount;   //��¼��ϸ����Χ��ϸ������
	public:
		Cell(unsigned int row=1,unsigned int col=1,bool m_life=false,int m_alive_amount=0);	//���캯������ʼ��ϸ����λ��
		~Cell();			//��������
		bool UpdateAlive();	//��¼��Χ��ϸ�����������
		bool checkLife();	//���life�Ǵ��
		void AliveLife();	//ʹ��life=true
		void DeadLife();	//ʹ��life=false
		void initLife();	//��ʼ��ϸ������״̬
};
//������������������������������������������������������������������������������������������������������������������������������������������#����ı���#��������������������������������������������������������������������������������������������������������������������������������������������������������������������
Cell ** canvas = NULL;	//����һ����������¼ÿһ��λ�õ�ϸ��״̬ 
void SetConsoleWindowSize( SHORT width, SHORT height );
void init(unsigned int & Hight,unsigned int & Width);
void showRegular();
void display();
void run();
void showStart();
void SavingFile();
void ReadingFile();
void doGame();
void End();
void ReadingFile();
bool Modify();
//����������������������������������������������������������������������������������������������������������������������������������������������#��������#����������������������������������������������������������������������������������������������������������������������������������������������������������������������
int main()
{
	srand((unsigned)time(NULL));// �������������
	showStart();				//���ƿ�ʼ���棨���Ľ���
	init(Hight,Width);			//��ʼ��			
	doGame();					//��Ϸ���߼�
	End();						//������Ϸ
	return 0;
}
//������������������������������������������������������������������������������������������������������������������������������������������������#������#����������������������������������������������������������������������������������������������������������������������������������������������������������������������
Cell::Cell(unsigned int row,unsigned int col,bool m_life,int m_alive_amount)						//���캯������������ʱ�Զ�����
{
	x = col;
	y = row;
	life = false;
}
Cell::~Cell()
{
	//Nothing
}
bool Cell::checkLife()				//��ȡ���״̬
{
	bool _life = life;
	return _life;
}
void Cell::AliveLife()				//ʹ��ϸ������
{
	life = true;
}
void Cell::DeadLife()				//ʹ��ϸ������
{
	life = false;
}
void Cell::initLife()				//�����ʼ��ϸ����״̬
{
	life = rand() % 2;
}
bool Cell::UpdateAlive()			//����ϸ�����״̬��������һ���Ĵ��boolֵ
{
	alive_amount = 0;
	
    for(int i = y-1 ; i<=y+1 ; ++i)     //�������������⣺����Եϸ������ʱ��������δ������ڴ浥Ԫ��
    {
        for(int j = x-1 ; j<=x+1 ; ++j)
        {
            if(i==y && j==x)
            {
                continue;
            }
            else if(canvas[i][j].life)
            {
                alive_amount++;
            }
        }
    }
    //�ж���ϸ����һ��������״̬
    if(life)                    //��ϸ��״̬
    {
        if( alive_amount < 2 )
        {
            life = false;       
        }
        else if( alive_amount > 3 )
        {
            life = false;
        }
    }
    else                        //��ϸ��״̬
    {
        if( alive_amount == 3 )
        {
            life = true;
        }
    }
    return life;
}
//����������������������������������������������������������������������������������������������������������������������������������������������#�ຯ������#����������������������������������������������������������������������������������������������������������������������������������������������������������������
void init(unsigned int & Hight,unsigned int & Width)		//���ϸ���ĳ�ʼ��
{
	if( DontInit )
	{
		return;
	}
	FLAG:
	cout << "Please enter the Lenght(>0):___\b\b\b";
	cin >> Hight;
	if( Hight <= 0 )
	{
		cout << "The lenght should > 0!" << endl;
		Sleep(1000);
		goto FLAG;
	}
	Hight += 2;
	Width = Hight;
	canvas = new Cell*[Hight];
	for(unsigned int row = 0; row < Hight; ++row)
	{
	    canvas[row] = new Cell[Width];
	    for(unsigned int col = 0; col < Width; ++col)
	    {
	    	canvas[row][col] = Cell(col, row);
	    	if( row == 0 || row == Hight-1 || col == 0 || col == Width-1 )
	    	{
	    		canvas[row][col].DeadLife();
	    		continue;
			}
	        else
	        {
	        	canvas[row][col].initLife();
			}
	    }
	}
	SetConsoleWindowSize( (Width+1)*2 , Hight+2 );	//�����ն˴�С
}
void display()										//��ʾϸ��
{
	for(unsigned int row = 1 ; row < Hight-1 ; ++row)
	{
		for(unsigned int col = 1 ; col < Width-1 ; ++col)
		{
			if(canvas[row][col].checkLife())
			{
				cout << "* " ;
			}
			else
			{
				cout <<"  ";
			}
		}
		cout << "\n"; 
	}
}
void SetConsoleWindowSize( SHORT width, SHORT height )	//�����ն˴�С
{
    HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT wrt = { 0, 0, width-1, height-1 };
    SetConsoleWindowInfo( hStdOutput, TRUE, &wrt ); // ���ô���ߴ�
    COORD coord = { width, height };
    SetConsoleScreenBufferSize( hStdOutput, coord ); // ���û���ߴ�
}
void run()							//ִ����һ��
{
	for(unsigned int row = 1 ; row < Hight-1 ; ++row)
	{
		for(unsigned int col = 1 ; col < Width-1 ; ++col)
		{
			canvas[row][col].UpdateAlive();
		}
	}
}
void showRegular()					//��ʾ��������
{
	cout << endl;
	cout << "Game Regular" << endl;
	cout << "When game is running:" << endl;
	cout << "	*You can press 'q' to quit" << endl;
	cout << "	*Press 'm' to modify the cell's life." << endl;
	cout << "	*Press another key to suspend." << endl;
	cout << endl;
}
void showStart()					//���ƿ�ʼ����
{
	cout << "Welcome to play this game! "<< endl;
	showRegular();
	cout << "Now you can press 'r' to read the file." << endl; 
	cout << "Or press another key to Start a new game:" << endl;
	char is_start;
	is_start = getch();
	if( is_start == 'r' )
	{
		ReadingFile();
		DontInit = true;
	}
}
void End()							//�����߼�����
{
	for(unsigned int row = 0; row < Hight; ++row)
	{
	    delete [] canvas[row];
	}
	delete [] canvas;
	system("cls");					//��ջ���
	cout << "GoodBye." << endl;
	getch();
}
void doGame()						//ִ����Ϸ������
{
	system("cls");
	while(true)
	{
		display();
		Sleep(500);
		run();
		char input;
		if (_kbhit()) 				// �ж��Ƿ�������
		{
			input = _getch(); 		
			if(input == 'q' )		//����q�˳�
			{
				cout << "You can press 's' to save." << endl;
				cout << "Or press another key to quit directly" << endl;
				char choice = getch();			//ͣ������ֱ���û��ٴΰ���ĳ��
				if( choice == 's' )
				{
					SavingFile();
				}
					break;
			}
			else if( input == 'm' )				//�û�Ҫ����ϸ����״̬
			{
				system("cls");
				while(!Modify())
				{
					system("cls");
				}
			}
			else					//���³���q��ĳ��,��ͣ
			{
				system("cls");
				for( unsigned int i = 0 ; i<Hight/2 ; ++i )
				{
					cout << endl;
				}
				cout << "Press any key to continue..." << endl;
				getch();
			}
		}
		system("cls");				//��ջ���
	}
}
void SavingFile()					//���溯��
{
	ofstream file_out;
	string filename_out;
	cout << "Enter your file name that you want to save (txt):___\b\b\b";
	cin >> filename_out;
	file_out.open(filename_out);

	file_out << Hight << " " << Width << endl;			//������������Ĵ�С:ǰ��������,Ҫ��ס���ǣ������ļ�ʱ�����ٶ���ֵ�����޸�,ֱ�ӵ���.
	for(unsigned int row = 0; row < Hight; ++row)
	{
	    for(unsigned int col = 0; col < Width; ++col)
	    {
	    	file_out << canvas[row][col].checkLife() << endl;	//����ÿ�������״̬:���������boolֵ������Ҫ��������ת����
	    }
	}
	file_out.close();
	cout << "You have saved the game." << endl;
}
void ReadingFile()					//��ȡ�浵����
{
	string filename_in;
	FLAG:
	system("cls");
	cout << "The target file needs to be located in the same level directory as this cpp file." << endl;
	cout << "Enter your file name that you want to read:___\b\b\b";
	cin >> filename_in;

	ifstream infile(filename_in);	//��һ�����ڶ�ȡ���ļ�

    if (!infile.is_open())
    {
        cout << "Error opening file!" << endl;
		cout << "Please try again.";
		Sleep(1000);
        goto FLAG;	//���û�ܳɹ����򷵻������ļ�����λ��
    }
	string line;
    getline(infile, line);
    stringstream ss(line);
    ss >> Hight >> Width;
	SetConsoleWindowSize( 2*(Width+2), Hight+2 );
	int num;
	canvas = new Cell*[Hight];
	for(unsigned int row = 0; row < Hight; ++row)
	{
	    canvas[row] = new Cell[Width];
	    for(unsigned int col = 0; col < Width; ++col)
	    {
	    	canvas[row][col] = Cell(col, row);
	    	infile >> num;     // ÿ�ζ�ȡһ������
			if (num)
			{
				canvas[row][col].AliveLife();
			}
			else
			{
				canvas[row][col].DeadLife();
			}
	    }
	}
	infile.close();    // �ر��ļ�
}
bool Modify()						//�û�����ָ��ϸ��״̬
{
	cout << "Now you can change the cell's life." << endl;
	cout << "Enter the cell's position which you want to change:(x,y Starting from 0)" << endl;
	cout <<"x:___\b\b\b";
	int x,y;
	cin >> x;
	if(!(x >= 0 && x < Width-2))
	{
		cout << "invalid value!" << endl;
		return false;
	}
	x++;
	//����Ƿ����
	cout <<"y:___\b\b\b";
	cin >> y;					//	      01234
	if(!(y >= 0 && y < Hight-2))//  5 7  0123456
	{
		cout << "invalid value!" << endl;
		return false;
	}
	y++;
	cout << "Please specify the state of the cell:(1 to alive,0 to die)";
	int life_state;
	cin >> life_state;
	if( life_state == 1 )
	{
		canvas[y][x].AliveLife();
	}
	else
	{
		canvas[y][x].DeadLife();
	}

	cout << "Modify Sucess!";
	Sleep(1000);
	return true;
}
//��������������������������������������������������������������������������������������������������������������������������������������������#���ܺ�������#��������������������������������������������������������������������������������������������������������������������������������������������������������������