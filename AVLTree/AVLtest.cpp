/*
Kane
2016.04.27
AVLÊ÷cppÊµÏÖ
*/
#include<iostream>
#include"AVL.h"
using namespace std;

bool main()
{
	TREESPACE::Tree T;
	int i;
	do
	{
		system("cls");
		cout << "The tree:\n" << T << endl
			<< "Dephy:" << T.Depyh()<<endl
			<< "0: exit\n"
			"1: insert\n"
			"2: delete\n"
			"  enter:";
		cin >> i;
		cin.get();
		switch (i)
		{
		default:
		case 0:
			i ? (cout << "Done.\n") : (cout << "Exit.\n");
			break;
		case 1:
			cout << "data:";
			cin >> T;
			break;
		case 2:
			cout << "data:";
			TREESPACE::ElemType e;
			cin >> e;
			T -= e;
		}
		cin.get();
	} while (i);
	return true;
}