#include "main.h"

// Variables
// const char* ssid = ""; //WIFI SSID
// const char* password = ""; //WIFI password

void setup()
{
	float temp[SENSOR_I2C_BUSSES][SENSOR_COUNT][SENSOR_DATA_FIELDS];
	char txBuffer[400] = {};
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
	webserial_queue = xQueueCreate(30, sizeof(txBuffer));

	if (webserial_queue == 0 || sensor_queue == 0 || sensor_avg_queue == 0)
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to create queues");
		printmessage(msg);
	}

	// First switch off all outputs which may have come randomly at power on
	init_registers();

	// Begin LittleFS
	if (!LittleFS.begin())
	{
		snprintf(msg, sizeof(msg), "[ERROR] An Error has occurred while mounting LittleFS");
		printmessage(msg);
		return;
	}

	// setup_wifi();

	// New config
	vTaskDelay(10000); // So can lauch serial monitor
	if (!parse_i2c_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse I2C settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed I2C settings");
		printmessage(msg);
	}

	if (!parse_rtc_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse RTC settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed RTC settings");
		printmessage(msg);
	}

	if (!parse_fan_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse fan settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed fan settings");
		printmessage(msg);
	}

	if (!parse_influxdb_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse Influxdb settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed Influxdb settings");
		printmessage(msg);
	}

	if (!parse_mqtt_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse MQTT settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed MQTT settings");
		printmessage(msg);
	}

	if (!parse_network_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse network settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed network settings");
		printmessage(msg);
	}

	if (!parse_sensor1_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse sensor1 settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed sensor1 settings");
		printmessage(msg);
	}

	if (!parse_sensor2_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse sensor2 settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed sensor2 settings");
		printmessage(msg);
	}

	if (!parse_statemachine_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse statemachine settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed statemachine settings");
		printmessage(msg);
	}
	if (!parse_state_day_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse state day settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed state day settings");
		printmessage(msg);
	}
	if (!parse_state_night_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse state night settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed state night settings");
		printmessage(msg);
	}

	if (!parse_state_highco2day_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse state highco2day settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed state highco2day settings");
		printmessage(msg);
	}

	if (!parse_state_highco2night_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse state highco2night settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed state highco2night settings");
		printmessage(msg);
	}

	if (!parse_state_highrhday_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse state highrhday settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed state highrhday settings");
		printmessage(msg);
	}

	if (!parse_state_highrhnight_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse state highrhnight settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed state highrhnight settings");
		printmessage(msg);
	}

	if (!parse_state_cooking_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse state cooking settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed state cooking settings");
		printmessage(msg);
	}

	if (!parse_state_cyclingday_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse state cyclingday settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed state cyclingday settings");
		printmessage(msg);
	}

	if (!parse_state_cyclingnight_settings())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse state cyclingnight settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed state cyclingnight settings");
		printmessage(msg);
	}

	if (!parse_actual_valve_positions())
	{
		snprintf(msg, sizeof(msg), "[ERROR] Failed to parse actual valve settings");
		printmessage(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "[INFO] Successfully parsed actual valve settings");
		printmessage(msg);
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

	eTaskState wifi_state = eTaskGetState(task_wifi);
	eTaskState i2c_state = eTaskGetState(task_i2c);
	eTaskState statemachine_state = eTaskGetState(task_statemach);
	eTaskState influxdb_state = eTaskGetState(task_influxdb);
	eTaskState mqtt_state = eTaskGetState(task_mqtt);
	eTaskState np_state = eTaskGetState(task_np);
	eTaskState system_state = eTaskGetState(task_sys);
	eTaskState valvectrl_state = eTaskGetState(task_valvectrl);
	eTaskState h_Task_web_state = eTaskGetState(h_Task_web);
	eTaskState websocket_state = eTaskGetState(task_websocket);

	if (wifi_state != eDeleted && wifi_state != eInvalid)
	{
		snprintf(msg, sizeof(msg), "[INFO] Task WIFI has started or is running.");
		printmessage(msg);
	}
	if (i2c_state != eDeleted && i2c_state != eInvalid)
	{
		snprintf(msg, sizeof(msg), "[INFO] Task I2C has started or is running.");
		printmessage(msg);
	}
	if (influxdb_state != eDeleted && influxdb_state != eInvalid)
	{
		snprintf(msg, sizeof(msg), "[INFO] Task InfluxDB has started or is running.");
		printmessage(msg);
	}
	if (mqtt_state != eDeleted && mqtt_state != eInvalid)
	{
		snprintf(msg, sizeof(msg), "[INFO] Task MQTT has started or is running.");
		printmessage(msg);
	}
	if (np_state != eDeleted && np_state != eInvalid)
	{
		snprintf(msg, sizeof(msg), "[INFO] Task Neopixel has started or is running.");
		printmessage(msg);
	}
	if (statemachine_state != eDeleted && statemachine_state != eInvalid)
	{
		snprintf(msg, sizeof(msg), "[INFO] Task statemachine has started or is running.");
		printmessage(msg);
	}
	if (system_state != eDeleted && system_state != eInvalid)
	{
		snprintf(msg, sizeof(msg), "[INFO] Task system has started or is running.");
		printmessage(msg);
	}
	if (valvectrl_state != eDeleted && valvectrl_state != eInvalid)
	{
		snprintf(msg, sizeof(msg), "[INFO] Task valvecontrol has started or is running.");
		printmessage(msg);
	}
	if (h_Task_web_state != eDeleted && h_Task_web_state != eInvalid)
	{
		snprintf(msg, sizeof(msg), "[INFO] Task web has started or is running.");
		printmessage(msg);
	}
	if (websocket_state  != eDeleted && websocket_state != eInvalid)
	{
		snprintf(msg, sizeof(msg), "[INFO] Task web has started or is running.");
		printmessage(msg);
	}
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
