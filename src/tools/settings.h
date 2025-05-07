

class FlightSettings {
    public:
        FlightSettings();
        ~FlightSettings();
        void init();
        //add in read from memory
        void setFlightSettings(int main_deploy_height, int drouge_deploy_delay, int liftoff_threshold){
            this->main_deploy_height = main_deploy_height;
            this->drouge_deploy_delay = drouge_deploy_delay;
            this->liftoff_threshold = liftoff_threshold;
        };
        int getMainDeployHeight() { return main_deploy_height; }
        int getDrougeDeployDelay() { return drouge_deploy_delay; }
        int getLiftoffThreshold() { return liftoff_threshold; }
        void setMainDeployHeight(int height) { main_deploy_height = height; }
        void setDrougeDeployDelay(int delay) { drouge_deploy_delay = delay; }
        void setLiftoffThreshold(int threshold) { liftoff_threshold = threshold; }

    private:
        int main_deploy_height;
        int drouge_deploy_delay;
        int liftoff_threshold;
};
