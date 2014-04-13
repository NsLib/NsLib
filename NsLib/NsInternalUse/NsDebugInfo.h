#ifndef NS_DEBUG_INFO_H
#define	NS_DEBUG_INFO_H

#include "NsCxx11Support.h"

// 输出调试信息
//#define _NS_DEBUG_

// 用于调试内存, 追踪内存的状态
//#define _NS_DEBUG_TRACE_MEMORRY_

// 追踪多继承条件下INewFromObjectPool类调用new和delete是否正确
//#define _NS_DEBUG_TRACE_NEW_AND_DELETE_FROM_OBJECT_POOL_

//------------------------------------------------------------------------------
// 防止编译器对未使用参数发出的警告

#define NS_UNUSED_PARAM(param) \
    ((void)(param))
#define NS_UNUSED_PARAM_2(param_1, param_2) \
    (NS_UNUSED_PARAM(param_1) || NS_UNUSED_PARAM(param_2))
#define NS_UNUSED_PARAM_3(param_1, param_2, param_3) \
    (NS_UNUSED_PARAM(param_1), NS_UNUSED_PARAM_2(param_2, param_3))
#define NS_UNUSED_PARAM_4(param_1, param_2, param_3, param_4) \
    (NS_UNUSED_PARAM(param_1), NS_UNUSED_PARAM_3(param_2, param_3, param_4))
#define NS_UNUSED_PARAM_5(param_1, param_2, param_3, param_4, param_5) \
    (NS_UNUSED_PARAM(param_1), \
    NS_UNUSED_PARAM_4(param_2, param_3, param_4, param_5))

//------------------------------------------------------------------------------

#ifdef _NS_DEBUG_
#include <iostream>

#define NS_DEBUG_MESSAGE(msg) \
    std::cout << msg << std::endl
#define NS_DEBUG_MESSAGE_2(msg_1, msg_2) \
    std::cout << msg_1 << msg_2 << std::endl
#define NS_DEBUG_MESSAGE_3(msg_1, msg_2, msg_3) \
    std::cout << msg_1 << msg_2 << msg_3 << std::endl
#define NS_DEBUG_MESSAGE_4(msg_1, msg_2, msg_3, msg_4) \
    std::cout << msg_1 << msg_2 << msg_3 << msg_4 << std::endl
#define NS_DEBUG_MESSAGE_5(msg_1, msg_2, msg_3, msg_4, msg_5) \
    std::cout << msg_1 << msg_2 << msg_3 << msg_4 << msg_5 << std::endl
#endif  // _NS_DEBUG_

//------------------------------------------------------------------------------

#ifdef _NS_DEBUG_TRACE_MEMORRY_
#include <iostream>
#include <typeinfo>

#define NS_TRACE_MEMORY(className, typeInfo, classInfo, memPtr) \
    std::cout << className << typeInfo << classInfo \
              << " -- Memory Address: " << memPtr << std::endl
#endif  // _NS_DEBUG_TRACE_MEMORRY_

//------------------------------------------------------------------------------

#ifdef _NS_DEBUG_TRACE_NEW_AND_DELETE_FROM_OBJECT_POOL_
#include <iostream>
#include <typeinfo>

#define NS_TRACE_NEW_FROM_OBJECT_POOL(ClassName) \
    std::cout << typeid(ClassName).name() << " -- new" << std::endl
#define NS_TRACE_DELETE_FROM_OBJECT_POOL(ClassName) \
    std::cout << typeid(ClassName).name() << " -- delete" << std::endl  
#endif  // _NS_DEBUG_TRACE_NEW_AND_DELETE_FROM_OBJECT_POOL_





#endif

