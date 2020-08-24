#pragma once

#include <iostream>
#include <vector>
#include <cstdint>
#include <unistd.h>
#include <stdio.h>

uint64_t read64(const void *buf) {
    return *static_cast<const uint64_t *>(buf);
}

uint32_t read32(const void *buf) {
    return *static_cast<const uint32_t *>(buf);
}

uint16_t read16(const void *buf) {
    return *static_cast<const uint16_t *>(buf);
}

struct packet_t {
    uint16_t channel;
    uint16_t length;
    uint32_t seqnum;
    uint64_t sendingtime;
    FILE *fd;
    size_t offset;
};

static packet_t read_packet(FILE *fd, const unsigned char *buf) {

    constexpr auto header = 4;

    packet_t packet;
    packet.channel = read16(buf);
    packet.length = read16(buf + 2) + header;
    packet.seqnum = read32(buf + 4);
    packet.sendingtime = read64(buf + 8);
    packet.offset = ftell(fd);
    packet.fd = fd;

    return packet;
}

bool operator<(const packet_t &lhs, const packet_t &rhs) {
    return lhs.sendingtime < rhs.sendingtime;
}

void merge_pcap(const std::vector<std::string> &files,
                const std::string &filename) {

    unsigned char buf[sizeof(packet_t)];

    std::vector<FILE *> descriptors;

    std::vector<packet_t> packets;
    for (auto &path: files) {
        auto fd = fopen(path.c_str(), "rb");
        descriptors.push_back(fd);
        while (true) {
            auto bytes = fread(buf, sizeof(buf), 1, fd);
            auto packet = read_packet(fd, buf);
            packets.push_back(packet);
            if (feof(fd)) {
                break;
            }
            fseek(fd, -sizeof(buf) + packet.length, SEEK_CUR);
        }
    }

    std::sort(packets.begin(), packets.end());
    auto fd_out = fopen(filename.c_str(), "wb");
    for (auto &packet: packets) {
        fseek(packet.fd, packet.offset, SEEK_SET);
        auto bytes = fread(buf, sizeof(buf), 1, packet.fd);
        fwrite(buf, packet.length, 1, fd_out);
    }
    fclose(fd_out);

    for (auto fd: descriptors) {
        fclose(fd);
    }

}

