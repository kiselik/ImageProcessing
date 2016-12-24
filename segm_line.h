#pragma once
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include "opencv2\opencv.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;
using namespace cv;

class segm_line
{
private:

	Mat image;
	vector<int> column_left;
	vector<int> column_right;
	vector<int> row_up;
	vector<int> row_down;

	vector <cv::Mat> test_picture;

	//return "true" then all pixels in column are white	
	bool Check_Bright_Column(int column)
	{
		bool flag = true;
		for (int i = 0;i < image.rows;i++)
			if (image.at<uchar>(i, column) != 255)
			{
				flag = false;
				break;
			}
		return flag;
	}

	//return "true" then all pixels in column are white
	bool Check_Bright_Row(int row_start, int tmp_column_left, int tmp_column_right)
	{
		bool flag = true;
		int row_final = 0;
		row_final += row_start;
		Clamp(row_final++, 0, image.rows);
		for (int i = row_start;i < row_final;i++)
			for (int j = tmp_column_left;j < tmp_column_right;j++)
				if (image.at<uchar>(i, j) != 255)
				{
					flag = false;
					break;
				}
		return flag;
	}

	//ищем все границы
	void Find_Border_Symbols()
	{
		Find_Left_Border();
		Find_Right_Border();
		for (int i = 0;i < column_left.size();i++)
		{
			Find_Up_Border(column_left[i], column_right[i]);
			Find_Down_Border(column_left[i], column_right[i]);
		}

	}

	//ищем левые границы
	void Find_Left_Border()
	{
		for (int j = 0; j < image.cols; j++)
		{
			// if current column has black pixsels AND  previous hasn't, we find new symbol 
			if ((!Check_Bright_Column(j)) && Check_Bright_Column(Clamp((j - 1), 0, image.cols)))
			{
				column_left.push_back(j - 1);
			}
		}
	}

	//ищем правые границы
	void Find_Right_Border()
	{
		for (int j = 0; j < image.cols; j++)
		{
			// if current column has black pixsels AND next hasn't, we find new symbol 
			if ((!Check_Bright_Column(j)) && Check_Bright_Column(Clamp((j + 1), 0, image.cols)))
			{
				column_right.push_back(j);
			}
		}
	}

	void Find_Up_Border(int tmp_column_left, int tmp_column_right)
	{
		for (int i = image.rows - 1; i > 0; i--)
		{
			// if current column has black pixsels AND  previous hasn't, we find new symbol 
			if (!Check_Bright_Row(i, tmp_column_left, tmp_column_right) && Check_Bright_Row(Clamp((i + 1), 0, image.rows), tmp_column_left, tmp_column_right))
			{
				row_up.push_back(i);
				break;
			}
		}
	}

	void Find_Down_Border(int tmp_column_left, int tmp_column_right)
	{
		for (int i = 0; i < image.rows; i++)
		{
			// if current column has black pixsels AND  previous hasn't, we find new symbol 
			if (!Check_Bright_Row(i, tmp_column_left, tmp_column_right) && Check_Bright_Row(Clamp((i - 1), 0, image.rows), tmp_column_left, tmp_column_right))
			{
				row_down.push_back(i);
				break;
			}
		}
	}

	void Create_Words()
	{
		Find_Border_Symbols();

		int delta = 5;
		for (int i = 0;i < column_left.size();i++)
		{
			//сосчитали временную ширину столбца
			int tmp_size_column = abs(column_right[i] - column_left[i]);
			int tmp_size_row = abs(row_down[i] - row_up[i]);

			test_picture.push_back(cv::Mat(tmp_size_row, tmp_size_column, CV_8U));

			image(cv::Rect(column_left[i], row_down[i], tmp_size_column, tmp_size_row)).copyTo(test_picture[i]);
			string str = "test_picture" + to_string(i);
			namedWindow(str, 0);
			imshow(str, test_picture[i]);
		}

	}

