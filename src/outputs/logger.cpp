#include "logger.h"

Logger::Logger() {
    #ifdef LOG_INTERNAL_FLASH
        printf("Logger created\n");
        logger = new FlashInternal();
    #endif
};

Logger::~Logger() {
    if (logger != nullptr) {
        delete logger;
        logger = nullptr;
    }

    printf("Logger destroyed\n");
}

void Logger::send(int* data){
    logger->send(data);
}
