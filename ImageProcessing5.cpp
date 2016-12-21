// ImageProcessing5.cpp: ���������� ����� ����� ��� ����������� ����������.
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
	//IplImage* img =  cvLoadImage("img.png");	//�������� ��������
	Mat img = imread("img.png");
	//�������� ��������
	if (img.empty())
	{
		cout << "File not found" << endl;
		system("pause");
		exit(0);
	}
	//����������� �������� � �/�
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

