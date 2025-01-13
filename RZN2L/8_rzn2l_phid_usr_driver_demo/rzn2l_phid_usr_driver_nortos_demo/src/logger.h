/*
 * logger.h
 *
 *
 *
 */

#ifndef APP_PUBLIC_LOGGER_H_
#define APP_PUBLIC_LOGGER_H_

#define __DEBUG    //日志模块总开关，注释掉将关闭日志输出

#ifdef __DEBUG
    #define DEBUG(format, ...) printf (format, ##__VA_ARGS__)
#else
    #define DEBUG(format, ...)
#endif

//定义日志级别
enum LOG_LEVEL {
    LOG_LEVEL_OFF=0,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_ALL,
};

#define log_fatal(level,format, ...) \
    do { \
         if(level>=LOG_LEVEL_FATAL)\
           DEBUG("\n->FATAL @ FUNC:%s FILE:%s LINE:%d \n" format "\n",\
                     __func__, __FILE__, __LINE__, ##__VA_ARGS__ );\
    } while (0)

#define log_err(level,format, ...) \
    do { \
         if(level>=LOG_LEVEL_ERR)\
           DEBUG("\n->ERR   @ FUNC:%s FILE:%s LINE:%d \n" format "\n",\
                     __func__, __FILE__, __LINE__, ##__VA_ARGS__ );\
    } while (0)

#define log_warn(level,format, ...) \
    do { \
         if(level>=LOG_LEVEL_WARN)\
           DEBUG("\n->WARN  @ FUNC:%s \n" format "\n",__func__, ##__VA_ARGS__ );\
    } while (0)

#define log_info(level,format, ...) \
    do { \
         if(level>=LOG_LEVEL_INFO)\
           DEBUG("\n->INFO  \n"format"\n",##__VA_ARGS__ );\
    } while (0)

#define log_debug(level,format, ...) \
    do { \
         if(level>=LOG_LEVEL_ALL)\
           DEBUG("\n->DEBUG \n"format"\n",##__VA_ARGS__ );\
    } while (0)

#endif /* APP_PUBLIC_LOGGER_H_ */
