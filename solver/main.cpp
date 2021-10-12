#include <stdio.h>
#include <vector>
#include <assert.h>
#include <algorithm>

#include <Nonogram.h>

class InputParser{
    public:
        InputParser (int &argc, char **argv){
            for (int i=1; i < argc; ++i)
                this->tokens.push_back(std::string(argv[i]));
        }

        const std::string& getCmdOption(const std::string &option) const{
            std::vector<std::string>::const_iterator itr;
            itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
            if (itr != this->tokens.end() && ++itr != this->tokens.end()){
                return *itr;
            }
            static const std::string empty_string("");
            return empty_string;
        }

        bool cmdOptionExists(const std::string &option) const{
            return std::find(this->tokens.begin(), this->tokens.end(), option)
                   != this->tokens.end();
        }
    private:
        std::vector <std::string> tokens;
};

void process_file (string &filename, bool rule_improve_log = false) {
    printf("Start processing: %s\n",filename.c_str());
    Nonogram *nonogram = new Nonogram(filename);
    if (nonogram->is_input_valid()) {
        if (rule_improve_log) {
            nonogram->enable_rule_improve_log();
        }
        nonogram->solve();
        nonogram->print();

        if(nonogram->is_solved()) {
            printf("Solved successfully\n");
        } else {
            printf("Unable to solve\n");
        }
    }
    delete nonogram;
    printf("End processing: %s\n",filename.c_str());
}

void print_usage() {
    printf("Usage:\n");
    printf("nonogram [-h] [-i] [-s] -f filename\n");
    printf("  Optional:\n");
    printf("  -h    Display this help text\n");
    printf("  -i    Log possible improvements for the rule mechanism\n");
    printf("  -s    Display solution as a image\n");
    printf("  Required:\n");
    printf("  -f    Input file name in txt of non format\n");
}

int main(int argc, char *argv[]) {

    

    InputParser input(argc, argv);
    if(input.cmdOptionExists("-h")){
        print_usage();
        exit(0);
    } else if (!input.cmdOptionExists("-f")) {
        printf("Missing -f parameter\n");
        print_usage();
        exit(0);
    }
    std::string filename = input.getCmdOption("-f");

    process_file (
        filename,
        input.cmdOptionExists("-i")
    );
    return 0;
}
