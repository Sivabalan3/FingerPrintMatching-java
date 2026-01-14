#include "r307_driver.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdexcept>
#include <chrono>
#include <thread>

using namespace std;
using namespace chrono;

// ---------------- R307 IMPLEMENTATION ----------------
class Packet {
public:
    vector<uint8_t> payload;
    Packet(uint8_t instr, const vector<uint8_t>& params = {}) {
        payload.push_back(instr);
        payload.insert(payload.end(), params.begin(), params.end());
    }

    vector<uint8_t> build() const {
        vector<uint8_t> pkt;
        pkt.push_back(0xEF); pkt.push_back(0x01);
        pkt.push_back(0xFF); pkt.push_back(0xFF);
        pkt.push_back(0xFF); pkt.push_back(0xFF);
        pkt.push_back(0x01);
        uint16_t len = payload.size() + 2;
        pkt.push_back(len >> 8); pkt.push_back(len & 0xFF);
        pkt.insert(pkt.end(), payload.begin(), payload.end());
        uint16_t chksum = 0x01 + (len >> 8) + (len & 0xFF);
        for(auto b : payload) chksum += b;
        pkt.push_back(chksum >> 8); pkt.push_back(chksum & 0xFF);
        return pkt;
    }
};

// ---------------- R307 METHODS ----------------
R307::R307(const string &port) {
    fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if(fd<0) throw runtime_error("Serial open failed");
    termios t{};
    tcgetattr(fd,&t);
    cfsetispeed(&t,B57600);
    cfsetospeed(&t,B57600);
    t.c_cflag = CS8 | CLOCAL | CREAD;
    t.c_lflag = 0; t.c_oflag = 0; t.c_iflag = 0;
    t.c_cc[VMIN] = 0; t.c_cc[VTIME] = 10;
    tcsetattr(fd,TCSANOW,&t);
}

R307::~R307(){ close(fd); }

vector<uint8_t> R307::readBytes(int n, int timeout_ms) {
    vector<uint8_t> buf(n);
    int total = 0;
    auto start = steady_clock::now();
    while(total < n){
        int r = read(fd, buf.data()+total, n-total);
        if(r>0) total += r;
        else this_thread::sleep_for(5ms);
        if(duration_cast<milliseconds>(steady_clock::now()-start).count()>timeout_ms) break;
    }
    buf.resize(total);
    return buf;
}

void R307::writeBytes(const vector<uint8_t>& data){
    write(fd, data.data(), data.size());
}

vector<uint8_t> R307::sendCommand(uint8_t instr, const vector<uint8_t>& params){
    auto pkt = Packet(instr, params).build();
    writeBytes(pkt);
    return readPacket();
}

vector<uint8_t> R307::readPacket(){
    auto header = readBytes(9);
    if(header.size()<9) return {};
    uint16_t len = (header[7]<<8)|header[8];
    auto body = readBytes(len);
    header.insert(header.end(), body.begin(), body.end());
    return header;
}

bool R307::readImage(){ auto r = sendCommand(0x01); return r.size()>9 && r[9]==0x00; }
void R307::image2Tz(uint8_t buf){ sendCommand(0x02,{buf}); }
void R307::createModel(){ sendCommand(0x05); }
vector<uint8_t> R307::downloadTemplate(){ 
    auto r = sendCommand(0x08,{0x01});
    if(r.size()<=11) return {};
    return vector<uint8_t>(r.begin()+9,r.end()-2);
}
void R307::storeModel(uint16_t id){ sendCommand(0x06,{uint8_t(id>>8), uint8_t(id&0xFF)}); }
void R307::uploadTemplate(uint8_t buf, const vector<uint8_t>& tpl){
    size_t pos = 0;
    while(pos<tpl.size()){
        size_t chunk = min<size_t>(128,tpl.size()-pos);
        vector<uint8_t> packet;
        packet.push_back(buf);
        packet.insert(packet.end(), tpl.begin()+pos, tpl.begin()+pos+chunk);
        sendCommand(0x0C, packet);
        pos += chunk;
    }
}
