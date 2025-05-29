#include "task_web_processors.h"

String sensor_config_processor(const String& var) {
    
    const char* path1 = "/json/sensor_config1.json";
    const char* path2 = "/json/sensor_config2.json";
    const char* status;
    bool sensor_config_file1_present;
    bool sensor_config_file2_present;
   
    sensor_config_file1_present = check_file_exists(path1);

    if(sensor_config_file1_present == 1) {
        status = "<font color=\"green\">Sensor config file found.</font>";
        if (var == "STATUS_SENSOR_CONFIG1_FILE")
            return (status);
        if (var == "WIRE_SENSOR0_TYPE")
            return (wire_sensor_data["wire_sensor0"]["type"]);
        if (var == "WIRE_SENSOR0_ADDRESS")
            return (wire_sensor_data["wire_sensor0"]["address"]);
        if (var == "WIRE_SENSOR0_VALVE")
            return (wire_sensor_data["wire_sensor0"]["valve"]);
        if (var == "WIRE_SENSOR0_LOCATION")
            return (wire_sensor_data["wire_sensor0"]["location"]);
        if (var == "WIRE_SENSOR0_RH")
            return (wire_sensor_data["wire_sensor0"]["rh"]);
        if (var == "WIRE_SENSOR0_CO2")
            return (wire_sensor_data["wire_sensor0"]["co2"]);
        if (var == "WIRE_SENSOR1_TYPE")
            return (wire_sensor_data["wire_sensor1"]["type"]);
        if (var == "WIRE_SENSOR1_ADDRESS")
            return (wire_sensor_data["wire_sensor1"]["address"]);
        if (var == "WIRE_SENSOR1_VALVE")
            return (wire_sensor_data["wire_sensor1"]["valve"]);
        if (var == "WIRE_SENSOR0_LOCATION")
            return (wire_sensor_data["wire_sensor1"]["location"]);
        if (var == "WIRE_SENSOR1_RH")
            return (wire_sensor_data["wire_sensor1"]["rh"]);
        if (var == "WIRE_SENSOR0_CO2")
            return (wire_sensor_data["wire_sensor1"]["co2"]);
        if (var == "WIRE_SENSOR2_TYPE")
            return (wire_sensor_data["wire_sensor2"]["type"]);
        if (var == "WIRE_SENSOR2_ADDRESS")
            return (wire_sensor_data["wire_sensor2"]["address"]);
        if (var == "WIRE_SENSOR2_VALVE")
            return (wire_sensor_data["wire_sensor2"]["valve"]);
        if (var == "WIRE_SENSOR2_LOCATION")
            return (wire_sensor_data["wire_sensor2"]["location"]);
        if (var == "WIRE_SENSOR2_RH")
            return (wire_sensor_data["wire_sensor2"]["rh"]);
        if (var == "WIRE_SENSOR2_CO2")
            return (wire_sensor_data["wire_sensor2"]["co2"]);
        if (var == "WIRE_SENSOR3_TYPE")
            return (wire_sensor_data["wire_sensor3"]["type"]);
        if (var == "WIRE_SENSOR3_ADDRESS")
            return (wire_sensor_data["wire_sensor3"]["address"]);
        if (var == "WIRE_SENSOR3_VALVE")
            return (wire_sensor_data["wire_sensor3"]["valve"]);
        if (var == "WIRE_SENSOR3_LOCATION")
            return (wire_sensor_data["wire_sensor3"]["location"]);
        if (var == "WIRE_SENSOR3_RH")
            return (wire_sensor_data["wire_sensor3"]["rh"]);
        if (var == "WIRE_SENSOR3_CO2")
            return (wire_sensor_data["wire_sensor3"]["co2"]);
        if (var == "WIRE_SENSOR4_TYPE")
            return (wire_sensor_data["wire_sensor4"]["type"]);
        if (var == "WIRE_SENSOR4_ADDRESS")
            return (wire_sensor_data["wire_sensor4"]["address"]);
        if (var == "WIRE_SENSOR4_VALVE")
            return (wire_sensor_data["wire_sensor4"]["valve"]);
        if (var == "WIRE_SENSOR4_LOCATION")
            return (wire_sensor_data["wire_sensor4"]["location"]);
        if (var == "WIRE_SENSOR4_RH")
            return (wire_sensor_data["wire_sensor4"]["rh"]);
        if (var == "WIRE_SENSOR4_CO2")
            return (wire_sensor_data["wire_sensor4"]["co2"]);
        if (var == "WIRE_SENSOR5_TYPE")
            return (wire_sensor_data["wire_sensor5"]["type"]);
        if (var == "WIRE_SENSOR5_ADDRESS")
            return (wire_sensor_data["wire_sensor5"]["address"]);
        if (var == "WIRE_SENSOR5_VALVE")
            return (wire_sensor_data["wire_sensor5"]["valve"]);
        if (var == "WIRE_SENSOR5_LOCATION")
            return (wire_sensor_data["wire_sensor5"]["location"]);
        if (var == "WIRE_SENSOR5_RH")
            return (wire_sensor_data["wire_sensor5"]["rh"]);
        if (var == "WIRE_SENSOR5_CO2")
            return (wire_sensor_data["wire_sensor5"]["co2"]);
        if (var == "WIRE_SENSOR6_TYPE")
            return (wire_sensor_data["wire_sensor6"]["type"]);
        if (var == "WIRE_SENSOR6_ADDRESS")
            return (wire_sensor_data["wire_sensor6"]["address"]);
        if (var == "WIRE_SENSOR6_VALVE")
            return (wire_sensor_data["wire_sensor6"]["valve"]);
        if (var == "WIRE_SENSOR6_LOCATION")
            return (wire_sensor_data["wire_sensor6"]["location"]);
        if (var == "WIRE_SENSOR6_RH")
            return (wire_sensor_data["wire_sensor6"]["rh"]);
        if (var == "WIRE_SENSOR6_CO2")
            return (wire_sensor_data["wire_sensor6"]["co2"]);
        if (var == "WIRE_SENSOR7_TYPE")
            return (wire_sensor_data["wire_sensor7"]["type"]);
        if (var == "WIRE_SENSOR7_ADDRESS")
            return (wire_sensor_data["wire_sensor7"]["address"]);
        if (var == "WIRE_SENSOR7_VALVE")
            return (wire_sensor_data["wire_sensor7"]["valve"]);
        if (var == "WIRE_SENSOR7_LOCATION")
            return (wire_sensor_data["wire_sensor7"]["location"]);
        if (var == "WIRE_SENSOR7_RH")
            return (wire_sensor_data["wire_sensor7"]["rh"]);
        if (var == "WIRE_SENSOR7_CO2")
            return (wire_sensor_data["wire_sensor0"]["co2"]);
    }
    else {
        status = "<font color=\"red\">Sensor config file not found. Create a file with button below.</font>";
        if (var == "STATUS_SENSOR_CONFIG1_FILE")
            return F(status);
    }

    sensor_config_file2_present = check_file_exists(path2);

    if(sensor_config_file2_present == 1) {
        status = "<font color=\"green\">Sensor config file found.</font>";
        if (var == "STATUS_SENSOR_CONFIG2_FILE")
            return F(status);
        if (var == "WIRE1_SENSOR0_TYPE")
            return (wire1_sensor_data["wire1_sensor0"]["type"]);
        if (var == "WIRE1_SENSOR0_ADDRESS")
            return (wire1_sensor_data["wire1_sensor0"]["address"]);
        if (var == "WIRE1_SENSOR0_VALVE")
            return (wire1_sensor_data["wire1_sensor0"]["valve"]);
        if (var == "WIRE1_SENSOR0_LOCATION")
            return (wire1_sensor_data["wire1_sensor0"]["location"]);
        if (var == "WIRE1_SENSOR0_RH")
            return (wire1_sensor_data["wire1_sensor0"]["rh"]);
        if (var == "WIRE1_SENSOR0_CO2")
            return (wire1_sensor_data["wire1_sensor0"]["co2"]);
        if (var == "WIRE1_SENSOR1_TYPE")
            return (wire1_sensor_data["wire1_sensor1"]["type"]);
        if (var == "WIRE1_SENSOR1_ADDRESS")
            return (wire1_sensor_data["wire1_sensor1"]["address"]);
        if (var == "WIRE1_SENSOR1_VALVE")
            return (wire1_sensor_data["wire1_sensor1"]["valve"]);
        if (var == "WIRE1_SENSOR0_LOCATION")
            return (wire1_sensor_data["wire1_sensor1"]["location"]);
        if (var == "WIRE1_SENSOR1_RH")
            return (wire1_sensor_data["wire1_sensor1"]["rh"]);
        if (var == "WIRE1_SENSOR0_CO2")
            return (wire1_sensor_data["wire1_sensor1"]["co2"]);
        if (var == "WIRE1_SENSOR2_TYPE")
            return (wire1_sensor_data["wire1_sensor2"]["type"]);
        if (var == "WIRE1_SENSOR2_ADDRESS")
            return (wire1_sensor_data["wire1_sensor2"]["address"]);
        if (var == "WIRE1_SENSOR2_VALVE")
            return (wire1_sensor_data["wire1_sensor2"]["valve"]);
        if (var == "WIRE1_SENSOR2_LOCATION")
            return (wire1_sensor_data["wire1_sensor2"]["location"]);
        if (var == "WIRE1_SENSOR2_RH")
            return (wire1_sensor_data["wire1_sensor2"]["rh"]);
        if (var == "WIRE1_SENSOR2_CO2")
            return (wire1_sensor_data["wire1_sensor2"]["co2"]);
        if (var == "WIRE1_SENSOR3_TYPE")
            return (wire1_sensor_data["wire1_sensor3"]["type"]);
        if (var == "WIRE1_SENSOR3_ADDRESS")
            return (wire1_sensor_data["wire1_sensor3"]["address"]);
        if (var == "WIRE1_SENSOR3_VALVE")
            return (wire1_sensor_data["wire1_sensor3"]["valve"]);
        if (var == "WIRE1_SENSOR3_LOCATION")
            return (wire1_sensor_data["wire1_sensor3"]["location"]);
        if (var == "WIRE1_SENSOR3_RH")
            return (wire1_sensor_data["wire1_sensor3"]["rh"]);
        if (var == "WIRE1_SENSOR3_CO2")
            return (wire1_sensor_data["wire1_sensor3"]["co2"]);
        if (var == "WIRE1_SENSOR4_TYPE")
            return (wire1_sensor_data["wire1_sensor4"]["type"]);
        if (var == "WIRE1_SENSOR4_ADDRESS")
            return (wire1_sensor_data["wire1_sensor4"]["address"]);
        if (var == "WIRE1_SENSOR4_VALVE")
            return (wire1_sensor_data["wire1_sensor4"]["valve"]);
        if (var == "WIRE1_SENSOR4_LOCATION")
            return (wire1_sensor_data["wire1_sensor4"]["location"]);
        if (var == "WIRE1_SENSOR4_RH")
            return (wire1_sensor_data["wire1_sensor4"]["rh"]);
        if (var == "WIRE1_SENSOR4_CO2")
            return (wire1_sensor_data["wire1_sensor4"]["co2"]);
        if (var == "WIRE1_SENSOR5_TYPE")
            return (wire1_sensor_data["wire1_sensor5"]["type"]);
        if (var == "WIRE1_SENSOR5_ADDRESS")
            return (wire1_sensor_data["wire1_sensor5"]["address"]);
        if (var == "WIRE1_SENSOR5_VALVE")
            return (wire1_sensor_data["wire1_sensor5"]["valve"]);
        if (var == "WIRE1_SENSOR5_LOCATION")
            return (wire1_sensor_data["wire1_sensor5"]["location"]);
        if (var == "WIRE1_SENSOR5_RH")
            return (wire1_sensor_data["wire1_sensor5"]["rh"]);
        if (var == "WIRE1_SENSOR5_CO2")
            return (wire1_sensor_data["wire1_sensor5"]["co2"]);
        if (var == "WIRE1_SENSOR6_TYPE")
            return (wire1_sensor_data["wire1_sensor6"]["type"]);
        if (var == "WIRE1_SENSOR6_ADDRESS")
            return (wire1_sensor_data["wire1_sensor6"]["address"]);
        if (var == "WIRE1_SENSOR6_VALVE")
            return (wire1_sensor_data["wire1_sensor6"]["valve"]);
        if (var == "WIRE1_SENSOR6_LOCATION")
            return (wire1_sensor_data["wire1_sensor6"]["location"]);
        if (var == "WIRE1_SENSOR6_RH")
            return (wire1_sensor_data["wire1_sensor6"]["rh"]);
        if (var == "WIRE1_SENSOR6_CO2")
            return (wire1_sensor_data["wire1_sensor6"]["co2"]);
        if (var == "WIRE1_SENSOR7_TYPE")
            return (wire1_sensor_data["wire1_sensor7"]["type"]);
        if (var == "WIRE1_SENSOR7_ADDRESS")
            return (wire1_sensor_data["wire1_sensor7"]["address"]);
        if (var == "WIRE1_SENSOR7_VALVE")
            return (wire1_sensor_data["wire1_sensor7"]["valve"]);
        if (var == "WIRE1_SENSOR7_LOCATION")
            return (wire1_sensor_data["wire1_sensor7"]["location"]);
        if (var == "WIRE1_SENSOR7_RH")
            return (wire1_sensor_data["wire1_sensor7"]["rh"]);
        if (var == "WIRE1_SENSOR7_CO2")
            return (wire1_sensor_data["wire1_sensor7"]["co2"]);
    }
    else {
        status = "<font color=\"red\">Sensor config file not found. Create a file with button below.</font>";
        if (var == "STATUS_SENSOR_CONFIG2_FILE")
            return F(status);
    }

    return String();
}

