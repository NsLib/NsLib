#ifndef NS_LOG_BASE_H
#define NS_LOG_BASE_H

#include "../NsInternalUse/NsCxx11Support.h"

#include <iostream>

namespace NsLib
{

class LogBase
{
public:
    typedef ::NsLib::LogDeviceType::LogDeviceTypes LogDeviceType;

public:
    explicit LogBase(enum ::NsLib::LogDeviceType::LogDeviceTypes logDeviceType);

    virtual ~LogBase() = 0;

    // 初始化日志设备
    virtual bool setUp() = 0;
    // 清理日志设备
    virtual bool tearDown() = 0;

    void logDebug() = 0;

    void logMessage() = 0;

    void logError() = 0;

    void logFatal() = 0;

    // 刷新日志流
    virtual void flush() = 0;

    LogDeviceType   getLogDeviceType()
    {
        return deviceType_;
    }

private:
    LogDeviceType   deviceType_;
};




}

#endif


