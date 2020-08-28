#include "CLI/CLI.hpp"
#include "mergecap_sbe.h"

int main(int argc, char **argv) {

    CLI::App app{"Merge CME Datamine pcap files."};

    std::vector<std::string> src_files;
    app.add_option("--input", src_files, "Input/Source file paths.")->required();

    std::string dest;
    app.add_option("--output", dest, "Output file path.")->required();

    CLI11_PARSE(app, argc, argv);

    merge_pcap(src_files, dest);

    return 0;
}
