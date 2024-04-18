#include "HW1.h"
using namespace cv;
using namespace std;

int main()
{
	int number;
	while (true)
	{
		cout << "1: C++ version" << endl;
		cout << "2: OpenCV version" << endl;
		cout << "0: exit" << endl;
		cout << "Choose C++ version or OpenCV version:";
		cin >> number;

		if (number == 1)
			HW1C_PLUS();
		else if (number == 2)
			HW1OPENCV();

		else if (number == 0)
			break;
		else if (number < 0 or number > 2)
			cout << "Invaild number, please enter a number in the range of 1 ~ 2" << endl;
		cout << "========================================================================" << endl;
	}
	cout << "Exit Program" << endl;
	system("pause");

	return 0;
}