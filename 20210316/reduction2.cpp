#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>

#include "tbb/concurrent_hash_map.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/tick_count.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/concurrent_vector.h"

using namespace std;

typedef tbb::concurrent_hash_map<long, int> iTbb_Vec_timestamp;
static iTbb_Vec_timestamp TbbVec_timestamp; 

vector<string> split(string& input, char delimiter)
{
    istringstream stream(input);
    string field;
    vector<string> result;
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }
    return result;
}

int main(int argc, char *argv[])
{
    ifstream ifs(argv[1]);

    int counter; 
    
    string line;

    std::map<long, int> m;
    
    counter = 0;
    while (getline(ifs, line)) {

      if(counter == 0)
	{
	  counter = counter + 1;
	  continue;
	}
      
        vector<string> strvec = split(line, ',');
	string tmp = strvec.at(0);

	string tmp2 = tmp.substr(0,18);
	
	for(size_t c = tmp.find_first_of(" "); c != string::npos; c = c = tmp.find_first_of(" ")){
    	      tmp.erase(c,1);
	}
	for(size_t c = tmp.find_first_of("-"); c != string::npos; c = c = tmp.find_first_of("-")){
    	      tmp.erase(c,1);
	}
	for(size_t c = tmp.find_first_of(":"); c != string::npos; c = c = tmp.find_first_of(":")){
    	      tmp.erase(c,1);
	}

	for(size_t c = tmp2.find_first_of(" "); c != string::npos; c = c = tmp2.find_first_of(" ")){
    	      tmp2.erase(c,1);
	}
	for(size_t c = tmp2.find_first_of("-"); c != string::npos; c = c = tmp2.find_first_of("-")){
    	      tmp2.erase(c,1);
	}
	for(size_t c = tmp2.find_first_of(":"); c != string::npos; c = c = tmp2.find_first_of(":")){
    	      tmp2.erase(c,1);
	}

	string tmp3 = tmp2 + "0";
	
	// std::cout << tmp3 << "," << strvec.at(1) << std::endl; 

	iTbb_Vec_timestamp::accessor t;
        TbbVec_timestamp.insert(t, stol(tmp3));
        t->second += stoi(strvec.at(1));
    }

    for(auto itr = TbbVec_timestamp.begin(); itr != TbbVec_timestamp.end(); ++itr) {
      m.insert(std::make_pair(long(itr->first), itr->second));
    }

    /*
    for(auto itr = m.begin(); itr != m.end(); ++itr) {
      cout << itr->first << "," << itr->second << endl; 
    }
    */

    for(auto itr = m.begin(); itr != m.end(); ++itr) {
      // cout << itr->first << "," << itr->second << endl;

      string tmpstr = to_string(itr->first);
      string year = tmpstr.substr(0,4);

      tmpstr = to_string(itr->first);
      string month = tmpstr.substr(4,2);

      tmpstr = to_string(itr->first);
      string day = tmpstr.substr(6,2);

      tmpstr = to_string(itr->first);
      string hour = tmpstr.substr(8,2);

      tmpstr = to_string(itr->first);
      string minute = tmpstr.substr(10,2);

      tmpstr = to_string(itr->first);
      string second = tmpstr.substr(12,2);
      
      cout << year << "/" << month << "/" << day << " " << hour << ":" << minute << ":" << second << "," << itr->second << endl;
    }
    
    /*
    for(auto itr = TbbVec_timestamp.begin(); itr != TbbVec_timestamp.end(); ++itr) {
      cout << itr->first << "," << itr->second << endl;

      string tmpstr = to_string(itr->first);
      string year = tmpstr.substr(0,4);

      tmpstr = to_string(itr->first);
      string month = tmpstr.substr(4,2);

      tmpstr = to_string(itr->first);
      string day = tmpstr.substr(6,2);

      tmpstr = to_string(itr->first);
      string hour = tmpstr.substr(8,2);

      tmpstr = to_string(itr->first);
      string minute = tmpstr.substr(10,2);

      tmpstr = to_string(itr->first);
      string second = tmpstr.substr(12,2);
      
      cout << year << "/" << month << "/" << day << " " << hour << ":" << minute << ":" << second << endl;
    }
    */
} 



