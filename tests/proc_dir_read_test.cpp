#include <filesystem>  // filesystem library
#include <iostream>
#include <stdio.h>// std::cout, std::endl
#include <string>

namespace fs = std::filesystem;

int main()
{
    const fs::path target_path{"/proc"};
    FILE* process;
    char buffer[1024];
    try {
        for (auto const& dir_entry : fs::directory_iterator{target_path})
        {
                if(dir_entry.is_directory())
                {
                    std::string path = dir_entry.path().string() + "/status";
                    process = fopen(path.c_str(), "r");
                    if(process == NULL){
                        std::cout << "couldn't open file " << path.c_str() << std::endl;
                        continue;
                    }
                    
                    while (fgets(buffer, sizeof(buffer), process) != NULL) {
                        std::cout << buffer;
                    }
                    std::cout << std::endl;
                    
                    fclose(process);
                } else {
                    std::cout << "not a directory" << dir_entry.path().string() << std::endl;
                }
        }
    }
    catch (fs::filesystem_error const& ex)
    {
        std::cout << "Error occurred during file operations!\n" << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
