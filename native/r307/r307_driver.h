#pragma once
#include <vector>
#include <string>
#include <cstdint>

class R307 {
    int fd;

    std::vector<uint8_t> readBytes(int n, int timeout_ms = 1000);
    void writeBytes(const std::vector<uint8_t>& data);

public:
    R307(const std::string &port);
    ~R307();

    std::vector<uint8_t> sendCommand(uint8_t instr, const std::vector<uint8_t>& params = {});
    std::vector<uint8_t> readPacket();

    bool readImage();          
    void image2Tz(uint8_t buf);
    void createModel();
    std::vector<uint8_t> downloadTemplate();
    void storeModel(uint16_t id);
    void uploadTemplate(uint8_t buf, const std::vector<uint8_t>& tpl);
};
