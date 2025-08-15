#include "mqtt.h"

WiFiClient OSventilation;
PubSubClient client(OSventilation);

void publish_sensor_data(void) {

    String measurement;
    char topic[200];
    char sensor_value[8];
    float queue_sensor_data[2][8][3];

    //For MQTT settings
    char mqtt_server_addr[50];
    int mqtt_port_tmp;

    String json;
    String mqtt_enable_str;
    String mqtt_server_str;
    String mqtt_base_topic_str; 

    if (settings_mqtt_mutex != NULL) {
        if(xSemaphoreTake(settings_mqtt_mutex, ( TickType_t ) 10 ) == pdTRUE) {
            mqtt_enable_str = enable_mqtt;
            mqtt_server_str = mqtt_server;
            mqtt_port_tmp = mqtt_port;
            mqtt_base_topic_str = mqtt_base_topic; 
            xSemaphoreGive(settings_mqtt_mutex);
        }
    }

    //Assign default MQTT base topic when setting is empty to prevent writing to MQTT root
    if(mqtt_base_topic_str == "") {
        mqtt_base_topic_str = "Change_me_in_settings";
    }

    mqtt_server_str.toCharArray(mqtt_server_addr,100);

    //Serial.print("\nPublish sensor data.");
    if (xQueuePeek(sensor_queue, &queue_sensor_data, 0 ) == pdTRUE) {  
       
        client.setServer(mqtt_server_addr, mqtt_port_tmp);

        if (client.connect("OSventilation")) {
            for (int bus=0;bus<2;bus++) {
                for (int slot=0;slot<8;slot++) {
                            
                    if (queue_sensor_data[bus][slot][0] > 2 )  {
                        measurement = "/temperature";
                        (mqtt_base_topic_str + "/bus/" + String(bus) + "/sensor" + String(slot) + measurement).toCharArray(topic,200);
                        String(queue_sensor_data[bus][slot][0]).toCharArray(sensor_value,8);
                        client.publish(topic, sensor_value);
                    }

                    if (queue_sensor_data[bus][slot][1] > 2 )  {
                        measurement = "/humidity";
                        (mqtt_base_topic_str + "/bus/" + String(bus) + "/sensor" + String(slot) + measurement).toCharArray(topic,200);
                        String(queue_sensor_data[bus][slot][1]).toCharArray(sensor_value,8);
                        client.publish(topic, sensor_value);
                    }

                    if (queue_sensor_data[bus][slot][2] > 2 )  {
                        measurement = "/CO2";
                        (mqtt_base_topic_str + "/bus/" + String(bus) + "/sensor" + String(slot) + measurement).toCharArray(topic,200);
                        String(queue_sensor_data[bus][slot][2]).toCharArray(sensor_value,8);
                        client.publish(topic, sensor_value);
                    }
                }
            }
        }
        else {
            Serial.print("Could not connect to MQTT server");
        }
    }
}

void publish_avg_sensor_data(void) {

    String measurement;
    char topic[200];
    char sensor_avg_value[8];
    float queue_sensor_avg_data[2][8][3];

    //For MQTT settings
    char mqtt_server_addr[50];
    int mqtt_port_tmp;
    String json;
    String mqtt_enable_str;
    String mqtt_server_str;
    String mqtt_base_topic_str;

    if (settings_mqtt_mutex != NULL) {
        if(xSemaphoreTake(settings_mqtt_mutex, ( TickType_t ) 10 ) == pdTRUE) {
            mqtt_enable_str = enable_mqtt;
            mqtt_server_str = mqtt_server;
            mqtt_port_tmp = mqtt_port;
            mqtt_base_topic_str = mqtt_base_topic; 
            xSemaphoreGive(settings_mqtt_mutex);
        }
    }

    //Assign default MQTT base topic when setting is empty to prevent writing to MQTT root
    if(mqtt_base_topic_str == "") {
        mqtt_base_topic_str = "Change_me_in_settings";
    }

    mqtt_server_str.toCharArray(mqtt_server_addr,100);

    //Serial.print("\nPublish average sensor data.");
    if (xQueuePeek(sensor_avg_queue, &queue_sensor_avg_data, 0 ) == pdTRUE) {  
       
        client.setServer(mqtt_server_addr, mqtt_port_tmp);

        if (client.connect("OSventilation")) {
            for (int bus=0;bus<2;bus++) {
                for (int slot=0;slot<8;slot++) {
                            
                    if (queue_sensor_avg_data[bus][slot][0] > 2 )  {
                        measurement = "/temperature";
                        (mqtt_base_topic_str + "/bus/" + String(bus) + "/sensor" + String(slot) + "_avg" + measurement).toCharArray(topic,200);
                        String(queue_sensor_avg_data[bus][slot][0]).toCharArray(sensor_avg_value,8);
                        client.publish(topic, sensor_avg_value);
                    }

                    if (queue_sensor_avg_data[bus][slot][1] > 2 )  {
                        measurement = "/humidity";
                        (mqtt_base_topic_str + "/bus/" + String(bus) + "/sensor" + String(slot) + "_avg" + measurement).toCharArray(topic,200);
                        String(queue_sensor_avg_data[bus][slot][1]).toCharArray(sensor_avg_value,8);
                        client.publish(topic, sensor_avg_value);
                    }

                    if (queue_sensor_avg_data[bus][slot][2] > 2 )  {
                        measurement = "/CO2";
                        (mqtt_base_topic_str + "/bus/" + String(bus) + "/sensor" + String(slot) + "_avg" + measurement).toCharArray(topic,200);
                        String(queue_sensor_avg_data[bus][slot][2]).toCharArray(sensor_avg_value,8);
                        client.publish(topic, sensor_avg_value);
                    }
                }
            }
        }
        else {
            Serial.print("Could not connect to MQTT server");
        }
    }
}

