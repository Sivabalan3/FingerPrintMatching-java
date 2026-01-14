// ----r307_capture.cpp ----

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <vector>
#include <cstring>

// ---- MOCK ISO TEMPLATE (REPLACE WITH REAL ISO CONVERSION) ----
std::string fakeIsoBase64() {
    return "AAECAwQFBgcICQoLDA0ODw==";
}

// ---- SERIAL OPEN ----
int openSerial(const char* dev) {
    int fd = open(dev, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    termios tty{};
    tcgetattr(fd, &tty);

    cfsetospeed(&tty, B57600);
    cfsetispeed(&tty, B57600);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    tcsetattr(fd, TCSANOW, &tty);
    return fd;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Usage: r307_capture enroll|verify\n";
        return 1;
    }

    std::string mode = argv[1];
    int fd = openSerial("/dev/ttyUSB0");

    std::cout << "Place your finger...\n";
    sleep(1);

    // 👉 Here you do:
    // - GenImg
    // - Img2Tz
    // - RegModel
    // - GetChar
    // - Convert to ISO 19794-2

    close(fd);

    // OUTPUT FOR JAVA
    std::cout << "ISO_TEMPLATE_BASE64="
              << fakeIsoBase64() << std::endl;

    return 0;
}
