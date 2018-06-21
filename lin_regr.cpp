///////////////////////////////////////////
// Linear regression model implemented using a least-square method to 
// measure statistical correlation of two variables, x in X, and y in Y,
// where y is assumed dependent on x.
//
/// Dustin Fast (dustin.fast@hotmail.com), 2017

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cmath>


using namespace std;

const string WELCOME_MSG = "\n-- Linear Regression Modeler --\n";

void getLineaerRegression(vector<vector<float>> v, int xCol, int yCol, float *arr);

int main()
{
	cout << WELCOME_MSG << endl;

	while (true)
	{
		string strFilename = "", strTemp = "";
		bool bDataContainsHeaders = true;
		vector<vector<float>> vData;						//data set
		int xCol = -1, yCol = -1;							//x and y col indexes in vector<float>
		ifstream fsIn;										
		float linearRegressionResults[4] = { 0, 0, 0, 0 };  //array to hold results of getLinearRegression()
		float maxx = 0;										//max x value found in data file, to be used as a rand() ceiling.

		//Prompt for user input
		while (strFilename == "")
		{
			cout << "Enter dataset CSV file, or 'q' to quit: ";
			cin >> strFilename;

			if (strFilename == "q")
				return  0;

			fsIn.open(strFilename);
			if (fsIn.fail())
			{
				cout << "Error: File could not be opened.\n";
				strFilename = "";
			}
		}

		while (xCol < 0 || yCol < 0)
		{
			cout << "Enter the column index (starting from 0) in " << strFilename << " containing the independant variable x: ";
			cin >> xCol;
			cout << "Enter the column index (starting from 0)  " << strFilename << " containing the dependent variable y: ";
			cin >> yCol;
		}
		while (strTemp != "y" && strTemp != "n")
		{
			cout << "Does your data file contain column headers (y/n): ";
			cin >> strTemp;
			if (strTemp == "n")
				bDataContainsHeaders = false;
		}

		//Trash first line, if it is headers
		if (bDataContainsHeaders)
			getline(fsIn, strTemp, '\n'); 

		//Populate vector array with data
		while (getline(fsIn, strTemp, '\n')) //iterate file by line
		{
			//new vector "row"
			vector<float> v;
			vData.push_back(v);

			//explode strTemp on ',' and push values to new vector "row"
			string str = "";
			for (unsigned int i = 0; i < strTemp.length(); i++)
			{
				if (strTemp[i] != ',')
					str = str + strTemp[i];
				else
				{
					try
					{
						vData[vData.size() - 1].push_back(stof(str));
					}
					catch (const invalid_argument& e)
					{
						cout << "UNRECOVERABLE ERROR: Data file contains invalid data.\n";
						if (bDataContainsHeaders == false)
							cout << "   Possible cause: User incorrectly specified existence of headers in data file.";
						return 0;
					}
					str = "";
				}
			}
			vData[vData.size() - 1].push_back(stof(str));
		}
	
		//validate file size
		if (vData.size() <= 0)
		{
			cout << "ERROR: File contained no data.\n\n";
			continue;
		}
		//validate given col indexes
		if (xCol >= vData[0].size() || yCol >= vData[0].size())
		{
			cout << "ERROR: One or more column indexes are out of bounds.\n\n";
			continue;
		}

		//Report data to be used
		cout << "\nFile contains the following x and y values: " << endl;
		for (unsigned int i = 0; i < vData.size(); i++)
		{
			float x = vData[i][xCol];
			float y = vData[i][yCol];
			if (x > maxx)
				maxx = x;
			cout << "x: " << x << " y: " << y << endl;
		}
		
		//Call linear regression algorithm func and show results
		getLineaerRegression(vData, xCol, yCol, linearRegressionResults); 
		cout << "\nmodel = " << linearRegressionResults[0] << " + "
			 << linearRegressionResults[1] << "x\nstd error = " << linearRegressionResults[2] 
		   	 << "\ncorrelation coef:  " << linearRegressionResults[3] << "\n\n";

		//Allow estimate of y given x
		cout << "Enter an arbitrary x to get an estimate of y (or 'q' to break)\n";
		while (true)
		{
			cout << "x: ";
			string strX;
			cin >> strX;

			if (strX == "q")
				break;

			float r = strtof((strX).c_str(), 0);
			float res = linearRegressionResults[0] + linearRegressionResults[1] * r;
			cout << "x = " << r << " gives y = " << res << endl << endl;
		}

		string strRestart = "";
		cout << "Enter 'q' to quit, or any other key to restart: ";
		cin >> strRestart;

		if (strRestart == "q")
			break;
	}

	return 0;
}


