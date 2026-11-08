// ============================================================
//  الملف 5: main.cpp
//  فيه القايمة (Menu) بس، وكل الشغل بيتعمل في VoleMachine
// ============================================================
#include <iostream>
#include <string>

#include "VoleMachine.h"

using namespace std;

int main() {
    VoleMachine machine;
    int choice = -1;

    while (choice != 0) {
        cout << "--- Vole Machine ---\n"
             << "1) Load program\n"
             << "2) Run\n"
             << "3) Step\n"
             << "4) Display state\n"
             << "5) Reset\n"
             << "0) Exit\n"
             << "Choice: ";
        if (!(cin >> choice)) break;  // لو الإدخال بوظ اخرج

        if (choice == 1) {
            // تحميل برنامج
            string filename;
            string addrStr;
            cout << "File name: ";
            cin >> filename;
            cout << "Start address (hex, e.g. 00 or 10): ";
            cin >> addrStr;

            unsigned addr = 0;
            try {
                addr = stoul(addrStr, nullptr, 16);  // تحويل النص من hex لرقم
            } catch (...) {
                cout << "Invalid address.\n";
                continue;
            }
            if (addr > 255) {
                cout << "Address must be between 00 and FF.\n";
                continue;
            }

            if (machine.loadProgram(filename, static_cast<uint8_t>(addr)))
                cout << "Program loaded successfully.\n";
            else
                cout << "Load failed: " << machine.getErrorMessage() << endl;

        } else if (choice == 2) {
            machine.run();           // تشغيل كامل
            machine.displayState();

        } else if (choice == 3) {
            machine.step();          // خطوة واحدة
            machine.displayState();

        } else if (choice == 4) {
            machine.displayState();  // عرض الحالة بس

        } else if (choice == 5) {
            machine.reset();         // إعادة ضبط
            cout << "Machine reset.\n";
        }
    }

    cout << "Goodbye!" << endl;
    return 0;
}
