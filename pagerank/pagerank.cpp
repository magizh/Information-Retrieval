#include<iostream>
#include "includes.h"
using namespace std;


int main(int argc, char** argv)
{
	map<string,int> temp;
	map<string,map<string,int> > in;
	map<string,int> out;
	map<string,float> pgr;
	map<string,float>::iterator pgr_itr;
	map<string,int>::iterator temp_itr;
	map<string,map<string,int> >::iterator in_itr;
	map<string,int>::iterator out_itr;

	 ifstream fp;
     fp.open(argv[1]);

     string line="";
     string word,name;
     int op,i;
     float sum;

 	  while(getline(fp,line).good())
     {
    	   temp.clear();
 		  std::istringstream ins;
    	  ins.str(line);
    	  ins>>name;
    	  ins>>op;
    	  out.insert(std::pair<string,int>(name,op));
    	  pgr.insert(std::pair<string,float>(name,1));
    	  while(ins>>word)
    	  {
            temp_itr=temp.find(word);
		if(temp_itr==temp.end())
              temp.insert(std::pair<string,int>(word,1));
               
    	  }
    	  in.insert(std::pair<string,map<string,int> >(name,temp));
    	//  cout<<line<<endl;
     }
      fp.close();
     for(i=0;i<10;i++)
     {
    	 for(pgr_itr=pgr.begin();pgr_itr!=pgr.end();pgr_itr++)
    	 {
    		 word=pgr_itr->first;
    		 sum=0;
    		 for(temp_itr=in.find(word)->second.begin();temp_itr!=in.find(word)->second.end();temp_itr++)
    		 {
                       if(out.find(temp_itr->first)->second==0)
                       continue;
    		//cout<<pgr.find(temp_itr->first)->second<<" "<<out.find(temp_itr->first)->second<<endl;
             sum+=pgr.find(temp_itr->first)->second/(float)out.find(temp_itr->first)->second;
         	 }
    		 pgr_itr->second=.15+.85*sum;
    		 //cout<<pgr_itr->first<<" "<<pgr_itr->second<<endl;
    	 }
     }
      ofstream fp1;
      fp1.open(argv[2]);
   sum=0;
     for(pgr_itr=pgr.begin();pgr_itr!=pgr.end();pgr_itr++)
       {
           if(pgr_itr->second<=.155)
     fp1<<pgr_itr->first<<".txt 0"<<endl;
           else if(pgr_itr->second>20)
                     fp1<<pgr_itr->first<<".txt 1"<<endl;
		else 
                      	fp1<<pgr_itr->first<<".txt "<<pgr_itr->second/20<<endl;
         }
      fp1.close();
// cout<<"PGR max="<<sum;
	return 1;
}
