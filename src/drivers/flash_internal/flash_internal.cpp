#include "flash_internal.h"

FlashInternal::FlashInternal() {
    printf("Flash Logger created\n");
};

void FlashInternal::send(int* data) {
    //fill temp_storage in 10byte??? incremt, have to write to whole sector at once
    //to add: on state = landing flash too

    int flight_data_size = 128;
    //for (int i = 0; i < flight_data_size; i++){
    //    printf("%d", data[i]);
    //    temp_storage[temp_fill_level+i] = data[i];
    //}
    //printf("\ntemp fill %d", temp_fill_level);
    temp_fill_level += 45;

    if (temp_fill_level > 3872) {
        printf("flashing now");
        int rc = flash_safe_execute(call_flash_range_erase, (void*)sector, UINT32_MAX);
        hard_assert(rc == PICO_OK);

        printf("\nProgramming target region...\n");
        uintptr_t params[] = { sector, (uintptr_t)temp_storage};
        rc = flash_safe_execute(call_flash_range_program, params, UINT32_MAX);
        hard_assert(rc == PICO_OK);

        temp_fill_level = 0;
        sector += 1024;
    }
};

void FlashInternal::call_flash_range_erase(void *param) {
    uint32_t offset = (uint32_t)param;
    flash_range_erase(offset, FLASH_SECTOR_SIZE);
}

void FlashInternal::call_flash_range_program(void *param) {
    uint32_t offset = ((uintptr_t*)param)[0];
    const uint8_t *data = (const uint8_t *)((uintptr_t*)param)[1];
    flash_range_program(offset, data, FLASH_PAGE_SIZE);
}