void publish_valve_positions(void) {

    const char* path = "/json/valvepositions.json";
    char mqtt_server_addr[50]; 
    char valve_pos[4];
    char valve_nr[10];
    char topic[100];
   
    bool status_file_present;

    String json;
    String mqtt_enable_str;
    String mqtt_server_str;
    int mqtt_port_tmp;
    String mqtt_base_topic_str; 

    JsonDocument doc;

    if (settings_mqtt_mutex != NULL) {
        if(xSemaphoreTake(settings_mqtt_mutex, ( TickType_t ) 10 ) == pdTRUE) {
            mqtt_enable_str = enable_mqtt;
            mqtt_server_str = mqtt_server;
            mqtt_port_tmp = mqtt_port;
            mqtt_base_topic_str = mqtt_base_topic; 
            xSemaphoreGive(settings_mqtt_mutex);
        }
    }

    mqtt_server_str.toCharArray(mqtt_server_addr,100);

    //Assign default MQTT base topic when setting is empty to prevent writing to MQTT root
    if(mqtt_base_topic_str == "") {
        mqtt_base_topic_str = "Change_me_in_settings";
    }

    //Serial.print("\nPublish valve positions.");
    client.setServer(mqtt_server_addr, mqtt_port_tmp); 

    if (valve_position_file_mutex != NULL) {
        if(xSemaphoreTake(valve_position_file_mutex, ( TickType_t ) 10 ) == pdTRUE) {
    
            status_file_present = check_file_exists(path);

            if (status_file_present == 1) {

                json = read_config_file(path);
                deserializeJson(doc, json);
            }
            xSemaphoreGive(valve_position_file_mutex);
            vTaskDelay(50);
        }
    }

    if (client.connect("OSventilation")) {
        for (int i=0;i<12;i++){
            String valve_nr_str = "valve" + String(i);
            String valve_pos_str = doc[String(valve_nr_str)];
            valve_nr_str.toCharArray(valve_nr,10);
            valve_pos_str.toCharArray(valve_pos,4);
            (mqtt_base_topic_str + "/position/" + valve_nr_str).toCharArray(topic,100);
            client.publish(topic,valve_pos);
        }
    }
    else {
        Serial.println("Could not connect to MQTT server");
    }
}


