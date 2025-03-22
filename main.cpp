#include <cstdio>
#include <string>
#include <sstream>
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "cJSON.h"

#define GNSS_ADDR 0x20

int main() {
    stdio_init_all();
    i2c_init(i2c0, 115200);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);

    uint8_t data[50] = {0};
    uint8_t addr = 0;

    while (true) {
        i2c_write_blocking(i2c0, GNSS_ADDR, &addr, 1, true);
        i2c_read_blocking(i2c0, GNSS_ADDR, data, 50, false);

        std::stringstream date;
        std::stringstream lat;
        std::stringstream lon;

        date << (data[0] << 8 | data[1]) << "-" << static_cast<int> (data[2])
        << "-" << static_cast<int> (data[3]) << " " << static_cast<int> (data[4]) << ":" << static_cast<int> (data[5]) << ":"
        << static_cast<int> (data[6]);

        lat << static_cast<int> (data[7]) << "." << (data[8] + (data[9] << 16 | data[10] << 8 | data[11])) / 60
        << " " << data[12];

        lon << static_cast<int> (data[13]) << "." << (data[14] + (data[15] << 16 | data[16] << 8 | data[17])) / 60
        << " " << data[18];

        std::string dates = date.str();
        std::string lats = lat.str();
        std::string lons = lon.str();


        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "date", dates.c_str());
        cJSON_AddStringToObject(root, "lat", lats.c_str());
        cJSON_AddStringToObject(root, "lon", lons.c_str());

        // TÄSSÄ ON BACKENDILLE JSON
        char *jsonstr = cJSON_PrintUnformatted(root);
        printf("%s", jsonstr);

        sleep_ms(5000);
    }
}