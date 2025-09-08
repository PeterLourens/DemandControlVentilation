#include "i2c.h"

LiquidCrystal_I2C lcd(0x27, LCD_COLUMNS, LCD_ROWS);
RTC_DS3231 rtc;

void read_sensors(void)
{
    float temp_sensor_data[SENSOR_I2C_BUSSES][SENSOR_COUNT][SENSOR_DATA_FIELDS] = {0};

    int bus0_multiplexer_addr = 0;
    int bus1_multiplexer_addr = 0;
    char msg[MSG_SIZE] = {};

    String sensor_type = "";
    String sensor = "";

    // Read address for TCA9548. I2C address for TCA9548 may be differently configured with resistors on the board.
    if (settings_i2c_mutex && xSemaphoreTake(settings_i2c_mutex, (TickType_t)10) == pdTRUE)
    {
        bus0_multiplexer_addr = i2csettings.bus0_multiplexer_address;
        bus1_multiplexer_addr = i2csettings.bus1_multiplexer_address;
        xSemaphoreGive(settings_i2c_mutex);
    }

    for (int bus = 0; bus < 2; bus++)
    {
        if (bus == 0)
        {
            Wire.begin(I2C_SDA1, I2C_SCL1, 100000);
        }
        if (bus == 1)
        {
            Wire1.begin(I2C_SDA2, I2C_SCL2, 100000);
        }

        for (int slot = 0; slot < 8; slot++)
        {
            if (bus == 0)
            {
                if (settings_sensor1_mutex && xSemaphoreTake(settings_sensor1_mutex, (TickType_t)10) == pdTRUE)
                {
                    sensor_type = String(sensor1settings[slot].wire_sensor_type);
                    xSemaphoreGive(settings_sensor1_mutex);
                }
                Wire.beginTransmission(bus0_multiplexer_addr);
                Wire.write(1 << slot);
                Wire.endTransmission();
            }
            if (bus == 1)
            {
                if (settings_sensor2_mutex && xSemaphoreTake(settings_sensor2_mutex, (TickType_t)100) == pdTRUE)
                {
                    sensor_type = String(sensor2settings[slot].wire1_sensor_type);
                    xSemaphoreGive(settings_sensor2_mutex);
                }
                Wire1.beginTransmission(bus1_multiplexer_addr);
                Wire1.write(1 << slot);
                Wire1.endTransmission();
            }
            if (sensor_type == "DHT20" || sensor_type == "AHT20")
            {
                if (bus == 0)
                {
                    DHT20 DHT1(&Wire);
                    DHT1.begin();
                    DHT1.read();

                    temp_sensor_data[bus][slot][0] = DHT1.getTemperature();
                    temp_sensor_data[bus][slot][1] = DHT1.getHumidity();
                    Wire.endTransmission();
                }
                if (bus == 1)
                {
                    DHT20 DHT2(&Wire1);
                    DHT2.begin();
                    DHT2.read();

                    temp_sensor_data[bus][slot][0] = DHT2.getTemperature();
                    temp_sensor_data[bus][slot][1] = DHT2.getHumidity();
                    Wire1.endTransmission();
                }
            }

            /*
            else if (sensor_type_temp == "AHT20") {
                if (bus==0) {

                    Adafruit_AHTX0 AHT20_1;
                    sensors_event_t humidity, temperature;

                    AHT20_1.begin();
                    AHT20_1.getEvent(&humidity, &temperature);

                    temp_sensor_data[bus][slot][0] = temperature.temperature;
                    temp_sensor_data[bus][slot][1] = humidity.relative_humidity;

                    Wire.endTransmission();

                    DFRobot_AHT20 AHT20_1;
                    AHT20_1.begin();
                    temp_sensor_data[bus][slot][0] = AHT20_1.getTemperature_C();
                    temp_sensor_data[bus][slot][1] = AHT20_1.getHumidity_RH();
                    Wire.endTransmission();

                }
                if (bus==1) {
                    Adafruit_AHTX0 AHT20_2;
                    sensors_event_t humidity, temperature;

                    AHT20_2.begin();
                    AHT20_2.getEvent(&humidity, &temperature);

                    temp_sensor_data[bus][slot][0] = temperature.temperature;
                    temp_sensor_data[bus][slot][1] = humidity.relative_humidity;

                    Wire1.endTransmission();

                    DFRobot_AHT20 AHT20_2;
                    AHT20_2.begin();
                    temp_sensor_data[bus][slot][0] = AHT20_2.getTemperature_C();
                    temp_sensor_data[bus][slot][1] = AHT20_2.getHumidity_RH();
                    Wire.endTransmission();
                }
            }*/

            else if (sensor_type == "SCD40" || sensor_type == "SCD41")
            {
                if (bus == 0)
                {
                    SensirionI2cScd4x SCD4X_1;
                    SCD4X_1.begin(Wire, SCD41_I2C_ADDR_62);
                    SCD4X_1.startPeriodicMeasurement();

                    uint16_t error;
                    uint16_t co2 = 0;
                    float temperature = 0.0f;
                    float humidity = 0.0f;

                    error = SCD4X_1.readMeasurement(co2, temperature, humidity);
                    if (error)
                    {
                        snprintf(msg, sizeof(msg), "[Error] Failed to execute readMeasurement().");
                        printmessage(msg);
                    }
                    else if (co2 == 0)
                    {
                        snprintf(msg, sizeof(msg), "[Error] Invalid sample detected, skipping.");
                        printmessage(msg);
                    }
                    else
                    {
                        temp_sensor_data[bus][slot][0] = temperature;
                        temp_sensor_data[bus][slot][1] = humidity;
                        temp_sensor_data[bus][slot][2] = co2;
                    }
                    Wire.endTransmission();
                }
                if (bus == 1)
                {
                    SensirionI2cScd4x SCD4X_2;
                    SCD4X_2.begin(Wire1, SCD41_I2C_ADDR_62);
                    SCD4X_2.startPeriodicMeasurement();

                    uint16_t error;
                    uint16_t co2 = 0;
                    float temperature = 0.0f;
                    float humidity = 0.0f;

                    error = SCD4X_2.readMeasurement(co2, temperature, humidity);
                    if (error)
                    {
                        snprintf(msg, sizeof(msg), "[Error] Failed to execute readMeasurement().");
                        printmessage(msg);
                    }
                    else if (co2 == 0)
                    {
                        snprintf(msg, sizeof(msg), "[Error] Invalid sample detected, skipping.");
                        printmessage(msg);
                    }
                    else
                    {
                        temp_sensor_data[bus][slot][0] = temperature;
                        temp_sensor_data[bus][slot][1] = humidity;
                        temp_sensor_data[bus][slot][2] = co2;
                    }
                    Wire1.endTransmission();
                }
            }
            else
            {
                temp_sensor_data[bus][slot][0] = 0.00;
                temp_sensor_data[bus][slot][1] = 0.00;
                temp_sensor_data[bus][slot][2] = 0.00;
            }
        }
    }

    vTaskDelay(200);

    // check if transmission is properly done, if not
    byte wire_status;
    byte wire1_status;

    wire_status = Wire.endTransmission();
    wire1_status = Wire1.endTransmission();

    if (wire_status == 0x00 && wire1_status == 0x00)
    {
        if (sensor_queue != NULL)
        {
            if (xQueueOverwrite(sensor_queue, &temp_sensor_data) != pdPASS)
            {
                snprintf(msg, sizeof(msg), "[Error] Failed to send data to queue.");
                printmessage(msg);
            }
        }
        else
        {
            snprintf(msg, sizeof(msg), "[Error] Send data to queue - Queue handle is NULL");
            printmessage(msg);
        }
    }
    else
    {
        snprintf(msg, sizeof(msg), "[Error] I2C communication problem.");
        printmessage(msg);
    }
}

