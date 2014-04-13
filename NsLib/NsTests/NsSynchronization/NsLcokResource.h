#ifndef NS_LCOK_RESOURCE_H
#define	NS_LCOK_RESOURCE_H

#include "../../NsSynchronization.h"

#include <iostream>
#include <thread>

#ifdef __GNUC__
#include <unistd.h>
#elif defined(_MSC_VER)
#include <Windows.h>
#else
#error Please port to your plateform yourself
#endif

namespace NsLibTest
{

static char threadSharedResource = 'X';
static ::NsLib::NsLcok threadMutex;

void threadFunA()
{
    int counter = 0;

    std::cout << "Start thread A" << std::endl;
    ::NsLib::Lock< ::NsLib::NsLcok> threadALcok{&threadMutex};

    while (true)
    {
        std::cout << "thread A" << std::endl;
        threadSharedResource = 'A';
        std::cout << "threadSharedResource: "
                  << threadSharedResource << std::endl;

        if (10 == ++counter)
        {
            break;
        }

#ifdef __GNUC__
        sleep(1);
#elif defined(_MSC_VER)
        Sleep(1000);
#else
#error Please port to your plateform yourself
#endif
    }
}

void threadFunB()
{
    sleep(2);
    std::cout << "Start thread B" << std::endl;

    while (true)
    {
        ::NsLib::Lock< ::NsLib::NsLcok> threadBLcok{&threadMutex};
        std::cout << "thread B" << std::endl;
        threadSharedResource = 'B';
        std::cout << "threadSharedResource: "
                  << threadSharedResource << std::endl;
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

