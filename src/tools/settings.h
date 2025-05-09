#include "../sensors/data.h"
#include "pico/flash.h"
#include "hardware/flash.h"

class FlightSettings
{
public:
    FlightSettings()
    {
        // Constructor CHANGE THIS VALUES
        main_deploy_height = 0;
        drouge_deploy_delay = 0;
        liftoff_threshold = 0;
    };
    ~FlightSettings() {
        // Destructor

    };
    void init();
    // add in read from memory
    void setFlightSettings(flash_internal_data setting)
    {
        this->main_deploy_height = setting.main_height;
        this->drouge_deploy_delay = setting.drouge_delay;
        this->liftoff_threshold = setting.liftoff_thresh;
    };
    int getMainDeployHeight() { return main_deploy_height; }
    int getDrougeDeployDelay() { return drouge_deploy_delay; }
    int getLiftoffThreshold() { return liftoff_threshold; }
    void setMainDeployHeight(int height) { main_deploy_height = height; }
    void setDrougeDeployDelay(int delay) { drouge_deploy_delay = delay; }
    void setLiftoffThreshold(int threshold) { liftoff_threshold = threshold; }

    void save_settings(flash_internal_data settings)
    {
        this->main_deploy_height = settings.main_height;
        this->drouge_deploy_delay = settings.drouge_delay;
        this->liftoff_threshold = settings.liftoff_thresh;
        write_setting();
    }

    flash_internal_data get_settings()
    {
        printf("Reading settings from flash memory...\n");
        // Implement the logic to read settings from flash memory
        // You can use flash_range_read to read the data from the specified offset
        // and store it in a buffer for later use.
        uint32_t offset = settings_sector;
        uint8_t buffer[FLASH_PAGE_SIZE];
        int *p, addr, value;
        int main, drouge, liftoff;
        int *main_addr, *drouge_addr, *liftoff_addr;

        // Compute the memory-mapped address, remembering to include the offset for RAM
        addr = XIP_BASE + settings_sector;
        main_addr = (int *)addr; // Place an int pointer at our memory-mapped address
        main = *main_addr;      // Store the value at this address into a variable for later use
        drouge_addr = (int *)(addr + 4);
        drouge = *drouge_addr;
        liftoff_addr = (int *)(addr + 8);
        liftoff = *liftoff_addr;

        flash_internal_data settings = {main, drouge, liftoff, 0};
        // Process the buffer to extract the settings
        // For example, you can convert the buffer data back to integers
        // int drouge_delay = *(int*)(buffer + 0);
        // int main_height = *(int*)(buffer + 4);
        // int liftoff_thresh = *(int*)(buffer + 8);
        // int last_log = *(int*)(buffer + 12);
        printf("Settings: Drouge Delay: %d, Main Height: %d, Liftoff Thresh: %d\n", settings.drouge_delay, settings.main_height, settings.liftoff_thresh);

       
        return settings;
    }

    // This function will be called when it's safe to call flash_range_program
    void write_setting()
    {

        int buf[FLASH_PAGE_SIZE / sizeof(int)];

        buf[0] = main_deploy_height;
        buf[1] = drouge_deploy_delay;
        buf[2] = liftoff_threshold;

        printf("writing: %d %d %d\n", buf[0], buf[1], buf[2]);
        printf("to address %d,\n", settings_sector);
        // Erase the last sector of the flash
        flash_range_erase((PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE), FLASH_SECTOR_SIZE);
        uint32_t ints = save_and_disable_interrupts();
        // Program buf[] into the first page of this sector
        flash_range_program(settings_sector, (uint8_t *)buf, FLASH_PAGE_SIZE);
        restore_interrupts(ints);
    }

private:
    int main_deploy_height;
    int drouge_deploy_delay;
    int liftoff_threshold;
    // last sector
    int const settings_sector{PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE};
};
