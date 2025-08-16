#include "task_web.h"

bool valve_move_locked;

const char *VALVE0_POSITION_MOVE = "valve0_position_move";
const char *VALVE0_DIRECTION = "valve0_direction";
const char *VALVE1_POSITION_MOVE = "valve1_position_move";
const char *VALVE1_DIRECTION = "valve1_direction";
const char *VALVE2_POSITION_MOVE = "valve2_position_move";
const char *VALVE2_DIRECTION = "valve2_direction";
const char *VALVE3_POSITION_MOVE = "valve3_position_move";
const char *VALVE3_DIRECTION = "valve3_direction";
const char *VALVE4_POSITION_MOVE = "valve4_position_move";
const char *VALVE4_DIRECTION = "valve4_direction";
const char *VALVE5_POSITION_MOVE = "valve5_position_move";
const char *VALVE5_DIRECTION = "valve5_direction";
const char *VALVE6_POSITION_MOVE = "valve6_position_move";
const char *VALVE6_DIRECTION = "valve6_direction";
const char *VALVE7_POSITION_MOVE = "valve7_position_move";
const char *VALVE7_DIRECTION = "valve7_direction";
const char *VALVE8_POSITION_MOVE = "valve8_position_move";
const char *VALVE8_DIRECTION = "valve8_direction";
const char *VALVE9_POSITION_MOVE = "valve9_position_move";
const char *VALVE9_DIRECTION = "valve9_direction";
const char *VALVE10_POSITION_MOVE = "valve10_position_move";
const char *VALVE10_DIRECTION = "valve10_direction";
const char *VALVE11_POSITION_MOVE = "valve11_position_move";
const char *VALVE11_DIRECTION = "valve11_direction";
const char *STORE_VALVE_POSITION_IN_FILE = "store_valve_position_in_file";
const char *CHECK_VALVE_POSITION = "check_valve_position";
const char *STATUS_VALVE_POSITION_FILE;

// Variables for sensor config page
const char *WIRE_SENSOR0_TYPE = "wire_sensor0_type";
const char *WIRE_SENSOR0_VALVE = "wire_sensor0_valve";
const char *WIRE_SENSOR0_LOCATION = "wire_sensor0_location";
const char *WIRE_SENSOR0_RH = "wire_sensor0_rh";
const char *WIRE_SENSOR0_CO2 = "wire_sensor0_co2";
const char *WIRE_SENSOR1_TYPE = "wire_sensor1_type";
const char *WIRE_SENSOR1_VALVE = "wire_sensor1_valve";
const char *WIRE_SENSOR1_LOCATION = "wire_sensor1_location";
const char *WIRE_SENSOR1_RH = "wire_sensor1_rh";
const char *WIRE_SENSOR1_CO2 = "wire_sensor1_co2";
const char *WIRE_SENSOR2_TYPE = "wire_sensor2_type";
const char *WIRE_SENSOR2_VALVE = "wire_sensor2_valve";
const char *WIRE_SENSOR2_LOCATION = "wire_sensor2_location";
const char *WIRE_SENSOR2_RH = "wire_sensor2_rh";
const char *WIRE_SENSOR2_CO2 = "wire_sensor2_co2";
const char *WIRE_SENSOR3_TYPE = "wire_sensor3_type";
const char *WIRE_SENSOR3_VALVE = "wire_sensor3_valve";
const char *WIRE_SENSOR3_LOCATION = "wire_sensor3_location";
const char *WIRE_SENSOR3_RH = "wire_sensor3_rh";
const char *WIRE_SENSOR3_CO2 = "wire_sensor3_co2";
const char *WIRE_SENSOR4_TYPE = "wire_sensor4_type";
const char *WIRE_SENSOR4_VALVE = "wire_sensor4_valve";
const char *WIRE_SENSOR4_LOCATION = "wire_sensor4_location";
const char *WIRE_SENSOR4_RH = "wire_sensor4_rh";
const char *WIRE_SENSOR4_CO2 = "wire_sensor4_co2";
const char *WIRE_SENSOR5_TYPE = "wire_sensor5_type";
const char *WIRE_SENSOR5_VALVE = "wire_sensor5_valve";
const char *WIRE_SENSOR5_LOCATION = "wire_sensor5_location";
const char *WIRE_SENSOR5_RH = "wire_sensor5_rh";
const char *WIRE_SENSOR5_CO2 = "wire_sensor5_co2";
const char *WIRE_SENSOR6_TYPE = "wire_sensor6_type";
const char *WIRE_SENSOR6_VALVE = "wire_sensor6_valve";
const char *WIRE_SENSOR6_LOCATION = "wire_sensor6_location";
const char *WIRE_SENSOR6_RH = "wire_sensor6_rh";
const char *WIRE_SENSOR6_CO2 = "wire_sensor6_co2";
const char *WIRE_SENSOR7_TYPE = "wire_sensor7_type";
const char *WIRE_SENSOR7_VALVE = "wire_sensor7_valve";
const char *WIRE_SENSOR7_LOCATION = "wire_sensor7_location";
const char *WIRE_SENSOR7_RH = "wire_sensor7_rh";
const char *WIRE_SENSOR7_CO2 = "wire_sensor7_co2";

const char *WIRE1_SENSOR0_TYPE = "wire1_sensor0_type";
const char *WIRE1_SENSOR0_VALVE = "wire1_sensor0_valve";
const char *WIRE1_SENSOR0_LOCATION = "wire1_sensor0_location";
const char *WIRE1_SENSOR0_RH = "wire1_sensor0_rh";
const char *WIRE1_SENSOR0_CO2 = "wire1_sensor0_co2";
const char *WIRE1_SENSOR1_TYPE = "wire1_sensor1_type";
const char *WIRE1_SENSOR1_VALVE = "wire1_sensor1_valve";
const char *WIRE1_SENSOR1_LOCATION = "wire1_sensor1_location";
const char *WIRE1_SENSOR1_RH = "wire1_sensor1_rh";
const char *WIRE1_SENSOR1_CO2 = "wire1_sensor1_co2";
const char *WIRE1_SENSOR2_TYPE = "wire1_sensor2_type";
const char *WIRE1_SENSOR2_VALVE = "wire1_sensor2_valve";
const char *WIRE1_SENSOR2_LOCATION = "wire1_sensor2_location";
const char *WIRE1_SENSOR2_RH = "wire1_sensor2_rh";
const char *WIRE1_SENSOR2_CO2 = "wire1_sensor2_co2";
const char *WIRE1_SENSOR3_TYPE = "wire1_sensor3_type";
const char *WIRE1_SENSOR3_VALVE = "wire1_sensor3_valve";
const char *WIRE1_SENSOR3_LOCATION = "wire1_sensor3_location";
const char *WIRE1_SENSOR3_RH = "wire1_sensor3_rh";
const char *WIRE1_SENSOR3_CO2 = "wire1_sensor3_co2";
const char *WIRE1_SENSOR4_TYPE = "wire1_sensor4_type";
const char *WIRE1_SENSOR4_VALVE = "wire1_sensor4_valve";
const char *WIRE1_SENSOR4_LOCATION = "wire1_sensor4_location";
const char *WIRE1_SENSOR4_RH = "wire1_sensor4_rh";
const char *WIRE1_SENSOR4_CO2 = "wire1_sensor4_co2";
const char *WIRE1_SENSOR5_TYPE = "wire1_sensor5_type";
const char *WIRE1_SENSOR5_VALVE = "wire1_sensor5_valve";
const char *WIRE1_SENSOR5_LOCATION = "wire1_sensor5_location";
const char *WIRE1_SENSOR5_RH = "wire1_sensor5_rh";
const char *WIRE1_SENSOR5_CO2 = "wire1_sensor5_co2";
const char *WIRE1_SENSOR6_TYPE = "wire1_sensor6_type";
const char *WIRE1_SENSOR6_VALVE = "wire1_sensor6_valve";
const char *WIRE1_SENSOR6_LOCATION = "wire1_sensor6_location";
const char *WIRE1_SENSOR6_RH = "wire1_sensor6_rh";
const char *WIRE1_SENSOR6_CO2 = "wire1_sensor6_co2";
const char *WIRE1_SENSOR7_TYPE = "wire1_sensor7_type";
const char *WIRE1_SENSOR7_VALVE = "wire1_sensor7_valve";
const char *WIRE1_SENSOR7_LOCATION = "wire1_sensor7_location";
const char *WIRE1_SENSOR7_RH = "wire1_sensor7_rh";
const char *WIRE1_SENSOR7_CO2 = "wire1_sensor7_co2";

const char *WIRE_SENSOR_CONFIG = "wire_sensor_config";
const char *WIRE1_SENSOR_CONFIG = "wire1_sensor_config";

const char *STATUS_NETWORK_CONFIG = "status_network_config";
const char *ENABLE_DHCP = "enable_dhcp";
const char *SSID = "ssid";
const char *WIFI_PASSWORD = "wifi_password";
const char *IP_ADDRESS = "ip_address";
const char *SUBNET_MASK = "subnet_mask";
const char *GATEWAY = "gateway";
const char *PRIMARY_DNS = "primary_dns";
const char *SECONDARY_DNS = "secondary_dns";

const char *STATUS_MQTT_CONFIG = "status_mqtt_config";
const char *ENABLE_MQTT = "enable_mqtt";
const char *MQTT_SERVER = "mqtt_server";
const char *MQTT_PORT = "mqtt_port";
const char *MQTT_BASE_TOPIC = "mqtt_base_topic";

const char *STATUS_I2C_CONFIG = "status_i2c_config";
const char *BUS0_MULTIPLEXER_ADDRESS = "bus0_multiplexer_address";
const char *BUS1_MULTIPLEXER_ADDRESS = "bus1_multiplexer_address";
const char *ENABLE_LCD = "enable_lcd";
const char *DISPLAY_I2C_ADDRESS = "display_i2c_address";

const char *STATUS_FAN_CONTROL_CONFIG = "status_fan_control_config";
const char *FAN_CONTROL_MODE = "fan_control_mode";
const char *FAN_CONTROL_MQTT_SERVER = "fan_control_mqtt_server";
const char *FAN_CONTROL_MQTT_PORT = "fan_control_mqtt_port";
const char *FAN_CONTROL_MQTT_TOPIC = "fan_control_mqtt_topic";
const char *FAN_CONTROL_URL_HIGH_SPEED = "fan_control_url_high_speed";
const char *FAN_CONTROL_URL_MEDIUM_SPEED = "fan_control_url_medium_speed";
const char *FAN_CONTROL_URL_LOW_SPEED = "fan_control_url_low_speed";

