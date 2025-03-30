class SPI{
    public:
        SPI(int sck, int mosi, int miso){ 
            this->sck = sck; 
            this->mosi = mosi; 
            this->miso = miso; 
        };
        void write(int data);
        int read();

    private:
        int sck;
        int mosi;
        int miso;
};

class I2C{
    public:
        I2C(int sda, int scl, int address);
        ~I2C();
        void write(int data);
        int read();
};