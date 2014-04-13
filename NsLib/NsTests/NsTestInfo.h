#ifndef NS_TEST_INFO_H
#define	NS_TEST_INFO_H

#include <iostream>

#define NS_TEST_MESSAGE(msg) \
    std::cout << msg << std::endl
#define NS_TEST_MESSAGE_2(msg_1, msg_2) \
    std::cout << msg_1 << msg_2 << std::endl
#define NS_TEST_MESSAGE_3(msg_1, msg_2, msg_3) \
    std::cout << msg_1 << msg_2 << msg_3 << std::endl
#define NS_TEST_MESSAGE_4(msg_1, msg_2, msg_3, msg_4) \
    std::cout << msg_1 << msg_2 << msg_3 << msg_4 << std::endl
#define NS_TEST_MESSAGE_5(msg_1, msg_2, msg_3, msg_4, msg_5) \
    std::cout << msg_1 << msg_2 << msg_3 << msg_4 << msg_5 << std::endl

#endif

