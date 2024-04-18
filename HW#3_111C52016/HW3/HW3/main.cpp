#include "HW3.h"

using namespace std;
using namespace cv;
int main()
{
	int number;
	while (true)
	{
		cout << "1: 3_1" << endl;
		cout << "2: 3_2" << endl;
		cout << "3: 3_3" << endl;
		cout << "4: bonus" << endl;
		cout << "0: exit" << endl;
		cout << "Enter the question number to select output result:";
		cin >> number;

		if (number == 1)
			HW3_1();
		else if (number == 2)
			HW3_2();
		else if (number == 3)
			HW3_3();
		else if (number == 4)
			Bonus();
		else if (number == 0)
			break;
		else if (number < 0 or number > 4)
			cout << "Invaild number, please enter a number in the range of 1 ~ 4" << endl;
		cout << "========================================================================" << endl;
	}
	cout << "Exit Program" << endl;
	system("pause");

	return 0;
}