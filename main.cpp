#include <cstdlib>
#include <iostream>
#include <cassert>
#include <typeinfo>
#include <new>
#include <vector>
#include <thread>

#include "NsLib/NsPool.h"

#include "NsLib/NsTests/NsPool/NsTestPool.h"
#include "NsLib/NsTests/NsSynchronization/NsTestSynchronization.h"
#include "NsLib/NsTests/NsUnitTest/NsTestRunner.h"

using namespace std;

int main(int argc, char *argv[])
{
    // 自己实现一些C++11特性，达到向后兼容的目的。
    NsLibTest::testObjectPoolUsePodType();
    NsLibTest::testObjectPoolUseMySimpleClass();
    NsLibTest::testIntrusiveObjectPoolUseIntrusivePoolClass();
    NsLibTest::testIntrusiveObjectPoolUseMultiInheritIntrusivePoolClass();

//    NsLibTest::testLock();
//    NsLibTest::testSynchronizedObject();

    NsLibTest::testMultiThreadTestRunner();

    return 0;
}



