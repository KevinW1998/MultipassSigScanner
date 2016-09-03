
#include "SigScanner.h"
#include "SigFileUtils.h"

#include <tclap/cmdline.h>
#include <json.hpp>

using json = nlohmann::json;

void main(int argc, char* argv[]) {

    try {
        TCLAP::CmdLine cmd("Multipass Sigscanner");

        TCLAP::UnlabeledValueArg<std::string> binaryFilename("binary file", "The binary file from which the signatures should be scanned to", true, "", "binaryFileName");
        TCLAP::UnlabeledValueArg<std::string> configFilename("config file", "The JSON config file, which provides all signature information", true, "", "configFileName");
        cmd.add(binaryFilename);
        cmd.add(configFilename);

        cmd.parse(argc, argv);

        std::vector<char> configData = MPSig::Utils::ReadFileBinary(configFilename.getValue());
        
        auto jsonObj = json::parse(configData.begin(), configData.end());
        


    } catch (const TCLAP::ArgException& ex) {
        std::cerr << "error: " << ex.error() << " for arg " << ex.argId() << std::endl;
    }


}
