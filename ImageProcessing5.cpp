// ImageProcessing5.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include "segm_line.h"
#include "string.h"
#include <conio.h>

using namespace std;
using namespace cv;


int main()
{
	int treashold = 0;
	//IplImage* img =  cvLoadImage("img.png");	//Загрузка картинки
	Mat img = imread("img.png");
	//Проверка загрузки
	if (img.empty())
	{
		cout << "File not found" << endl;
		system("pause");
		exit(0);
	}
	//Преобразуем картинку в ч/б
	Mat image;

	cvtColor(img, image, COLOR_BGR2GRAY);
	segm_line a(image);
	a.do_segmentation();
	

	

	imshow("Picture1", img);
	imshow("Picture", image);
	waitKey(0);

	system("pause");
	return 0;
}

