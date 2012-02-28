
#include "includes.h"


class Query_processor
{
public:

	std::string query_no,query_title,word,sb,file_name,line,sem_line,line2;
	int  wc,l1,i,postings_length,file_id,len;
     //score
    std::map<std::string,float> file_score;  
    std::multimap<float,std::string> final_score_map;    
    std::multimap<float,std::string>::iterator score_itr;    
    float score,score1;
    std::ifstream myReadFile2;

    std::map<int,std::vector<float> > file_tf_idf;
    std::map<int,std::vector<float> > okapi;
    std::map<int,std::vector<float> >::iterator okapi_itr;
    std::map<int,std::vector<float> >::iterator file_tf_idf_itr;
    std::multimap<float,int> cosine;
    std::multimap<float,int> multi_okapi;
    std::multimap<float,int>::iterator multi_okapi_itr;
    std::multimap<float,int>::iterator cosine_itr;
     std::vector<float> tfidf;
     std::vector<float> query;
     std::vector<float> rsv;
     std::vector<float>::iterator query_itr;
     std::vector<float>::iterator doc_itr;
     float idf,value,tf,tdf,sum,que_sum,doc_sum;
      std::string third,fourth;
      int eof_flag;
    int count;
	std::map<int,int> boolean_map;
	std::multimap<int,int> multi_bool;

	std::map<std::string,int>::iterator term_map_itr;
	std::map<int,std::vector<int> >::iterator inv_itr;
	std::vector<int>::iterator it;
    std::map<int,int>::iterator bool_map_itr;
    std::multimap<int,int>::iterator multi_bool_map_itr;

	std::ifstream fp;
    std::ifstream sem_fp;
	std::ifstream qfp;
	Query_processor();
	~Query_processor();
	std::string term;
	int id,val,offset;
	std::ifstream myReadFile;
    std::string folder_name;
	std::map<std::string, int> term_dict;
	std::map<int,std::vector<int> > inv;
	std::map<int,int> file_dict;
      bool load_file_score(std::string file_path);
	bool load_file_dict(std::string file_path);
	    bool load_term_dict(std::string file_path);
	    bool load_inv(std::string base_directory_path);
	bool process_query(std::string query_file_path);
	bool process_online_query();
	bool load_author_check(std::string file_path);//for the author check during online query
	    std::map<std::string,std::string> author_check;  
            std::map<std::string,std::string>::iterator ac_itr;  
	    std::map<std::string,std::string> cate_check;  
            std::map<std::string,std::string>::iterator cate_itr;  
  
         std::string aut_name;

};