	void Threshold(Mat & tmp)
	{
		for (int i = 0;i < tmp.rows;i++)
			for (int j = 0;j < tmp.cols;j++)
				if (tmp.at<uchar>(i, j) != 255)
				{
					tmp.at<uchar>(i, j) = 0;
				}

	}

	int Check_image(Mat test_image, vector <cv::Mat> standart)
	{
		int min = 10000000;
		int kmin = 0;
		int sum;
		for (int k = 0;k < standart.size();k++)
		{
			sum = 0;
			for (int i = 0;i < test_image.rows;i++)
			{
				for (int j = 0;j < test_image.cols;j++)
				{
					//test_image.at<uchar>(i, j)*test_image.at<uchar>(i, j) - standart[k].at<uchar>(i, j)*standart[k].at<uchar>(i, j);
					sum += sqrt(abs(test_image.at<uchar>(i, j)*test_image.at<uchar>(i, j) - standart[k].at<uchar>(i, j)*standart[k].at<uchar>(i, j)));
				}
			}
			sum = (float)sum / (test_image.rows*test_image.cols);
			//cout << sum << endl;
			if (sum < min)
			{
				min = sum;
				kmin = k;
			}

		}


		return kmin;
	}

public:
	segm_line(string img)
	{
		Mat tmp = imread(img);
		if (tmp.empty())
		{
			cout << "File not found" << endl;
			system("pause");
			exit(0);
		}
		cvtColor(tmp, image, COLOR_BGR2GRAY);
		Threshold(image);



	}

	~segm_line()
	{}

	int Clamp(int value, int min, int max)
	{
		if (value < min)
			return min;
		if (value > max)
			return max;
		return value;
	}

	string Do_segmentation(vector <cv::Mat> & standart)
	{
		Create_Words();
		string result;
		for (int i = 0;i < test_picture.size();i++)
		{

			int tmp = 0;
			resize(/*sourse*/test_picture[i], /*destination*/test_picture[i], cvSize(16, 16));
			tmp = Check_image(test_picture[i], standart);
			result += convert(tmp);
		}
		return result;
	}

	string convert(int word)
	{
		string tmp;
		switch (word)
		{
		case 0:
			tmp = 'А';
			break;
		case 1:
			tmp = 'Б';
			break;
		case 2:
			tmp = 'В';
			break;
		case 3:
			tmp = 'Г';
			break;
		case 4:
			tmp = 'Д';
			break;
		case 5:
			tmp = 'Е';
			break;

		case 6:
			tmp = 'Ё';
			break;
		case 7:
			tmp = 'Ж';
			break;
		case 8:
			tmp = 'З';
			break;
		case 9:
			tmp = 'И';
			break;
		case 10:
			tmp = 'Й';
			break;
		case 11:
			tmp = 'К';
			break;
		case 12:
			tmp = 'Л';
			break;
		case 13:
			tmp = 'М';
			break;
		case 14:
			tmp = 'Н';
			break;
		case 15:
			tmp = 'О';
			break;
		case 16:
			tmp = 'П';
			break;
		case 17:
			tmp = 'Р';
			break;
		case 18:
			tmp = 'С';
			break;
		case 19:
			tmp = 'Т';
			break;
		case 20:
			tmp = 'У';
			break;
		case 21:
			tmp = 'Ф';
			break;
		case 22:
			tmp = 'Х';
			break;
		case 23:
			tmp = 'Ц';
			break;
		case 24:
			tmp = 'Ч';
			break;
		case 25:
			tmp = 'Ш';
			break;
		case 26:
			tmp = 'Щ';
			break;
		case 27:
			tmp = 'Ъ';
			break;
		case 28:
			tmp = 'Ь';
			break;
		case 29:
			tmp = 'Э';
			break;
		case 30:
			tmp = 'Ю';
			break;
		case 31:
			tmp = 'Я';
			break;
		default:
			tmp = "Возможно, я пока не знаю такой буквы";
			break;
		}
		return tmp;
	}





};


