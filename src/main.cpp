#include "main.h"

// Variables
// const char* ssid = ""; //WIFI SSID
// const char* password = ""; //WIFI password

void setup()
{
	float temp[SENSOR_I2C_BUSSES][SENSOR_COUNT][SENSOR_DATA_FIELDS];
	char txBuffer[200] = {};
	char msg[MSG_SIZE] = {};

	Serial.begin(115200);

	pinMode(LATCHPIN1, OUTPUT);
	pinMode(CLOCKPIN1, OUTPUT);
	pinMode(DATAPIN1, OUTPUT);

	pinMode(LATCHPIN2, OUTPUT);
	pinMode(CLOCKPIN2, OUTPUT);
	pinMode(DATAPIN2, OUTPUT);

	// File mutexes
	sensor_config_file_mutex = xSemaphoreCreateMutex();
	valve_position_file_mutex = xSemaphoreCreateMutex();
	settings_files_mutex = xSemaphoreCreateMutex();

	// Settings mutexes
	settings_sensor1_mutex = xSemaphoreCreateMutex();
	settings_sensor2_mutex = xSemaphoreCreateMutex();
	settings_network_mutex = xSemaphoreCreateMutex();
	settings_mqtt_mutex = xSemaphoreCreateMutex();
	settings_i2c_mutex = xSemaphoreCreateMutex();
	settings_fan_mutex = xSemaphoreCreateMutex();
	settings_influxdb_mutex = xSemaphoreCreateMutex();
	settings_rtc_mutex = xSemaphoreCreateMutex();

	settings_statemachine_mutex = xSemaphoreCreateMutex();
	settings_state_day_mutex = xSemaphoreCreateMutex();
	settings_state_night_mutex = xSemaphoreCreateMutex();
	settings_state_highco2day_mutex = xSemaphoreCreateMutex();
	settings_state_highco2night_mutex = xSemaphoreCreateMutex();
	settings_state_highrhday_mutex = xSemaphoreCreateMutex();
	settings_state_highrhnight_mutex = xSemaphoreCreateMutex();
	settings_state_cooking_mutex = xSemaphoreCreateMutex();
	settings_state_cyclingday_mutex = xSemaphoreCreateMutex();
	settings_state_cyclingnight_mutex = xSemaphoreCreateMutex();

	// Global data mutexes
	date_time_mutex = xSemaphoreCreateMutex();
	statemachine_state_mutex = xSemaphoreCreateMutex();
	valve_control_data_mutex = xSemaphoreCreateMutex();
	fanspeed_mutex = xSemaphoreCreateMutex();
	lock_valve_move_mutex = xSemaphoreCreateMutex();
	ap_active_mutex = xSemaphoreCreateMutex();

	// Init queues for sensors and Webserial
	sensor_queue = xQueueCreate(1, sizeof(temp));
	sensor_avg_queue = xQueueCreate(1, sizeof(temp));
	webserial_queue = xQueueCreate(40, sizeof(txBuffer));

	if (webserial_queue == 0 || sensor_queue == 0 || sensor_avg_queue == 0)
	{
		snprintf(msg, sizeof(msg), "Failed to create queues");
		printmessage(LOG_ERROR, msg);
	}

	// First switch off all outputs which may have come randomly at power on
	init_registers();

	// Begin LittleFS
	if (!LittleFS.begin())
	{
		snprintf(msg, sizeof(msg), "An Error has occurred while mounting LittleFS");
		printmessage(LOG_ERROR, msg);
		return;
	}

	// setup_wifi();

	// New config
	// vTaskDelay(10000); // So can lauch serial monitor
	if (!parse_i2c_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse I2C settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed I2C settings");
		printmessage(LOG_INFO, msg);
	}

	if (!parse_rtc_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse RTC settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed RTC settings");
		printmessage(LOG_INFO, msg);
	}

	if (!parse_fan_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse fan settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed fan settings");
		printmessage(LOG_INFO, msg);
	}

	if (!parse_influxdb_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse Influxdb settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed Influxdb settings");
		printmessage(LOG_INFO, msg);
	}

	if (!parse_mqtt_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse MQTT settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed MQTT settings");
		printmessage(LOG_INFO, msg);
	}

	if (!parse_network_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse network settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed network settings");
		printmessage(LOG_INFO, msg);
	}

	if (!parse_sensor1_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse sensor1 settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed sensor1 settings");
		printmessage(LOG_INFO, msg);
	}

	if (!parse_sensor2_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse sensor2 settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed sensor2 settings");
		printmessage(LOG_INFO, msg);
	}

	if (!parse_statemachine_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse statemachine settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed statemachine settings");
		printmessage(LOG_INFO, msg);
	}
	if (!parse_state_day_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse state day settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed state day settings");
		printmessage(LOG_INFO, msg);
	}
	if (!parse_state_night_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse state night settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed state night settings");
		printmessage(LOG_INFO, msg);
	}

	if (!parse_state_highco2day_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse state highco2day settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed state highco2day settings");
		printmessage(LOG_INFO, msg);
	}

	if (!parse_state_highco2night_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse state highco2night settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed state highco2night settings");
		printmessage(LOG_INFO, msg);
	}

	if (!parse_state_highrhday_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse state highrhday settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed state highrhday settings");
		printmessage(LOG_INFO, msg);
	}

	if (!parse_state_highrhnight_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse state highrhnight settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed state highrhnight settings");
		printmessage(LOG_INFO, msg);
	}

	if (!parse_state_cooking_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse state cooking settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed state cooking settings");
		printmessage(LOG_INFO, msg);
	}

	if (!parse_state_cyclingday_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse state cyclingday settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed state cyclingday settings");
		printmessage(LOG_INFO, msg);
	}

	if (!parse_state_cyclingnight_settings())
	{
		snprintf(msg, sizeof(msg), "Failed to parse state cyclingnight settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed state cyclingnight settings");
		printmessage(LOG_INFO, msg);
	}

	if (!parse_actual_valve_positions())
	{
		snprintf(msg, sizeof(msg), "Failed to parse actual valve settings");
		printmessage(LOG_ERROR, msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Successfully parsed actual valve settings");
		printmessage(LOG_INFO, msg);
	}

	// parse_state_temp_settings();
	vTaskDelay(100);
	start_task_wifi();
	start_task_i2c();
	start_task_statemachine();
	start_task_valvecontrol();
	start_task_mqtt();
	start_task_neopixel();
	start_task_system();
	startTaskwebcode();
	start_task_websocket();

	vTaskDelay(60000); // Only write to influxDB when all tasks are running
	// start_task_espnow();
	start_task_influxdb();

	//check_task_status();
}

void loop() {}

void init_registers(void)
{
	all_outputs_off(DATAPIN1, CLOCKPIN1, LATCHPIN1);
	all_outputs_off(DATAPIN2, CLOCKPIN2, LATCHPIN2);
}

/*
void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
	Serial.println("WiFi Failed!");
	return;
  }

  Serial.print("\nIP Address: ");
  Serial.print(WiFi.localIP());
  vTaskDelay(1000);
  sync_rtc_ntp();
  vTaskDelay(1000);
}*/
