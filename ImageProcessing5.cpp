// ImageProcessing5.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include "segm_line.h"
#include "string.h"
#include <conio.h>

using namespace std;
using namespace cv;

void load_standart(vector <cv::Mat> standart_picture)
{
	
	for (int i = 0;i < 32;i++)
	{
		string str = "Picture" + to_string(i)+".png";
		standart_picture.push_back(imread(str));

		if (standart_picture[i].empty())
		{
			cout << "File not found" << endl;
			system("pause");
			exit(0);
		}

	}

}

int main()
{
	vector <cv::Mat> standart_picture;
	load_standart(standart_picture);
	Mat image;


	segm_line a("img.png");
	a.do_segmentation();
	

	

	//imshow("Picture1", img);
	//imshow("Picture", image);
	waitKey(0);

	system("pause");
	return 0;
}

