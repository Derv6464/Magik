

namespace logger{
    void log(const char* message){
        printf("%s\n", message);
    }
    void log(int message){
        printf("%d\n", message);
    }
    void log(float message){
        printf("%f\n", message);
    }
    void log(uint32_t message){
        printf("%d\n", message);
    }
    void log(uint16_t message){
        printf("%d\n", message);
    }
    void log(uint8_t message){
        printf("%d\n", message);
    }
    void log(bool message){
        printf("%d\n", message);
    }
    void log(Status_led::Color message){
        printf("%d\n", message);
    }
}