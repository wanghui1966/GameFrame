#include "singleton.h"

#include <iostream>
using namespace std;

class Wind : public Singleton<Wind>
{
public:
    Wind() { cout << "Wind constructor." << endl; }
    ~Wind() { cout << "Wind destructor." << endl; }

    void Output() { cout << "Wind Output." << endl; }
};

#define sWind (*Wind::Instance())

int main()
{
    sWind.NewInstance();

    sWind.Output();
    sWind.Output();

    sWind.DeleteInstance();
    return 0;
}