void display_sensors(void)
{

    /*
        0 	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15  16  17  18  19
       -------------------------------------------------------------------------------
    0 |	B	u	s	:	0		S	e	n	s	o	r	:	1	1
    1 |	V   :	1   1       N   :   B   e   d   r   o   o   m   1
    2 |	T	:	2	2	,	8	7	°	C		H	:	4	7   .   9   9	%
    3 |	C	O	2	:	1	2	0	0   .   0   0	p	p	m
    */

    float queue_sensor_data[SENSOR_I2C_BUSSES][SENSOR_COUNT][SENSOR_DATA_FIELDS]; // local variable to store sensor data from queue
    String valve;
    String location;
    String rh;
    String co2;

    Wire1.begin(I2C_SDA2, I2C_SCL2, 100000); // Display is on Wire1 bus
    lcd.init();
    lcd.backlight();
    lcd.noAutoscroll();
    lcd.noCursor();

    if (xQueuePeek(sensor_queue, &queue_sensor_data, 0) == pdTRUE)
    {
        for (int bus = 0; bus < 2; bus++)
        {
            for (int slot = 0; slot < 8; slot++)
            {
                // Only display measurements if sensor is present, i.e. if temperature measurement is not zero
                if (queue_sensor_data[bus][slot][0] != 0)
                {
                    if (bus == 0)
                    {
                        if (settings_sensor1_mutex != NULL)
                        {
                            if (xSemaphoreTake(settings_sensor1_mutex, (TickType_t)10) == pdTRUE)
                            {
                                valve = sensor1settings[slot].wire_sensor_valve;
                                location = sensor1settings[slot].wire_sensor_location;
                                rh = sensor1settings[slot].wire_sensor_rh;
                                co2 = sensor1settings[slot].wire_sensor_co2;
                                xSemaphoreGive(settings_sensor1_mutex);
                            }
                        }
                    }
                    if (bus == 1)
                    {
                        if (settings_sensor1_mutex != NULL)
                        {
                            if (xSemaphoreTake(settings_sensor2_mutex, (TickType_t)10) == pdTRUE)
                            {
                                valve = sensor2settings[slot].wire1_sensor_valve;
                                location = sensor2settings[slot].wire1_sensor_location;
                                rh = sensor2settings[slot].wire1_sensor_rh;
                                co2 = sensor2settings[slot].wire1_sensor_co2;
                                xSemaphoreGive(settings_sensor2_mutex);
                            }
                        }
                    }

                    // row0
                    lcd.setCursor(0, 0);
                    lcd.print("B");
                    lcd.print(bus);
                    lcd.print("-");
                    lcd.print("S");
                    lcd.print(slot);
                    lcd.print(" RH:");
                    lcd.print(rh);
                    lcd.print(" CO2:");
                    lcd.print(co2);

                    // row1
                    lcd.setCursor(0, 1);
                    lcd.print(valve);
                    lcd.print(" ");
                    lcd.print(location);

                    // row2
                    lcd.setCursor(0, 2);
                    lcd.print("T:");
                    lcd.print(queue_sensor_data[bus][slot][0]);
                    lcd.print((char)223);
                    lcd.print("C");

                    lcd.setCursor(10, 2);
                    lcd.print("H:");
                    lcd.print(queue_sensor_data[bus][slot][1]);
                    lcd.print("%");

                    // row3 - Only display CO2 is sensor has this measurement
                    if (queue_sensor_data[bus][slot][2] != 0)
                    {
                        lcd.setCursor(0, 3);
                        lcd.print("CO2:");
                        lcd.print(queue_sensor_data[bus][slot][2]);
                        lcd.print("ppm");
                    }
                    // Refresh sensor data every 5 seconds and clear display
                    vTaskDelay(5000);
                    lcd.clear();
                }
                // No sensor connected to port
                /*else {
                    lcd.setCursor(0, 2);
                    lcd.print("No sensor");
                    vTaskDelay(5000);
                    lcd.clear();
                }*/
            }
        }
    }
    Wire1.endTransmission();
}

