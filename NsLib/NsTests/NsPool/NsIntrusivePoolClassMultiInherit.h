#ifndef NS_INTRUSIVE_POOL_CLASS_MULTI_INHERIT_H
#define NS_INTRUSIVE_POOL_CLASS_MULTI_INHERIT_H

#include "../NsTestInfo.h"
#include "../../NsPool.h"

namespace NsLibTest
{

class BaseInstrusivePoolClass_1 :
public ::NsLib::INewFromObjectPool<BaseInstrusivePoolClass_1, 3>
{
public:
    BaseInstrusivePoolClass_1()
    {
        NS_TEST_MESSAGE("BaseInstrusivePoolClass_1()");
    }

    virtual ~BaseInstrusivePoolClass_1()
    {
        NS_TEST_MESSAGE("~BaseInstrusivePoolClass_1()");
    }

    int     field_1_1_;
    int     fiels_1_2_;
    double  field_1_3_;
};

class BaseInstrusivePoolClass_2 :
    public ::NsLib::INewFromObjectPool<BaseInstrusivePoolClass_2, 3>
{
public:
    BaseInstrusivePoolClass_2()
    {
        NS_TEST_MESSAGE("BaseInstrusivePoolClass_2()");
    }

    virtual ~BaseInstrusivePoolClass_2()
    {
        NS_TEST_MESSAGE("~BaseInstrusivePoolClass_2()");
    }

    int     field_2_1_;
    int     fiels_2_2_;
    double  field_2_3_;
};

class DerivedInstrusivePoolClass :
    public ::NsLibTest::BaseInstrusivePoolClass_1,
    public ::NsLibTest::BaseInstrusivePoolClass_2,
    public ::NsLib::INewFromObjectPool<DerivedInstrusivePoolClass, 3>
{
public:
    NS_INTRUSIVE_OBJECT_POOL_MULTI_INHERIT(DerivedInstrusivePoolClass, 3);

    DerivedInstrusivePoolClass()
    {
        NS_TEST_MESSAGE("DerivedInstrusivePoolClass()");
    }

    virtual ~DerivedInstrusivePoolClass()
    {
        NS_TEST_MESSAGE("~DerivedInstrusivePoolClass()");
    }

    int     field_3_1_;
    int     fiels_3_2_;
    double  field_3_3_;
};

}

#endif

