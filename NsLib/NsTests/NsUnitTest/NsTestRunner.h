#ifndef NS_TEST_RUNNER_H
#define	NS_TEST_RUNNER_H

#include <iostream>
#include <exception>
#include <unistd.h>
#include <thread>

#include "../../NsUnitTest.h"
#include "NsTestClass.h"

namespace NsLibTest
{

static ::NsLib::TestRunner  threadATestRunner;
static ::NsLib::TestRunner  threadBTestRunner;
static ::NsLib::TestRunner  threadCTestRunner;

void runnerThreadFunA()
{
    threadATestRunner.output();
}

void runnerThreadFunB()
{
    threadBTestRunner.output();
}

void runnerThreadFunC()
{
    threadCTestRunner.output();
}

void testMultiThreadTestRunner()
{
    ::NsLibTest::TestClass  a;
    ::NsLibTest::TestClass  b;
    ::NsLibTest::TestClass  c;
    ::NsLibTest::TestClass  d;
    ::NsLibTest::TestClass  e;
    ::NsLibTest::TestClass  f;
    ::NsLibTest::TestClass  g;

    threadATestRunner.addTests(&a);
    threadATestRunner.addTests(&b);
    threadBTestRunner.addTests(&c);
    threadBTestRunner.addTests(&d);
    threadCTestRunner.addTests(&e);
    threadCTestRunner.addTests(&f);
    threadCTestRunner.addTests(&g);


    threadATestRunner.run();
    threadBTestRunner.run();
    threadCTestRunner.run();

    std::thread threadA{::NsLibTest::runnerThreadFunA};
    std::thread threadB{NsLibTest::runnerThreadFunB};
    std::thread threadC{NsLibTest::runnerThreadFunC};

    while (true)
    {
        std::cout << __func__ << std::endl;
        sleep(3);
    }
}

}

#endif