String status_processor(const String& var) {

    /*Valve positions part of processor*/
    const char* path = "/json/valvepositions.json";
    //const char* status;
    bool status_file_present;
    
    String json;
    JsonDocument doc;
    
    if (valve_position_file_mutex != NULL) {
        if(xSemaphoreTake(valve_position_file_mutex, ( TickType_t ) 10 ) == pdTRUE) {
            //Serial.print("mutex taken");
            status_file_present = check_file_exists(path);

            if (status_file_present == 1) {

                json = read_config_file(path);
                deserializeJson(doc, json);
            }
            xSemaphoreGive(valve_position_file_mutex);
        }
    }

    if(var == "VALVE0_POS")
    return (doc[String("valve0")]);
    if(var == "VALVE1_POS")
    return (doc[String("valve1")]);
    if(var == "VALVE2_POS")
    return (doc[String("valve2")]);
    if(var == "VALVE3_POS")
    return (doc[String("valve3")]);
    if(var == "VALVE4_POS")
    return (doc[String("valve4")]);
    if(var == "VALVE5_POS")
    return (doc[String("valve5")]);
    if(var == "VALVE6_POS")
    return (doc[String("valve6")]);
    if(var == "VALVE7_POS")
    return (doc[String("valve7")]);
    if(var == "VALVE8_POS")
    return (doc[String("valve8")]);
    if(var == "VALVE9_POS")
    return (doc[String("valve9")]);
    if(var == "VALVE10_POS")
    return (doc[String("valve10")]);
    if(var == "VALVE11_POS")
    return (doc[String("valve11")]);

    //Copy data from queue in local variable
    float temp_sensor_data[2][8][3];
    if( sensor_queue != 0 ) {
        if (xQueuePeek(sensor_queue, &temp_sensor_data, ( TickType_t ) 10 )) { 
        }
    }     

    if(var == "BUS0_SENSOR0_TEMP")
        return (String(temp_sensor_data[0][0][0]));
    if(var == "BUS0_SENSOR0_HUM")
        return (String(temp_sensor_data[0][0][1]));
    if(var == "BUS0_SENSOR0_CO2")
        return (String(temp_sensor_data[0][0][2]));
    if(var == "BUS0_SENSOR1_TEMP")
        return (String(temp_sensor_data[0][1][0]));
    if(var == "BUS0_SENSOR1_HUM")
        return (String(temp_sensor_data[0][1][1]));
    if(var == "BUS0_SENSOR1_CO2")
        return (String(temp_sensor_data[0][1][2]));
    if(var == "BUS0_SENSOR2_TEMP")
        return (String(temp_sensor_data[0][2][0]));
    if(var == "BUS0_SENSOR2_HUM")
        return (String(temp_sensor_data[0][2][1]));
    if(var == "BUS0_SENSOR2_CO2")
        return (String(temp_sensor_data[0][2][2]));
    if(var == "BUS0_SENSOR3_TEMP")
        return (String(temp_sensor_data[0][3][0]));
    if(var == "BUS0_SENSOR3_HUM")
        return (String(temp_sensor_data[0][3][1]));
    if(var == "BUS0_SENSOR3_CO2")
        return (String(temp_sensor_data[0][3][2]));
    if(var == "BUS0_SENSOR4_TEMP")
        return (String(temp_sensor_data[0][4][0]));
    if(var == "BUS0_SENSOR4_HUM")
        return (String(temp_sensor_data[0][4][1]));
    if(var == "BUS0_SENSOR4_CO2")
        return (String(temp_sensor_data[0][4][2]));
    if(var == "BUS0_SENSOR5_TEMP")
        return (String(temp_sensor_data[0][5][0]));
    if(var == "BUS0_SENSOR5_HUM")
        return (String(temp_sensor_data[0][5][1]));
    if(var == "BUS0_SENSOR5_CO2")
        return (String(temp_sensor_data[0][5][2]));
    if(var == "BUS0_SENSOR6_TEMP")
        return (String(temp_sensor_data[0][6][0]));
    if(var == "BUS0_SENSOR6_HUM")
        return (String(temp_sensor_data[0][6][1]));
    if(var == "BUS0_SENSOR6_CO2")
        return (String(temp_sensor_data[0][6][2]));
    if(var == "BUS0_SENSOR7_TEMP")
        return (String(temp_sensor_data[0][7][0]));
    if(var == "BUS0_SENSOR7_HUM")
        return (String(temp_sensor_data[0][7][1]));
    if(var == "BUS0_SENSOR7_CO2")
        return (String(temp_sensor_data[0][7][2]));

    if(var == "BUS1_SENSOR0_TEMP")
        return (String(temp_sensor_data[1][0][0]));
    if(var == "BUS1_SENSOR0_HUM")
        return (String(temp_sensor_data[1][0][1]));
    if(var == "BUS1_SENSOR0_CO2")
        return (String(temp_sensor_data[1][0][2]));
    if(var == "BUS1_SENSOR1_TEMP")
        return (String(temp_sensor_data[1][1][0]));
    if(var == "BUS1_SENSOR1_HUM")
        return (String(temp_sensor_data[1][1][1]));
    if(var == "BUS1_SENSOR1_CO2")
        return (String(temp_sensor_data[1][1][2]));
    if(var == "BUS1_SENSOR2_TEMP")
        return (String(temp_sensor_data[1][2][0]));
    if(var == "BUS1_SENSOR2_HUM")
        return (String(temp_sensor_data[1][2][1]));
    if(var == "BUS1_SENSOR2_CO2")
        return (String(temp_sensor_data[1][2][2]));
    if(var == "BUS1_SENSOR3_TEMP")
        return (String(temp_sensor_data[1][3][0]));
    if(var == "BUS1_SENSOR3_HUM")
        return (String(temp_sensor_data[1][3][1]));
    if(var == "BUS1_SENSOR3_CO2")
        return (String(temp_sensor_data[1][3][2]));
    if(var == "BUS1_SENSOR4_TEMP")
        return (String(temp_sensor_data[1][4][0]));
    if(var == "BUS1_SENSOR4_HUM")
        return (String(temp_sensor_data[1][4][1]));
    if(var == "BUS1_SENSOR4_CO2")
        return (String(temp_sensor_data[1][4][2]));
    if(var == "BUS1_SENSOR5_TEMP")
        return (String(temp_sensor_data[1][5][0]));
    if(var == "BUS1_SENSOR5_HUM")
        return (String(temp_sensor_data[1][5][1]));
    if(var == "BUS1_SENSOR5_CO2")
        return (String(temp_sensor_data[1][5][2]));
    if(var == "BUS1_SENSOR6_TEMP")
        return (String(temp_sensor_data[1][6][0]));
    if(var == "BUS1_SENSOR6_HUM")
        return (String(temp_sensor_data[1][6][1]));
    if(var == "BUS1_SENSOR6_CO2")
        return (String(temp_sensor_data[1][6][2]));
    if(var == "BUS1_SENSOR7_TEMP")
        return (String(temp_sensor_data[1][7][0]));
    if(var == "BUS1_SENSOR7_HUM")
        return (String(temp_sensor_data[1][7][1]));
    if(var == "BUS1_SENSOR7_CO2")
        return (String(temp_sensor_data[1][7][2]));

    return String();
}

