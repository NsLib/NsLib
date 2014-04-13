#ifndef NS_TEST_SYNCHRONIZATION_H
#define	NS_TEST_SYNCHRONIZATION_H

#include "NsLcokResource.h"
#include "NsSynchronizedObjectClass.h"

#include <thread>

namespace NsLibTest
{

void testLock()
{
    std::thread threadA{NsLibTest::threadFunA};
    std::thread threadB{NsLibTest::threadFunB};

    while (true)
    {
#ifdef __GNUC__
        sleep(1);
#elif defined(_MSC_VER)
        Sleep(1000);
#else
#error Please port to your plateform yourself
#endif
    }
}

void testSynchronizedObject()
{
    std::thread threadC{NsLibTest::threadFunC};
    std::thread threadD{NsLibTest::threadFunD};

    while (true)
    {
#ifdef __GNUC__
        sleep(1);
#elif defined(_MSC_VER)
        Sleep(1000);
#else
#error Please port to your plateform yourself
#endif
    }
}

}

#endif

