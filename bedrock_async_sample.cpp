#include "LLMInterface.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_message>" << std::endl;
        return 1;
    }

    std::string input_message = argv[1];
    LLMInterface::invokeAsync(input_message);
    
    return 0;
}
