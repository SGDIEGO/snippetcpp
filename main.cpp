#include <filesystem>
#include <iostream>
#include <fstream>
#include <json/json.h>

void error_show(std::string mssge){
    std::cout<<"ERROR: "<<mssge;
    exit(1);
}

int main(int, const char* argv[] ){
    // File snippets
    const std::string path =  "Code - OSS/User/snippets/cpp.json";
    if(argv[1] == nullptr) error_show("Missing specific directory snippet!\n");

    // Directory from snippets
    std::string directory = "/home/diego04/Snippets/";
    directory.append(argv[1]); // Add carpet snippets

    // Read json file snippet
    std::ifstream read_json(path);

    // Show error
    if(!read_json.is_open()) error_show("Error reading json!\n");
    
    // Parse text to json format
    Json::Value snippets_json;
    read_json >> snippets_json;

    auto dir = std::filesystem::directory_iterator(directory);
//    if(!dir->is_directory() || !dir->exists()) error_show("Directory not exists!");

    // Read all files in directory snippets
    for(const auto& entry : dir){
        // Get file name
        std::cout<<"->Loading "<<entry.path()<<'\n';

        // Open file
        std::ifstream file(entry.path());

        if(!file.is_open()){
            std::cout<<entry.path().filename()<<" not load";
            continue;
        }

        // Info snippets
        std::string line;
        std::vector<std::string> name;
        std::vector<std::string> info;
        int size_snippets = 0;

        // Read file line to line
        while(file) {
            std::getline(file, line); // Get line

            // Load name for snippet
            if(line[0] == '.'){
                size_snippets++;
                name.push_back(line.substr(1, line.size())); // Title
                continue;
            }

            if(size_snippets == 0){
                std::cout<<"\t->Missing name snippet: .'name'\n";
                continue;
            }

            if(info.size() != size_snippets) info.push_back("");
            info[size_snippets-1].append(line + '\n');
        }

        // Create key 'name_file' with value 'info'
        for(int i = 0; i < size_snippets; i++){
            std::cout<<"\t->Added "<<name[i] + "_snippet\n";
            snippets_json[name[i] + "_snippet"]["body"] = info[i];
            snippets_json[name[i] + "_snippet"]["prefix"] = name[i];
        }
    }

    // Write new json to snippet vscode
    std::ofstream input_json(path);
    input_json << snippets_json;

    // Close files
    input_json.close();
    read_json.close();
}