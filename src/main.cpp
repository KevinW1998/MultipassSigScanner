
#include "SigScanner.h"
#include "SigFileUtils.h"

#include <iostream>

#include <tclap/cmdline.h>
#include <json.hpp>

using json = nlohmann::json;

int main(int argc, char* argv[]) {

    try {
        TCLAP::CmdLine cmd("Multipass Sigscanner");

        TCLAP::UnlabeledValueArg<std::string> binaryFilename("binary-file", "The binary file from which the signatures should be scanned to", true, "", "binary-file");
        TCLAP::UnlabeledValueArg<std::string> configFilename("config-file", "The JSON config file, which provides all signature information", true, "", "config-file");
        cmd.add(binaryFilename);
        cmd.add(configFilename);
        cmd.parse(argc, argv);

        std::vector<char> configData = MPSig::Utils::ReadFileBinary(configFilename.getValue());
        auto jsonObj = json::parse(configData.begin(), configData.end());
        
        
        MPSig::SigScanner scanner(binaryFilename.getValue());
        scanner.SetOffset(jsonObj.at("offset"));
        std::vector<std::string> functionStartSigs = jsonObj.at("function-start-sig");
        scanner.SetFunctionStartSig(MPSig::SigScannerEntry{ functionStartSigs });

        for (const auto& nextObj : jsonObj.at("signatures")) {
            std::string name = nextObj.at("name");
            std::vector<std::string> sigs = nextObj.at("sigs");
            MPSig::SigScannerEntry entry{ sigs };
            
            std::cout << "Scanning \"" << name << "\"\n";
            auto scanResult = scanner.Scan(entry);
            std::printf("Possible scan hit is 0x%08x and function hit is 0x%08x\n", scanResult.first, scanResult.second);
        }

    } catch (const TCLAP::ArgException& ex) {
        std::cerr << "error: " << ex.error() << " for arg " << ex.argId() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "error: " << ex.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
