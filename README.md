# mergecap-sbe

Merges CME datamine pcap files based on `sendingtime`.

## Why

The CME SBE market data pcap files are binary encoded, making them incompatible with wireshark's `mergecap`.

## Usage

```
$ ./mergecap_sbe \
    --output merged.pcap
    --input 20200602-PCAP_310_0_*_0-20200602 \
            20200601-PCAP_310_0_*_0-20200601 \
            20200601-PCAP_318_0_*_0-20200601 \
            20200601-PCAP_318_0_*_0-20200531 \
            20200601-PCAP_310_0_*_0-20200531 

$ ls -lrth merged.pcap
-rw-rw-r-- 1 working example 6.2G Aug 24 15:53 merged.pcap

```