const char *STATUS_INFLUXDB_CONFIG = "status_influxdb_config";
const char *ENABLE_INFLUXDB = "enable_influxdb";
const char *INFLUXDB_URL = "influxdb_url";
const char *INFLUXDB_ORG = "influxdb_org";
const char *INFLUXDB_BUCKET = "influxdb_bucket";
const char *INFLUXDB_TOKEN = "influxdb_token";

const char *STATUS_RTC_CONFIG = "status_rtc_config";
const char *TIMEZONE = "timezone";
const char *NTP_SERVER = "ntp_server";

// Statemachine settings
const char *STATUS_STATEMACHINE_CONFIG = "status_statemachine_config";
const char *WEEKDAY_DAY_HOUR_START = "weekday_day_hour_start";
const char *WEEKDAY_DAY_MINUTE_START = "weekday_day_minute_start";
const char *WEEKDAY_NIGHT_HOUR_START = "weekday_night_hour_start";
const char *WEEKDAY_NIGHT_MINUTE_START = "weekday_night_minute_start";
const char *WEEKEND_DAY_HOUR_START = "weekend_day_hour_start";
const char *WEEKEND_DAY_MINUTE_START = "weekend_day_minute_start";
const char *WEEKEND_NIGHT_HOUR_START = "weekend_night_hour_start";
const char *WEEKEND_NIGHT_MINUTE_START = "weekend_night_minute_start";
const char *WEEKEND_DAY_1 = "weekend_day_1";
const char *WEEKEND_DAY_2 = "weekend_day_2";
const char *MINIMUM_STATE_TIME = "minimum_state_time";

const char *STATUS_STATE_DAY_CONFIG = "status_state_day_config";
const char *STATE_DAY_FANSPEED = "state_day_fanspeed";
const char *ENABLE_STATE_DAY = "enable_state_day";
const char *NAME_STATE_DAY = "name_state_day";
const char *VALVE0_POSITION_DAY = "valve0_position_day";
const char *VALVE1_POSITION_DAY = "valve1_position_day";
const char *VALVE2_POSITION_DAY = "valve2_position_day";
const char *VALVE3_POSITION_DAY = "valve3_position_day";
const char *VALVE4_POSITION_DAY = "valve4_position_day";
const char *VALVE5_POSITION_DAY = "valve5_position_day";
const char *VALVE6_POSITION_DAY = "valve6_position_day";
const char *VALVE7_POSITION_DAY = "valve7_position_day";
const char *VALVE8_POSITION_DAY = "valve8_position_day";
const char *VALVE9_POSITION_DAY = "valve9_position_day";
const char *VALVE10_POSITION_DAY = "valve10_position_day";
const char *VALVE11_POSITION_DAY = "valve11_position_day";

const char *STATUS_STATE_NIGHT_CONFIG = "statemachine_valve_night_config";
const char *STATE_NIGHT_FANSPEED = "state_night_fanspeed";
const char *ENABLE_STATE_NIGHT = "enable_state_night";
const char *NAME_STATE_NIGHT = "name_state_night";
const char *VALVE0_POSITION_NIGHT = "valve0_position_night";
const char *VALVE1_POSITION_NIGHT = "valve1_position_night";
const char *VALVE2_POSITION_NIGHT = "valve2_position_night";
const char *VALVE3_POSITION_NIGHT = "valve3_position_night";
const char *VALVE4_POSITION_NIGHT = "valve4_position_night";
const char *VALVE5_POSITION_NIGHT = "valve5_position_night";
const char *VALVE6_POSITION_NIGHT = "valve6_position_night";
const char *VALVE7_POSITION_NIGHT = "valve7_position_night";
const char *VALVE8_POSITION_NIGHT = "valve8_position_night";
const char *VALVE9_POSITION_NIGHT = "valve9_position_night";
const char *VALVE10_POSITION_NIGHT = "valve10_position_night";
const char *VALVE11_POSITION_NIGHT = "valve11_position_night";

const char *STATUS_STATE_HIGHCO2DAY_CONFIG = "statemachine_valve_highco2day_config";
const char *STATE_HIGHCO2DAY_FANSPEED = "state_highco2day_fanspeed";
const char *ENABLE_STATE_HIGHCO2DAY = "enable_state_highco2day";
const char *NAME_STATE_HIGHCO2DAY = "name_state_highco2day";
const char *CO2_LOW_STATE_HIGHCO2DAY = "co2_low_state_highco2day";
const char *CO2_HIGH_STATE_HIGHCO2DAY = "co2_high_state_highco2day";
const char *VALVE0_POSITION_HIGHCO2DAY = "valve0_position_highco2day";
const char *VALVE1_POSITION_HIGHCO2DAY = "valve1_position_highco2day";
const char *VALVE2_POSITION_HIGHCO2DAY = "valve2_position_highco2day";
const char *VALVE3_POSITION_HIGHCO2DAY = "valve3_position_highco2day";
const char *VALVE4_POSITION_HIGHCO2DAY = "valve4_position_highco2day";
const char *VALVE5_POSITION_HIGHCO2DAY = "valve5_position_highco2day";
const char *VALVE6_POSITION_HIGHCO2DAY = "valve6_position_highco2day";
const char *VALVE7_POSITION_HIGHCO2DAY = "valve7_position_highco2day";
const char *VALVE8_POSITION_HIGHCO2DAY = "valve8_position_highco2day";
const char *VALVE9_POSITION_HIGHCO2DAY = "valve9_position_highco2day";
const char *VALVE10_POSITION_HIGHCO2DAY = "valve10_position_highco2day";
const char *VALVE11_POSITION_HIGHCO2DAY = "valve11_position_highco2day";

const char *STATUS_STATE_HIGHCO2NIGHT_CONFIG = "statemachine_valve_highco2night_config";
const char *STATE_HIGHCO2NIGHT_FANSPEED = "state_highco2night_fanspeed";
const char *ENABLE_STATE_HIGHCO2NIGHT = "enable_state_highco2night";
const char *NAME_STATE_HIGHCO2NIGHT = "name_state_highco2night";
const char *CO2_LOW_STATE_HIGHCO2NIGHT = "co2_low_state_highco2night";
const char *CO2_HIGH_STATE_HIGHCO2NIGHT = "co2_high_state_highco2night";
const char *VALVE0_POSITION_HIGHCO2NIGHT = "valve0_position_highco2night";
const char *VALVE1_POSITION_HIGHCO2NIGHT = "valve1_position_highco2night";
const char *VALVE2_POSITION_HIGHCO2NIGHT = "valve2_position_highco2night";
const char *VALVE3_POSITION_HIGHCO2NIGHT = "valve3_position_highco2night";
const char *VALVE4_POSITION_HIGHCO2NIGHT = "valve4_position_highco2night";
const char *VALVE5_POSITION_HIGHCO2NIGHT = "valve5_position_highco2night";
const char *VALVE6_POSITION_HIGHCO2NIGHT = "valve6_position_highco2night";
const char *VALVE7_POSITION_HIGHCO2NIGHT = "valve7_position_highco2night";
const char *VALVE8_POSITION_HIGHCO2NIGHT = "valve8_position_highco2night";
const char *VALVE9_POSITION_HIGHCO2NIGHT = "valve9_position_highco2night";
const char *VALVE10_POSITION_HIGHCO2NIGHT = "valve10_position_highco2night";
const char *VALVE11_POSITION_HIGHCO2NIGHT = "valve11_position_highco2night";

const char *STATUS_STATE_HIGHRHDAY_CONFIG = "statemachine_valve_highrhday_config";
const char *STATE_HIGHRHDAY_FANSPEED = "state_highrhday_fanspeed";
const char *ENABLE_STATE_HIGHRHDAY = "enable_state_highrhday";
const char *NAME_STATE_HIGHRHDAY = "name_state_highrhday";
const char *RH_LOW_STATE_HIGHRHDAY = "rh_low_state_highrhday";
const char *RH_HIGH_STATE_HIGHRHDAY = "rh_high_state_highrhday";
const char *VALVE0_POSITION_HIGHRHDAY = "valve0_position_highrhday";
const char *VALVE1_POSITION_HIGHRHDAY = "valve1_position_highrhday";
const char *VALVE2_POSITION_HIGHRHDAY = "valve2_position_highrhday";
const char *VALVE3_POSITION_HIGHRHDAY = "valve3_position_highrhday";
const char *VALVE4_POSITION_HIGHRHDAY = "valve4_position_highrhday";
const char *VALVE5_POSITION_HIGHRHDAY = "valve5_position_highrhday";
const char *VALVE6_POSITION_HIGHRHDAY = "valve6_position_highrhday";
const char *VALVE7_POSITION_HIGHRHDAY = "valve7_position_highrhday";
const char *VALVE8_POSITION_HIGHRHDAY = "valve8_position_highrhday";
const char *VALVE9_POSITION_HIGHRHDAY = "valve9_position_highrhday";
const char *VALVE10_POSITION_HIGHRHDAY = "valve10_position_highrhday";
const char *VALVE11_POSITION_HIGHRHDAY = "valve11_position_highrhday";

const char *STATUS_STATE_HIGHRHNIGHT_CONFIG = "statemachine_valve_highrhnight_config";
const char *STATE_HIGHRHNIGHT_FANSPEED = "state_highrhnight_fanspeed";
const char *ENABLE_STATE_HIGHRHNIGHT = "enable_state_highrhnight";
const char *NAME_STATE_HIGHRHNIGHT = "name_state_highrhnight";
const char *RH_LOW_STATE_HIGHRHNIGHT = "rh_low_state_highrhnight";
const char *RH_HIGH_STATE_HIGHRHNIGHT = "rh_high_state_highrhnight";
const char *VALVE0_POSITION_HIGHRHNIGHT = "valve0_position_highrhnight";
const char *VALVE1_POSITION_HIGHRHNIGHT = "valve1_position_highrhnight";
const char *VALVE2_POSITION_HIGHRHNIGHT = "valve2_position_highrhnight";
const char *VALVE3_POSITION_HIGHRHNIGHT = "valve3_position_highrhnight";
const char *VALVE4_POSITION_HIGHRHNIGHT = "valve4_position_highrhnight";
const char *VALVE5_POSITION_HIGHRHNIGHT = "valve5_position_highrhnight";
const char *VALVE6_POSITION_HIGHRHNIGHT = "valve6_position_highrhnight";
const char *VALVE7_POSITION_HIGHRHNIGHT = "valve7_position_highrhnight";
const char *VALVE8_POSITION_HIGHRHNIGHT = "valve8_position_highrhnight";
const char *VALVE9_POSITION_HIGHRHNIGHT = "valve9_position_highrhnight";
const char *VALVE10_POSITION_HIGHRHNIGHT = "valve10_position_highrhnight";
const char *VALVE11_POSITION_HIGHRHNIGHT = "valve11_position_highrhnight";

