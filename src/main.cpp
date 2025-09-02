#include "main.h"

// Variables
// const char* ssid = ""; //WIFI SSID
// const char* password = ""; //WIFI password

void setup()
{

	Serial.begin(115200);

	pinMode(latchPin1, OUTPUT);
	pinMode(clockPin1, OUTPUT);
	pinMode(dataPin1, OUTPUT);

	pinMode(latchPin2, OUTPUT);
	pinMode(clockPin2, OUTPUT);
	pinMode(dataPin2, OUTPUT);

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

	// Init queues for sensors
	float temp[2][8][3];
	char txBuffer[400];
	sensor_queue = xQueueCreate(1, sizeof(temp));
	sensor_avg_queue = xQueueCreate(1, sizeof(temp));
	webserial_queue = xQueueCreate(30, sizeof(txBuffer));

	String message = "";

	if (webserial_queue == 0 || sensor_queue == 0 || sensor_avg_queue == 0)
	{
		Serial.print("\nFailed to create queues");
	}

	// First switch off all outputs which may have come randomly at power on
	init_registers();

	// Begin LittleFS
	if (!LittleFS.begin())
	{
		Serial.println("\nAn Error has occurred while mounting LittleFS");
		return;
	}

	// setup_wifi();

	// New config
	vTaskDelay(10000); // So can lauch serial monitor
	if (!parse_i2c_settings())
	{
		message = "Failed to parse I2C settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed I2C settings";
		print_message(message);
	}

	if (!parse_rtc_settings())
	{
		message = "Failed to parse RTC settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed RTC settings";
		print_message(message);
	}

	if (!parse_fan_settings())
	{
		message = "Failed to parse fan settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed fan settings";
		print_message(message);
	}

	if (!parse_influxdb_settings())
	{
		message = "Failed to parse influxdb settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed influxdb settings";
		print_message(message);
	}

	if (!parse_mqtt_settings())
	{
		message = "Failed to parse MQTT settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed MQTT settings";
		print_message(message);
	}

	if (!parse_network_settings())
	{
		message = "Failed to parse network settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed network settings";
		print_message(message);
	}

	if (!parse_sensor1_settings())
	{
		message = "Failed to parse sensor1 settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed sensor1 settings";
		print_message(message);
	}

	if (!parse_sensor2_settings())
	{
		message = "Failed to parse sensor2 settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed sensor2 settings";
		print_message(message);
	}

	if (!parse_statemachine_settings())
	{
		message = "Failed to parse statemachine settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed statemachine settings";
		print_message(message);
	}
	if (!parse_state_day_settings())
	{
		message = "Failed to parse state_day settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed state_day settings";
		print_message(message);
	}
	if (!parse_state_night_settings())
	{
		message = "Failed to parse state_night settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed state_night settings";
		print_message(message);
	}

	if (!parse_state_highco2day_settings())
	{
		message = "Failed to parse state_highco2day settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed state_highco2day settings";
		print_message(message);
	}

	if (!parse_state_highco2night_settings())
	{
		message = "Failed to parse highco2night settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed highco2night settings";
		print_message(message);
	}

	if (!parse_state_highrhday_settings())
	{
		message = "Failed to parse state_highco2night settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed state_highrhday settings";
		print_message(message);
	}

	if (!parse_state_highrhnight_settings())
	{
		message = "Failed to parse state_rhnight settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed state_highrhnight settings";
		print_message(message);
	}

	if (!parse_state_cooking_settings())
	{
		message = "Failed to parse state_cooking settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed state_cooking settings";
		print_message(message);
	}

	if (!parse_state_cyclingday_settings())
	{
		message = "Failed to parse state_cyclingday settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed state_cyclingday settings";
		print_message(message);
	}

	if (!parse_state_cyclingnight_settings())
	{
		message = "Failed to parse state_cyclingnight settings";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed state_cyclingnight settings";
		print_message(message);
	}

	if (!parse_actual_valve_positions())
	{
		message = "failed to parse actual valve positions";
		print_message(message);
	}
	else
	{
		message = "Successfully parsed actual valve positions ";
		print_message(message);
	}

	// parse_state_temp_settings();
	vTaskDelay(100);
	start_task_wifi();
	start_task_i2c();
	start_task_statemachine();
	start_task_valvecontrol();
	start_task_mqtt();
	start_task_neopixel();
	// start_task_system();
	startTaskwebcode();
	start_task_websocket();

	vTaskDelay(60000); // Only write to influxDB when all tasks are running
	// start_task_espnow();
	start_task_influxdb();
}

void loop() {}

void init_registers(void)
{
	all_outputs_off(dataPin1, clockPin1, latchPin1);
	all_outputs_off(dataPin2, clockPin2, latchPin2);
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