String valvecontrol_processor(const String& var) {

  const char* path = "/json/valvepositions.json";
  const char* status;
  bool status_file_present;
  
  String json;
  JsonDocument doc;
  
  status_file_present = check_file_exists(path);

  if (status_file_present == 1) {
    status = "<font color=\"green\">Valve status file found.</font>";
    if (var == "STATUS_VALVE_POSITION_FILE") {
      return F(status);
    }
  }
  else {
    status = "<font color=\"red\">Valve status file not found. Create a file with button below.</font>";
    if (var == "STATUS_VALVE_POSITION_FILE") {
      return F(status);
    }
  }

  return String();
}

String settings_processor(const String& var) {

    const char* settings_network_path = "/json/settings_network.json";
    const char* settings_mqtt_path = "/json/settings_mqtt.json";
    const char* settings_i2c_path = "/json/settings_i2c.json";
    const char* settings_fan_path = "/json/settings_fan.json";
    const char* settings_statemachine_path = "/json/settings_statemachine.json";
    const char* status = "";
    
    bool settings_network_file_present = 0;
    bool settings_mqtt_file_present = 0;
    bool settings_i2c_file_present = 0;
    bool settings_fan_file_present = 0;
    bool settings_statemachine_file_present = 0;

    String settings_network_json = "";
    String settings_mqtt_json = "";
    String settings_i2c_json = "";
    String settings_fan_json = "";
    String settings_statemachine_json = "";
    
    JsonDocument settings_network_doc;
    JsonDocument settings_mqtt_doc;
    JsonDocument settings_i2c_doc;
    JsonDocument settings_fan_doc;
    JsonDocument settings_statemachine_doc;
  
    /*Network settings processor*/
    if (settings_network_mutex != NULL) {
        if(xSemaphoreTake(settings_network_mutex, ( TickType_t ) 10 ) == pdTRUE) {
            settings_network_file_present = check_file_exists(settings_network_path);
            if (settings_network_file_present == 1) {
                settings_network_json = read_config_file(settings_network_path);
                deserializeJson(settings_network_doc, settings_network_json);
            }
            xSemaphoreGive(settings_network_mutex);
        }
    }

    if (settings_network_file_present == 1) {
        status = "<font color=\"green\">Network settings file found.</font>";
        if (var == "STATUS_NETWORK_CONFIG")
            return F(status);
        if(var == "ENABLE_DHCP")
            return (settings_network_doc[String("enable_dhcp")]);
        if(var == "SSID")
            return (settings_network_doc[String("ssid")]);
        if(var == "WIFI_PASSWORD")
            return (settings_network_doc[String("wifi_password")]);
        if(var == "IP_ADDRESS")
            return (settings_network_doc[String("ip_address")]);
        if(var == "SUBNET_MASK")
            return (settings_network_doc[String("subnet_mask")]);
        if(var == "GATEWAY")
            return (settings_network_doc[String("gateway")]);
        if(var == "PRIMARY_DNS")
            return (settings_network_doc[String("primary_dns")]);
        if(var == "SECONDARY_DNS")
            return (settings_network_doc[String("secondary_dns")]);
    }
    else {
        status = "<font color=\"red\">Network settings file not found.</font>";
        if (var == "STATUS_NETWORK_CONFIG") {
            return F(status);
        }
    }
    
    /*MQTT Settings processor*/
    if (settings_mqtt_mutex != NULL) {
        if(xSemaphoreTake(settings_mqtt_mutex, ( TickType_t ) 10 ) == pdTRUE) {
            settings_mqtt_file_present = check_file_exists(settings_mqtt_path);
            if (settings_mqtt_file_present == 1) {
                settings_mqtt_json = read_config_file(settings_mqtt_path);
                deserializeJson(settings_mqtt_doc, settings_mqtt_json);
            }
            xSemaphoreGive(settings_mqtt_mutex);
        }
    }

    if (settings_mqtt_file_present == 1) {
        status = "<font color=\"green\">MQTT settings file found.</font>";
        if (var == "STATUS_MQTT_CONFIG") 
            return F(status);
        if(var == "ENABLE_MQTT")
            return (settings_mqtt_doc[String("enable_mqtt")]);
        if(var == "MQTT_SERVER")
            return (settings_mqtt_doc[String("mqtt_server")]);
        if(var == "MQTT_PORT")
            return (settings_mqtt_doc[String("mqtt_port")]);
        if(var == "MQTT_BASE_TOPIC")
            return (settings_mqtt_doc[String("mqtt_base_topic")]);
    }
    else {
        status = "<font color=\"red\">MQTT settings file not found.</font>";
        if (var == "STATUS_MQTT_CONFIG") {
            return F(status);
        }
    }

    /*I2C Settings processor*/
    if (settings_i2c_mutex != NULL) {
        if(xSemaphoreTake(settings_i2c_mutex, ( TickType_t ) 10 ) == pdTRUE) {
            settings_i2c_file_present = check_file_exists(settings_i2c_path);
            if (settings_i2c_file_present == 1) {
                settings_i2c_json = read_config_file(settings_i2c_path);
                deserializeJson(settings_i2c_doc, settings_i2c_json);
            }
            xSemaphoreGive(settings_i2c_mutex);
        }
    }

    if (settings_i2c_file_present == 1) {
        status = "<font color=\"green\">I2C settings file found.</font>";
        if (var == "STATUS_I2C_HARDWARE_CONFIG")
            return F(status);
        if(var == "BUS0_MULTIPLEXER_ADDRESS")
            return (settings_i2c_doc[String("bus0_multiplexer_address")]);
        if(var == "BUS1_MULTIPLEXER_ADDRESS")
            return (settings_i2c_doc[String("bus1_multiplexer_address")]);
        if(var == "ENABLE_LCD")   
            return (settings_i2c_doc[String("enable_lcd")]);
        if(var == "DISPLAY_I2C_ADDRESS")
            return (settings_i2c_doc[String("display_i2c_address")]);
    }
    else {
        status = "<font color=\"red\">I2C settings file not found.</font>";
        if (var == "STATUS_I2C_HARDWARE_CONFIG") {
            return F(status);
        }
    }

    /*Fan settings processor*/
    if (settings_fan_mutex != NULL) {
        if(xSemaphoreTake(settings_fan_mutex, ( TickType_t ) 10 ) == pdTRUE) {
            settings_fan_file_present = check_file_exists(settings_fan_path);
            if (settings_fan_file_present == 1) {
                settings_fan_json = read_config_file(settings_fan_path);
                deserializeJson(settings_i2c_doc, settings_fan_json);
            }
            xSemaphoreGive(settings_fan_mutex);
        }
    }

    if (settings_fan_file_present == 1) {
        status = "<font color=\"green\">Fan settings file found.</font>";
        if (var == "STATUS_FAN_CONTROL_CONFIG")
            return F(status);
        if(var == "FAN_CONTROL_MODE")
            return (settings_i2c_doc[String("fan_control_mode")]);
        if(var == "FAN_CONTROL_MQTT_TOPIC")
            return (settings_i2c_doc[String("fan_control_mqtt_topic")]);
        if(var == "FAN_CONTROL_URL_HIGH_SPEED")
            return (settings_i2c_doc[String("fan_control_url_high_speed")]);
        if(var == "FAN_CONTROL_URL_MEDIUM_SPEED")
            return (settings_i2c_doc[String("fan_control_url_medium_speed")]);
        if(var == "FAN_CONTROL_URL_LOW_SPEED")
            return (settings_i2c_doc[String("fan_control_url_low_speed")]);
    }
    else {
        status = "<font color=\"red\">Fan settings file not found.</font>";
        if (var == "STATUS_FAN_CONTROL_CONFIG") {
            return F(status);
        }
    }

    /*Statemachine settings processor*/
    if (settings_statemachine_mutex != NULL) {
        if(xSemaphoreTake(settings_statemachine_mutex, ( TickType_t ) 10 ) == pdTRUE) {
            settings_statemachine_file_present = check_file_exists(settings_statemachine_path);
            if (settings_statemachine_file_present == 1) {
                settings_statemachine_json = read_config_file(settings_statemachine_path);
                deserializeJson(settings_statemachine_doc, settings_statemachine_json);
            }
            xSemaphoreGive(settings_statemachine_mutex);
        }
    }

    if (settings_statemachine_file_present == 1) {
        status = "<font color=\"green\">Statemachine settings file found.</font>";
        if (var == "STATUS_STATEMACHINE_CONFIG") 
            return F(status);
        if(var == "STATEMACHINE_RH_SENSOR")
            return (settings_statemachine_doc[String("statemachine_rh_sensor")]);
        if(var == "STATEMACHINE_CO2_SENSOR")
            return (settings_statemachine_doc[String("statemachine_co2_sensor")]);
    }
    else {
        status = "<font color=\"red\">Statemachine settings file not found.</font>";
        if (var == "STATUS_STATEMACHINE_CONFIG") {
            return F(status);
        }
    }

    return String();
}

