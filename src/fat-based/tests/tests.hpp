#include "legacyTest.hpp"

bool test(bool result, string testName = "") {
    if (result) {
        cout << "test passed : " << testName << endl;
    } else {
        cout << "!- TEST FAILED : " << testName << endl;
    };
    return result;
}

void runFatTests() {
    //legacyTest();

    cout << endl;
    cout << "Running Fat Tests" << endl << endl;

    float size = 2;
    float passed = 0;

    if (test(calculateFragmentation(), "calculateFragmentation")) { passed++; };
    if (test(calculateFragmentationAllBlocksFree(), "calculateFragmentationAllBlocksFree")) { passed++; };

    cout << endl;
    if (passed == size) {
        cout << "SUCCESS! " << passed << " of " << size << " tests passed!" << endl << endl;
    } else {
        cout << "FAILURE! " << passed << " of " << size << " tests passed. " << passed / size * 100.0 << "%" << endl
             << endl;
    }
}

