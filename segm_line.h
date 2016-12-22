#pragma once
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include "opencv2\opencv.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

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

	//return "true" then all pixels in column are white
	bool Check_Bright_Column(int column_start)
	{
		bool flag = true;
		int column_final = 0;
		column_final += column_start;
		Clamp(column_final++, 0, image.cols);
		for (int i = column_start;i < column_final;i++)
			for (int j = 0;j < image.rows;j++)
				if (image.at<uchar>(j, i) == 0)
				{
					flag = false;
					break;
				}
		return flag;
	}
	bool Check_Bright_Column_(int column)
	{
		bool flag = true;
		for (int i = 0;i < image.rows;i++)
			if (image.at<uchar>(i, column) == 0)
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
				if (image.at<uchar>(i, j)!=255)
				{
					flag = false;
					break;
				}
		return flag;
	}
	bool Check_Bright_Row_(int row_start)
	{
		bool flag = true;
		for (int j = 0; j < image.cols; j++)
			if (image.at<uchar>(row_start, j) != 255)
			{
				flag = false;
				break;
			}
		return flag;
	}

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

	void Find_Right_Border()
	{
		for (int j = 0; j < image.cols; j++)
		{
			// if current column has black pixsels AND next hasn't, we find new symbol 
			if ((!Check_Bright_Column(j)) && Check_Bright_Column(Clamp((j + 1), 0, image.cols)))
			{
				column_right.push_back(j + 1);
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
	void Draw(int col,int row, int cols, int rows)
	{
		for (int i =row ;i < rows;i++)
			for (int j = col;j <cols;j++)
			{
				image.at<uchar>(i, j) = 0;
			}

	}

	void create_Words()
	{
		Find_Border_Symbols();

		vector <cv::Mat> picture;

		for (int i = 0;i < column_left.size();i++)
		{
			//сосчитали временную ширину столбца
			int tmp_size_column = abs(column_right[i] - column_left[i]);
			int tmp_size_row = abs(row_down[i] - row_up[i]);
			//Draw(column_left[i], row_down[i], column_left[i]+tmp_size_column, row_down[i]+ tmp_size_row);
		}
		imshow("tmp",image);

		int delta = 5;
		for (int i = 0;i < column_left.size();i++)
		{
			//сосчитали временную ширину столбца
			int tmp_size_column = abs(column_right[i] - column_left[i]);
			int tmp_size_row = abs(row_down[i] - row_up[i]);

			picture.push_back(cv::Mat(tmp_size_row + 2 * delta, tmp_size_column + 2 * delta, CV_8U));
			image(cv::Rect(column_left[i], row_down[i], tmp_size_column, tmp_size_row)).copyTo(picture[i]);

			//Mat *picture = new Mat*(tmp_size_row, tmp_size_column, COLOR_BGR2GRAY);
			string str = "Picture" + to_string(i);
			namedWindow(str, 0);
			imshow(str, picture[i]);
		}



	}

public:
	segm_line(Mat img)
	{
		image = img;
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



	void do_segmentation()
	{
		create_Words();
		cout << column_left.size() << endl;
		cout << column_right.size() << endl;
		cout << row_up.size() << endl;
		cout << row_down.size() << endl;
	}


};


