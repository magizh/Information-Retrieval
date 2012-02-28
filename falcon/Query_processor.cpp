/*
 * Query_processor.cpp
 *
 *  Created on: Oct 16, 2011
 *      Author: magi
 */
/*
 * File_getter.cpp
 *
 *  Created on: Sep 18, 2011
 *      Author: magi
 */

#include "Query_processor.h"
#include "stemmer.h"
#include "vector"
#include <iostream>


using namespace std;

Query_processor::Query_processor()
{
   wc=0;
   query_no="";
 }

Query_processor::~Query_processor()
{

 }


bool Query_processor::load_author_check(std::string file_path) {

	cout<<"Please wait for prompt"<<endl;
	 myReadFile.open(file_path.c_str());

	 while ((myReadFile >>term).good()) {
                  line="";
		 getline(myReadFile,line);
		 author_check.insert(std::pair<string,string>(term,line));
	   }

	 myReadFile.close();

	 myReadFile.open("/cse535/yagoo/output/src_online/categoryinfo.txt");

	 while ((myReadFile >>term).good()) {
                  line="";
		 getline(myReadFile,line);
		 cate_check.insert(std::pair<string,string>(term,line));
	   }

	 myReadFile.close();

	 return true;
}


bool Query_processor::load_file_score(std::string file_path)
{
	 myReadFile.open(file_path.c_str());
         myReadFile2.open("/cse535/yagoo/output/src_online/specificity_score.txt");


	 while (getline( myReadFile,line).good()) {
		 file_name=line.substr(0,line.find("$"));
                 line=line.substr(line.find("$")+1,line.length());

	      len=file_name.length();
     		    for(i = 0; i <len; i++)
     		         {
     		              switch(file_name[i])
     		              {
     		                   case ' ':
     		                   file_name[i] = '_';
     		                   break;
     		              }
     		         }
                     file_name+=".txt";
		      stringstream ss (stringstream::in | stringstream::out);
	            ss.str(line);
                  ss>>score;
                    score*=.25;
  //                cout<<file_name<<" "<<score<<endl;
		getline( myReadFile2,line2);
//                cout<<line2;
		line2=line2.substr(line2.find("$")+1,line2.length());
  		stringstream ss1 (stringstream::in | stringstream::out);
	            ss1.str(line2);
                 ss1>>score1;
                  score1*=.35;
                //     cout<<" "<<score1<<endl;
//                cout<<file_name<<" "<<score<<endl;
		 file_score.insert(std::pair<string,float>(file_name,score+score1));
	   }

	 myReadFile.close();
            myReadFile2.close();
         map<string,float>::iterator score_read_itr;
            myReadFile.open("/cse535/yagoo/output/src_online/authority_score.txt");
         	 while (getline( myReadFile,line).good()) {
		 file_name=line.substr(0,line.find("$"));
                 line=line.substr(line.find("$")+1,line.length());
                 len=file_name.length();
     		    for(i = 0; i <len; i++)
     		         {
     		              switch(file_name[i])
     		              {
     		                   case ' ':
     		                   file_name[i] = '_';
     		                   break;
     		              }
     		         }
                     file_name+=".txt";
        	      stringstream ss (stringstream::in | stringstream::out);
	             ss.str(line);
                  ss>>score;
                  score*=.20; 
		 score_read_itr=file_score.find(file_name);
                 score_read_itr->second+=score;
	   }
        myReadFile.close();
//loading pagerank score 
            myReadFile.open("/cse535/yagoo/output/src_online/PageRankScore.txt");
         	 while ((myReadFile>>file_name).good()) {
                     file_name+=".txt";
                  myReadFile>>score;
                  score*=.20; 
		 score_read_itr=file_score.find(file_name);
                 score_read_itr->second+=score;
	   }
        myReadFile.close();

	 return true;

}


bool Query_processor::load_file_dict(std::string file_path) {


	 myReadFile.open(file_path.c_str());

	 while ((myReadFile >>id).good()) {
		 myReadFile >> offset;
		 file_dict.insert(std::pair<int,int>(id,offset));
	   }

	 myReadFile.close();

	 return true;
}

bool Query_processor::load_term_dict(std::string file_path) {


	 myReadFile.open(file_path.c_str());

	 while ((myReadFile >>term).good()) {
		 myReadFile >> id;
		 term_dict.insert(std::pair<string,int>(term,id));
	   }

	 myReadFile.close();

	 return true;
}


