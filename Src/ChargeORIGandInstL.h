#include <iostream>
#include <sstream>
#include "TString.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#ifndef instlumi_h
#define instlumi_h
#include "nonme11_first.h"
#include "nonme11_second.h"
#include "me11.h"
using namespace std;
double instlumi(int runnb, int lumis, TString);
std::pair<double,double> UncorrGasGain_HVInitial(double , int , int , int );
#endif
