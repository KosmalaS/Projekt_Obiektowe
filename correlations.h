#pragma once

#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

template <typename T>
double mean( vector<T>& v );

template <typename T>
double stddev( vector<T> v );

double pearson_correlation( vector<double>& x, vector<double>& y );

double spearman_correlation( vector<double>& x, vector<double>& y );

typedef pair<double, double> Point;
int countConcordantPairs( const vector<Point>& points );
int countDiscordantPairs( const vector<Point>& points );
double kendall_correlation( vector<double>& x, vector<double>& y );

vector<double> cross_correlation( const vector<double>& x, const vector<double>& y );