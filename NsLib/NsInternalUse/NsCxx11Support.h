#ifndef NS_CXX11_SUPPORT_H
#define	NS_CXX11_SUPPORT_H

// 去除operator =()中的指针比较


//------------------------------------------------------------------------------
// 编译器版本支持

#ifdef __GNUC__
    #if (__GNUC__ < 4)
        #error Need GCC version at least 4.6
    #else   // (__GNUC__ < 4)
        #if (__GNUC__ == 4 && __GNUC_MINOR__ < 6)
            #error Need GCC version at least 4.6
        #endif
    #endif  // (__GNUC__ < 4)
#elif defined(_MSC_VER)
    #if _MSC_VER < 1600
        #error Need Microsoft Visual C++ version at least 10.0
    #endif
#else
    #error I have not yet port NsLib to your compiler yet, maybe support in next version
#endif  // __GNUC__


//------------------------------------------------------------------------------
// 编译器对C++11特性的支持情况

#ifdef __GNUC__
    // 编译器是否支持alignof关键字
    #define NS_KEYWORD_ALIGNOF_SUPPORT

    // 编译器是否支持Attributes
    //#define NS_ATTRIBUTES_SUPPORT

    // 编译器是否支持__func__
    #define NS___FUNC___SUPPORT

    // 编译器是否支持noexcept关键字
    #define NS_NOEXCEPT_SUPPORT


#elif defined(_MSC_VER)

#else
    #error I have not yet port NsLib to your compiler yet, maybe support in next version
#endif  // __GNUC__

//------------------------------------------------------------------------------





#include "NsCxx11BackwardCompatible.h"

#endif

