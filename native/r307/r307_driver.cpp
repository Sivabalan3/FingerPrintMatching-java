#include "r307_driver.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <iostream>

using namespace std;
using namespace chrono;

// Packet Helper
struct Packet {
    uint8_t type;
    vector<uint8_t> payload;

    Packet(uint8_t instr, const vector<uint8_t>& params = {}) {
        type = 0x01; // Command packet
        payload.push_back(instr);
        payload.insert(payload.end(), params.begin(), params.end());
    }

    vector<uint8_t> build() const {
        vector<uint8_t> pkt = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, type};
        uint16_t len = payload.size() + 2;
        pkt.push_back(len >> 8); 
        pkt.push_back(len & 0xFF);
        pkt.insert(pkt.end(), payload.begin(), payload.end());
        
        uint16_t sum = type + (len >> 8) + (len & 0xFF);
        for(auto b : payload) sum += b;
        pkt.push_back(sum >> 8); 
        pkt.push_back(sum & 0xFF);
        return pkt;
    }
};

R307::R307(const string &port) {
    fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if(fd < 0) throw runtime_error("Could not open serial port: " + port);
    
    termios t{};
    tcgetattr(fd, &t);
    cfsetispeed(&t, B57600);
    cfsetospeed(&t, B57600);
    t.c_cflag = CS8 | CLOCAL | CREAD;
    t.c_iflag = 0; t.c_oflag = 0; t.c_lflag = 0;
    t.c_cc[VMIN] = 0; t.c_cc[VTIME] = 1; 
    tcsetattr(fd, TCSANOW, &t);
}

R307::~R307() { if(fd >= 0) close(fd); }

vector<uint8_t> R307::readBytes(int n, int timeout_ms) {
    vector<uint8_t> buf(n);
    int total = 0;
    auto start = steady_clock::now();
    while(total < n) {
        int r = read(fd, buf.data() + total, n - total);
        if(r > 0) total += r;
        if(duration_cast<milliseconds>(steady_clock::now() - start).count() > timeout_ms) break;
        usleep(1000); 
    }
    buf.resize(total);
    return buf;
}

void R307::writeBytes(const vector<uint8_t>& data) {
    write(fd, data.data(), data.size());
}

vector<uint8_t> R307::readPacket() {
    auto header = readBytes(9, 500);
    if(header.size() < 9 || header[0] != 0xEF || header[1] != 0x01) return {};
    
    uint16_t len = (header[7] << 8) | header[8];
    auto body = readBytes(len, 500);
    if(body.size() < len) return {};
    
    header.insert(header.end(), body.begin(), body.end());
    return header;
}

vector<uint8_t> R307::sendCommand(uint8_t instr, const vector<uint8_t>& params) {
    writeBytes(Packet(instr, params).build());
    return readPacket();
}

// Sensor Actions
bool R307::readImage() { auto r = sendCommand(0x01); return r.size() > 9 && r[9] == 0x00; }
bool R307::image2Tz(uint8_t slot) { auto r = sendCommand(0x02, {slot}); return r.size() > 9 && r[9] == 0x00; }
bool R307::createModel() { auto r = sendCommand(0x05); return r.size() > 9 && r[9] == 0x00; }
void R307::storeModel(uint16_t id) { sendCommand(0x06, {0x01, uint8_t(id >> 8), uint8_t(id & 0xFF)}); }

// THE CORE FIX: Reading multiple data packets
vector<uint8_t> R307::downloadTemplate() {
    auto ack = sendCommand(0x08, {0x01}); // Request buffer 1
    if(ack.size() < 10 || ack[9] != 0x00) return {};

    vector<uint8_t> templateData;
    bool finished = false;

    // The sensor will now send several Data packets (0x02) and one End packet (0x08)
    while(!finished) {
        auto pkt = readPacket();
        if(pkt.size() < 11) break; // Error or timeout

        uint8_t type = pkt[6];
        uint16_t len = (pkt[7] << 8) | pkt[8];
        int dataSize = len - 2; // Subtract checksum length

        // Append only the raw data (skipping 9-byte header)
        templateData.insert(templateData.end(), pkt.begin() + 9, pkt.begin() + 9 + dataSize);

        if(type == 0x08) finished = true; 
    }
    return templateData;
}