const char *STATUS_STATE_COOKING_CONFIG = "statemachine_valve_cooking_config";
const char *STATE_COOKING_FANSPEED = "state_cooking_fanspeed";
const char *ENABLE_STATE_COOKING = "enable_state_cooking";
const char *NAME_STATE_COOKING = "name_state_cooking";
const char *START_HOUR_STATE_COOKING = "start_hour_state_cooking";
const char *START_MIN_STATE_COOKING = "start_min_state_cooking";
const char *STOP_HOUR_STATE_COOKING = "stop_hour_state_cooking";
const char *STOP_MIN_STATE_COOKING = "stop_min_state_cooking";
const char *VALVE0_POSITION_COOKING = "valve0_position_cooking";
const char *VALVE1_POSITION_COOKING = "valve1_position_cooking";
const char *VALVE2_POSITION_COOKING = "valve2_position_cooking";
const char *VALVE3_POSITION_COOKING = "valve3_position_cooking";
const char *VALVE4_POSITION_COOKING = "valve4_position_cooking";
const char *VALVE5_POSITION_COOKING = "valve5_position_cooking";
const char *VALVE6_POSITION_COOKING = "valve6_position_cooking";
const char *VALVE7_POSITION_COOKING = "valve7_position_cooking";
const char *VALVE8_POSITION_COOKING = "valve8_position_cooking";
const char *VALVE9_POSITION_COOKING = "valve9_position_cooking";
const char *VALVE10_POSITION_COOKING = "valve10_position_cooking";
const char *VALVE11_POSITION_COOKING = "valve11_position_cooking";

const char *STATUS_STATE_CYCLINGDAY_CONFIG = "statemachine_valve_cyclingday_config";
const char *STATE_CYCLINGDAY_FANSPEED = "state_cyclingday_fanspeed";
const char *ENABLE_STATE_CYCLINGDAY = "enable_state_cyclingday";
const char *NAME_STATE_CYCLINGDAY = "name_state_cyclingday";
const char *VALVE0_POSITION_CYCLINGDAY = "valve0_position_cyclingday";
const char *VALVE1_POSITION_CYCLINGDAY = "valve1_position_cyclingday";
const char *VALVE2_POSITION_CYCLINGDAY = "valve2_position_cyclingday";
const char *VALVE3_POSITION_CYCLINGDAY = "valve3_position_cyclingday";
const char *VALVE4_POSITION_CYCLINGDAY = "valve4_position_cyclingday";
const char *VALVE5_POSITION_CYCLINGDAY = "valve5_position_cyclingday";
const char *VALVE6_POSITION_CYCLINGDAY = "valve6_position_cyclingday";
const char *VALVE7_POSITION_CYCLINGDAY = "valve7_position_cyclingday";
const char *VALVE8_POSITION_CYCLINGDAY = "valve8_position_cyclingday";
const char *VALVE9_POSITION_CYCLINGDAY = "valve9_position_cyclingday";
const char *VALVE10_POSITION_CYCLINGDAY = "valve10_position_cyclingday";
const char *VALVE11_POSITION_CYCLINGDAY = "valve11_position_cyclingday";

const char *STATUS_STATE_CYCLINGNIGHT_CONFIG = "statemachine_valve_cyclingnight_config";
const char *STATE_CYCLINGNIGHT_FANSPEED = "state_cyclingnight_fanspeed";
const char *ENABLE_STATE_CYCLINGNIGHT = "enable_state_cyclingnight";
const char *NAME_STATE_CYCLINGNIGHT = "name_state_cyclingnight";
const char *VALVE0_POSITION_CYCLINGNIGHT = "valve0_position_cyclingnight";
const char *VALVE1_POSITION_CYCLINGNIGHT = "valve1_position_cyclingnight";
const char *VALVE2_POSITION_CYCLINGNIGHT = "valve2_position_cyclingnight";
const char *VALVE3_POSITION_CYCLINGNIGHT = "valve3_position_cyclingnight";
const char *VALVE4_POSITION_CYCLINGNIGHT = "valve4_position_cyclingnight";
const char *VALVE5_POSITION_CYCLINGNIGHT = "valve5_position_cyclingnight";
const char *VALVE6_POSITION_CYCLINGNIGHT = "valve6_position_cyclingnight";
const char *VALVE7_POSITION_CYCLINGNIGHT = "valve7_position_cyclingnight";
const char *VALVE8_POSITION_CYCLINGNIGHT = "valve8_position_cyclingnight";
const char *VALVE9_POSITION_CYCLINGNIGHT = "valve9_position_cyclingnight";
const char *VALVE10_POSITION_CYCLINGNIGHT = "valve10_position_cyclingnight";
const char *VALVE11_POSITION_CYCLINGNIGHT = "valve11_position_cyclingnight";

String valve0_direction = "", valve1_direction = "", valve2_direction = "", valve3_direction = "", valve4_direction = "", valve5_direction = "";
String valve6_direction = "", valve7_direction = "", valve8_direction = "", valve9_direction = "", valve10_direction = "", valve11_direction = "";
String check_valve_position = "";		  // True when check is required if valve moves within operating range
String store_valve_position_in_file = ""; // True to enable storing of new position in valve position file
String message = "";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void startTaskwebcode(void)
{
	xTaskCreate(Taskwebcode, "Task_web", 10000, NULL, 9, &h_Task_web);
}

