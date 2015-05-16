#include <iostream>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
#include "md6.h"
#include <vector>

using namespace std;
using namespace boost::filesystem;
class Filex{
	public:
	string name;
	string size;
	string path;
	string hash;
	int changes;
	Filex(string a, string b, string c, string d,int e){
		name = a;
		size = b;
		path = c;
		hash = d;
		changes = e;
	}
	
};
bool isEqual(Filex a, Filex b){
	if ((a.name == b.name) && (a.size == b.size) && (a.path == b.path) && (a.hash == b.hash)) return 1;
	else return 0;
}
bool Changed(Filex a, Filex b){
	if ((a.name == b.name) && (a.path == b.path) && (a.hash != b.hash)) return 1;
	else return 0;
}
void show(Filex a){
	if (!exists(a.path)) cout << a.path << " deleted" << endl;
	else{
		if (a.changes == 0){
			cout << a.path << " Changed" << endl;
		}

		if (a.changes == 1) cout << a.path << " is a new file" << endl;
	}
}
path main_path_get(){
	cout << "Enter your path" << endl;
	path p;
	string s;
	getline(cin, s);
	getline(cin, s);
	p = s;
	while (!is_directory(p)){
		cout << "Error: " << p << " is not valid path" << endl
			<< "Enter valid path" << endl;
		getline(cin, s);
		p = s;
	}
	return p;
}
path file_path_get(){
	cout << "Enter csv File path" << endl;
	path p;
	string s;
	getline(cin, s);
	p = s;
	while (!is_regular_file(p)){
		cout << "Error: " << p << " is not valid File path" << endl
			<< "Enter valid path" << endl;
		
		getline(cin, s);
		p = s;
	}
	return p;
}
string md6constructor(path p){
	ifstream o;

	o.open(p.string(), ifstream::in);
	string text;
	while (!o.eof()){
		text += o.get();
	}
	string a = md6(text);
	o.close();
	return a;
};

void dir_runner(){
	path fpth;
	cout << "1:Make csv file" << endl;
	cout << "2:Scan for changes" << endl;
	int inp;
	cin >> inp;
	path main_p = main_path_get();
	if (inp == 2) {
		fpth = file_path_get();
	}
	ofstream o;
	ifstream i;
	vector<Filex> filesx;
	string name;
	string size;
	string paths;
	string hash;

	if (inp==2){
		i.open(fpth.string(), ifstream::in);
		i.ignore(256, '\n');
		
		char str[256];
		while (!i.eof()){
			i.get(str, 256, ',');
			name = str;
			i.get();
			i.get(str, 256, ',');
			size = str;
			i.get();
			i.get(str, 256, ',');
			paths = str;
			i.get();
			i.get(str, 256, '\n');
			hash = str;
			i.get();
			filesx.push_back(Filex(name, size, paths, hash,0));

		}
		filesx.pop_back();
	}


	o.open("output.csv", ofstream::out);
	o << "File Name,File Size,File Path,File Hash" << endl;
	for (recursive_directory_iterator dir_itr(main_p);
		dir_itr != recursive_directory_iterator();
		++dir_itr)
	{
		if (is_regular_file(*dir_itr)){
			
				path p = *dir_itr;
				if (p.filename().string() != "output.csv"){
			bool deleted = false;
			bool changed = false;
			name = p.filename().string();
			size = to_string(file_size(p));
			paths = p.string();
			hash = md6constructor(p);
			if (inp == 2){
				Filex a(name, size, paths, hash, 1);
				for (int i = 0; i < filesx.size(); i++){
					bool t = isEqual(a, filesx[i]);
					bool ch = Changed(a, filesx[i]);
					if (ch == true){
						changed = true;
					}
					if (t == true){
						filesx.erase(filesx.begin() + i);
						deleted = true;
					}
				}
				if (deleted == false && changed == false){
					filesx.push_back(a);
				}

			}




			o << name << ",";
			o << size << ",";
			o << paths << ",";
			o << hash << endl;
		}
			}
		}
		o.close();
		if (inp == 2){
			if (filesx.size() == 0)cout << "No files were changed" << endl;
			else for_each(filesx.begin(), filesx.end(), show);
		}
	}
int main(){
	dir_runner();
	system("pause");
	return 0;
}