String settings_valve_state(const String& var) {
    
    const char* settings_state_day_path = "/json/settings_state_day.json";
    const char* settings_state_night_path = "/json/settings_state_night.json";
    const char* settings_state_highco2day_path = "/json/settings_state_highco2day.json";
    const char* settings_state_highco2night_path = "/json/settings_state_highco2night.json";
    const char* settings_state_highrhday_path = "/json/settings_state_highrhday.json";
    const char* settings_state_highrhnight_path = "/json/settings_state_highrhnight.json";
    const char* settings_state_cooking_path = "/json/settings_state_cooking.json";
    const char* settings_state_cyclingday_path = "/json/settings_state_cyclingday.json";
    const char* settings_state_cyclingnight_path = "/json/settings_state_cyclingnight.json";
    const char* status;
    
    bool settings_state_day_present;
    bool settings_state_night_present;
    bool settings_state_highco2day_present;
    bool settings_state_highco2night_present;
    bool settings_state_highrhday_present;
    bool settings_state_highrhnight_present;
    bool settings_state_cooking_present;
    bool settings_state_cyclingday_present;
    bool settings_state_cyclingnight_present;
  
    /*Day state*/
    settings_state_day_present = check_file_exists(settings_state_day_path);
    if (settings_state_day_present == 1) {
        status = "<font color=\"green\">Settings ok.</font>";
        if (var == "STATUS_STATE_DAY_CONFIG") 
            return F(status);
        if(var == "ENABLE_STATE_DAY")
            return (settings_state_day[String("enable_state_day")]);
        if(var == "NAME_STATE_DAY")
            return (settings_state_day[String("name_state_day")]);
        if(var == "VALVE0_POSITION_DAY")
            return (settings_state_day[String("valve0_position_day")]);
        if(var == "VALVE1_POSITION_DAY")
            return (settings_state_day[String("valve1_position_day")]);
        if(var == "VALVE2_POSITION_DAY")
            return (settings_state_day[String("valve2_position_day")]);
        if(var == "VALVE3_POSITION_DAY")
            return (settings_state_day[String("valve3_position_day")]);
        if(var == "VALVE4_POSITION_DAY")
            return (settings_state_day[String("valve4_position_day")]);
        if(var == "VALVE5_POSITION_DAY")
            return (settings_state_day[String("valve5_position_day")]);
        if(var == "VALVE6_POSITION_DAY")
            return (settings_state_day[String("valve6_position_day")]);
        if(var == "VALVE7_POSITION_DAY")
            return (settings_state_day[String("valve7_position_day")]);
        if(var == "VALVE8_POSITION_DAY")
            return (settings_state_day[String("valve8_position_day")]);
        if(var == "VALVE9_POSITION_DAY")
            return (settings_state_day[String("valve9_position_day")]);
        if(var == "VALVE10_POSITION_DAY")
            return (settings_state_day[String("valve10_position_day")]);
        if(var == "VALVE11_POSITION_DAY")
            return (settings_state_day[String("valve11_position_day")]);
    }
    else {
        status = "<font color=\"red\">Settings file not found.</font>";
        if (var == "STATUS_STATE_DAY_CONFIG") {
            return F(status);
        }
    }

    /*Night state*/
    settings_state_night_present = check_file_exists(settings_state_night_path);
    if (settings_state_night_present == 1) {
        status = "<font color=\"green\">Settings Ok.</font>";
        if (var == "STATUS_STATE_NIGHT_CONFIG") 
            return F(status);
        if(var == "ENABLE_STATE_NIGHT")
            return (settings_state_night[String("enable_state_night")]);
        if(var == "NAME_STATE_NIGHT")
            return (settings_state_night[String("name_state_night")]);
        if(var == "VALVE0_POSITION_NIGHT")
            return (settings_state_night[String("valve0_position_night")]);
        if(var == "VALVE1_POSITION_NIGHT")
            return (settings_state_night[String("valve1_position_night")]);
        if(var == "VALVE2_POSITION_NIGHT")
            return (settings_state_night[String("valve2_position_night")]);
        if(var == "VALVE3_POSITION_NIGHT")
            return (settings_state_night[String("valve3_position_night")]);
        if(var == "VALVE4_POSITION_NIGHT")
            return (settings_state_night[String("valve4_position_night")]);
        if(var == "VALVE5_POSITION_NIGHT")
            return (settings_state_night[String("valve5_position_night")]);
        if(var == "VALVE6_POSITION_NIGHT")
            return (settings_state_night[String("valve6_position_night")]);
        if(var == "VALVE7_POSITION_NIGHT")
            return (settings_state_night[String("valve7_position_night")]);
        if(var == "VALVE8_POSITION_NIGHT")
            return (settings_state_night[String("valve8_position_night")]);
        if(var == "VALVE9_POSITION_NIGHT")
            return (settings_state_night[String("valve9_position_night")]);
        if(var == "VALVE10_POSITION_NIGHT")
            return (settings_state_night[String("valve10_position_night")]);
        if(var == "VALVE11_POSITION_NIGHT")
            return (settings_state_night[String("valve11_position_night")]);
    }
    else {
        status = "<font color=\"red\">Settings file not found.</font>";
        if (var == "STATUS_STATE_NIGHT_CONFIG") {
            return F(status);
        }
    }

    /*High CO2 day state*/
    settings_state_highco2day_present = check_file_exists(settings_state_highco2day_path);
    if (settings_state_highco2day_present == 1) {
        status = "<font color=\"green\">Settings Ok.</font>";
        if (var == "STATUS_STATE_HIGHCO2DAY_CONFIG") 
            return F(status);
        if(var == "ENABLE_STATE_HIGHCO2DAY")
            return (settings_state_highco2day[String("enable_state_highco2day")]);
        if(var == "NAME_STATE_HIGHCO2DAY")
            return (settings_state_highco2day[String("name_state_highco2day")]);
        if(var == "VALVE0_POSITION_HIGHCO2DAY")
            return (settings_state_highco2day[String("valve0_position_highco2day")]);
        if(var == "VALVE1_POSITION_HIGHCO2DAY")
            return (settings_state_highco2day[String("valve1_position_highco2day")]);
        if(var == "VALVE2_POSITION_HIGHCO2DAY")
            return (settings_state_highco2day[String("valve2_position_highco2day")]);
        if(var == "VALVE3_POSITION_HIGHCO2DAY")
            return (settings_state_highco2day[String("valve3_position_highco2day")]);
        if(var == "VALVE4_POSITION_HIGHCO2DAY")
            return (settings_state_highco2day[String("valve4_position_highco2day")]);
        if(var == "VALVE5_POSITION_HIGHCO2DAY")
            return (settings_state_highco2day[String("valve5_position_highco2day")]);
        if(var == "VALVE6_POSITION_HIGHCO2DAY")
            return (settings_state_highco2day[String("valve6_position_highco2day")]);
        if(var == "VALVE7_POSITION_HIGHCO2DAY")
            return (settings_state_highco2day[String("valve7_position_highco2day")]);
        if(var == "VALVE8_POSITION_HIGHCO2DAY")
            return (settings_state_highco2day[String("valve8_position_highco2day")]);
        if(var == "VALVE9_POSITION_HIGHCO2DAY")
            return (settings_state_highco2day[String("valve9_position_highco2day")]);
        if(var == "VALVE10_POSITION_HIGHCO2DAY")
            return (settings_state_highco2day[String("valve10_position_highco2day")]);
        if(var == "VALVE11_POSITION_HIGHCO2DAY")
            return (settings_state_highco2day[String("valve11_position_highco2day")]);
    }
    else {
        status = "<font color=\"red\">Settings file not found.</font>";
        if (var == "STATUS_STATE_HIGHCO2DAY_CONFIG") {
            return F(status);
        }
    }

    /*High CO2 night state*/
    settings_state_highco2night_present = check_file_exists(settings_state_highco2night_path);
    if (settings_state_highco2night_present == 1) {
        status = "<font color=\"green\">Settings ok.</font>";
        if (var == "STATUS_STATE_HIGHCO2NIGHT_CONFIG") 
            return F(status);
        if(var == "ENABLE_STATE_HIGHCO2NIGHT")
            return (settings_state_highco2night[String("enable_state_highco2night")]);
        if(var == "NAME_STATE_HIGHCO2NIGHT")
            return (settings_state_highco2night[String("name_state_highco2night")]);
        if(var == "VALVE0_POSITION_HIGHCO2NIGHT")
            return (settings_state_highco2night[String("valve0_position_highco2night")]);
        if(var == "VALVE1_POSITION_HIGHCO2NIGHT")
            return (settings_state_highco2night[String("valve1_position_highco2night")]);
        if(var == "VALVE2_POSITION_HIGHCO2NIGHT")
            return (settings_state_highco2night[String("valve2_position_highco2night")]);
        if(var == "VALVE3_POSITION_HIGHCO2NIGHT")
            return (settings_state_highco2night[String("valve3_position_highco2night")]);
        if(var == "VALVE4_POSITION_HIGHCO2NIGHT")
            return (settings_state_highco2night[String("valve4_position_highco2night")]);
        if(var == "VALVE5_POSITION_HIGHCO2NIGHT")
            return (settings_state_highco2night[String("valve5_position_highco2night")]);
        if(var == "VALVE6_POSITION_HIGHCO2NIGHT")
            return (settings_state_highco2night[String("valve6_position_highco2night")]);
        if(var == "VALVE7_POSITION_HIGHCO2NIGHT")
            return (settings_state_highco2night[String("valve7_position_highco2night")]);
        if(var == "VALVE8_POSITION_HIGHCO2NIGHT")
            return (settings_state_highco2night[String("valve8_position_highco2night")]);
        if(var == "VALVE9_POSITION_HIGHCO2NIGHT")
            return (settings_state_highco2night[String("valve9_position_highco2night")]);
        if(var == "VALVE10_POSITION_HIGHCO2NIGHT")
            return (settings_state_highco2night[String("valve10_position_highco2night")]);
        if(var == "VALVE11_POSITION_HIGHCO2NIGHT")
            return (settings_state_highco2night[String("valve11_position_highco2night")]);
    }
    else {
        status = "<font color=\"red\">Settings file not found.</font>";
        if (var == "STATUS_STATE_HIGHCO2NIGHT_CONFIG") {
            return F(status);
        }
    }

    /*High RH day state*/
    settings_state_highrhday_present = check_file_exists(settings_state_highrhday_path);
    if (settings_state_highrhday_present == 1) {
        status = "<font color=\"green\">Settings ok.</font>";
        if (var == "STATUS_STATE_HIGHRHDAY_CONFIG") 
            return F(status);
        if(var == "ENABLE_STATE_HIGHRHDAY")
            return (settings_state_highrhday[String("enable_state_highrhday")]);
        if(var == "NAME_STATE_HIGHRHDAY")
            return (settings_state_highrhday[String("name_state_highrhday")]);
        if(var == "VALVE0_POSITION_HIGHRHDAY")
            return (settings_state_highrhday[String("valve0_position_highrhday")]);
        if(var == "VALVE1_POSITION_HIGHRHDAY")
            return (settings_state_highrhday[String("valve1_position_highrhday")]);
        if(var == "VALVE2_POSITION_HIGHRHDAY")
            return (settings_state_highrhday[String("valve2_position_highrhday")]);
        if(var == "VALVE3_POSITION_HIGHRHDAY")
            return (settings_state_highrhday[String("valve3_position_highrhday")]);
        if(var == "VALVE4_POSITION_HIGHRHDAY")
            return (settings_state_highrhday[String("valve4_position_highrhday")]);
        if(var == "VALVE5_POSITION_HIGHRHDAY")
            return (settings_state_highrhday[String("valve5_position_highrhday")]);
        if(var == "VALVE6_POSITION_HIGHRHDAY")
            return (settings_state_highrhday[String("valve6_position_highrhday")]);
        if(var == "VALVE7_POSITION_HIGHRHDAY")
            return (settings_state_highrhday[String("valve7_position_highrhday")]);
        if(var == "VALVE8_POSITION_HIGHRHDAY")
            return (settings_state_highrhday[String("valve8_position_highrhday")]);
        if(var == "VALVE9_POSITION_HIGHRHDAY")
            return (settings_state_highrhday[String("valve9_position_highrhday")]);
        if(var == "VALVE10_POSITION_HIGHRHDAY")
            return (settings_state_highrhday[String("valve10_position_highrhday")]);
        if(var == "VALVE11_POSITION_HIGHRHDAY")
            return (settings_state_highrhday[String("valve11_position_highrhday")]);
    }
    else {
        status = "<font color=\"red\">Settings file not found.</font>";
        if (var == "STATUS_STATE_HIGHRHDAY_CONFIG") {
            return F(status);
        }
    }

    /*High RH night state*/
    settings_state_highrhnight_present = check_file_exists(settings_state_highrhnight_path);
    if (settings_state_highrhnight_present == 1) {
        status = "<font color=\"green\">Settings ok.</font>";
        if (var == "STATUS_STATE_HIGHRHNIGHT_CONFIG") 
            return F(status);
        if(var == "ENABLE_STATE_HIGHRHNIGHT")
            return (settings_state_highrhnight[String("enable_state_highrhnight")]);
        if(var == "NAME_STATE_HIGHRHNIGHT")
            return (settings_state_highrhnight[String("name_state_highrhnight")]);
        if(var == "VALVE0_POSITION_HIGHRHNIGHT")
            return (settings_state_highrhnight[String("valve0_position_highrhnight")]);
        if(var == "VALVE1_POSITION_HIGHRHNIGHT")
            return (settings_state_highrhnight[String("valve1_position_highrhnight")]);
        if(var == "VALVE2_POSITION_HIGHRHNIGHT")
            return (settings_state_highrhnight[String("valve2_position_highrhnight")]);
        if(var == "VALVE3_POSITION_HIGHRHNIGHT")
            return (settings_state_highrhnight[String("valve3_position_highrhnight")]);
        if(var == "VALVE4_POSITION_HIGHRHNIGHT")
            return (settings_state_highrhnight[String("valve4_position_highrhnight")]);
        if(var == "VALVE5_POSITION_HIGHRHNIGHT")
            return (settings_state_highrhnight[String("valve5_position_highrhnight")]);
        if(var == "VALVE6_POSITION_HIGHRHNIGHT")
            return (settings_state_highrhnight[String("valve6_position_highrhnight")]);
        if(var == "VALVE7_POSITION_HIGHRHNIGHT")
            return (settings_state_highrhnight[String("valve7_position_highrhnight")]);
        if(var == "VALVE8_POSITION_HIGHRHNIGHT")
            return (settings_state_highrhnight[String("valve8_position_highrhnight")]);
        if(var == "VALVE9_POSITION_HIGHRHNIGHT")
            return (settings_state_highrhnight[String("valve9_position_highrhnight")]);
        if(var == "VALVE10_POSITION_HIGHRHNIGHT")
            return (settings_state_highrhnight[String("valve10_position_highrhnight")]);
        if(var == "VALVE11_POSITION_HIGHRHNIGHT")
            return (settings_state_highrhnight[String("valve11_position_highrhnight")]);
    }
    else {
        status = "<font color=\"red\">Settings file not found.</font>";
        if (var == "STATUS_STATE_HIGHRHNIGHT_CONFIG") {
            return F(status);
        }
    }

    /*Cooking state*/
    settings_state_cooking_present = check_file_exists(settings_state_cooking_path);
    if (settings_state_cooking_present == 1) {
        status = "<font color=\"green\">Settings ok.</font>";
        if (var == "STATUS_STATE_COOKING_CONFIG") 
            return F(status);
        if(var == "ENABLE_STATE_COOKING")
            return (settings_state_cooking[String("enable_state_cooking")]);
        if(var == "NAME_STATE_COOKING")
            return (settings_state_cooking[String("name_state_cooking")]);
        if(var == "VALVE0_POSITION_COOKING")
            return (settings_state_cooking[String("valve0_position_cooking")]);
        if(var == "VALVE1_POSITION_COOKING")
            return (settings_state_cooking[String("valve1_position_cooking")]);
        if(var == "VALVE2_POSITION_COOKING")
            return (settings_state_cooking[String("valve2_position_cooking")]);
        if(var == "VALVE3_POSITION_COOKING")
            return (settings_state_cooking[String("valve3_position_cooking")]);
        if(var == "VALVE4_POSITION_COOKING")
            return (settings_state_cooking[String("valve4_position_cooking")]);
        if(var == "VALVE5_POSITION_COOKING")
            return (settings_state_cooking[String("valve5_position_cooking")]);
        if(var == "VALVE6_POSITION_COOKING")
            return (settings_state_cooking[String("valve6_position_cooking")]);
        if(var == "VALVE7_POSITION_COOKING")
            return (settings_state_cooking[String("valve7_position_cooking")]);
        if(var == "VALVE8_POSITION_COOKING")
            return (settings_state_cooking[String("valve8_position_cooking")]);
        if(var == "VALVE9_POSITION_COOKING")
            return (settings_state_cooking[String("valve9_position_cooking")]);
        if(var == "VALVE10_POSITION_COOKING")
            return (settings_state_cooking[String("valve10_position_cooking")]);
        if(var == "VALVE11_POSITION_COOKING")
            return (settings_state_cooking[String("valve11_position_cooking")]);
    }
    else {
        status = "<font color=\"red\">Settings file not found.</font>";
        if (var == "STATUS_STATE_COOKING_CONFIG") {
            return F(status);
        }
    }

    /*Valve cycling day state*/
    settings_state_cyclingday_present = check_file_exists(settings_state_cyclingday_path);
    if (settings_state_cyclingday_present == 1) {
        status = "<font color=\"green\">Settings ok.</font>";
        if (var == "STATUS_STATE_CYCLINGDAY_CONFIG") 
            return F(status);
        if(var == "ENABLE_STATE_CYCLINGDAY")
            return (settings_state_cyclingday[String("enable_state_cyclingday")]);
        if(var == "NAME_STATE_CYCLINGDAY")
            return (settings_state_cyclingday[String("name_state_cyclingday")]);
        if(var == "VALVE0_POSITION_CYCLINGDAY")
            return (settings_state_cyclingday[String("valve0_position_cyclingday")]);
        if(var == "VALVE1_POSITION_CYCLINGDAY")
            return (settings_state_cyclingday[String("valve1_position_cyclingday")]);
        if(var == "VALVE2_POSITION_CYCLINGDAY")
            return (settings_state_cyclingday[String("valve2_position_cyclingday")]);
        if(var == "VALVE3_POSITION_CYCLINGDAY")
            return (settings_state_cyclingday[String("valve3_position_cyclingday")]);
        if(var == "VALVE4_POSITION_CYCLINGDAY")
            return (settings_state_cyclingday[String("valve4_position_cyclingday")]);
        if(var == "VALVE5_POSITION_CYCLINGDAY")
            return (settings_state_cyclingday[String("valve5_position_cyclingday")]);
        if(var == "VALVE6_POSITION_CYCLINGDAY")
            return (settings_state_cyclingday[String("valve6_position_cyclingday")]);
        if(var == "VALVE7_POSITION_CYCLINGDAY")
            return (settings_state_cyclingday[String("valve7_position_cyclingday")]);
        if(var == "VALVE8_POSITION_CYCLINGDAY")
            return (settings_state_cyclingday[String("valve8_position_cyclingday")]);
        if(var == "VALVE9_POSITION_CYCLINGDAY")
            return (settings_state_cyclingday[String("valve9_position_cyclingday")]);
        if(var == "VALVE10_POSITION_CYCLINGDAY")
            return (settings_state_cyclingday[String("valve10_position_cyclingday")]);
        if(var == "VALVE11_POSITION_CYCLINGDAY")
            return (settings_state_cyclingday[String("valve11_position_cyclingday")]);
    }
    else {
        status = "<font color=\"red\">Settings file not found.</font>";
        if (var == "STATUS_STATE_CYCLINGDAY_CONFIG") {
            return F(status);
        }
    }

    /*Valve cycling night state*/
    settings_state_cyclingnight_present = check_file_exists(settings_state_cyclingnight_path);
    if (settings_state_cyclingnight_present == 1) {
        status = "<font color=\"green\">Settings ok.</font>";
        if (var == "STATUS_STATE_CYCLINGNIGHT_CONFIG") 
            return F(status);
        if(var == "ENABLE_STATE_CYCLINGNIGHT")
            return (settings_state_cyclingnight[String("enable_state_cyclingnight")]);
        if(var == "NAME_STATE_CYCLINGNIGHT")
            return (settings_state_cyclingnight[String("name_state_cyclingnight")]);
        if(var == "VALVE0_POSITION_CYCLINGNIGHT")
            return (settings_state_cyclingnight[String("valve0_position_cyclingnight")]);
        if(var == "VALVE1_POSITION_CYCLINGNIGHT")
            return (settings_state_cyclingnight[String("valve1_position_cyclingnight")]);
        if(var == "VALVE2_POSITION_CYCLINGNIGHT")
            return (settings_state_cyclingnight[String("valve2_position_cyclingnight")]);
        if(var == "VALVE3_POSITION_CYCLINGNIGHT")
            return (settings_state_cyclingnight[String("valve3_position_cyclingnight")]);
        if(var == "VALVE4_POSITION_CYCLINGNIGHT")
            return (settings_state_cyclingnight[String("valve4_position_cyclingnight")]);
        if(var == "VALVE5_POSITION_CYCLINGNIGHT")
            return (settings_state_cyclingnight[String("valve5_position_cyclingnight")]);
        if(var == "VALVE6_POSITION_CYCLINGNIGHT")
            return (settings_state_cyclingnight[String("valve6_position_cyclingnight")]);
        if(var == "VALVE7_POSITION_CYCLINGNIGHT")
            return (settings_state_cyclingnight[String("valve7_position_cyclingnight")]);
        if(var == "VALVE8_POSITION_CYCLINGNIGHT")
            return (settings_state_cyclingnight[String("valve8_position_cyclingnight")]);
        if(var == "VALVE9_POSITION_CYCLINGNIGHT")
            return (settings_state_cyclingnight[String("valve9_position_cyclingnight")]);
        if(var == "VALVE10_POSITION_CYCLINGNIGHT")
            return (settings_state_cyclingnight[String("valve10_position_cyclingnight")]);
        if(var == "VALVE11_POSITION_CYCLINGNIGHT")
            return (settings_state_cyclingnight[String("valve11_position_cyclingnight")]);
    }
    else {
        status = "<font color=\"red\">Settings file not found.</font>";
        if (var == "STATUS_STATE_CYCLINGNIGHT_CONFIG") {
            return F(status);
        }
    }
        
    return String();
}