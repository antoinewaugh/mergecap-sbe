#pragma once

#include <iostream>
#include <vector>
#include <cstdint>
#include <unistd.h>
#include <stdio.h>
#include <cassert>

inline uint64_t read64(const void *buf) {
    return *static_cast<const uint64_t *>(buf);
}

inline uint16_t read16(const void *buf) {
    return *static_cast<const uint16_t *>(buf);
}

struct packet_t {
    uint64_t sendingtime;
    FILE *fd;
    size_t offset;
    uint16_t length;
};

static packet_t read_packet(FILE *fd, const unsigned char *buf, size_t size) {

    constexpr auto header = 4;

    packet_t packet;
    packet.length = read16(buf + 2) + header;
    packet.sendingtime = read64(buf + 8);
    packet.offset = ftell(fd) - size;
    packet.fd = fd;

    return packet;
}

bool operator<(const packet_t &lhs, const packet_t &rhs) {
    return lhs.sendingtime < rhs.sendingtime;
}

void merge_pcap(const std::vector<std::string> &files,
                const std::string &filename) {

    unsigned char rbuf[sizeof(packet_t)];

    std::vector<FILE *> descriptors;

    std::vector<packet_t> packets;
    for (auto &path: files) {
        auto fd = fopen(path.c_str(), "rb");
        descriptors.push_back(fd);
        while (auto bytes = fread(rbuf, sizeof(rbuf), 1, fd) > 0) {
            auto packet = read_packet(fd, rbuf, sizeof(rbuf));
            assert(packet.length < 1500);
            packets.push_back(packet);
            fseek(fd, -sizeof(rbuf) + packet.length, SEEK_CUR);
        }
    }

    std::sort(packets.begin(), packets.end());

    auto fd_out = fopen(filename.c_str(), "wb");
    unsigned char wbuf[1500];
    for (auto &packet: packets) {
        fseek(packet.fd, packet.offset, SEEK_SET);
        auto bytes = fread(wbuf, sizeof(wbuf), 1, packet.fd);
        fwrite(wbuf, packet.length, 1, fd_out);
    }
    fclose(fd_out);

    for (auto fd: descriptors) {
        fclose(fd);
    }

}

