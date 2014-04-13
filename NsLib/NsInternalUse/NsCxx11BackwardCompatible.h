#ifndef NS_CXX11_BACKWARD_COMPATIBLE_H
#define	NS_CXX11_BACKWARD_COMPATIBLE_H

//------------------------------------------------------------------------------
// 编译器是否支持alignof关键字

#ifdef NS_KEYWORD_ALIGNOF_SUPPORT
    #define NS_ALIGNOF  alignof
#else
        #if defined(_MSC_VER)
            #define NS_ALIGNOF    __alignof
        #else
            #error Need support keyword alignof or __alignof
        #endif
#endif

//------------------------------------------------------------------------------
// 编译器是否支持Attributes

#ifdef NS_ATTRIBUTES_SUPPORT
    #define NS_ATTR_FINAL       [[final]]
    #define NS_ATTR_OVERRIDE    [[override]]
    #define NS_ATTR_NORETURN    [[noreturn]]
#else
    #define NS_ATTR_FINAL
    #define NS_ATTR_OVERRIDE
    #define NS_ATTR_NORETURN
#endif

//------------------------------------------------------------------------------
// 编译器是否支持__func__

#ifdef NS___FUNC___SUPPORT
    #define NS___FUNC___    __func__
#else
    #if defined(_MSC_VER)
        #define NS___FUNC___    __FUNCTION__
    #else
        #define NS___FUNC___    "Need C99 Preprocessor __func__ support"
    #endif
#endif

//------------------------------------------------------------------------------
// 编译器是否支持noexcept关键字

#ifdef NS_NOEXCEPT_SUPPORT
    #define NS_NOEXCEPT     noexcept
#else
    #define NS_NOEXCEPT     throw ()
#endif

//------------------------------------------------------------------------------
//

//------------------------------------------------------------------------------
//







#endif