void display_valve_positions(void)
{
    /* Display layout

            0 	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15  16  17  18  19  20
            -----------------------------------------------------------------------------------
    0   |	v   0	:	i	i		    v	1	:	i	i			v   2	:	i	i
    1   |	v   3	:	i	i		    v	4	:	i	i			v   5	:	i	i
    2   |	v   6	:	i   i		    v	7	:	i	i			v   8	:	i	i
    3   |	v   9	:	i   i		v   1	0	:	i	i		v   1   1	:	i	i
    */

    bool status_file_present = 0;
    char buffer[512] = {};
    char msg[MSG_SIZE] = {};
    String json = "";
    JsonDocument doc;

    Wire1.begin(I2C_SDA2, I2C_SCL2, 100000); // Display is on Wire1 bus
    lcd.init();
    lcd.backlight();

    if (read_settings(VALVE_POSITIONS_PATH, buffer, sizeof(buffer), valve_position_file_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[Error] Failed to parse %s with error %s", VALVE_POSITIONS_PATH, error);
            printmessage(msg);
        }
    }

    int valve0_pos = doc["valve0"];
    int valve1_pos = doc["valve1"];
    int valve2_pos = doc["valve2"];
    int valve3_pos = doc["valve3"];
    int valve4_pos = doc["valve4"];
    int valve5_pos = doc["valve5"];
    int valve6_pos = doc["valve6"];
    int valve7_pos = doc["valve7"];
    int valve8_pos = doc["valve8"];
    int valve9_pos = doc["valve9"];
    int valve10_pos = doc["valve10"];
    int valve11_pos = doc["valve11"];

    lcd.setCursor(0, 0);
    lcd.print("v0:");
    lcd.print(valve0_pos);
    lcd.setCursor(7, 0);
    lcd.print("v1:");
    lcd.print(valve1_pos);
    lcd.setCursor(14, 0);
    lcd.print("v2:");
    lcd.print(valve2_pos);

    lcd.setCursor(0, 1);
    lcd.print("v3:");
    lcd.print(valve3_pos);
    lcd.setCursor(7, 1);
    lcd.print("v4:");
    lcd.print(valve4_pos);
    lcd.setCursor(14, 1);
    lcd.print("v5:");
    lcd.print(valve5_pos);

    lcd.setCursor(0, 2);
    lcd.print("v6:");
    lcd.print(valve6_pos);
    lcd.setCursor(7, 2);
    lcd.print("v7:");
    lcd.print(valve7_pos);
    lcd.setCursor(14, 2);
    lcd.print("v8:");
    lcd.print(valve8_pos);

    lcd.setCursor(0, 3);
    lcd.print("v9:");
    lcd.print(valve9_pos);
    lcd.setCursor(6, 3);
    lcd.print("v10:");
    lcd.print(valve10_pos);
    lcd.setCursor(13, 3);
    lcd.print("v11:");
    lcd.print(valve11_pos);
    vTaskDelay(5000);
    lcd.clear();
    Wire1.endTransmission();
}

