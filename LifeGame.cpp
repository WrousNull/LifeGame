#include<iostream> 
#include<cstdlib>
#include<conio.h>
#include<windows.h>
#include<fstream>
#include<string>
#include <ctime>
#include <sstream>
//———————————————————————————————————————————————————————————————————————#使用的库#———————————————————————————————————————————————————————————————————————————————————
using namespace std;

unsigned int Hight;
unsigned int Width;

bool DontInit = false;		//是否跳过初始化
class Cell
{
	private:
		int x;
		int y;
		bool life;
		int alive_amount;   //记录该细胞周围活细胞数量
	public:
		Cell(unsigned int row=1,unsigned int col=1,bool m_life=false,int m_alive_amount=0);	//构造函数，初始化细胞的位置
		~Cell();			//析构函数
		bool UpdateAlive();	//记录周围的细胞的生存情况
		bool checkLife();	//检查life是存活
		void AliveLife();	//使得life=true
		void DeadLife();	//使得life=false
		void initLife();	//初始化细胞生存状态
};
//—————————————————————————————————————————————————————————————————————#定义的变量#——————————————————————————————————————————————————————————————————————————————————
Cell ** canvas = NULL;	//创建一个画布，记录每一个位置的细胞状态 
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
//———————————————————————————————————————————————————————————————————————#函数声明#———————————————————————————————————————————————————————————————————————————————————
int main()
{
	srand((unsigned)time(NULL));// 设置随机数种子
	showStart();				//绘制开始界面（待改进）
	init(Hight,Width);			//初始化			
	doGame();					//游戏主逻辑
	End();						//结束游戏
	return 0;
}
//————————————————————————————————————————————————————————————————————————#主函数#———————————————————————————————————————————————————————————————————————————————————
Cell::Cell(unsigned int row,unsigned int col,bool m_life,int m_alive_amount)						//构造函数，创建对象时自动调用
{
	x = col;
	y = row;
	life = false;
}
Cell::~Cell()
{
	//Nothing
}
bool Cell::checkLife()				//获取存活状态
{
	bool _life = life;
	return _life;
}
void Cell::AliveLife()				//使得细胞复活
{
	life = true;
}
void Cell::DeadLife()				//使得细胞死亡
{
	life = false;
}
void Cell::initLife()				//随机初始化细胞的状态
{
	life = rand() % 2;
}
bool Cell::UpdateAlive()			//更新细胞存活状态，返回下一代的存活bool值
{
	alive_amount = 0;
	
    for(int i = y-1 ; i<=y+1 ; ++i)     //计数，存在问题：当边缘细胞更新时，将访问未定义的内存单元。
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
    //判定该细胞下一代的生存状态
    if(life)                    //活细胞状态
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
    else                        //死细胞状态
    {
        if( alive_amount == 3 )
        {
            life = true;
        }
    }
    return life;
}
//———————————————————————————————————————————————————————————————————————#类函数定义#————————————————————————————————————————————————————————————————————————————————
void init(unsigned int & Hight,unsigned int & Width)		//完成细胞的初始化
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
	SetConsoleWindowSize( (Width+1)*2 , Hight+2 );	//设置终端大小
}
void display()										//显示细胞
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
void SetConsoleWindowSize( SHORT width, SHORT height )	//设置终端大小
{
    HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT wrt = { 0, 0, width-1, height-1 };
    SetConsoleWindowInfo( hStdOutput, TRUE, &wrt ); // 设置窗体尺寸
    COORD coord = { width, height };
    SetConsoleScreenBufferSize( hStdOutput, coord ); // 设置缓冲尺寸
}
void run()							//执行下一代
{
	for(unsigned int row = 1 ; row < Hight-1 ; ++row)
	{
		for(unsigned int col = 1 ; col < Width-1 ; ++col)
		{
			canvas[row][col].UpdateAlive();
		}
	}
}
void showRegular()					//显示按键规则
{
	cout << endl;
	cout << "Game Regular" << endl;
	cout << "When game is running:" << endl;
	cout << "	*You can press 'q' to quit" << endl;
	cout << "	*Press 'm' to modify the cell's life." << endl;
	cout << "	*Press another key to suspend." << endl;
	cout << endl;
}
void showStart()					//绘制开始界面
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
void End()							//结束逻辑函数
{
	for(unsigned int row = 0; row < Hight; ++row)
	{
	    delete [] canvas[row];
	}
	delete [] canvas;
	system("cls");					//清空画面
	cout << "GoodBye." << endl;
	getch();
}
void doGame()						//执行游戏主函数
{
	system("cls");
	while(true)
	{
		display();
		Sleep(500);
		run();
		char input;
		if (_kbhit()) 				// 判断是否有输入
		{
			input = _getch(); 		
			if(input == 'q' )		//按下q退出
			{
				cout << "You can press 's' to save." << endl;
				cout << "Or press another key to quit directly" << endl;
				char choice = getch();			//停下来，直到用户再次按下某键
				if( choice == 's' )
				{
					SavingFile();
				}
					break;
			}
			else if( input == 'm' )				//用户要调整细胞的状态
			{
				system("cls");
				while(!Modify())
				{
					system("cls");
				}
			}
			else					//按下除了q的某键,暂停
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
		system("cls");				//清空画面
	}
}
void SavingFile()					//保存函数
{
	ofstream file_out;
	string filename_out;
	cout << "Enter your file name that you want to save (txt):___\b\b\b";
	cin >> filename_out;
	file_out.open(filename_out);

	file_out << Hight << " " << Width << endl;			//读入设置网格的大小:前两个数字,要记住的是，读入文件时将不再对两值进行修改,直接导入.
	for(unsigned int row = 0; row < Hight; ++row)
	{
	    for(unsigned int col = 0; col < Width; ++col)
	    {
	    	file_out << canvas[row][col].checkLife() << endl;	//读入每个网格的状态:后面的所有bool值【可能要进行类型转换】
	    }
	}
	file_out.close();
	cout << "You have saved the game." << endl;
}
void ReadingFile()					//读取存档函数
{
	string filename_in;
	FLAG:
	system("cls");
	cout << "The target file needs to be located in the same level directory as this cpp file." << endl;
	cout << "Enter your file name that you want to read:___\b\b\b";
	cin >> filename_in;

	ifstream infile(filename_in);	//打开一个用于读取的文件

    if (!infile.is_open())
    {
        cout << "Error opening file!" << endl;
		cout << "Please try again.";
		Sleep(1000);
        goto FLAG;	//如果没能成功打开则返回输入文件名的位置
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
	    	infile >> num;     // 每次读取一个数字
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
	infile.close();    // 关闭文件
}
bool Modify()						//用户调整指定细胞状态
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
	//检查是否合理
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
//——————————————————————————————————————————————————————————————————————#功能函数定义#———————————————————————————————————————————————————————————————————————————————