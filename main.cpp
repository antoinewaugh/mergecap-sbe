#include "mergecap_sbe.h"

int main() {

    auto outfile = "out.pcap";

    merge_pcap({"20200602-PCAP_310_0_*_0-20200602",
                "20200601-PCAP_310_0_*_0-20200601",
                "20200601-PCAP_318_0_*_0-20200601",
                "20200601-PCAP_318_0_*_0-20200531",
                "20200601-PCAP_310_0_*_0-20200531",
               }, outfile);

    return 0;
}
