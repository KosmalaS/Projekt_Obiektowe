#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "correlations.h"

using namespace std;

class Date {
public:
	int day;
	int month;
	int year;
	friend bool operator==( const Date& left, const Date& right ) {
		if( left.day == right.day && left.month == right.month && left.year == right.year ) {
			return true;
		}
		return false;
	}
	friend bool operator>( const Date& left, const Date& right ) {
		if( left.year > right.year ) {
			return true;
		}
		else if( left.year == right.year ) {
			if( left.month > right.month ) {
				return true;
			}
			else if( left.month == right.month ) {
				if( left.day > right.day ) {
					return true;
				}
			}
		}
		return false;
	}

	friend bool operator<( const Date& left, const Date& right ) {
		if( left > right || left == right ) return false;
		return true;
	}
	friend bool operator>=( const Date& left, const Date& right ) {
		if( left > right || left == right ) return true;
		return false;
	}
	friend bool operator<=( const Date& left, const Date& right ) {
		if( left < right || left == right ) return true;
		return false;
	}
	friend ostream& operator<<( ostream& os, const Date& date ) {
		os << date.day << "-" << date.month << "-" << date.year;
		return os;
	}
};





struct gold { 
	Date date;
	double price; //USD
};

struct apple {
	Date date;
	double price; //USD
};

struct suicide {
	string country;
	int year;
	string age;
	int number;
	int population;
	int gdp_per_capita;
	string generation;
};


//=======================================Czytanie CSV==========================================
vector<vector<string>> read_csv( const string& file_name ) {
	vector<vector<string>> data; //pusty wektor z wektorami

	ifstream file( file_name ); //otworz plik
	string line; 

	while( getline( file, line ) ) { // dopoki jest linia do przeczytania
		vector<string> row; 
		int pos = 0;
		string field; 

		while( (pos = line.find( ',' )) != string::npos ) { // dopoki jest przecinek w linii
			field = line.substr( 0, pos ); // wszystko do przecinka leci do fielda
			row.push_back( field );
			line.erase( 0, pos + 1 ); // usuwa field+przecinek
		}
		row.push_back( line ); // za ostatnim przecinkeim
		data.push_back( row );
	}
	return data;
}

//=======================================Ładowanie daty========================================
//YYYY-MM-DD
 Date load_date( const string& date ) {
	return Date{ stoi( date.substr( 8, 2 ) ),stoi( date.substr( 5, 2 ) ),stoi( date.substr( 0, 4 ) ) }; //convert string to date
}

 //=======================================Czytanie Danych złota=================================
 //Zwraca wektor z gold structami
vector<gold> load_golds( const vector<vector<string>>& data_gold ) {
	vector<gold> golds; //trzyma gold structy

	for( int i = 1; i < data_gold.size(); i++ ) { //dla kazdego rzedu
		gold g{ load_date( data_gold[i][0] ), stod( data_gold[i][1] ) }; 
		golds.push_back( g ); //dodajemy nowego gold structa do gigawektora
	}
	return golds;
}

//=======================================Czytanie Danych Jabłka================================
vector<apple> load_apples( const vector<vector<string>>& data_apple ) {
	vector<apple> apples; 

	for( int i = 1; i < data_apple.size(); i++ ) { //dla kazdego rzedu
		double price = 0;
		if( data_apple[i][4] != "null" )	price = stod( data_apple[i][4] ); //pomijanie malo zmienia, mozna interpolacje zastosować ew.
		apple a{ load_date( data_apple[i][0] ), price }; 
		apples.push_back( a );
	}
	return apples;
}

//=======================================Czytanie Danych Harakiri==============================
vector<suicide> load_suicides( const vector<vector<string>>& data_suicide ) {
	vector<suicide> suicides;

	for( int i = 1; i < data_suicide.size(); i++ ) {
		suicide s{ data_suicide[i][0], stoi( data_suicide[i][1] ), data_suicide[i][3], stoi( data_suicide[i][4] ), stoi( data_suicide[i][5] ),
			stoi( data_suicide[i][10] ), data_suicide[i][11] }; 
		suicides.push_back( s );
	}

	return suicides;
}

//co to robi???
void printVector( const vector<double>& v ) {
	for( auto a : v ) {
		cout << a << " ";
	}
	cout << endl;
}



void corr( vector<double>& v1, vector<double>& v2 ) {
	int input = 0;
	cout << "Choose correlation method: " << endl;
	cout << "1. Pearson correlation coefficient: Measures the linear correlation between two variables." << endl;
	cout << "2. Spearman rank correlation coefficient: Measures the monotonic association between two variables." << endl;
	cout << "3. Cross-correlation function: Measures the similarity between two time series as a function of the lag applied to one of them." << endl;
reset:
	cin >> input;
	switch( input ) {
	case 1:
		cout << "Pearson correlation is: " << pearson_correlation( v1, v2 ) << endl;
		break;
	case 2:
		cout << "Spearman correlation is: " << spearman_correlation( v1, v2 ) << endl;
		break;
	case 3:
		printVector( cross_correlation( v1, v2 ) );
		break;
	default:
		cout << "Wrong input" << endl;
		goto reset;
	}
}