bool Query_processor::load_inv(std::string file_path) {

	 myReadFile.open(file_path.c_str());

		 while ((myReadFile >>id).good()) {
             getline( myReadFile,line);
             stringstream ss (stringstream::in | stringstream::out);

             vector<int> fids;
             ss.str(line);

               while(ss>>val)
                     fids.push_back(val);

			 inv.insert(std::pair<int,vector<int> > (id,fids));
		   }

		 myReadFile.close();

	return true;
}

bool Query_processor::process_online_query()
{
int author_flag=0;
int category_flag=0;
int no_of_docs=0;
string author="none";
string cate="none";
int ret_no=0;
string first="";
int first_flag=0;
while(1)
{
first_flag=0;
cout<<endl<<"Enter Query as >>  no_of_docs  query [Author=name]"<<endl<<endl;
    author="none";
    cate="none";
    author_flag=0;
   category_flag=0;
    no_of_docs=0;
	ret_no=0;
     wc=0;
     query.clear();
        char input[100];
	line="";
	cin.getline (input,100);
         std::istringstream ins;

     struct timeval start, end;
    long mtime, seconds, useconds;
	    gettimeofday(&start, NULL);
      cout<<endl;
     ins.str(input);
         ins>>no_of_docs;
       while(ins>>word)
         {
           if(first_flag==0)
           {
                  first_flag=1;
                    first=word;
              }
         if(word.find("[Aut")!=-1)
        {
             int iw=0;
               author="";
		while(word[iw]!='=')
			iw++;
		iw++;
		while(word[iw]!=']')
		{
				author+=word[iw];
				iw++;
		}
               author_flag=1;
         }

         if(word.find("[Cat")!=-1)
        {
             int iw=0;
               cate="";
		while(word[iw]!='=')
			iw++;
		iw++;
		while(word[iw]!=']')
		{
				cate+=word[iw];
				iw++;
		}
		category_flag=1;
             break;
         }
        
	if(author_flag==1)
           break;
//         cout<<"query"<<word<<endl;
         sb="";
   	     l1=word.length();
   	     vector<int> fids;

   	    	    	 for(i=0;i<l1;i++)
   	    	    		 {
   	    	    		 if ((word[i] >= '0' && word[i] <= '9')|| (word[i] >= 'a' && word[i] <= 'z')|| (word[i] >= 'A' && word[i] <= 'Z'))
   	    	    		 {
   	    	    		      if(word[i] >= 'A' && word[i] <= 'Z')
   	    	    		             word[i]+=32;
   	    	    			 sb=sb+word[i];
   	    	    		 }
   	    	    		 }
   	    	      //stemming
   	    	                char *stw=new char[sb.size()+1];
   	    	                 stw[sb.size()]=0;
   	    	                  memcpy(stw,sb.c_str(),sb.size());
   	    	                  int y=stem(stw,0,sb.length()-1);
   	    	                  sb=sb.substr(0,y);
                           //cout<<sb<<endl;
   	                      term_map_itr=term_dict.find(sb);
                               inv_itr=inv.find(term_map_itr->second);
                               if(inv_itr==inv.end())
                            	   continue;
                             fids=inv_itr->second;
                            postings_length=fids.size()/3;
                            idf=log(53476/postings_length);
                             query.push_back(idf);
                            cout<<sb<<" "<<term_map_itr->second<<" idf= "<<idf<<endl;

                            for(it=fids.begin();it!=fids.end();it++)
                            {
                            	file_id=*it;
                            	it++;
                            	tf=*it;
                            	it++;
                            	tdf=*it;
                            	tfidf.clear();
                            	 file_tf_idf_itr=file_tf_idf.find(file_id);
                            	 if(file_tf_idf_itr==file_tf_idf.end())
                            	 {
                            		 for(i=0;i<wc;i++)
                            			 tfidf.push_back(0);
                            		 tfidf.push_back(((float)tf/(float)tdf)*idf);

                            		 file_tf_idf.insert(std::pair<int,vector<float> >(file_id,tfidf));
                            	 }
                            	 else
                            	 {
                            		 tfidf=file_tf_idf_itr->second;
                                     len=tfidf.size();
                                    	 for(i=len;i<wc;i++)
                                    	 {
                                    		 tfidf.push_back(0);
                                       	 }
                                    tfidf.push_back(((float)tf/(float)tdf)*idf);
                                    file_tf_idf_itr->second=tfidf;
                            	 }

                            }
                wc++;
   	    }

       if(author.compare("none")!=0)
                author_flag=1;
       if(cate.compare("none")!=0)
                category_flag=1;

         // cout<<author<<endl;
	    query_itr=query.begin();
   	     que_sum=0;
   	   	 while(query_itr!=query.end())
   	   	 {
   	   		 que_sum+=pow((*query_itr),2);
   	   		 query_itr++;
   	   	 }
   	  que_sum=sqrt(que_sum);
   	   	 cout<<"quesum= "<<que_sum<<endl;
  cout<<"-------------------------------------------------------------------------------------------------------------------------------------------------------------";

   	    for(file_tf_idf_itr=file_tf_idf.begin();file_tf_idf_itr!=file_tf_idf.end();file_tf_idf_itr++)
   	    		{
                       ret_no++;
   	    	sum=doc_sum=0;
   	    	query_itr=query.begin();
            for(doc_itr=file_tf_idf_itr->second.begin();doc_itr!=file_tf_idf_itr->second.end();doc_itr++)
                    {
            	           sum+=(*query_itr)*(*doc_itr);
               	            doc_sum+=pow((*doc_itr),2);
				query_itr++;
                  }
            //cout<<" "<<que_sum<<" "<<doc_sum<<endl;

            doc_sum=sqrt(doc_sum);
            float result=sum/(que_sum*doc_sum);
           // if(result==1)
             //cout<<endl<<file_tf_idf_itr->first<<" "<<file_tf_idf_itr->second.size()<<" "<<sum<<" "<<que_sum<<" "<<doc_sum<<" "<<result<<endl;
             cosine.insert(std::pair<float,int>(result,file_tf_idf_itr->first));
             
            
       }
	 

     final_score_map.clear();
    //rearranging based on static scores 
	 fp.open("/cse535/yagoo/output/src_online/src/filenames.txt");
           count=1;
           for(cosine_itr=cosine.end();cosine_itr!=cosine.begin();)
           	    {
			 cosine_itr--;
			fp.seekg(file_dict.find(cosine_itr->second)->second,ios::beg);
       	    	    	char c;
       	    	    	file_name="";
       	    	    	fp.get(c);
       	    	    	while(c!='$')
       	    	    	{
       	    	    		file_name+=c;
       	    	    		fp.get(c);
       	    	    	}
                      cout<<endl<<file_name<<" "<<file_score.find(file_name)->second;
                          final_score_map.insert(std::pair<float,string>(file_score.find(file_name)->second,file_name));
                   count++;
                    if(count==10)
                      break; 
                    }
              fp.close();


	  fp.open("/cse535/yagoo/output/src_online/src/filenames.txt");
   	// fp.open("src/filenames.txt");
      	       	    count=1;
            eof_flag=0;
     score_itr=final_score_map.end();
      score_itr--;
     for(cosine_itr=cosine.end();cosine_itr!=cosine.begin();)
           	    		{
 	 	 	 	 	  
			 cosine_itr--;
                        if(count<11&&author_flag==0)
			{
       	    	    	file_name=score_itr->second;                         
                         score_itr--;
                       }
                       else
			{
       	    	    	fp.seekg(file_dict.find(cosine_itr->second)->second,ios::beg);
       	    	    	char c;
       	    	    	file_name="";
       	    	    	fp.get(c);
       	    	    	while(c!='$')
       	    	    	{
       	    	    		file_name+=c;
       	    	    		fp.get(c);
       	    	    	}
                        }//end of else 
       	    	    	if(count==no_of_docs+1)
       	    	    		break;
                   
                        string  sem_file_name="/cse535/cse535_fall2011/data/wiki/"+file_name;
                        sem_fp.open(sem_file_name.c_str());
                       if (!sem_fp.is_open()&&author_flag==0)
                            { 
                                    cout<<endl<<count<<"\t"<<file_name<<endl<<endl;
                                    count++;
                                  file_name="/cse535/cse535_fall2011/data/news/"+file_name;                             
 				 sem_fp.open(file_name.c_str());
                                 getline(sem_fp,sem_line);                                   
                                while(sem_line.find(first)==-1)
                                  {
                                        
                                        getline(sem_fp,sem_line);
                                                 if(sem_fp.eof())
                                                        break;
                                   }
                                  cout<<sem_line<<endl<<endl;
	                             sem_fp.close();  
                                  continue;                 
                            }
                             else 
                                sem_fp.close();
                                  sem_file_name="/cse535/yagoo/output/src_online/semwiki/sem_"+file_name;
                                 sem_fp.open(sem_file_name.c_str());
                                
                                  getline(sem_fp,sem_line);
                                 getline(sem_fp,sem_line);
                                //   cout<<"author= "<<sem_line;
                                   if(author_flag==1)
                                          {
               
                                            if(sem_line.find(author)==-1) 	
                                              { 
						sem_fp.close(); 			                                                  
                                                  continue;
                                               }
                                                 
                                        }
					if(category_flag==1)
                                              {
                                                cout<<endl<<"hello"<<endl;
                                                if(cate_check.find(file_name)->second.find(cate)==-1)
                                                  {
                                                   cout<<endl<<file_name<<" "<<cate_check.find(file_name)->second<<endl;
                                                  sem_fp.close(); 			                                                                             								continue;
                                                  }
                                              }
	       	    	    	  cout<<endl<<count<<"\t"<<file_name<<endl<<endl;
                                   //sem_line="";

	       	    	    	  cout<<"\tClick to view actual Source: http://en.wikipedia.org/wiki/"<<file_name.substr(0,file_name.length()-4)<<endl<<endl;
                                   getline(sem_fp,third);
                                     getline(sem_fp,third);
                                     getline(sem_fp,fourth);
                                     sem_fp.close();      
                            //     cout<<"Author="<<sem_line<<endl;
                                file_name="/cse535/cse535_fall2011/data/wiki/"+file_name;                             
 				 sem_fp.open(file_name.c_str());
                                 getline(sem_fp,sem_line);                                   
                                while(sem_line.find(first)==-1)
                                  {
                                        
                                        getline(sem_fp,sem_line);
                                                 if(sem_fp.eof())
                                                    { 
                                                      eof_flag=1;
                                                        break;
                                                    }
                                   }
                                   if(eof_flag==1)
                                     cout<<third<<endl<<fourth<<endl;
                                    else
                                  cout<<sem_line<<endl<<endl;
                             sem_fp.close();  
                             count++;
          	    }//End of display 

 //code for choice!
	
	 std::map<string,int> author_map;
   	std::map<string,int>::iterator aut_itr;
  for(cosine_itr=cosine.end();cosine_itr!=cosine.begin();)
           	    		{
        	          
			 cosine_itr--;
                   	fp.seekg(file_dict.find(cosine_itr->second)->second,ios::beg);
       	    	    	char c;
       	    	    	file_name="";
       	    	    	fp.get(c);
       	    	    	while(c!='$')
       	    	    	{
       	    	    		file_name+=c;
       	    	    		fp.get(c);
       	    	    	}
                        ac_itr=author_check.find(file_name);               
                       if (ac_itr!=author_check.end())
                            { 
                                   aut_name=ac_itr->second;
                                     if(aut_name==" ")
                                             aut_name="Anonymous";
                                  aut_itr=author_map.find(aut_name);
                                 if(aut_itr==author_map.end())
                                     author_map.insert(std::pair<string,int>(aut_name,1)); 
                                   
                                 else 
                                     aut_itr->second++;
                           }
                              
                }
	std::multimap<int,string> multi_author_map;
   	std::multimap<int,string>::iterator multi_aut_itr;
            for(aut_itr=author_map.begin();aut_itr!=author_map.end();aut_itr++)
               {
                  multi_author_map.insert(std::pair<int,string>(aut_itr->second,aut_itr->first)); 
                }
cout<<"--------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl<<endl;
cout<<"Search by Authors"<<endl;
              count=0;
            for(multi_aut_itr=multi_author_map.end();multi_aut_itr!=multi_author_map.begin();)
               {
                  multi_aut_itr--;
                   count++;
                  cout<<multi_aut_itr->second<<" - "<<multi_aut_itr->first<<"\t\t"; 
                  if(count%3==0)
                     cout<<endl;
                  if(count==9)
                      break;
                }
cout<<"--------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl<<endl;
                       

   	    fp.close();
	     gettimeofday(&end, NULL);
             seconds  = end.tv_sec  - start.tv_sec;
     	    useconds = end.tv_usec - start.tv_usec;

     	    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
 cout<<"-------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl;

     	    cout<<endl<<ret_no<<" files retrieved in "<<mtime<<" milliseconds"<<endl<<endl;
 cout<<"-------------------------------------------------------------------------------------------------------------------------------------------------------------";

     file_tf_idf.clear();
     cosine.clear();
}

   return true;
 }



