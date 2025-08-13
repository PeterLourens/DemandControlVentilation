#include "general.h"

void print_message(String message) {
    
    char txBuffer[400];

    if (debug_mode == true) {
        strcpy(txBuffer, message.c_str());            
        if(webserial_queue != 0) {
            if (xQueueSend(webserial_queue, txBuffer, 50)) { } 
            else {
                Serial.println("Failed to send message to webserial queue.");
            }
        }
    }
}

String formatted_datetime(void) {
    
    String temp_datetime = "";
    String temp_year = "";
    String temp_month = "";
    String temp_day = "";
    String temp_hour = "";
    String temp_minute = "";
    String temp_second = "";
    
    if (date_time_mutex != NULL) {
        if(xSemaphoreTake(date_time_mutex, ( TickType_t ) 10 ) == pdTRUE) {
            temp_year = yearStr;
            temp_month = monthStr;
            temp_day = dayStr;
            temp_hour = hourStr;
            temp_minute = minuteStr;
            temp_second = secondStr;
            xSemaphoreGive(date_time_mutex);
        }
    }
    //temp_datetime =  + "/" + monthStr + "/" + dayStr + " - " + hourStr + ":" + minuteStr + ":" + secondStr;
    temp_datetime = temp_year + "/" + temp_month + "/" + temp_day + "-" + temp_hour + ":" + temp_minute + ":" + temp_second;
    return temp_datetime;
}

String concatJson(String json1, String json2)
{
    // Remove the closing brace from json1
    if (json1.endsWith("}"))
    {
        json1.remove(json1.length() - 1);
    }

    // Remove the opening brace from json2
    if (json2.startsWith("{"))
    {
        json2.remove(0, 1);
    }

    // Concatenate with a comma in between
    String result = json1 + "," + json2;

    return result;
}