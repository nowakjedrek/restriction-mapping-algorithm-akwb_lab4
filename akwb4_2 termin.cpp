#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <numeric>
#include <optional>
#include <chrono>
#include <iterator>
#include <algorithm>

using namespace std;

typedef chrono::high_resolution_clock Clock;
typedef Clock::time_point ClockTime;

void printExecutionTime(ClockTime start_time, ClockTime end_time)
{
	auto execution_time_ms = std::chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();
	auto execution_time_sec = std::chrono::duration_cast<chrono::seconds>(end_time - start_time).count();
	auto execution_time_min = std::chrono::duration_cast<chrono::minutes>(end_time - start_time).count();

	cout << "Czas pracy: ";
	if (execution_time_min > 0)
		cout << "" << execution_time_min % 60 << " Minutes, ";
	if (execution_time_min > 60)
		cout << "Zbyt długi czas obliczeń";
	if (execution_time_sec > 0)
		cout << "" << execution_time_sec % 60 << " Seconds, ";
	if (execution_time_ms > 0)
		cout << "" << execution_time_ms % long(1E+3) << " MicroSeconds\n";
}

vector<int> multisetA;
vector<int> restriction_map;
int maxind = 0;

bool find_map(int ind, int end) {
    cout << "ind = " << ind << endl;
    cout << "Aktualna mapa: ";
    for(auto i : restriction_map)
        cout << i << " ";
    cout << endl;
    
    vector<int> used;
    for (int i = 0; i < restriction_map.size(); ++i) {
        used.push_back(restriction_map[i]);
        
        if (i != 0) {
            int from_left_end = restriction_map[i];
            for (int j = i - 1; j >= 0; j--) {       
                from_left_end += restriction_map[j];
                used.push_back(from_left_end);
            }
        }
    }
    
    if (ind == maxind) {
        cout << "Mapa ma tyle elementow ile powinna" << endl;
        return true;
    }
    
    else {
        int candidate;
        int deleted =0;
        for (int i : multisetA) {    
            if (i == deleted) continue;
            int el_in_multiset = 0;
            for (int j : multisetA) {
                if (j == i)
                    el_in_multiset++;
            }
            int el_in_used = 0;
            for (int j : used) {
                if (j == i)
                    el_in_used++;
            }
            if (el_in_multiset > el_in_used) {
                candidate = i;
                //cout << "Sprawdzam " << candidate << endl;
                int from_left_end = candidate;
                bool between_cuts = true;
                for (int j = restriction_map.size() - 1; j >= 0; j--) {
                    from_left_end += restriction_map[j];   
                   
                    
                    int b_in_multiset = 0, b_in_used = 0;
                    for (int k : multisetA)
                        if (k == from_left_end)
                            b_in_multiset++;
                    for (int k : used)
                        if (k == from_left_end)
                            b_in_used++;
                    //cout << "W multizbiorze: " << b_in_multiset << " W used: " << b_in_used << endl;
                   
                    if (b_in_multiset == 0 || b_in_used == b_in_multiset) {
                        between_cuts = false;
                        //cout << "Brak w multizbiorze" << endl;
                    }
                }
                int from_right_end = end - from_left_end;   
                //cout << "Od lewej: " << from_left_end << "  Od prawej: " << from_right_end << endl;
                int r_in_multiset = 0, l_in_multiset = 0;    
                for (int j : multisetA) {
                    if (j == from_left_end)
                        l_in_multiset++;
                    if (j == from_right_end)
                        r_in_multiset++;
                }
                int r_in_used = 0, l_in_used = 0;  
                for (int j : used) {
                    if (j == from_left_end)
                        l_in_used++;
                    if (j == from_right_end)
                        r_in_used++;
                }
               
                bool approved = false;
                if (maxind - 1 == ind) { 
                    if (from_right_end == 0 && from_left_end == end && between_cuts)
                        approved = true;
                }
                else {  
                    if (l_in_multiset > 0 && r_in_multiset > 0 && l_in_multiset > l_in_used && r_in_multiset > r_in_used && between_cuts)
                        approved = true;
                }
                if (approved) {
                    
                    restriction_map.push_back(candidate);
                    //cout << "Do mapy dodaje " << candidate << endl << endl;
                    bool map_found = find_map(restriction_map.size(), end);
                    if (map_found)
                        return true;
                    else {
                        cout << "Usuwam ostatni element z mapy" << endl;
                        deleted = restriction_map[restriction_map.size() - 1];
                        restriction_map.pop_back();
                    }
                }
            }
        }
        return false;
    }
}

int main() {
	string filename;
	cout << "Podaj nazwe pliku: ";
	cin >> filename;

	

	fstream input_file("path_to_multisetA" + filename + ".txt", fstream::in);
	


    ClockTime start_time = Clock::now();
	if (input_file.good())
	{

        int number;
        do {
            input_file >> number;
            multisetA.push_back(number);
        } while (!input_file.eof());
        cout << "Wczytana instancja: ";
        for (int i = 0; i < multisetA.size(); i++) {
            cout<< multisetA[i]<<" ";
        }
        cout << "\n";
        int sizeA = multisetA.size();
        cout<<"Dlugosc instancji: " << sizeA << endl;
        int n = sqrt(1 + 8 * sizeA);
        if ((n - 3) % 2 == 0) {
            int k = (n - 3) / 2;     
            cout << "Przewidywana liczba ciec: " << k<<endl;
            maxind = k + 1;    
            cout << "Przewidywana liczba elementow mapy: " << maxind << endl;
            int biggest_el = 0;
            for (int i : multisetA)      
                if (i > biggest_el)
                    biggest_el = i;
            int sec_biggest = 0;
            for (int j : multisetA)      
                if (j > sec_biggest && j != biggest_el)
                    sec_biggest = j;
            int first = biggest_el - sec_biggest;   
            int found_first = false;
            for (int l : multisetA) 
                if (l == first)
                    found_first = true;
            if (found_first) {
                restriction_map.push_back(first);
                
                
                bool map_found = find_map(1, biggest_el);
              
                ClockTime end_time = Clock::now();
                if (map_found) {
                    cout << "Znaleziona mapa: ";
                    for (int i : restriction_map)
                        cout << i << " ";
                    cout << endl;
                }
                else
                    cout << "Nie znaleziono mapy" << endl;
                printExecutionTime(start_time, end_time);
               
            }
            else cout << "Brak w multizbiorze obliczonego pierwszego elementu. Blad w instancji." << endl;
        }
        else cout << "Niepoprawna ilosc elementow w multizbiorze." << endl;
    }

    else cout << "Blad podczas otwierania pliku!";
   
	
	return 0;
}