void publish_uptime(void) {

    char topic[100];
    char uptime[200];

    //For MQTT settings
    char mqtt_server_addr[50];
    int mqtt_port_tmp;
    String json;
    String mqtt_enable_str;
    String mqtt_server_str;
    String mqtt_base_topic_str;
    String uptime_str;

    if (settings_mqtt_mutex != NULL) {
        if(xSemaphoreTake(settings_mqtt_mutex, ( TickType_t ) 10 ) == pdTRUE) {
            mqtt_enable_str = enable_mqtt;
            mqtt_server_str = mqtt_server;
            mqtt_port_tmp = mqtt_port;
            mqtt_base_topic_str = mqtt_base_topic; 
            xSemaphoreGive(settings_mqtt_mutex);
        }
    }

    //Assign default MQTT base topic when setting is empty to prevent writing to MQTT root
    if(mqtt_base_topic_str == "") {
        mqtt_base_topic_str = "Change_me_in_settings";
    }

    mqtt_server_str.toCharArray(mqtt_server_addr,100);

    //Serial.print("\nPublish uptime.");
    client.setServer(mqtt_server_addr, mqtt_port_tmp);
    
    if (client.connect("OSventilation")) {
        (mqtt_base_topic_str + "/system/uptime").toCharArray(topic,100);
        //uptime_str = String(esp_timer_get_time()/1000000/60) + " minutes";                   //in minutes
        uptime_str = formatted_uptime();
        uptime_str.toCharArray(uptime, 200);
        //(uptime_formatter::getUptime()).toCharArray(uptime,200);
        client.publish(topic,uptime);
    }
    else {
        Serial.print("\nCould not connect to MQTT server");
    }
}

void publish_fanspeed(void) {
    
    String temp_fanspeed;
    char topic[100];
    char fan[20];

    //For MQTT settings
    char mqtt_server_addr[50];
    int mqtt_port_tmp;
    String json;
    String mqtt_enable_str;
    String mqtt_server_str;
    String mqtt_base_topic_str;

    if (settings_mqtt_mutex != NULL) {
        if(xSemaphoreTake(settings_mqtt_mutex, ( TickType_t ) 10 ) == pdTRUE) {
            mqtt_enable_str = enable_mqtt;
            mqtt_server_str = mqtt_server;
            mqtt_port_tmp = mqtt_port;
            mqtt_base_topic_str = mqtt_base_topic; 
            xSemaphoreGive(settings_mqtt_mutex);
        }
    }

    //Assign default MQTT base topic when setting is empty to prevent writing to MQTT root
    if(mqtt_base_topic_str == "") {
        mqtt_base_topic_str = "Change_me_in_settings";
    }

    mqtt_server_str.toCharArray(mqtt_server_addr,100);

    //Serial.print("\nPublish fan speed.");
    client.setServer(mqtt_server_addr, mqtt_port_tmp);

    if (fanspeed_mutex != NULL) {
        if(xSemaphoreTake(fanspeed_mutex, ( TickType_t ) 10 ) == pdTRUE) {
            temp_fanspeed = fanspeed;
            xSemaphoreGive(fanspeed_mutex);
        }
    }

    if (client.connect("OSventilation")) {
        temp_fanspeed.toCharArray(fan,20);
        (mqtt_base_topic_str + "/status/fanspeed").toCharArray(topic,100);
        client.publish(topic,fan);
    }
    else {
        Serial.println("Could not connect to MQTT server");
    }
}

void publish_state(void) {

    char topic[100];
    char temp_state[20];

    //For MQTT settings
    char mqtt_server_addr[50];
    int mqtt_port_tmp;
    String json;
    String mqtt_enable_str;
    String mqtt_server_str;
    String mqtt_base_topic_str;

    if (settings_mqtt_mutex != NULL) {
        if(xSemaphoreTake(settings_mqtt_mutex, ( TickType_t ) 10 ) == pdTRUE) {
            mqtt_enable_str = enable_mqtt;
            mqtt_server_str = mqtt_server;
            mqtt_port_tmp = mqtt_port;
            mqtt_base_topic_str = mqtt_base_topic; 
            xSemaphoreGive(settings_mqtt_mutex);
        }
    }

    //Assign default MQTT base topic when setting is empty to prevent writing to MQTT root
    if(mqtt_base_topic_str == "") {
        mqtt_base_topic_str = "Change_me_in_settings";
    }

    mqtt_server_str.toCharArray(mqtt_server_addr,100);

    if (statemachine_state_mutex != NULL) {
        if(xSemaphoreTake(statemachine_state_mutex, ( TickType_t ) 10 ) == pdTRUE) {
            state.toCharArray(temp_state,20);
            xSemaphoreGive(statemachine_state_mutex);
        }
    }
    
    //Serial.print("\nPublish statemachine state.");
    client.setServer(mqtt_server_addr, mqtt_port_tmp);

    if (client.connect("OSventilation")) {
        (mqtt_base_topic_str + "/status/state").toCharArray(topic,100);
        client.publish(topic,temp_state);
    }
    else {
        Serial.println("Could not connect to MQTT server");
    }
}


