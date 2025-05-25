#include "Lexer.h"
#include "Parser.h"
#include "AST.h"
#include "CodeGen.h"
#include "VM.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>

using namespace MyLang;

static bool readFileToString(const std::string& path, std::string& out) {
    std::ifstream ifs(path);
    if (!ifs) return false;
    out.assign(std::istreambuf_iterator<char>(ifs),
               std::istreambuf_iterator<char>());
    return true;
}

static bool writeBytecodeToFile(const std::string& path,
                                const std::vector<uint8_t>& bc) {
    std::ofstream ofs(path, std::ios::binary);
    if (!ofs) return false;
    ofs.write(reinterpret_cast<const char*>(bc.data()), bc.size());
    return true;
}

static void printUsage(const char* prog) {
    std::cerr << "Usage:\n"
              << "  " << prog << " compile <source.ml> <out.myb>\n"
              << "  " << prog << " run <source.ml>\n";
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printUsage(argv[0]);
        return 1;
    }
    std::string mode = argv[1];
    std::string srcPath = argv[2];

    std::string source;
    if (!readFileToString(srcPath, source)) {
        std::cerr << "Error: cannot read source file '" << srcPath << "'\n";
        return 1;
    }

    // Parse
    Parser parser(source);
    auto program = parser.parseProgram();
    if (!program) {
        std::cerr << "Parsing failed.\n";
        return 1;
    }

    // Generate bytecode
    std::vector<uint8_t> bytecode;
    program->codegen(bytecode);

    if (mode == "compile") {
        if (argc < 4) {
            printUsage(argv[0]);
            return 1;
        }
        std::string outPath = argv[3];
        if (!writeBytecodeToFile(outPath, bytecode)) {
            std::cerr << "Error: cannot write bytecode file '" << outPath << "'\n";
            return 1;
        }
        std::cout << "Compiled '" << srcPath << "' → '" << outPath << "' (" 
                  << bytecode.size() << " bytes)\n";
        return 0;
    }
    else if (mode == "run") {
        // Directly run via VM
        VM vm;
        try {
            vm.run(bytecode);
        } catch (const std::exception& e) {
            std::cerr << "Runtime error: " << e.what() << "\n";
            return 1;
        }
        return 0;
    }
    else {
        printUsage(argv[0]);
        return 1;
    }
}