void Taskwebcode(void *pvParameters)
{

	// Main page
	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(200, "text/html", index_html); });

	// Test main page
	server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(200, "text/html", test2_html); });

	// Request for CSS file
	server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(200, "text/css", styles_css); });
	server.on("/pure-min.css", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(200, "text/css", pure_min_css); });

	// Request for Javascript files
	server.on("/ui.js", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(200, "text/javascript", ui_js); });
	server.on("/websocket.js", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(200, "text/javascript", websocket_js); });
	server.on("/jquery-min.js", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(200, "text/javascript", jquery_min_js); });

	// Favicon image
	server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/img/favicon.ico", "image/x-icon"); });

	// Not found handling
	server.onNotFound([](AsyncWebServerRequest *request)
					  { request->send(404, "text/plain", "The content you are looking for was not found."); });

	// Settings web pages processing
	server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(200, "text/html", settings_html); });

	// Webserial handling
	server.on("/web_serial", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(200, "text/html", web_serial_html, webserial_processor); });

	// Reboot ESP32 button
	server.on("/restart_esp32", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		request->send(200, "text/html", settings_html);
		esp_restart(); });

	// Download JSON Config files
	server.on("/sensor_config1", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/sensor_config1.json", "text/json", true); });
	server.on("/sensor_config2", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/sensor_config2.json", "text/json", true); });
	server.on("/settings_fan", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/settings_fan.json", "text/json", true); });
	server.on("/settings_i2c", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/settings_i2c.json", "text/json", true); });
	server.on("/settings_influxdb", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/settings_influxdb.json", "text/json", true); });
	server.on("/settings_mqtt", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/settings_mqtt.json", "text/json", true); });
	server.on("/settings_network", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/settings_network.json", "text/json", true); });
	server.on("/settings_rtc", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/settings_rtc.json", "text/json", true); });
	server.on("/settings_state_cooking", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/settings_state_cooking.json", "text/json", true); });
	server.on("/settings_state_cyclingday", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/settings_state_cyclingday.json", "text/json", true); });
	server.on("/settings_state_cyclingnight", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/settings_state_cyclingnight.json", "text/json", true); });
	server.on("/settings_state_day", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/settings_state_day.json", "text/json", true); });
	server.on("/settings_state_highco2day", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/settings_state_highco2day.json", "text/json", true); });
	server.on("/settings_state_highco2night", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/settings_state_highco2night.json", "text/json", true); });
	server.on("/settings_state_highrhday", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/settings_state_highrhday.json", "text/json", true); });
	server.on("/settings_state_highrhnight", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/settings_state_highrhnight.json", "text/json", true); });
	server.on("/settings_state_night", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/settings_state_night.json", "text/json", true); });
	server.on("/settings_statemachine", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/settings_statemachine.json", "text/json", true); });
	server.on("/valvepositions", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(LittleFS, "/json/valvepositions.json", "text/json", true); });

	// Save settings from network settings
	server.on("/settings_network", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		
		int params = request->params();
		const char* path = "/json/settings_network.json";
		String settings_network_str;
		JsonDocument settings_network_data;
		
		for(int i=0;i<params;i++){
			const AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()) {
				if (p->name() == ENABLE_DHCP)
					settings_network_data["enable_dhcp"] = p->value().c_str();;
				if (p->name() == SSID)
					settings_network_data["ssid"] = p->value().c_str();;
				if (p->name() == WIFI_PASSWORD) 
					settings_network_data["wifi_password"] = p->value().c_str();;
				if (p->name() == IP_ADDRESS) 
					settings_network_data["ip_address"] = p->value().c_str();;
				if (p->name() == SUBNET_MASK)
					settings_network_data["subnet_mask"] = p->value().c_str();;
				if (p->name() == GATEWAY)
					settings_network_data["gateway"] = p->value().c_str();;
				if (p->name() == PRIMARY_DNS)
					settings_network_data["primary_dns"] = p->value().c_str();;
				if (p->name() == SECONDARY_DNS)
					settings_network_data["secondary_dns"] = p->value().c_str();;
			}
		}
		serializeJson(settings_network_data, settings_network_str);
		write_config_file(path, settings_network_str);
		request->send(200, "text/html", settings_html); });

	// Save settings from MQTT settings
	server.on("/settings_mqtt", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		
		int params = request->params();
		const char* path = "/json/settings_mqtt.json";
		String settings_mqtt_str;
		JsonDocument settings_mqtt_data;

		for(int i=0;i<params;i++){
			const AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				if (p->name() == STATUS_MQTT_CONFIG)
					settings_mqtt_data["status_mqtt_config"] = p->value().c_str();
				if (p->name() == ENABLE_MQTT)
					settings_mqtt_data["enable_mqtt"] = p->value().c_str();
				if (p->name() == MQTT_SERVER)
					settings_mqtt_data["mqtt_server"] = p->value().c_str();
				if (p->name() == MQTT_PORT)
					settings_mqtt_data["mqtt_port"] = p->value().c_str();
				if (p->name() == MQTT_BASE_TOPIC)
					settings_mqtt_data["mqtt_base_topic"] = p->value().c_str();
			}
		}
		serializeJson(settings_mqtt_data, settings_mqtt_str);
		write_config_file(path, settings_mqtt_str);	
		request->send(200, "text/html", settings_html); });

	// Save settings from I2C settings
	server.on("/settings_i2c", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		
		int params = request->params();
		const char* path = "/json/settings_i2c.json";
		String settings_i2c_str;
		JsonDocument settings_i2c_data;

		for(int i=0;i<params;i++){
			const AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				if (p->name() == STATUS_I2C_CONFIG)
					settings_i2c_data["status_i2c_config"] = p->value().c_str();;
				if (p->name() == BUS0_MULTIPLEXER_ADDRESS)
					settings_i2c_data["bus0_multiplexer_address"] = p->value().c_str();;
				if (p->name() == BUS1_MULTIPLEXER_ADDRESS) 
					settings_i2c_data["bus1_multiplexer_address"] = p->value().c_str();;
				if (p->name() == ENABLE_LCD)
					settings_i2c_data["enable_lcd"] = p->value().c_str();;
				if (p->name() == DISPLAY_I2C_ADDRESS) 
					settings_i2c_data["display_i2c_address"] = p->value().c_str();;
			}
		}
		serializeJson(settings_i2c_data, settings_i2c_str);
		write_config_file(path, settings_i2c_str);
		request->send(200, "text/html", settings_html); });

	// Save settings from fan control settings
	server.on("/settings_fan", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		
		int params = request->params();
		const char* path = "/json/settings_fan.json";
		String settings_fan_str;
		
		if (settings_fan_mutex != NULL) {
			if(xSemaphoreTake(settings_fan_mutex, ( TickType_t ) 10 ) == pdTRUE) {
				for(int i=0;i<params;i++){
					const AsyncWebParameter* p = request->getParam(i);
					if(p->isPost()){
						if (p->name() == STATUS_FAN_CONTROL_CONFIG)
							settings_fan_data["status_fan_control_config"] = p->value().c_str();
						if (p->name() == FAN_CONTROL_MODE) 
							settings_fan_data["fan_control_mode"] = p->value().c_str();
						if (p->name() == FAN_CONTROL_MQTT_SERVER) 
							settings_fan_data["fan_control_mqtt_server"] = p->value().c_str();
						if (p->name() == FAN_CONTROL_MQTT_PORT) 
							settings_fan_data["fan_control_mqtt_port"] = p->value().c_str();
						if (p->name() == FAN_CONTROL_MQTT_TOPIC) 
							settings_fan_data["fan_control_mqtt_topic"] = p->value().c_str();
						if (p->name() == FAN_CONTROL_URL_HIGH_SPEED) 
							settings_fan_data["fan_control_url_high_speed"] = p->value().c_str();
						if (p->name() == FAN_CONTROL_URL_MEDIUM_SPEED) 
							settings_fan_data["fan_control_url_medium_speed"] = p->value().c_str();
						if (p->name() == FAN_CONTROL_URL_LOW_SPEED) 
							settings_fan_data["fan_control_url_low_speed"] = p->value().c_str();
					}
				}
				serializeJson(settings_fan_data, settings_fan_str);    
				xSemaphoreGive(settings_fan_mutex);
			}
		}
		write_config_file(path, settings_fan_str);
		request->send(200, "text/html", settings_html); });

	// Save settings from statemachine settings
	/*server.on("/settings_statemachine", HTTP_POST, [](AsyncWebServerRequest *request) {
		if (settings_statemachine_mutex != NULL) {
			if(xSemaphoreTake(settings_statemachine_mutex, ( TickType_t ) 10 ) == pdTRUE) {
				int params = request->params();
				for(int i=0;i<params;i++){
					const AsyncWebParameter* p = request->getParam(i);
					if(p->isPost()){
						if (p->name() == STATUS_STATEMACHINE_CONFIG)
							settings_statemachine_data["status_statemachine_config"] = p->value().c_str();
						if (p->name() == STATEMACHINE_RH_SENSOR)
							settings_statemachine_data["statemachine_rh_sensor"] = p->value().c_str();
						if (p->name() == STATEMACHINE_CO2_SENSOR)
							settings_statemachine_data["statemachine_co2_sensor"] = p->value().c_str();
					}
				}
				const char* path = "/json/settings_statemachine.json";
				String settings_statemachine_str;
				serializeJson(settings_statemachine_data, settings_statemachine_str);
				write_config_file(path, settings_statemachine_str);
				xSemaphoreGive(settings_statemachine_mutex);
			}
		}
		request->send(LittleFS, "/html/settings.html", String(), false, settings_processor);
	});*/

	// Save settings from InfluxDB settings
	server.on("/settings_influxdb", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		
		int params = request->params();
		const char* path = "/json/settings_influxdb.json";
		String settings_influxdb_str;
		JsonDocument settings_influxdb_data;
				
		for(int i=0;i<params;i++){
			const AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				if (p->name() == STATUS_INFLUXDB_CONFIG)
					settings_influxdb_data["status_influxdb_config"] = p->value().c_str();
				if (p->name() == ENABLE_INFLUXDB)
					settings_influxdb_data["enable_influxdb"] = p->value().c_str();
				if (p->name() == INFLUXDB_URL)
					settings_influxdb_data["influxdb_url"] = p->value().c_str();
				if (p->name() == INFLUXDB_ORG)
					settings_influxdb_data["influxdb_org"] = p->value().c_str();
				if (p->name() == INFLUXDB_BUCKET)
					settings_influxdb_data["influxdb_bucket"] = p->value().c_str();
				if (p->name() == INFLUXDB_TOKEN)
					settings_influxdb_data["influxdb_token"] = p->value().c_str();
			}
		}
		serializeJson(settings_influxdb_data, settings_influxdb_str);
		write_config_file(path, settings_influxdb_str);
		request->send(200, "text/html", settings_html); });

	// Save settings from RTC settings
	server.on("/settings_rtc", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
	
		int params = request->params();
		const char* path = "/json/settings_rtc.json";
		String settings_rtc_str;
		JsonDocument settings_rtc_data;

		for(int i=0;i<params;i++){
			const AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				if (p->name() == STATUS_RTC_CONFIG)
					settings_rtc_data["status_rtc_config"] = p->value().c_str();
				if (p->name() == TIMEZONE)
					settings_rtc_data["timezone"] = p->value().c_str();
				if (p->name() == NTP_SERVER) 
					settings_rtc_data["ntp_server"] = p->value().c_str();
			}
		}

		serializeJson(settings_rtc_data, settings_rtc_str);
		write_config_file(path, settings_rtc_str);	
		request->send(200, "text/html", settings_html); });

	// Valve control web pages processing
	server.on("/valvecontrol", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(200, "text/html", valvecontrol_html); });

	// Response for POST action in webform valvecontrol manual move valves
	server.on("/valvecontrol", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		if (valve_control_data_mutex != NULL) {
			
			int params = request->params();
			
			if(xSemaphoreTake(valve_control_data_mutex, ( TickType_t ) 10 ) == pdTRUE) {
				for(int i=0;i<params;i++){
					const AsyncWebParameter* p = request->getParam(i);
					if(p->isPost()) {
						if (p->name() == VALVE0_POSITION_MOVE) {
							valve_control_data["valve0_data"][0] = 0;
							valve_control_data["valve0_data"][1] =  p->value().toInt();
						}
						if (p->name() == VALVE0_DIRECTION) {
							valve0_direction = p->value().c_str();
							if (valve0_direction == "Close") {
								valve_control_data["valve0_data"][2] = 0;
							}
							else {
								valve_control_data["valve0_data"][2] = 1;
							}
						}
						if (p->name() == VALVE1_POSITION_MOVE) {
							valve_control_data["valve1_data"][0] = 1;
							valve_control_data["valve1_data"][1] =  p->value().toInt();
						}
						if (p->name() == VALVE1_DIRECTION) {
							valve1_direction = p->value().c_str();
							if (valve1_direction == "Close") {
								valve_control_data["valve1_data"][2] = 0;
							}
							else {
								valve_control_data["valve1_data"][2] = 1;
							}
						}
						if (p->name() == VALVE2_POSITION_MOVE) {
							valve_control_data["valve2_data"][0] = 2;
							valve_control_data["valve2_data"][1] =  p->value().toInt();
							}
						if (p->name() == VALVE2_DIRECTION) {
							valve2_direction = p->value().c_str();
							if (valve2_direction == "Close") {
								valve_control_data["valve2_data"][2] = 0;
							}
							else {
								valve_control_data["valve2_data"][2] = 1;
							}
						}
						if (p->name() == VALVE3_POSITION_MOVE) {
							valve_control_data["valve3_data"][0] = 3;
							valve_control_data["valve3_data"][1] =  p->value().toInt();
						}
						if (p->name() == VALVE3_DIRECTION) {
							valve3_direction = p->value().c_str();
							if (valve3_direction == "Close") {
								valve_control_data["valve3_data"][2] = 0;
							}
							else {
								valve_control_data["valve3_data"][2] = 1;
							}
						}
						if (p->name() == VALVE4_POSITION_MOVE) {
							valve_control_data["valve4_data"][0] = 4;
							valve_control_data["valve4_data"][1] =  p->value().toInt();
							}
							if (p->name() == VALVE4_DIRECTION) {
							valve4_direction = p->value().c_str();
							if (valve4_direction == "Close") {
								valve_control_data["valve4_data"][2] = 0;
							}
							else {
								valve_control_data["valve4_data"][2] = 1;
							}
						}
						if (p->name() == VALVE5_POSITION_MOVE) {
							valve_control_data["valve5_data"][0] = 5;
							valve_control_data["valve5_data"][1] =  p->value().toInt();
						}
						if (p->name() == VALVE5_DIRECTION) {
							valve5_direction = p->value().c_str();
							if (valve5_direction == "Close") {
								valve_control_data["valve5_data"][2] = 0;
							}
							else {
								valve_control_data["valve5_data"][2] = 1;
							}
						}
						if (p->name() == VALVE6_POSITION_MOVE) {
							valve_control_data["valve6_data"][0] = 6;
							valve_control_data["valve6_data"][1] =  p->value().toInt();
						}
						if (p->name() == VALVE6_DIRECTION) {
							valve6_direction = p->value().c_str();
							if (valve6_direction == "Close") {
								valve_control_data["valve6_data"][2] = 0;
							}
							else {
								valve_control_data["valve6_data"][2] = 1;
							}
						}
						if (p->name() == VALVE7_POSITION_MOVE) {
							valve_control_data["valve7_data"][0] = 7;
							valve_control_data["valve7_data"][1] =  p->value().toInt();
						}
						if (p->name() == VALVE7_DIRECTION) {
							valve7_direction = p->value().c_str();
							if (valve7_direction == "Close") {
								valve_control_data["valve7_data"][2] = 0;
							}
							else {
								valve_control_data["valve7_data"][2] = 1;
							}
						}
						if (p->name() == VALVE8_POSITION_MOVE) {
							valve_control_data["valve8_data"][0] = 8;
							valve_control_data["valve8_data"][1] =  p->value().toInt();
							}
							if (p->name() == VALVE8_DIRECTION) {
							valve8_direction = p->value().c_str();
							if (valve8_direction == "Close") {
								valve_control_data["valve8_data"][2] = 0;
							}
							else {
								valve_control_data["valve8_data"][2] = 1;
							}
						}
						if (p->name() == VALVE9_POSITION_MOVE) {
							valve_control_data["valve9_data"][0] = 9;
							valve_control_data["valve9_data"][1] =  p->value().toInt();
							}
							if (p->name() == VALVE9_DIRECTION) {
							valve9_direction = p->value().c_str();
							if (valve9_direction == "Close") {
								valve_control_data["valve9_data"][2] = 0;
							}
							else {
								valve_control_data["valve9_data"][2] = 1;
							}
						}
						if (p->name() == VALVE10_POSITION_MOVE) {
							valve_control_data["valve10_data"][0] = 10;
							valve_control_data["valve10_data"][1] =  p->value().toInt();
						}
						if (p->name() == VALVE10_DIRECTION) {
							valve10_direction = p->value().c_str();
							if (valve10_direction == "Close") {
								valve_control_data["valve10_data"][2] = 0;
							}
							else {
								valve_control_data["valve10_data"][2] = 1;
							}
						}
						if (p->name() == VALVE11_POSITION_MOVE) {
							valve_control_data["valve11_data"][0] = 11;
							valve_control_data["valve11_data"][1] =  p->value().toInt();
							}
							if (p->name() == VALVE11_DIRECTION) {
							valve11_direction = p->value().c_str();
							if (valve11_direction == "Close") {
								valve_control_data["valve11_data"][2] = 0;
							}
							else {
								valve_control_data["valve11_data"][2] = 1;
							}
						}
						if (p->name() == STORE_VALVE_POSITION_IN_FILE) {
							store_valve_position_in_file = p->value().c_str();
							if (store_valve_position_in_file == "true") {
								valve_control_data["checks"][0] = 1;
							}
							else {
								valve_control_data["checks"][0] = 0;
							}
						}
						if (p->name() == CHECK_VALVE_POSITION) {
							check_valve_position = p->value().c_str();
							if (check_valve_position == "true") {
								valve_control_data["checks"][1] = 1;
							}
							else {
								valve_control_data["checks"][1] = 0;
							}
						}
					}
				}
				xSemaphoreGive(valve_control_data_mutex);
			}
		}
		request->send(200, "text/html", valvecontrol_html);
		
		// Disable valve moving when valves are already moving
		if (lock_valve_move_mutex != NULL) {
			if(xSemaphoreTake(lock_valve_move_mutex, ( TickType_t ) 10 ) == pdTRUE) { 
				valve_move_locked = lock_valve_move;
				xSemaphoreGive(lock_valve_move_mutex);
			}
		}
		
		// Only move valves when not moving by another function
		if (valve_move_locked == 0) {
			xTaskNotifyGive(xTaskGetHandle("task_valvectrl"));
		}
		else {
			message = "Valves are locked for moving, try again";
			print_message(message);
		} });

	// POST on button create config file - name must match with action of the form submit
	server.on("/create_config_file", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		valve_status_file_create();
		request->send(200, "text/html", valvecontrol_html); });

	// POST on button delete config file - name must match with action of the form submit
	server.on("/delete_config_file", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		const char* path = "/json/valvepositions.json";
		delete_file(path);
		request->send(200, "text/html", valvecontrol_html); });

	// Stop statemachine
	server.on("/stop_statemachine", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		if (statemachine_state_mutex != NULL) {
			if(xSemaphoreTake(statemachine_state_mutex, ( TickType_t ) 10 ) == pdTRUE) {
				state = "stopped";
				xSemaphoreGive(statemachine_state_mutex);
			}
		}
		request->send(200, "text/html", valvecontrol_html); });

	// Start statemachine, back to init state
	server.on("/start_statemachine", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		if (statemachine_state_mutex != NULL) {
			if(xSemaphoreTake(statemachine_state_mutex, ( TickType_t ) 10 ) == pdTRUE) {
				state = "init";
				xSemaphoreGive(statemachine_state_mutex);
			}
		}
		request->send(200, "text/html", valvecontrol_html); });

	// Sensor config web page processing
	server.on("/sensorconfig", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(200, "text/html", sensor_config_html); });

	// Delete sensor config file 1
	server.on("/delete_sensor_config_file1", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		const char* path = "/json/sensor_config1.json";
		delete_file(path);
		request->send(200, "text/html", sensor_config_html); });

	// Delete sensor config file 2
	server.on("/delete_sensor_config_file2", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		const char* path = "/json/sensor_config2.json";
		delete_file(path);
		request->send(200, "text/html", sensor_config_html); });

	// Write sensor config input to file
	server.on("/sensorconfig1", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		
		int params = request->params();
		const char* path1 = "/json/sensor_config1.json";
		String sensor_config1;

		for(int i=0;i<params;i++){
			const AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				if (p->name() == WIRE_SENSOR0_TYPE) 
					wire_sensor_data["wire_sensor0_type"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR0_VALVE) 
					wire_sensor_data["wire_sensor0_valve"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR0_LOCATION) 
					wire_sensor_data["wire_sensor0_location"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR0_RH) 
					wire_sensor_data["wire_sensor0_rh"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR0_CO2) 
					wire_sensor_data["wire_sensor0_co2"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR1_TYPE) 
					wire_sensor_data["wire_sensor1_type"] = p->value().c_str();   
				if (p->name() == WIRE_SENSOR1_VALVE)
					wire_sensor_data["wire_sensor1_valve"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR1_LOCATION)
					wire_sensor_data["wire_sensor1 location"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR1_RH)
					wire_sensor_data["wire_sensor1_rh"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR1_CO2)
					wire_sensor_data["wire_sensor1_co2"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR2_TYPE)
					wire_sensor_data["wire_sensor2_type"] = p->value().c_str();  
				if (p->name() == WIRE_SENSOR2_VALVE)
					wire_sensor_data["wire_sensor2_valve"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR2_LOCATION)
					wire_sensor_data["wire_sensor2_location"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR2_RH)
					wire_sensor_data["wire_sensor2_rh"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR2_CO2)
					wire_sensor_data["wire_sensor2_co2"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR3_TYPE)
					wire_sensor_data["wire_sensor3_type"] = p->value().c_str();    
				if (p->name() == WIRE_SENSOR3_VALVE)
					wire_sensor_data["wire_sensor3_valve"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR3_LOCATION)
					wire_sensor_data["wire_sensor3_location"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR3_RH)
					wire_sensor_data["wire_sensor3_rh"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR3_CO2)
					wire_sensor_data["wire_sensor3_co2"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR4_TYPE)
					wire_sensor_data["wire_sensor4_type"] = p->value().c_str();  
				if (p->name() == WIRE_SENSOR4_VALVE)
					wire_sensor_data["wire_sensor4_valve"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR4_LOCATION)
					wire_sensor_data["wire_sensor4_location"] = p->value().c_str(); 
				if (p->name() == WIRE_SENSOR4_RH)
					wire_sensor_data["wire_sensor4_rh"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR4_CO2)
					wire_sensor_data["wire_sensor4_co2"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR5_TYPE)
					wire_sensor_data["wire_sensor5_type"] = p->value().c_str();    
				if (p->name() == WIRE_SENSOR5_VALVE)
					wire_sensor_data["wire_sensor5_valve"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR5_LOCATION)
					wire_sensor_data["wire_sensor5_location"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR5_RH)
					wire_sensor_data["wire_sensor6_rh"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR5_CO2)
					wire_sensor_data["wire_sensor6_co2"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR6_TYPE)
					wire_sensor_data["wire_sensor6_type"] = p->value().c_str();   
				if (p->name() == WIRE_SENSOR6_VALVE)
					wire_sensor_data["wire_sensor6_valve"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR6_LOCATION)
					wire_sensor_data["wire_sensor6_location"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR6_RH)
					wire_sensor_data["wire_sensor6_rh"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR6_CO2)
					wire_sensor_data["wire_sensor6_co2"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR7_TYPE)
					wire_sensor_data["wire_sensor7_type"] = p->value().c_str();   
				if (p->name() == WIRE_SENSOR7_VALVE)
					wire_sensor_data["wire_sensor7_valve"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR7_LOCATION)
					wire_sensor_data["wire_sensor7_location"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR7_RH)
					wire_sensor_data["wire_sensor7_rh"] = p->value().c_str();
				if (p->name() == WIRE_SENSOR7_CO2)
					wire_sensor_data["wire_sensor7_co2"] = p->value().c_str();
			}
		}
		serializeJson(wire_sensor_data, sensor_config1);
		write_config_file(path1, sensor_config1);
		request->send(200, "text/html", sensor_config_html); });

	server.on("/sensorconfig2", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		int params = request->params();
		const char* path2 = "/json/sensor_config2.json";
		String sensor_config2;
		
		for(int i=0;i<params;i++){
			const AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()) {
				if (p->name() == WIRE1_SENSOR0_TYPE)
					wire1_sensor_data["wire1_sensor0_type"] = p->value().c_str(); 
				if (p->name() == WIRE1_SENSOR0_VALVE)
				  	wire1_sensor_data["wire1_sensor0_valve"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR0_LOCATION)
				  	wire1_sensor_data["wire1_sensor0_location"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR0_RH)
				  	wire1_sensor_data["wire1_sensor0_rh"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR0_CO2)
				  	wire1_sensor_data["wire1_sensor0_co2"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR1_TYPE)
					wire1_sensor_data["wire1_sensor1_type"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR1_VALVE)
					wire1_sensor_data["wire1_sensor1_valve"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR1_LOCATION)
					wire1_sensor_data["wire1_sensor1_location"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR1_RH)
					wire1_sensor_data["wire1_sensor1_rh"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR1_CO2)
					wire1_sensor_data["wire1_sensor1_co2"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR2_TYPE)
					wire1_sensor_data["wire1_sensor2_type"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR2_VALVE)
					wire1_sensor_data["wire1_sensor2_valve"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR2_LOCATION)
					wire1_sensor_data["wire1_sensor2_location"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR2_RH)
					wire1_sensor_data["wire1_sensor2_rh"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR2_CO2)
					wire1_sensor_data["wire1_sensor2_co2"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR3_TYPE)
					wire1_sensor_data["wire1_sensor3_type"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR3_VALVE)
					wire1_sensor_data["wire1_sensor3_valve"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR3_LOCATION)
					wire1_sensor_data["wire1_sensor3_location"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR3_RH)
					wire1_sensor_data["wire1_sensor3_rh"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR3_CO2)
					wire1_sensor_data["wire1_sensor3_co2"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR4_TYPE)
					wire1_sensor_data["wire1_sensor4_type"] = p->value().c_str();  
				if (p->name() == WIRE1_SENSOR4_VALVE)
					wire1_sensor_data["wire1_sensor4_valve"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR4_LOCATION)
					wire1_sensor_data["wire1_sensor4_location"] = p->value().c_str(); 
				if (p->name() == WIRE1_SENSOR4_RH)
					wire1_sensor_data["wire1_sensor4_rh"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR4_CO2)
					wire1_sensor_data["wire1_sensor4_co2"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR5_TYPE)
					wire1_sensor_data["wire1_sensor5_type"] = p->value().c_str();   
				if (p->name() == WIRE1_SENSOR5_VALVE)
					wire1_sensor_data["wire1_sensor5_valve"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR5_LOCATION)
					wire1_sensor_data["wire1_sensor5_location"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR5_RH)
					wire1_sensor_data["wire1_sensor5_rh"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR5_CO2)
					wire1_sensor_data["wire1_sensor5_co2"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR6_TYPE)
					wire1_sensor_data["wire1_sensor6_type"] = p->value().c_str(); 
				if (p->name() == WIRE1_SENSOR6_VALVE)
					wire1_sensor_data["wire1_sensor6_valve"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR6_LOCATION)
				  	wire1_sensor_data["wire1_sensor6_location"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR6_RH)
				  	wire1_sensor_data["wire1_sensor6_rh"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR6_CO2)
				  	wire1_sensor_data["wire1_sensor6_co2"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR7_TYPE)
				  	wire1_sensor_data["wire1_sensor7_type"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR7_VALVE)
				  	wire1_sensor_data["wire1_sensor7_valve"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR7_LOCATION)
				  	wire1_sensor_data["wire1_sensor7_location"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR7_RH)
				  	wire1_sensor_data["wire1_sensor7_rh"] = p->value().c_str();
				if (p->name() == WIRE1_SENSOR7_CO2)
				  	wire1_sensor_data["wire1_sensor7_co2"] = p->value().c_str();
			}
		}
		serializeJson(wire1_sensor_data, sensor_config2);
		write_config_file(path2, sensor_config2);
		request->send(200, "text/html", sensor_config_html); });

	// Statemachine web pages processing
	server.on("/statemachine", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send(200, "text/html", statemachine_html); });

	// Save settings from statemachine settings
	server.on("/settings_statemachine", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		if (settings_statemachine_mutex != NULL) {
			if(xSemaphoreTake(settings_statemachine_mutex, ( TickType_t ) 10 ) == pdTRUE) {
				int params = request->params();
				for(int i=0;i<params;i++){
					const AsyncWebParameter* p = request->getParam(i);
					if(p->isPost()){
						if (p->name() == STATUS_STATEMACHINE_CONFIG)
							settings_statemachine_data["status_statemachine_config"] = p->value().c_str();
						if (p->name() == WEEKDAY_DAY_HOUR_START)
							settings_statemachine_data["weekday_day_hour_start"] = p->value().c_str();
						if (p->name() == WEEKDAY_DAY_MINUTE_START)
							settings_statemachine_data["weekday_day_minute_start"] = p->value().c_str();
						if (p->name() == WEEKDAY_NIGHT_HOUR_START)
							settings_statemachine_data["weekday_night_hour_start"] = p->value().c_str();
						if (p->name() == WEEKDAY_NIGHT_MINUTE_START)
							settings_statemachine_data["weekday_night_minute_start"] = p->value().c_str();
						if (p->name() == WEEKEND_DAY_HOUR_START)
							settings_statemachine_data["weekend_day_hour_start"] = p->value().c_str();
						if (p->name() == WEEKEND_DAY_MINUTE_START)
							settings_statemachine_data["weekend_day_minute_start"] = p->value().c_str();
						if (p->name() == WEEKEND_NIGHT_HOUR_START)
							settings_statemachine_data["weekend_night_hour_start"] = p->value().c_str();
						if (p->name() == WEEKEND_NIGHT_MINUTE_START)
							settings_statemachine_data["weekend_night_minute_start"] = p->value().c_str();
						if (p->name() == WEEKEND_DAY_1)
							settings_statemachine_data["weekend_day_1"] = p->value().c_str();
						if (p->name() == WEEKEND_DAY_2)
							settings_statemachine_data["weekend_day_2"] = p->value().c_str();
						if (p->name() == MINIMUM_STATE_TIME)
							settings_statemachine_data["minimum_state_time"] = p->value().c_str();
					}
				}
				const char* path = "/json/settings_statemachine.json";
				String settings_statemachine_str;
				serializeJson(settings_statemachine_data, settings_statemachine_str);
				write_config_file(path, settings_statemachine_str);
				xSemaphoreGive(settings_statemachine_mutex);
			}
		}
		request->send(200, "text/html", statemachine_html); });

	// Settings statemachine day
	server.on("/settings_valve_day", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		
		int params = request->params();
		const char* path_day = "/json/settings_state_day.json";
		String settings_state_day_str;

		for(int i=0;i<params;i++) {
			const AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()) {
				if (p->name() == ENABLE_STATE_DAY)
					settings_state_day["enable_state_day"] = p->value().c_str();
				if (p->name() == STATE_DAY_FANSPEED)
					settings_state_day["state_day_fanspeed"] = p->value().c_str();
				if (p->name() == NAME_STATE_DAY)
					settings_state_day["name_state_day"] = p->value().c_str();
				if (p->name() == VALVE0_POSITION_DAY) 
					settings_state_day["valve0_position_day"] = p->value().c_str();
				if (p->name() ==  VALVE1_POSITION_DAY) 
					settings_state_day["valve1_position_day"] = p->value().c_str();
				if (p->name() ==  VALVE2_POSITION_DAY) 
					settings_state_day["valve2_position_day"] = p->value().c_str();
				if (p->name() ==  VALVE3_POSITION_DAY) 
					settings_state_day["valve3_position_day"] = p->value().c_str();
				if (p->name() ==  VALVE4_POSITION_DAY) 
					settings_state_day["valve4_position_day"] = p->value().c_str();
				if (p->name() ==  VALVE5_POSITION_DAY) 
					settings_state_day["valve5_position_day"] = p->value().c_str();
				if (p->name() ==  VALVE6_POSITION_DAY) 
					settings_state_day["valve6_position_day"] = p->value().c_str();
				if (p->name() ==  VALVE7_POSITION_DAY) 
					settings_state_day["valve7_position_day"] = p->value().c_str();
				if (p->name() ==  VALVE8_POSITION_DAY) 
					settings_state_day["valve8_position_day"] = p->value().c_str();
				if (p->name() ==  VALVE9_POSITION_DAY) 
					settings_state_day["valve9_position_day"] = p->value().c_str();
				if (p->name() ==  VALVE10_POSITION_DAY) 
					settings_state_day["valve10_position_day"] = p->value().c_str();
				if (p->name() ==  VALVE11_POSITION_DAY) 
					settings_state_day["valve11_position_day"] = p->value().c_str();
			}
		}
		serializeJson(settings_state_day, settings_state_day_str);
		write_config_file(path_day, settings_state_day_str);
		request->send(200, "text/html", statemachine_html); });

	// Settings statemachine night
	server.on("/settings_valve_night", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		
		int params = request->params();
		const char* path_night = "/json/settings_state_night.json";
		String settings_state_night_str;
		
		for(int i=0;i<params;i++){
			const AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				if (p->name() == ENABLE_STATE_NIGHT) 
					settings_state_night["enable_state_night"] = p->value().c_str();
				if (p->name() == STATE_NIGHT_FANSPEED)
					settings_state_day["state_night_fanspeed"] = p->value().c_str();
				if (p->name() == NAME_STATE_NIGHT) 
					settings_state_night["name_state_night"] = p->value().c_str();
				if (p->name() == VALVE0_POSITION_NIGHT)
					settings_state_night["valve0_position_night"] = p->value().c_str();
				if (p->name() ==  VALVE1_POSITION_NIGHT) 
					settings_state_night["valve1_position_night"] = p->value().c_str();
				if (p->name() ==  VALVE2_POSITION_NIGHT) 
					settings_state_night["valve2_position_night"] = p->value().c_str();
				if (p->name() ==  VALVE3_POSITION_NIGHT) 
					settings_state_night["valve3_position_night"] = p->value().c_str();
				if (p->name() ==  VALVE4_POSITION_NIGHT) 
					settings_state_night["valve4_position_night"] = p->value().c_str();
				if (p->name() ==  VALVE5_POSITION_NIGHT) 
					settings_state_night["valve5_position_night"] = p->value().c_str();
				if (p->name() ==  VALVE6_POSITION_NIGHT) 
					settings_state_night["valve6_position_night"] = p->value().c_str();
				if (p->name() ==  VALVE7_POSITION_NIGHT) 
					settings_state_night["valve7_position_night"] = p->value().c_str();
				if (p->name() ==  VALVE8_POSITION_NIGHT) 
					settings_state_night["valve8_position_night"] = p->value().c_str();
				if (p->name() ==  VALVE9_POSITION_NIGHT) 
					settings_state_night["valve9_position_night"] = p->value().c_str();
				if (p->name() ==  VALVE10_POSITION_NIGHT) 
					settings_state_night["valve10_position_night"] = p->value().c_str();
				if (p->name() ==  VALVE11_POSITION_NIGHT) 
					settings_state_night["valve11_position_night"] = p->value().c_str();
			}
		}
		serializeJson(settings_state_night, settings_state_night_str);
		write_config_file(path_night, settings_state_night_str);
		request->send(200, "text/html", statemachine_html); });

	// Settings statemachine highco2day
	server.on("/settings_valve_highco2day", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		
		int params = request->params();
		const char* path_highco2day = "/json/settings_state_highco2day.json";
		String settings_state_highco2day_str;

		for(int i=0;i<params;i++){
			const AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				if (p->name() == ENABLE_STATE_HIGHCO2DAY) 
					settings_state_highco2day["enable_state_highco2day"] = p->value().c_str();
				if (p->name() == STATE_HIGHCO2DAY_FANSPEED)
					settings_state_highco2day["state_highco2day_fanspeed"] = p->value().c_str();
				if (p->name() == NAME_STATE_HIGHCO2DAY) 
					settings_state_highco2day["name_state_highco2day"] = p->value().c_str();
				if (p->name() == CO2_LOW_STATE_HIGHCO2DAY) 
					settings_state_highco2day["co2_low_state_highco2day"] = p->value().c_str();
				if (p->name() == CO2_HIGH_STATE_HIGHCO2DAY) 
					settings_state_highco2day["co2_high_state_highco2day"] = p->value().c_str();
				if (p->name() == VALVE0_POSITION_HIGHCO2DAY)
					settings_state_highco2day["valve0_position_highco2day"] = p->value().c_str();
				if (p->name() ==  VALVE1_POSITION_HIGHCO2DAY) 
					settings_state_highco2day["valve1_position_highco2day"] = p->value().c_str();
				if (p->name() ==  VALVE2_POSITION_HIGHCO2DAY) 
					settings_state_highco2day["valve2_position_highco2day"] = p->value().c_str();
				if (p->name() ==  VALVE3_POSITION_HIGHCO2DAY) 
					settings_state_highco2day["valve3_position_highco2day"] = p->value().c_str();
				if (p->name() ==  VALVE4_POSITION_HIGHCO2DAY) 
					settings_state_highco2day["valve4_position_highco2day"] = p->value().c_str();
				if (p->name() ==  VALVE5_POSITION_HIGHCO2DAY) 
					settings_state_highco2day["valve5_position_highco2day"] = p->value().c_str();
				if (p->name() ==  VALVE6_POSITION_HIGHCO2DAY) 
					settings_state_highco2day["valve6_position_highco2day"] = p->value().c_str();
				if (p->name() ==  VALVE7_POSITION_HIGHCO2DAY) 
					settings_state_highco2day["valve7_position_highco2day"] = p->value().c_str();
				if (p->name() ==  VALVE8_POSITION_HIGHCO2DAY) 
					settings_state_highco2day["valve8_position_highco2day"] = p->value().c_str();
				if (p->name() ==  VALVE9_POSITION_HIGHCO2DAY) 
					settings_state_highco2day["valve9_position_highco2day"] = p->value().c_str();
				if (p->name() ==  VALVE10_POSITION_HIGHCO2DAY) 
					settings_state_highco2day["valve10_position_highco2day"] = p->value().c_str();
				if (p->name() ==  VALVE11_POSITION_HIGHCO2DAY) 
					settings_state_highco2day["valve11_position_highco2day"] = p->value().c_str();
			}
		}
		serializeJson(settings_state_highco2day, settings_state_highco2day_str);
		write_config_file(path_highco2day, settings_state_highco2day_str);
		request->send(200, "text/html", statemachine_html); });

	// Settings statemachine highco2night
	server.on("/settings_valve_highco2night", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		
		int params = request->params();
		const char* path_highco2night = "/json/settings_state_highco2night.json";
		String settings_state_highco2night_str;
		
		for(int i=0;i<params;i++){
			const AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				if (p->name() == ENABLE_STATE_HIGHCO2NIGHT) 
					settings_state_highco2night["enable_state_highco2night"] = p->value().c_str();
				if (p->name() == STATE_HIGHCO2NIGHT_FANSPEED)
					settings_state_highco2night["state_highco2night_fanspeed"] = p->value().c_str();
				if (p->name() == NAME_STATE_HIGHCO2NIGHT) 
					settings_state_highco2night["name_state_highco2night"] = p->value().c_str();
				if (p->name() == CO2_LOW_STATE_HIGHCO2NIGHT) 
					settings_state_highco2night["co2_low_state_highco2night"] = p->value().c_str();
				if (p->name() == CO2_HIGH_STATE_HIGHCO2NIGHT) 
					settings_state_highco2night["co2_high_state_highco2night"] = p->value().c_str();
				if (p->name() == VALVE0_POSITION_HIGHCO2NIGHT)
					settings_state_highco2night["valve0_position_highco2night"] = p->value().c_str();
				if (p->name() ==  VALVE1_POSITION_HIGHCO2NIGHT) 
					settings_state_highco2night["valve1_position_highco2night"] = p->value().c_str();
				if (p->name() ==  VALVE2_POSITION_HIGHCO2NIGHT) 
					settings_state_highco2night["valve2_position_highco2night"] = p->value().c_str();
				if (p->name() ==  VALVE3_POSITION_HIGHCO2NIGHT) 
					settings_state_highco2night["valve3_position_highco2night"] = p->value().c_str();
				if (p->name() ==  VALVE4_POSITION_HIGHCO2NIGHT) 
					settings_state_highco2night["valve4_position_highco2night"] = p->value().c_str();
				if (p->name() ==  VALVE5_POSITION_HIGHCO2NIGHT) 
					settings_state_highco2night["valve5_position_highco2night"] = p->value().c_str();
				if (p->name() ==  VALVE6_POSITION_HIGHCO2NIGHT) 
					settings_state_highco2night["valve6_position_highco2night"] = p->value().c_str();
				if (p->name() ==  VALVE7_POSITION_HIGHCO2NIGHT) 
					settings_state_highco2night["valve7_position_highco2night"] = p->value().c_str();
				if (p->name() ==  VALVE8_POSITION_HIGHCO2NIGHT) 
					settings_state_highco2night["valve8_position_highco2night"] = p->value().c_str();
				if (p->name() ==  VALVE9_POSITION_HIGHCO2NIGHT) 
					settings_state_highco2night["valve9_position_highco2night"] = p->value().c_str();
				if (p->name() ==  VALVE10_POSITION_HIGHCO2NIGHT) 
					settings_state_highco2night["valve10_position_highco2night"] = p->value().c_str();
				if (p->name() ==  VALVE11_POSITION_HIGHCO2NIGHT) 
					settings_state_highco2night["valve11_position_highco2night"] = p->value().c_str();
			}
		}
		serializeJson(settings_state_highco2night, settings_state_highco2night_str);
		write_config_file(path_highco2night, settings_state_highco2night_str);
		request->send(200, "text/html", statemachine_html); });

	// Settings statemachine highrhday
	server.on("/settings_valve_highrhday", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
    	
		int params = request->params();
		const char* path_highrhday = "/json/settings_state_highrhday.json";
		String settings_state_highrhday_str;
		
    	for(int i=0;i<params;i++){
			const AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				if (p->name() == ENABLE_STATE_HIGHRHDAY) 
					settings_state_highrhday["enable_state_highrhday"] = p->value().c_str();
				if (p->name() == STATE_HIGHRHDAY_FANSPEED)
					settings_state_highrhday["state_highrhday_fanspeed"] = p->value().c_str();
				if (p->name() == NAME_STATE_HIGHRHDAY) 
					settings_state_highrhday["name_state_highrhday"] = p->value().c_str();
				if (p->name() == RH_LOW_STATE_HIGHRHDAY) 
					settings_state_highrhday["rh_low_state_highrhday"] = p->value().c_str();
				if (p->name() == RH_HIGH_STATE_HIGHRHDAY) 
					settings_state_highrhday["rh_high_state_highrhday"] = p->value().c_str();
				if (p->name() == VALVE0_POSITION_HIGHRHDAY)
					settings_state_highrhday["valve0_position_highrhday"] = p->value().c_str();
				if (p->name() ==  VALVE1_POSITION_HIGHRHDAY) 
					settings_state_highrhday["valve1_position_highrhday"] = p->value().c_str();
				if (p->name() ==  VALVE2_POSITION_HIGHRHDAY) 
					settings_state_highrhday["valve2_position_highrhday"] = p->value().c_str();
				if (p->name() ==  VALVE3_POSITION_HIGHRHDAY) 
					settings_state_highrhday["valve3_position_highrhday"] = p->value().c_str();
				if (p->name() ==  VALVE4_POSITION_HIGHRHDAY) 
					settings_state_highrhday["valve4_position_highrhday"] = p->value().c_str();
				if (p->name() ==  VALVE5_POSITION_HIGHRHDAY) 
					settings_state_highrhday["valve5_position_highrhday"] = p->value().c_str();
				if (p->name() ==  VALVE6_POSITION_HIGHRHDAY) 
					settings_state_highrhday["valve6_position_highrhday"] = p->value().c_str();
				if (p->name() ==  VALVE7_POSITION_HIGHRHDAY) 
					settings_state_highrhday["valve7_position_highrhday"] = p->value().c_str();
				if (p->name() ==  VALVE8_POSITION_HIGHRHDAY) 
					settings_state_highrhday["valve8_position_highrhday"] = p->value().c_str();
				if (p->name() ==  VALVE9_POSITION_HIGHRHDAY) 
					settings_state_highrhday["valve9_position_highrhday"] = p->value().c_str();
				if (p->name() ==  VALVE10_POSITION_HIGHRHDAY) 
					settings_state_highrhday["valve10_position_highrhday"] = p->value().c_str();
				if (p->name() ==  VALVE11_POSITION_HIGHRHDAY) 
					settings_state_highrhday["valve11_position_highrhday"] = p->value().c_str();
			}
		}
		serializeJson(settings_state_highrhday, settings_state_highrhday_str);
		write_config_file(path_highrhday, settings_state_highrhday_str);
		request->send(200, "text/html", statemachine_html); });

	// Settings statemachine highrhnight
	server.on("/settings_valve_highrhnight", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		
		int params = request->params();
		const char* path_highrhnight = "/json/settings_state_highrhnight.json";
		String settings_state_highrhnight_str;

		for(int i=0;i<params;i++){
			const AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				if (p->name() == ENABLE_STATE_HIGHRHNIGHT) 
					settings_state_highrhnight["enable_state_highrhnight"] = p->value().c_str();
				if (p->name() == STATE_HIGHRHNIGHT_FANSPEED)
					settings_state_highrhnight["state_highrhnight_fanspeed"] = p->value().c_str();
				if (p->name() == NAME_STATE_HIGHRHNIGHT) 
					settings_state_highrhnight["name_state_highrhnight"] = p->value().c_str();
				if (p->name() == RH_LOW_STATE_HIGHRHNIGHT) 
					settings_state_highrhnight["rh_low_state_highrhnight"] = p->value().c_str();
				if (p->name() == RH_HIGH_STATE_HIGHRHNIGHT) 
					settings_state_highrhnight["rh_high_state_highrhnight"] = p->value().c_str();
				if (p->name() == VALVE0_POSITION_HIGHRHNIGHT)
					settings_state_highrhnight["valve0_position_highrhnight"] = p->value().c_str();
				if (p->name() ==  VALVE1_POSITION_HIGHRHNIGHT) 
					settings_state_highrhnight["valve1_position_highrhnight"] = p->value().c_str();
				if (p->name() ==  VALVE2_POSITION_HIGHRHNIGHT) 
					settings_state_highrhnight["valve2_position_highrhnight"] = p->value().c_str();
				if (p->name() ==  VALVE3_POSITION_HIGHRHNIGHT) 
					settings_state_highrhnight["valve3_position_highrhnight"] = p->value().c_str();
				if (p->name() ==  VALVE4_POSITION_HIGHRHNIGHT) 
					settings_state_highrhnight["valve4_position_highrhnight"] = p->value().c_str();
				if (p->name() ==  VALVE5_POSITION_HIGHRHNIGHT) 
					settings_state_highrhnight["valve5_position_highrhnight"] = p->value().c_str();
				if (p->name() ==  VALVE6_POSITION_HIGHRHNIGHT) 
					settings_state_highrhnight["valve6_position_highrhnight"] = p->value().c_str();
				if (p->name() ==  VALVE7_POSITION_HIGHRHNIGHT) 
					settings_state_highrhnight["valve7_position_highrhnight"] = p->value().c_str();
				if (p->name() ==  VALVE8_POSITION_HIGHRHNIGHT) 
					settings_state_highrhnight["valve8_position_highrhnight"] = p->value().c_str();
				if (p->name() ==  VALVE9_POSITION_HIGHRHNIGHT) 
					settings_state_highrhnight["valve9_position_highrhnight"] = p->value().c_str();
				if (p->name() ==  VALVE10_POSITION_HIGHRHNIGHT) 
					settings_state_highrhnight["valve10_position_highrhnight"] = p->value().c_str();
				if (p->name() ==  VALVE11_POSITION_HIGHRHNIGHT) 
					settings_state_highrhnight["valve11_position_highrhnight"] = p->value().c_str();
			}
		}
		serializeJson(settings_state_highrhnight, settings_state_highrhnight_str);
		write_config_file(path_highrhnight, settings_state_highrhnight_str);
		request->send(200, "text/html", statemachine_html); });

	// Settings statemachine cooking
	server.on("/settings_valve_cooking", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		
		int params = request->params();
		const char* path_cooking = "/json/settings_state_cooking.json";
		String settings_state_cooking_str;

		for(int i=0;i<params;i++){
			const AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				if (p->name() == ENABLE_STATE_COOKING) 
					settings_state_cooking["enable_state_cooking"] = p->value().c_str();
				if (p->name() == STATE_COOKING_FANSPEED)
					settings_state_cooking["state_cooking_fanspeed"] = p->value().c_str();
				if (p->name() == NAME_STATE_COOKING) 
					settings_state_cooking["name_state_cooking"] = p->value().c_str();
				if (p->name() == START_HOUR_STATE_COOKING) 
					settings_state_cooking["start_hour_state_cooking"] = p->value().c_str();
				if (p->name() == START_MIN_STATE_COOKING) 
					settings_state_cooking["start_min_state_cooking"] = p->value().c_str();
				if (p->name() == STOP_HOUR_STATE_COOKING) 
					settings_state_cooking["stop_hour_state_cooking"] = p->value().c_str();
				if (p->name() == STOP_MIN_STATE_COOKING) 
					settings_state_cooking["stop_min_state_cooking"] = p->value().c_str();
				if (p->name() == VALVE0_POSITION_COOKING)
					settings_state_cooking["valve0_position_cooking"] = p->value().c_str();
				if (p->name() ==  VALVE1_POSITION_COOKING) 
					settings_state_cooking["valve1_position_cooking"] = p->value().c_str();
				if (p->name() ==  VALVE2_POSITION_COOKING) 
					settings_state_cooking["valve2_position_cooking"] = p->value().c_str();
				if (p->name() ==  VALVE3_POSITION_COOKING) 
					settings_state_cooking["valve3_position_cooking"] = p->value().c_str();
				if (p->name() ==  VALVE4_POSITION_COOKING) 
					settings_state_cooking["valve4_position_cooking"] = p->value().c_str();
				if (p->name() ==  VALVE5_POSITION_COOKING) 
					settings_state_cooking["valve5_position_cooking"] = p->value().c_str();
				if (p->name() ==  VALVE6_POSITION_COOKING) 
					settings_state_cooking["valve6_position_cooking"] = p->value().c_str();
				if (p->name() ==  VALVE7_POSITION_COOKING) 
					settings_state_cooking["valve7_position_cooking"] = p->value().c_str();
				if (p->name() ==  VALVE8_POSITION_COOKING) 
					settings_state_cooking["valve8_position_cooking"] = p->value().c_str();
				if (p->name() ==  VALVE9_POSITION_COOKING) 
					settings_state_cooking["valve9_position_cooking"] = p->value().c_str();
				if (p->name() ==  VALVE10_POSITION_COOKING) 
					settings_state_cooking["valve10_position_cooking"] = p->value().c_str();
				if (p->name() ==  VALVE11_POSITION_COOKING) 
					settings_state_cooking["valve11_position_cooking"] = p->value().c_str();
			}
		}
		serializeJson(settings_state_cooking, settings_state_cooking_str);
		write_config_file(path_cooking, settings_state_cooking_str);
		request->send(200, "text/html", statemachine_html); });

	// Settings statemachine valvecyclingday
	server.on("/settings_valve_cyclingday", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		
		int params = request->params();
		const char* path_cyclingday = "/json/settings_state_cyclingday.json";
		String settings_state_cyclingday_str;

		for(int i=0;i<params;i++){
			const AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				if (p->name() == ENABLE_STATE_CYCLINGDAY) 
					settings_state_cyclingday["enable_state_cyclingday"] = p->value().c_str();
				if (p->name() == STATE_CYCLINGDAY_FANSPEED)
					settings_state_cyclingday["state_cyclingday_fanspeed"] = p->value().c_str();
				if (p->name() == NAME_STATE_CYCLINGDAY) 
					settings_state_cyclingday["name_state_cyclingday"] = p->value().c_str();
				if (p->name() == VALVE0_POSITION_CYCLINGDAY)
					settings_state_cyclingday["valve0_position_cyclingday"] = p->value().c_str();
				if (p->name() ==  VALVE1_POSITION_CYCLINGDAY) 
					settings_state_cyclingday["valve1_position_cyclingday"] = p->value().c_str();
				if (p->name() ==  VALVE2_POSITION_CYCLINGDAY) 
					settings_state_cyclingday["valve2_position_cyclingday"] = p->value().c_str();
				if (p->name() ==  VALVE3_POSITION_CYCLINGDAY) 
					settings_state_cyclingday["valve3_position_cyclingday"] = p->value().c_str();
				if (p->name() ==  VALVE4_POSITION_CYCLINGDAY) 
					settings_state_cyclingday["valve4_position_cyclingday"] = p->value().c_str();
				if (p->name() ==  VALVE5_POSITION_CYCLINGDAY) 
					settings_state_cyclingday["valve5_position_cyclingday"] = p->value().c_str();
				if (p->name() ==  VALVE6_POSITION_CYCLINGDAY) 
					settings_state_cyclingday["valve6_position_cyclingday"] = p->value().c_str();
				if (p->name() ==  VALVE7_POSITION_CYCLINGDAY) 
					settings_state_cyclingday["valve7_position_cyclingday"] = p->value().c_str();
				if (p->name() ==  VALVE8_POSITION_CYCLINGDAY) 
					settings_state_cyclingday["valve8_position_cyclingday"] = p->value().c_str();
				if (p->name() ==  VALVE9_POSITION_CYCLINGDAY) 
					settings_state_cyclingday["valve9_position_cyclingday"] = p->value().c_str();
				if (p->name() ==  VALVE10_POSITION_CYCLINGDAY) 
					settings_state_cyclingday["valve10_position_cyclingday"] = p->value().c_str();
				if (p->name() ==  VALVE11_POSITION_CYCLINGDAY) 
					settings_state_cyclingday["valve11_position_cyclingday"] = p->value().c_str();
			}
		}
		serializeJson(settings_state_cyclingday, settings_state_cyclingday_str);
		write_config_file(path_cyclingday, settings_state_cyclingday_str);
		request->send(200, "text/html", statemachine_html); });

	// Settings statemachine valvecyclingnight
	server.on("/settings_valve_cyclingnight", HTTP_POST, [](AsyncWebServerRequest *request)
			  {
		
		int params = request->params();
		const char* path_cyclingnight = "/json/settings_state_cyclingnight.json";
		String settings_state_cyclingnight_str;

		for(int i=0;i<params;i++){
			const AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				if (p->name() == ENABLE_STATE_CYCLINGNIGHT) 
					settings_state_cyclingnight["enable_state_cyclingnight"] = p->value().c_str();
				if (p->name() == STATE_CYCLINGNIGHT_FANSPEED)
					settings_state_cyclingnight["state_cyclingnight_fanspeed"] = p->value().c_str();
				if (p->name() == NAME_STATE_CYCLINGNIGHT) 
					settings_state_cyclingnight["name_state_cyclingnight"] = p->value().c_str();
				if (p->name() == VALVE0_POSITION_CYCLINGNIGHT)
					settings_state_cyclingnight["valve0_position_cyclingnight"] = p->value().c_str();
				if (p->name() ==  VALVE1_POSITION_CYCLINGNIGHT) 
					settings_state_cyclingnight["valve1_position_cyclingnight"] = p->value().c_str();
				if (p->name() ==  VALVE2_POSITION_CYCLINGNIGHT) 
					settings_state_cyclingnight["valve2_position_cyclingnight"] = p->value().c_str();
				if (p->name() ==  VALVE3_POSITION_CYCLINGNIGHT) 
					settings_state_cyclingnight["valve3_position_cyclingnight"] = p->value().c_str();
				if (p->name() ==  VALVE4_POSITION_CYCLINGNIGHT) 
					settings_state_cyclingnight["valve4_position_cyclingnight"] = p->value().c_str();
				if (p->name() ==  VALVE5_POSITION_CYCLINGNIGHT) 
					settings_state_cyclingnight["valve5_position_cyclingnight"] = p->value().c_str();
				if (p->name() ==  VALVE6_POSITION_CYCLINGNIGHT) 
					settings_state_cyclingnight["valve6_position_cyclingnight"] = p->value().c_str();
				if (p->name() ==  VALVE7_POSITION_CYCLINGNIGHT) 
					settings_state_cyclingnight["valve7_position_cyclingnight"] = p->value().c_str();
				if (p->name() ==  VALVE8_POSITION_CYCLINGNIGHT) 
					settings_state_cyclingnight["valve8_position_cyclingnight"] = p->value().c_str();
				if (p->name() ==  VALVE9_POSITION_CYCLINGNIGHT) 
					settings_state_cyclingnight["valve9_position_cyclingnight"] = p->value().c_str();
				if (p->name() ==  VALVE10_POSITION_CYCLINGNIGHT) 
					settings_state_cyclingnight["valve10_position_cyclingnight"] = p->value().c_str();
				if (p->name() ==  VALVE11_POSITION_CYCLINGNIGHT) 
					settings_state_cyclingnight["valve11_position_cyclingnight"] = p->value().c_str();
			}
		}
		serializeJson(settings_state_cyclingnight, settings_state_cyclingnight_str);
		write_config_file(path_cyclingnight, settings_state_cyclingnight_str);
		request->send(200, "text/html", statemachine_html); });

	// Start server
	server.begin();
	vTaskDelete(NULL);
}
