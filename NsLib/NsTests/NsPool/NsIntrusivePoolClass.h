#ifndef NS_INTRUSIVE_POOL_CLASS_H
#define	NS_INTRUSIVE_POOL_CLASS_H

#include "../NsTestInfo.h"
#include "../../NsPool.h"

namespace NsLibTest
{

class IntrusivePoolClass :
    public ::NsLib::INewFromObjectPool<IntrusivePoolClass, 3>
{
public:
    IntrusivePoolClass()
    {
        NS_TEST_MESSAGE("IntrusivePoolClass()");
    }

    IntrusivePoolClass(int a, int b)
    {
        NS_TEST_MESSAGE_4("IntrusivePoolClass(int a, int b) -- a = ",
                          a, ", b = ", b);
    }

    ~IntrusivePoolClass()
    {
        NS_TEST_MESSAGE("~IntrusivePoolClass()");
    }

    int     field_1_;
    int     fiels_2_;
    double  field_3_;
};

}

#endif

