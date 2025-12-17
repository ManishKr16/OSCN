#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

class Hamming {
public:
    string data;
    int m, r = 0;
    char* msg;

    Hamming(string data) {
        this->data = data;
        reverse(data.begin(), data.end());

        m = data.size();
        int power = 1;

        while (power < (m + r + 1)) {
            r++;
            power *= 2;
        }

        msg = new char[m + r + 1];

        int curr = 0;

        for (int i = 1; i <= m + r; i++) {
            if (i & (i - 1)) {
                msg[i] = data[curr++];
            } else {
                msg[i] = 'n';
            }
        }

        setParityBits();
    }

    void showMessage() {
        cout << "The data packet to be sent is: ";
        for (int i = m + r; i >= 1; i--) {
            cout << msg[i] << " ";
        }
        cout << endl;
    }

    void setParityBits() {
        int bit = 0;

        for (int i = 1; i <= m + r; i *= 2) {
            int count = 0;

            for (int j = i + 1; j <= m + r; j++) {
                if (j & (1 << bit)) {
                    if (msg[j] == '1') count++;
                }
            }

            msg[i] = (count % 2 == 0) ? '0' : '1';
            bit++;
        }

        showMessage();
    }

    void receiver() {
        string ans = "";
        int bit = 0;

        for (int i = 1; i <= m + r; i *= 2) {
            int count = 0;

            for (int j = i + 1; j <= m + r; j++) {
                if (j & (1 << bit)) {
                    if (msg[j] == '1') count++;
                }
            }

            if (count % 2 == 0) {
                ans.push_back(msg[i] == '0' ? '0' : '1');
            } else {
                ans.push_back(msg[i] == '1' ? '0' : '1');
            }

            bit++;
        }

        if (ans.find('1') != string::npos) {
            int wrongbit = 0, power = 1;

            for (int i = 0; i < ans.size(); i++) {
                if (ans[i] == '1') wrongbit += power;
                power *= 2;
            }

            cout << "Error detected at bit position: " << wrongbit << endl;
        }
        else {
            cout << "Correct data packet received" << endl;
        }
    }
};

int main() {
    string data;
    cout << "Enter data bits: ";
    cin >> data;

    Hamming h(data);

    // Uncomment this to simulate an error
    // h.msg[3] = (h.msg[3] == '0') ? '1' : '0';

    h.receiver();
    return 0;
}
