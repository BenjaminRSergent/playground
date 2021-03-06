#include <iostream>
#include <string>

struct MakeMe {
public:
    MakeMe(float floatNum, bool boolVal, std::string myString)
        : floatNum(floatNum), boolVal(boolVal), myString(myString) {
        std::cout << "Called MakeMe constructor\n";
    }

    ~MakeMe() {
        std::cout << "Called MakeMe destructor\n";
    }
    float floatNum;
    bool boolVal;
    std::string myString;
};

int main(int argc, char** argv) {
    uint8_t test[sizeof(MakeMe)];
    new (test) MakeMe(2.7, false, "Hello World");

    auto* makeMeCast = reinterpret_cast<MakeMe*>(test);

    std::cout << "Expect 2.7, 0, Hello World: " << makeMeCast->floatNum << ", " << makeMeCast->boolVal << ", "
              << makeMeCast->myString << "\n";

     reinterpret_cast<MakeMe*>(test)->~MakeMe();

    return 0;
}