int main() {
	//wczytanie plików
	vector<vector<string>> data_suicide = read_csv( "master.csv" );
	vector<vector<string>> data_apple = read_csv( "AAPL.csv" );
	vector<vector<string>> data_gold = read_csv( "daily_gold_rate.csv" );

	//konwersja na kontenery
	vector<gold> golds = load_golds( data_gold );
	vector<apple> apples = load_apples( data_apple );
	vector<suicide> suicides = load_suicides( data_suicide );

	int input = 0;

	cout << "Choose for which data you want to calculate correlation coefficient: " << endl;
	cout << "1. Apple shares and gold prices." << endl;
	cout << "2. Apple shares and suicide stats." << endl;
	cout << "3. Suicide stats and gold prices." << endl;
	cin >> input;

	if( input == 1 ) {
		vector<gold> temp_gold;
		vector<apple> temp_apple;

		for( auto& i : apples ) {
			auto it = golds.begin();
			while( it != golds.end() && !(it->date == i.date) ) {
				++it;
			}
			if( it != golds.end() ) {
				temp_apple.push_back( i );
				temp_gold.push_back( *it );
			}
		}

		cout << "Choose range from: " << temp_apple.begin()->date << " to: " << (temp_apple.end() - 1)->date << endl;

		int year, month, day;

		cout << "Starting with:" << endl;
		cout << "Enter a year:" << endl;
		cin >> year;
		if( year < 1985 || year>2020 ){ cout << "Wrong input"; return -1; }
		cout << "Enter a month:" << endl;
		cin >> month;
		if( month < 1 || month>12 ){ cout << "Wrong input"; return -1; }
		cout << "Enter a day:" << endl;
		cin >> day;
		if( day < 1 || day>31 ){ cout << "Wrong input"; return -1; }
		Date d1{ day,month,year };

		cout << "Ending with:" << endl;
		cout << "Enter a year:" << endl;
		cin >> year;
		if( year < 1985 || year>2020 || year < d1.year ){ cout << "Wrong input"; return -1; }
		cout << "Enter a month:" << endl;
		cin >> month;
		if( month < 1 || month>12 ) { cout << "Wrong input"; return -1; }
		cout << "Enter a day:" << endl;
		cin >> day;
		if( day < 1 || day>31 ){ cout << "Wrong input"; return -1; }
		Date d2{ day,month,year };

		vector<double> data1( temp_apple.size() );
		vector<double> data2( temp_gold.size() );


		for( int i = 0; i < temp_apple.size(); i++ ) { //wypełnij wektory data
			if( temp_apple[i].date < d1 || temp_apple[i].date > d2 ) continue; 
			data1.push_back( temp_apple[i].price );
			data2.push_back( temp_gold[i].price );
		}

		cout << "DATA SIZES:   " << data1.size() << "   " << data2.size() << endl;

		corr( data1, data2 );
	}
	else
		if( input == 2 ) {

			// map<ROK, WARTOSC>
			map<int, int> year_sum_suicides; 
			map<int, double> year_avg_apple_price; 
			map<int, int> counts; 

			//suma samobojstw coroczna
			for( const auto& s : suicides ) {
				year_sum_suicides[s.year] += s.number;
			}

			//roczna srednia appla
			for( const auto& a : apples ) {
				year_avg_apple_price[a.date.year] += a.price;
				counts[a.date.year]++;
			}
			for( auto& a : year_avg_apple_price ) {
				a.second /= counts.find( a.first )->second;
			}


			int year_start, year_end;
			cout << "Choose year range from: " << year_avg_apple_price.begin()->first << " to: " << (--year_avg_apple_price.end())->first << endl; 
			cout << "Start with: " << endl;
			cin >> year_start;
			if( year_start < year_avg_apple_price.begin()->first || year_start>( --year_avg_apple_price.end() )->first ) { cout << "Wrong input"; return -1; }
			cout << "End with: " << endl;
			cin >> year_end; 
			if( year_end < year_avg_apple_price.begin()->first || year_start > ( --year_avg_apple_price.end() )->first || year_end <= year_start ) { cout << "Wrong input"; return -1; }

			//wektorki do liczenia
			vector<double> data1, data2;

			for( const auto& s : year_sum_suicides ) {
				if( s.first < year_start || s.first > year_end ) continue;
				data1.push_back( s.second );
			}

			for( const auto& a : year_avg_apple_price ) {
				if( a.first < year_start || a.first > year_end ) continue;
				data2.push_back( a.second );
			}

			corr( data1, data2 );
		}
		else
			if( input == 3 ) { 
				map<int, int> year_sum_suicides; 
				map<int, double> year_avg_gold_price; 
				map<int, int> counts;

				for( const auto& s : suicides ) {
					year_sum_suicides[s.year] += s.number;
				}

				for( const auto& a : apples ) {
					year_avg_gold_price[a.date.year] += a.price;
					counts[a.date.year]++;
				}

				for( auto& a : year_avg_gold_price ) {
					a.second /= counts.find( a.first )->second;
				}

				int year_start, year_end;
				cout << "Choose year range from: " << year_avg_gold_price.begin()->first << " to: " << (--year_avg_gold_price.end())->first << endl;
				cout << "Start with: " << endl;
				cin >> year_start;
				if( year_start < year_avg_gold_price.begin()->first || year_start>( --year_avg_gold_price.end() )->first ) { cout << "Wrong input"; return -1; }
				cout << "End with: " << endl;
				cin >> year_end;
				if( year_end < year_avg_gold_price.begin()->first || year_start > ( --year_avg_gold_price.end() )->first || year_end <= year_start ) { cout << "Wrong input"; return -1; }

				vector<double> data1, data2;
				for( const auto& s : year_sum_suicides ) {
					if( s.first < year_start || s.first > year_end ) continue;
					data1.push_back( s.second );
				}

				for( const auto& a : year_avg_gold_price ) {
					if( a.first < year_start || a.first > year_end ) continue;
					data2.push_back( a.second );
				}

				corr( data1, data2 );
			}
			else cout << "Wrong input!" << endl;
}