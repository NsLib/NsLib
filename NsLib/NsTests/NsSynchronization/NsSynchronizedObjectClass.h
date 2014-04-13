#ifndef NS_SYNCHRONIZED_OBJECT_CLASS_H
#define	NS_SYNCHRONIZED_OBJECT_CLASS_H

#include "../../NsSynchronization.h"

#include <unistd.h>

#include <iostream>

namespace NsLibTest
{

class SynchronizedObjectClass :
    public ::NsLib::SynchronizedObject< ::NsLib::NsLcok>
{
public:
    SynchronizedObjectClass(LockType *lockObject,
                            LockType *lockMethod,
                            char flag) :
        SynchronizedObject(lockObject, lockMethod),
        flag_{flag}
    {
            std::cout << __func__ << " flag: " << flag_ << std::endl;
    }

    virtual ~SynchronizedObjectClass()
    {
    }

    void lockedMethod()
    {
        std::cout << __func__ << " flag: " << flag_ << " entry"<< std::endl;

        NsLib::Lock<LockType> lockThisMethod(lockMethod_);

        int counter{0};

        while (true)
        {
            std::cout << __func__ << " flag: " << flag_ << std::endl;

            sleep(1);

            if (10 == ++counter)
            {
                break;
            }
        }

        std::cout << __func__ << " flag: " << flag_ << " done"<< std::endl;
    }

private:
    char    flag_;
};

static ::NsLib::NsLcok mutexForObject;
static ::NsLib::NsLcok mutexForMethod;

void threadFunC()
{
    SynchronizedObjectClass
        threadCObject{&mutexForObject, &mutexForMethod, 'C'};

    threadCObject.lockedMethod();
}

void threadFunD()
{
    SynchronizedObjectClass
        threadDObject{&mutexForObject, &mutexForMethod, 'D'};

    threadDObject.lockedMethod();
}

}

#endif

