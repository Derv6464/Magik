struct flight_data
{
    int altitude;
    int velocity;
    accle_data acceleration;
    int temperature;
    bool setting_pin;
};

struct accle_data
{
    int x;
    int y;
    int z;
};

