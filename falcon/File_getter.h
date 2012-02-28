
#include "includes.h"
#include "Query_processor.h"

class File_getter
{

public:

	int flag_inv;

    std::string folder_name;
	std::ofstream fp;//fp for writing filename in txt file
	File_getter();
	~File_getter();
	bool files_to_process(std::string base_directory_path,std::string file);
	Query_processor obj;

};