void display_time_and_date(void)
{
    /*
           0 	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15  16  17  18  19
          -------------------------------------------------------------------------------
       0 |	W   e   d   n   e   s   d   a   y       3   1   -   1   2   -   2   0   2   5
       1 |	2   3   :   5   9   :   0   0
       2 |	U   p   t   i   m   e   :       9   9   9   9   9   9       m   i   n
       3 |	I   P   :   x   x   x   .   x   x   x   .   x   x   x   .   x   x   x
   */

    int64_t uptime = 0;

    char day_buffer[10];
    char date_buffer[20];
    char time_buffer[20];
    char uptime_buffer[50];
    char ip_buffer[20];

    Wire1.begin(I2C_SDA2, I2C_SCL2, 100000); // Display is on Wire1 bus
    lcd.init();
    lcd.backlight();

    formatted_day(day_buffer, sizeof(day_buffer));
    formatted_date(date_buffer, sizeof(date_buffer));
    formatted_time(time_buffer, sizeof(time_buffer));
    formatted_uptime(uptime_buffer, sizeof(uptime_buffer));
    ip_address(ip_buffer, sizeof(ip_buffer));

    lcd.setCursor(0, 0);
    lcd.print(String(day_buffer) + " " + String(time_buffer));

    lcd.setCursor(0, 1);
    lcd.print(String(date_buffer));

    uptime = esp_timer_get_time();
    lcd.setCursor(0, 2);
    lcd.print("Up: " + String(uptime_buffer));

    lcd.setCursor(0, 3);
    lcd.print("IP: " + String(ip_buffer));

    vTaskDelay(5000);
    lcd.clear();

    Wire1.endTransmission();
}

void display_state_fan(void)
{
    /*
           0 	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15  16  17  18  19
          -------------------------------------------------------------------------------
       0 |	S   t   a   t   e   :       f   a   n   h   i   g   h   s   p   e   e   d
       1 |	F   a   n   s   p   e   e   d   :   M   e   d   i   u   m
       2 |
       3 |
   */

    String temp_state = "";
    String temp_fanspeed = "";

    Wire1.begin(I2C_SDA2, I2C_SCL2, 100000); // Display is on Wire1 bus
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("State: ");

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        temp_state = state;
        xSemaphoreGive(statemachine_state_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        temp_fanspeed = fanspeed;
        xSemaphoreGive(fanspeed_mutex);
    }

    lcd.print(state);
    lcd.setCursor(0, 1);
    lcd.print("Fan speed: ");
    lcd.print(fanspeed);

    lcd.setCursor(0, 2);
    lcd.print("CO2 sensors high:");
    lcd.print(parse_co2_sensors_high());

    lcd.setCursor(0, 3);
    lcd.print("RH sensors high:");
    lcd.print(parse_rh_sensors_high());

    vTaskDelay(5000);
    lcd.clear();
    Wire1.endTransmission();
}