//Linear regression algorithm
// Using the Least Square method, calculates Linear regression line y' = b_0 + b_1 * x + e
//   where b_0 = y-intercept, b_1 = slope, and e = standard error of estimate.
//	 Also determines Correlation Coefficient (R^2).
// Accepts: A vector<vector<float>>, i.e. rows having properties to be used as x and y values.
//		   Params denoting the vector<float> column indexes that contain x and y values.
//		   A ptr to a destination float array of size 4, to be populated with the resulting b_0 and b_1 values: 
//				dest[0] = b_0 = y-intercept
//				dest[1] = b_1 = slope
//				dest[2] = e   = standard error on estimate
//				dest[3] = R^2 = correlation coefficient
void getLineaerRegression(vector<vector<float>> v, int xCol, int yCol, float *dest)
{
	int rowcount = v.size();
	float sumx = 0, sumy = 0;			//Sums: sumx = sum(x_i) and sumy = sum(y_i), where i denotes row index
	float xbar = 0, ybar = 0;			//Mean values: xbar = sumx/rowcount, ybar = sumy/ybar 
	float rsquared = 0;					//R^2 = correlation coeffient = q/p
	float stderror = 0;					//e. i.e. standard error of estimate
	float b_0 = 0, b_1 = 0;				//Params 
	float m = 0, n = 0;					//b_1 variables (b_1 = n/m): m = sum((x_i-xbar)^2), n = sum((x_i-xbar)(y_i-ybar))
	float p = 0, q = 0;					//R^2 variables (R^2 = q/p): p = sum((y_i-ybar)^2), q = sum((y'-ybar)^2)
	float f = 0;						//e (e = sqrt(f/rowcount-2)) variable f = sum((y' -y)^2) 

	//Determine sums
	for (int i = 0; i < rowcount; i++)
	{
		sumx = sumx + v[i][xCol];
		sumy = sumy + v[i][yCol];
	}

	//Calculate mean values
	xbar = (sumx / rowcount);
	ybar = (sumy / rowcount);

	//Determine m, n, and p.
	for (int i = 0; i < rowcount; i++)
	{
		m = m + (v[i][xCol] - xbar) * (v[i][xCol] - xbar);
		n = n + (v[i][xCol] - xbar) * (v[i][yCol] - ybar);
		p = p + (v[i][yCol] - ybar) * (v[i][yCol] - ybar);
	}

	//Calculate params
	b_1 = n / m;
	b_0 = ybar - b_1*xbar;

	//Determine f and q (we do this here because we didn't previously know the params for y')
	for (int i = 0; i < rowcount; i++)
	{
		float yprime = b_0 + b_1 * v[i][xCol];
		f = f + (yprime - v[i][yCol]) * (yprime - v[i][yCol]);
		q = q + (yprime - ybar) * (yprime - ybar);
	}

	//Calculate R^2 and standard error of estimate
	rsquared = q / p;
	stderror = sqrt(f / (rowcount - 2));

	//assign result to dest array
	dest[0] = b_0;
	dest[1] = b_1;
	dest[2] = stderror;
	dest[3] = rsquared;
	
	return;
}
