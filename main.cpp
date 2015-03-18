#include <iostream>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;
 
path main_path_get(){
	cout << "Enter your path" << endl;
	path p;
	string s;
	getline(cin, s);
	p = s;
	while (!is_directory(p)){
		cout << "Error: " << p << " is not valid path"<< endl
			<<"Enter valid path" << endl ;
		getline(cin, s);
		p = s;
	}
	return p;
} 
 
void dir_runner(path main_p){
	ofstream o;
	o.open("output.csv",ofstream::out);
	o<<"File Name,File Size,File Path"<<endl;
	for (recursive_directory_iterator dir_itr(main_p);
		dir_itr != recursive_directory_iterator();
		++dir_itr)
	{
		if (is_regular_file(*dir_itr)){
				path p = *dir_itr;
 
				o<<p.filename().string()<<",";
				o<< file_size(p) <<",";
				o<< p.string()<<endl;
 
 
 
 
		}
	}
	o.close();
}
 
 
int main(){
	path p=main_path_get();
	dir_runner(p);
	system("pause");
	return 0;
}