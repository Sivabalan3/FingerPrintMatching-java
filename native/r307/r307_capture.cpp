#include <iostream>
#include <unistd.h>
#include "r307_driver.h"
#include "base64.h"

using namespace std;

void waitFinger(R307& sensor, const string& msg) {
    cout << msg << flush;
    while (!sensor.readImage()) {
        usleep(100000);
    }
    cout << " -> OK!" << endl;
}

int main(int argc, char* argv[]) {
    try {
        R307 sensor("/dev/ttyUSB0");

        waitFinger(sensor, "Place finger");
        if(!sensor.image2Tz(1)) throw runtime_error("Img2Tz 1 failed");

        waitFinger(sensor, "Place same finger again");
        if(!sensor.image2Tz(2)) throw runtime_error("Img2Tz 2 failed");

        if(!sensor.createModel()) throw runtime_error("Model creation failed");

        auto tpl = sensor.downloadTemplate();
        if(tpl.empty()) {
            cout << "RESULT=FAILED_DOWNLOAD" << endl;
            return 1;
        }

        string b64 = base64_encode(tpl.data(), tpl.size());
        
        // This is the output your Java app should parse
        cout << "ISO_TEMPLATE_BASE64=" << b64 << endl;
        cout << "SIZE_BYTES=" << tpl.size() << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}