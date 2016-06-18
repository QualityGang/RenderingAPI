#pragma once

#include <string>
#include <vector>

#include "DLLExport.h"

namespace stdex {

void split_string(const std::string &str, const std::string &delimiter, std::vector<std::string> *strings);

}