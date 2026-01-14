#include <iostream>
#include "r307_driver.h"
#include <vector>
#include <string>
#include <chrono>
#include <thread>

using namespace std;
using namespace chrono;

void waitFinger(R307 &sensor, const string &msg){
    cout << msg << endl;
    while(!sensor.readImage()) this_thread::sleep_for(200ms);
}

int main() {
    try {
        R307 sensor("/dev/ttyUSB0");
        cout << "Select mode:\n1) Enroll\n2) Verify\nChoice: ";
        int mode; cin >> mode;

        if(mode == 1){
            string name; cout << "Enter name: "; cin >> name;
            waitFinger(sensor,"🟢 Place finger (30%)"); sensor.image2Tz(1);
            waitFinger(sensor,"🟡 Place SAME finger (60%)"); sensor.image2Tz(2);
            waitFinger(sensor,"🔵 Place SAME finger again (100%)"); sensor.image2Tz(1);

            sensor.createModel();
            auto iso_template = sensor.downloadTemplate();
            cout << "✅ Enrollment complete!\n";
            cout << "Name: " << name << "\nISO size: " << iso_template.size() << " bytes\n";
        }
        else if(mode == 2){
            waitFinger(sensor,"Place finger for verification...");
            sensor.image2Tz(1);
            auto iso_template = sensor.downloadTemplate();
            cout << "🔍 Verification captured, ISO size: " << iso_template.size() << " bytes\n";
        }
        else cout << "Invalid choice\n";

    } catch(const exception &e){
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }
    return 0;
}
