#include "SigScannerJsonConfig.h"
#include "SigFileUtils.h"

#include <json.hpp>

using json = nlohmann::json;

MPSig::SigScannerJsonConfig::SigScannerJsonConfig(const std::string& fileName)
{
    std::vector<char> data = MPSig::Utils::ReadFileBinary(fileName);
    auto jsonConfig = json::parse(data.begin(), data.end());

    
}
