#ifndef NS_LOG_DEVICE_TYPE_H
#define	NS_LOG_DEVICE_TYPE_H

namespace NsLib
{

namespace LogDeviceType
{

enum LogDeviceTypes
{
    StdOut      = 0,
    StdErr      = 5,
    Console     = 10,
    File        = 15,
    // 未完成
    //Gui         = 20,
    //Socket      = 25,
    //System      = 30
};

}   // LogDeviceType

}   // NsLib

#endif

