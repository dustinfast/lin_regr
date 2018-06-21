# Linear Regression Modeler

Creates a linear regression model to determine bivariate statistical correlation.

## Usage

Compile with `g++ -std=c++11 lin_regr.cpp -o lin_regr.exe` (or equivelant).
Run with `./lin_regr.exe` and follow follow prompts.

Data set input format is a single csv (first line headers ok) with variables in columns.
After model build, an arbitrary value of x may be entered to get an estimated y (y^).

## Model

Linear regression model implemented using a least-square method to measure statistical correlation of two variables, x in X, and y in Y, where x is assumed independent of y and y is assumed dependent on x.

y' = b_0 + b_1x = Regression Model
R^2 = Correlation cooeficient
e = Standard Error
y^ = an estimation of y (given some x) according to y'.

## Analysis

This tool was used to analyze Alaskan Socket Salmon harvest rate data as provided by the Alaska Department of Fish and Game. (dip-har-eff.csv, drift-har-eff.csv, and set-har-eff.csv). See analysis.docx.

### TODO:

Handle invalid input.
Accept y' as input.
Calculate variance, kth moment, etc., for X and Y
Output model graph