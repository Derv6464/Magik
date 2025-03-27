class Sensor{
    public:
        Sensor();
        ~Sensor();
        virtual void start();
        virtual void stop();
        virtual int get_data();
    private:
        virtual void process() = 0;
        virtual void read() = 0;
};