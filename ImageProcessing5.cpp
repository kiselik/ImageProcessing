// ImageProcessing5.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include "segm_line.h"
#include "string.h"
#include <conio.h>

using namespace std;
using namespace cv;

void do_Threshold(Mat & tmp)
{
	for (int i = 0;i < tmp.rows;i++)
		for (int j = 0;j < tmp.cols;j++)
			if (tmp.at<uchar>(i, j) != 255)
			{
				tmp.at<uchar>(i, j) = 0;
			}

}

void load_standart(vector <cv::Mat> & standart_picture)
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

		cvtColor(standart_picture[i], standart_picture[i], COLOR_BGR2GRAY);
		resize(/*sourse*/standart_picture[i], /*destination*/standart_picture[i], cvSize(16, 16));
		do_Threshold(standart_picture[i]);

	}
	namedWindow("Picture_check.png", 0);
	imshow("Picture_check.png", standart_picture[16]);

}

int main()
{
	setlocale(LC_ALL, "Russian");
	vector <cv::Mat> standart_picture;
	load_standart(standart_picture);
	segm_line a("img.png");
	string str=a.Do_segmentation(standart_picture);
	cout << str<<endl;
	//waitKey(0);
	system("pause");
	return 0;
}