void current_time(void)
{
    char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    Wire.begin(I2C_SDA1, I2C_SCL1, 100000);
    rtc.begin(&Wire);

    DateTime now = rtc.now();

    if (date_time_mutex && xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
    {
        rtcdatetime.year = now.year();
        rtcdatetime.month = now.month();
        rtcdatetime.day = now.day();
        rtcdatetime.hour = now.hour();
        rtcdatetime.minute = now.minute();
        rtcdatetime.second = now.second();
        rtcdatetime.day_of_week = now.dayOfTheWeek();
        xSemaphoreGive(date_time_mutex);
    }
    Wire.endTransmission();
}

void sync_rtc_ntp(void)
{
    char ntp_server[LARGE_CONFIG_ITEM] = {};
    char timezone[LARGE_CONFIG_ITEM] = {};
    char msg[MSG_SIZE] = {};

    struct tm timeinfo;

    RTC_DS3231 rtc;

    Wire.begin(I2C_SDA1, I2C_SCL1, 100000);
    rtc.begin(&Wire);

    // Read ntp server and timezone settings from config file
    if (settings_rtc_mutex && xSemaphoreTake(settings_rtc_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(ntp_server, rtcsettings.ntp_server, sizeof(ntp_server) - 1);
        ntp_server[sizeof(ntp_server) - 1] = '\0';

        strncpy(timezone, rtcsettings.timezone, sizeof(timezone) - 1);
        timezone[sizeof(timezone) - 1] = '\0';
        xSemaphoreGive(settings_rtc_mutex);
    }

    // configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "pool.ntp.org");
    configTzTime(timezone, ntp_server);
    if (!getLocalTime(&timeinfo))
    {
        snprintf(msg, sizeof(msg), "[ERROR] Failed to obtain time.");
        printmessage(msg);
        return;
    }
    else
    {
        snprintf(msg, sizeof(msg), "[INFO] ESP32 Time synchronized with NTP server.");
        printmessage(msg);
    }

    // Sync the RTC with the NTP time
    rtc.adjust(DateTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec));
    Wire.endTransmission();
}

void IRAM_ATTR lcd_baclight_pb_isr()
{
    pb_toggle = true;
}

void pb_start_display(void)
{
    int display_i2c_addr = 0;
    char enable_lcd[SMALL_CONFIG_ITEM] = {};
    char msg[MSG_SIZE] = {};

    pb_toggle = false; // global variable

    // Only start display when enabled. Configured with global variable
    if (settings_i2c_mutex && xSemaphoreTake(settings_i2c_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_lcd, i2csettings.enable_lcd, sizeof(enable_lcd) - 1);
        enable_lcd[sizeof(enable_lcd) - 1] = '\0';

        display_i2c_addr = i2csettings.display_i2c_address;
        xSemaphoreGive(settings_i2c_mutex);
    }

    if (strcmp(enable_lcd, "On") == 0)
    {
        display_time_and_date();
        display_state_fan();
        display_sensors();
        display_valve_positions();

        // LiquidCrystal_I2C lcd(display_i2c_addr_tmp, LCD_COLUMNS, LCD_ROWS);
        Wire1.begin(I2C_SDA2, I2C_SCL2, 100000);
        lcd.noBacklight();
        Wire1.endTransmission();
    }
    else
    {
        snprintf(msg, sizeof(msg), "[INFO] Display is not enabled in settings.");
        printmessage(msg);
    }
}

void init_display_off(void)
{
    char enable_lcd[SMALL_CONFIG_ITEM] = {};
    char msg[MSG_SIZE] = {};

    // Only start display when enabled.
    if (settings_i2c_mutex && xSemaphoreTake(settings_i2c_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_lcd, i2csettings.enable_lcd, sizeof(enable_lcd) - 1);
        enable_lcd[sizeof(enable_lcd) - 1] = '\0';
        xSemaphoreGive(settings_i2c_mutex);
    }

    if (strcmp(enable_lcd, "On") == 0)
    {
        Wire1.begin(I2C_SDA2, I2C_SCL2, 100000);
        lcd.init();
        lcd.clear();
        lcd.noBacklight();
        Wire1.endTransmission();
    }
    else
    {
        snprintf(msg, sizeof(msg), "[INFO] Display is not enabled in settings.");
        printmessage(msg);
    }
}
