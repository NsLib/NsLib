#ifndef NS_SIMPLE_CLASS_H
#define	NS_SIMPLE_CLASS_H


#include "../NsTestInfo.h"
#include "../../NsPool.h"

namespace NsLibTest
{

class MySimpleClass
{
public:
    MySimpleClass()
    {
        NS_TEST_MESSAGE("MySimpleClass()");
    }    

    MySimpleClass(int a)
    {
        NS_TEST_MESSAGE_2("MySimpleClass(int a) -- a = ", a);
    }
    
    ~MySimpleClass()
    {
        NS_TEST_MESSAGE("~MySimpleClass()");
    }
    
    int     field_1_;
    int     fiels_2_;
    double  field_3_;
};

}

#endif

