#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>
#include "r307_driver.h"
#include "base64.h"

using namespace std;

// Wait for finger to be placed
void waitFinger(R307& sensor, const string& msg) {
    cout << msg << endl;
    while (!sensor.readImage()) {
        usleep(200000); // 200ms
    }
    usleep(500000); // small delay after finger capture
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "Usage: ./r307_capture enroll|verify\n";
        return 1;
    }

    string mode = argv[1];

    try {
        R307 sensor("/dev/ttyUSB0"); // adjust if your port is different

        if (mode == "enroll") {
            // Step 1: Place first finger
            waitFinger(sensor, "🟢 Place finger (1/2)");
            sensor.image2Tz(1);

            // Step 2: Place same finger again
            waitFinger(sensor, "🟡 Place SAME finger (2/2)");
            sensor.image2Tz(2);

            // Step 3: Create model
            sensor.createModel();

            // Step 4: Download template
            auto tpl = sensor.downloadTemplate();
            if (tpl.empty()) {
                cerr << "❌ Template generation failed\n";
                return 1;
            }

            // Step 5: Convert to Base64 and output
            string b64 = base64_encode(tpl.data(), tpl.size());
            cout << "ISO_TEMPLATE_BASE64=" << b64 << endl;

            // Optional: store on sensor (ID=1)
            sensor.storeModel(1);
        }
        else if (mode == "verify") {
            waitFinger(sensor, "🔍 Place finger for verification");
            sensor.image2Tz(1);

            auto search_result = sensor.sendCommand(0x04, {0x01, 0x00, 0x00, 0x03, 0xE8}); // search 0-1000
            if (search_result.size() > 13 && search_result[9] == 0x00) {
                int matched_id = (search_result[10] << 8) | search_result[11];
                cout << "✅ MATCH FOUND! Sensor ID: " << matched_id << endl;
            } else {
                cout << "❌ NO MATCH FOUND\n";
            }
        }
    }
    catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }

    return 0;
}
