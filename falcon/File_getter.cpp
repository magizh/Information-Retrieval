/*
 * File_getter.cpp
 *
 *  Created on: Sep 18, 2011
 *      Author: magi
 */

#include "File_getter.h"

using namespace std;

File_getter::File_getter()
{

 }

File_getter::~File_getter()
{
fp.close();
}

bool File_getter::files_to_process(std::string base_directory_path,std::string qfile) {


	string file_resource_path;
	DIR *dirp;
	struct dirent *dp;
	struct stat buf;


	dirp = opendir(base_directory_path.c_str());
	if (!dirp) {
		cout << "Dirp is null" << endl;
		return false;
	           }
	string file_resource_name;
	while ((dp = readdir(dirp))) {
		file_resource_name = dp->d_name;
		if ((file_resource_name == ".") || (file_resource_name == "..")
				|| (file_resource_name == "CVS"))
			continue;

		file_resource_path = base_directory_path + '/' + file_resource_name;
				cout << "dp->d_name = " << file_resource_name << endl;

		if (stat(file_resource_path.c_str(), &buf) == 0) {
			// recursively process directories
			if (buf.st_mode & S_IFDIR)
				files_to_process(file_resource_path,qfile);
			else if(file_resource_name.compare("filenames.txt")==0)
                                  continue;
			else if(file_resource_name.compare("quality_score.txt")==0)
                           obj.load_file_score(file_resource_path);
			else if(file_resource_name.compare("term_dict.txt")==0)
                           obj.load_term_dict(file_resource_path);
			else if(file_resource_name.compare("authorcheck.txt")==0)
                           obj.load_author_check(file_resource_path);
			else if(file_resource_name.compare("file_dict.txt")==0)
         	           obj.load_file_dict(file_resource_path);
			else
				obj.load_inv(file_resource_path);
		}

	}
	closedir(dirp);

	if(qfile.compare("dummy")==0)
		 obj.process_online_query();
	else
	    obj.process_query(qfile);

	return true;
}


