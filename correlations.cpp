#include "correlations.h"

using namespace std;

template <typename T>
double mean( vector<T>& v ) {
    double sum = 0;
    for( auto& x : v ) {
        sum += x;
    }
    return sum / v.size();
}


template <typename T>
double stddev( vector<T> v ) {
    double meanVal = mean( v );
    double sum = 0;
    for( T v_i : v ) {
        sum += (v_i - meanVal) * (v_i - meanVal);
    }
    return sqrt( sum / (v.size() - 1) );
}

double pearson_correlation( vector<double>& x, vector<double>& y ) {

    int n = min( x.size(), y.size() );
    double x_mean = mean( x );
    double y_mean = mean( y );
    double x_stddev = stddev( x );
    double y_stddev = stddev( y );
    double xy_covariance = 0;

    for( int i = 0; i < n; i++ ) {
        xy_covariance += (x[i] - x_mean) * (y[i] - y_mean);
    }

    xy_covariance /= n - 1;

    return xy_covariance / (x_stddev * y_stddev);
}

double spearman_correlation( vector<double>& x, vector<double>& y ) {
    int n = min( x.size(), y.size() );
    vector<double> x_rank( n );
    vector<double> y_rank( n );
    for( int i = 0; i < n; i++ ) {
        x_rank[i] = x[i];
        y_rank[i] = y[i];
    }

    sort( x_rank.begin(), x_rank.end() );
    sort( y_rank.begin(), y_rank.end() );

    return pearson_correlation( x_rank, y_rank );
}


vector<double> cross_correlation( const vector<double>& x, const vector<double>& y ) {

    int n = x.size();
    int m = y.size();

    vector<double> x_padded( n + m - 1 );
    vector<double> y_flipped( y.rbegin(), y.rend() );
    vector<double> result( n + m - 1 );

    for( int i = 0; i < n; i++ ) {
        x_padded[i] = x[i];
    }

    for( int i = 0; i < n + m - 1; i++ ) {
        for( int j = 0; j < m; j++ ) {
            if( i - j < 0 || i - j >= n ) {
                continue;
            }
            result[i] += x_padded[i - j] * y_flipped[j];
        }
    }
    return result;
}
