#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <regex>
#include "extensions.h"

namespace Helpers{
    Extension getExtension(std::string filepath){
        std::regex extensionRegex(R"(\.([^./\\]+)$)");
        std::smatch match;
        
        if (std::regex_search(filepath, match, extensionRegex)){
            std::string extension = match[1].str();

            if (extension == "mp3")
                return MP3;

            if (extension == "wav")
                return WAV;
        }
        return UNKNOWN;
    }
}

#endif