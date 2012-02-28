/*
 * falcon.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: magi
 */
/*
 * falcon.cpp
 *
 *  Created on: Sep 9, 2011
 *      Author: magi
 */


#include "includes.h"
#include "File_getter.h"

using namespace std;


int main(int argc, char** argv)
{

	if(argc<2)
     {
     cout<<"Insufficient arguments";
     return 1;
     }
    string mode=argv[1];
    string base_dir_name;

    if(mode.compare("-s")==0)
        base_dir_name="/cse535/yagoo/output/src_online/src";
        else
         base_dir_name="/cse535/yagoo/output/src_offline";


	// 	Call to File_gett.cpp

   cout<<"Please wait for index to load"<<endl;
    File_getter ob;
    string qfile="dummy";
    if(mode.compare("-s")==0)
    	ob.files_to_process(base_dir_name,qfile);
    else
       {
         qfile=argv[2];
         ob.files_to_process(base_dir_name,qfile);
        }

	return 0;
}




