include "sensor.h"

class Barometer : public Sensor{
    public:
        Barometer();
        ~Barometer();
        void start();
        void stop();
        int get_data();
    private:
        void process();
        void read();
};