bool Query_processor::process_query(std::string query_file_path)
{

	int flag=0;
   qfp.open(query_file_path.c_str());
    ofstream op;
    ofstream op1;
    ofstream op2;
    op.open("/cse535/yagoo/output_bool");
    op1.open("/cse535/yagoo/output_cosine");
    op2.open("/cse535/yagoo/output_okapi");
  fp.open("/cse535/yagoo/output/src_offline/filenames.txt");
    //fp.open("src/filenames.txt");
   		    while (getline(qfp,line).good()) {


   while(line.find("<num>")!=0)
   {
        getline(qfp,line);
        if(qfp.eof())
        	{
        	flag=1;
        	  op.close();
        	break;
        	}
   }
    
   wc=0;
   query.clear();
   std::istringstream ins;
     ins.str(line);
     ins>>word;
     ins>>word;
     ins>>query_no;

     cout<<query_no<<" ";
       getline(qfp,query_title);
       std::istringstream ins2;
       ins2.str(query_title);
        ins2>>word;
       while(ins2>>word)
   	    {
   	     sb="";
   	     l1=word.length();
   	     vector<int>  fids;

   	    	    	 for(i=0;i<l1;i++)
   	    	    		 {
   	    	    		 if ((word[i] >= '0' && word[i] <= '9')|| (word[i] >= 'a' && word[i] <= 'z')|| (word[i] >= 'A' && word[i] <= 'Z'))
   	    	    		 {
   	    	    		      if(word[i] >= 'A' && word[i] <= 'Z')
   	    	    		             word[i]+=32;
   	    	    			 sb=sb+word[i];
   	    	    		 }
   	    	    		 }
   	    	      //stemming
   	    	                char *stw=new char[sb.size()+1];
   	    	                 stw[sb.size()]=0;
   	    	                  memcpy(stw,sb.c_str(),sb.size());
   	    	                  int y=stem(stw,0,sb.length()-1);
   	    	                  sb=sb.substr(0,y);

   	                       term_map_itr=term_dict.find(sb);
                               inv_itr=inv.find(term_map_itr->second);
                               if(inv_itr==inv.end())
                            	   continue;
                               fids=inv_itr->second;
                            postings_length=fids.size()/3;
                            idf=log(53476/postings_length);
                             query.push_back(idf);


                            for(it=fids.begin();it!=fids.end();it++)
                            {
                            	file_id=*it;
                            	it++;
                            	tf=*it;
                            	it++;
                            	tdf=*it;
                            	tfidf.clear();
                            	rsv.clear();
                            	bool_map_itr=boolean_map.find(file_id);
                            	 if(bool_map_itr==boolean_map.end())
                            	 {
                            		 boolean_map.insert(std::pair<int,int>(file_id,1));

                            		 for(i=0;i<wc;i++)
                            		 {
                            			 tfidf.push_back(0);
                            			 rsv.push_back(0);
                            		 }
                            		 tfidf.push_back(((float)tf/(float)tdf)*idf);
                            	        rsv.push_back((tf*3)/(2*(.25+(tdf/400))+tf));
                            		// cout<<endl<<tf<<" "<<tdf<<" "<<idf<<" "<<(tf/tdf)*idf<<endl;
                            		 file_tf_idf.insert(std::pair<int,vector<float> >(file_id,tfidf));
                            		 okapi.insert(std::pair<int,vector<float> >(file_id,rsv));

                            	 }
                            	 else
                            	 {
                            		 bool_map_itr->second++;
                            	     file_tf_idf_itr=file_tf_idf.find(file_id);
                            	     okapi_itr=okapi.find(file_id);
                            	     rsv=okapi_itr->second;
                            	      tfidf=file_tf_idf_itr->second;
                                     len=tfidf.size();
                                    	 for(i=len;i<wc;i++)
                                    	 {
                                    		 tfidf.push_back(0);
                                    		 rsv.push_back(0);
                                    	 }
                                    tfidf.push_back(((float)tf/(float)tdf)*idf);
                                    rsv.push_back((tf*3)/(2*(.25+(tdf/400))+tf));
                                    file_tf_idf_itr->second=tfidf;
                                    okapi_itr->second=rsv;

                            	 }

                            }
                wc++;
   	    }


   	    for(bool_map_itr=boolean_map.begin();bool_map_itr!=boolean_map.end();bool_map_itr++)
   	    {
   	    	if(bool_map_itr->second>=3)
   	    	{
   	    		multi_bool.insert(std::pair<int,int>(bool_map_itr->second,bool_map_itr->first));
   	    	    //cout<<bool_map_itr->second<<" "<<bool_map_itr->first<<endl;
   	    	}
   	    }

   	     query_itr=query.begin();
   	     que_sum=0;
   	   	 while(query_itr!=query.end())
   	   	 {
   	   		 que_sum+=pow((*query_itr),2);
   	   		 query_itr++;
   	   	 }
   	  que_sum=sqrt(que_sum);
   	   	 //cout<<"quesum= "<<que_sum<<endl;

   	    for(file_tf_idf_itr=file_tf_idf.begin();file_tf_idf_itr!=file_tf_idf.end();file_tf_idf_itr++)
   	    		{
   	    	sum=doc_sum=0;
   	    	query_itr=query.begin();
            for(doc_itr=file_tf_idf_itr->second.begin();doc_itr!=file_tf_idf_itr->second.end();doc_itr++)
                    {
            	           sum+=(*query_itr)*(*doc_itr);
               	            doc_sum+=pow((*doc_itr),2);
							query_itr++;
                  }

            doc_sum=sqrt(doc_sum);
            float result=sum/(que_sum*doc_sum);
             cosine.insert(std::pair<float,int>(result,file_tf_idf_itr->first));
       }


   	 for(okapi_itr=file_tf_idf.begin();okapi_itr!=file_tf_idf.end();okapi_itr++)
   	    	    		{
   	    	    	sum=0;
   	    	    	query_itr=query.begin();
   	             for(doc_itr=okapi_itr->second.begin();doc_itr!=okapi_itr->second.end();doc_itr++)
   	                     {
   	             	           sum+=(*query_itr)*(*doc_itr);
   	                	        query_itr++;
   	                     }
   	                multi_okapi.insert(std::pair<float,int>(sum,okapi_itr->first));
   	        }



          count=1;

   	    for(multi_bool_map_itr=multi_bool.end();multi_bool_map_itr!=multi_bool.begin();)
   	    {
   	    	multi_bool_map_itr--;
   	    	fp.seekg(file_dict.find(multi_bool_map_itr->second)->second,ios::beg);

   	    	char c;
   	    	file_name="";
   	    	fp.get(c);
   	    	while(c!='$')
   	    	{
   	    		file_name+=c;
   	    		fp.get(c);
   	    	}
   	    	op<<query_no<<"\t0\t"<<file_name<<"\t"<<count<<"\t"<<1<<"\t"<<"yagoo"<<endl;
   	    	count++;

  	    }

   	    count=1;
     for(cosine_itr=cosine.end();cosine_itr!=cosine.begin();)
           	    		{
       	 	 	 	 	 	 cosine_itr--;
       	    	    	fp.seekg(file_dict.find(cosine_itr->second)->second,ios::beg);

       	    	    	char c;
       	    	    	file_name="";
       	    	    	fp.get(c);
       	    	    	while(c!='$')
       	    	    	{
       	    	    		file_name+=c;
       	    	    		fp.get(c);
       	    	    	}
       	    	    	op1<<query_no<<"\t0\t"<<file_name<<"\t"<<count<<"\t"<<cosine_itr->first<<"\t"<<"yagoo"<<endl;
       	    	    	count++;

           	    		}



   	 count=1;
     for(multi_okapi_itr=multi_okapi.end();multi_okapi_itr!=multi_okapi.begin();)
           	    		{
       	 	 	 	 	 	 multi_okapi_itr--;
       	    	    	fp.seekg(file_dict.find(multi_okapi_itr->second)->second,ios::beg);
       	    	    	char c;
       	    	    	file_name="";
       	    	    	fp.get(c);
       	    	    	while(c!='$')
       	    	    	{
       	    	    		file_name+=c;
       	    	    		fp.get(c);
       	    	    	}
       	    	    	op2<<query_no<<"\t0\t"<<file_name<<"\t"<<count<<"\t"<<multi_okapi_itr->first<<"\t"<<"yagoo"<<endl;
       	    	    	count++;


           	    		}


     multi_bool.clear();
     boolean_map.clear();
     file_tf_idf.clear();
     multi_okapi.clear();
     cosine.clear();
     okapi.clear();
   	 getline(qfp,line);
   }


        cout<<endl<<"Processed queries! output_boolean output_cosine output_okapi are the output files";  
  		  op.close();
   		  op1.close();
   		  op2.close();

   	    fp.close();
   return true;
 }







