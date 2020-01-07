#include "singleton.h"

#include "debug.h"

class Wind : public Singleton<Wind>
{
public:
    Wind() { NLOG("Wind constructor"); }
    ~Wind() { NLOG("Wind destructor"); }

    void Output() { NLOG("Wind Output"); }
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
