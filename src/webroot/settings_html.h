const char settings_html[] = R"rawliteral(
<!doctype html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="The front-end for configuration of the open source demand controlled ventilation system">
    <title>Open Source demand driven ventilation system &ndash; Configuration and Monitoring</title>
    <link rel="stylesheet" href="pure-min.css">
    <link rel="stylesheet" href="styles.css">
</head>
<body>

<div id="layout">
    <!-- Menu toggle -->
    <a href="#menu" id="menuLink" class="menu-link">
        <!-- Hamburger icon -->
        <span></span>
    </a>

    <div id="menu">
        <div class="pure-menu">
            <li class="pure-menu-heading">OSVentilation</li>

            <ul class="pure-menu-list">
                <li class="pure-menu-item"><a href="/" class="pure-menu-link">Status</a></li>
                <li class="pure-menu-item"><a href="/settings" class="pure-menu-link">Settings</a></li>
                <li class="pure-menu-item"><a href="/valvecontrol" class="pure-menu-link">Valve Control</a></li>
                <li class="pure-menu-item"><a href="/sensorconfig" class="pure-menu-link">Sensors</a></li>
                <li class="pure-menu-item"><a href="/statemachine" class="pure-menu-link">Statemachine</a></li>
                <li class="pure-menu-item"><a href="/web_serial" class="pure-menu-link">Webserial</a></li>
            </ul>
        </div>
    </div>

    <div id="main">
        <div class="header">
            <h2>Settings</h2>
        </div>

        <div class="content">
            <p>
                <form method ="POST" action="/restart_esp32">
                    <input type="submit" value="Restart ESP32">
                </form>
            </p>
            <h4 class="content-subhead">Network configuration - %STATUS_NETWORK_CONFIG% (requires ESP32 restart)</h4>
            <p>
                <form class="pure-form pure-form-stacked" method="POST" action="/settings_network">
                    <fieldset>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-1-1">
                                <label for="enable_dhcp">Enable DHCP:</label>
                                <select id="wire_sensor0_type" name="enable_dhcp" id="enable_dhcp" class="pure-input-1-1">
                                    <option selected>%ENABLE_DHCP%</option>
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="ssid">SSID:</label>
                                <input type="text" value="%SSID%" name="ssid" id="ssid" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="wifi_password">WIFI Password:</label>
                                <input type="text" value="%WIFI_PASSWORD%" name="wifi_password" id="wifi_password" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="ip_address">IP Address:</label>
                                <input type="text" placeholder="Leave blank for DHCP" value="%IP_ADDRESS%" name="ip_address" id="ip_address" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="subnet_mask">Subnet mask:</label>
                                <input type="text" placeholder="Leave blank for DHCP" value="%SUBNET_MASK%" name="subnet_mask" id="subnet_mask" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="gateway">Gateway:</label>
                                <input type="text" placeholder="Leave blank for DHCP" value="%GATEWAY%" name="gateway" id="wifi_password" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="primary_dns">Primary DNS server:</label>
                                <input type="text" placeholder="Leave blank for DHCP" value="%PRIMARY_DNS%" name="primary_dns" id="primary_dns" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="secondary_dns">Secundary DNS server (optional):</label>
                                <input type="text" placeholder="Leave blank for DHCP" value="%SECONDARY_DNS%" name="secondary_dns" id="secondary_dns" class="pure-input-3-4" />
                            </div>
                        </div>
                        <br>
                        <input type="submit" value="Save network configuration" class="pure-button-primary button-small pure-button">
                        <br>
                    </fieldset>
                </form>
            </p>
            <h4 class="content-subhead">MQTT configuration - %STATUS_MQTT_CONFIG%</h4>
            <p>
                <form class="pure-form pure-form-stacked" method="POST" action="/settings_mqtt">
                    <fieldset>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-1-1">
                                <label for="enable_mqtt">Enable MQTT:</label>
                                <select id="enable_mqtt" name="enable_mqtt" id="enable_mqtt" class="pure-input-1-1">
                                    <option selected>%ENABLE_MQTT%</option>
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="mqtt_server">MQTT Server:</label>
                                <input type="text" placeholder="MQTT Server ipdress or hostname" value="%MQTT_SERVER%" name="mqtt_server" id="mqtt_server" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="mqtt_port">MQTT Server port:</label>
                                <input type="text" placeholder="default is 1883" value="%MQTT_PORT%" name="mqtt_port" id="mqtt_port" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="mqtt_base_topic">MQTT Base topic:</label>
                                <input type="text" placeholder="Fill in base topic" value="%MQTT_BASE_TOPIC%" name="mqtt_base_topic" id="mqtt_base_topic" class="pure-input-3-4" />
                            </div>
                        </div>
                        <br>
                        <input type="submit" value="Save MQTT settings" class="pure-button-primary button-ssmall pure-button">
                        <br>
                    </fieldset>
                </form>
            </p> 
            <h4 class="content-subhead">Hardware I2C configuration - %STATUS_I2C_HARDWARE_CONFIG% (requires ESP32 restart)</h4>   
            <p>
                <form class="pure-form pure-form-stacked" method="POST" action="/settings_i2c">
                    <fieldset>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="bus0_multiplexer_address">Bus0 (Wire) I2C Multiplexer address (in dec):</label>
                                <input type="text" placeholder="default 0x70" value="%BUS0_MULTIPLEXER_ADDRESS%" name="bus0_multiplexer_address" id="bus0_multiplexer_address" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="bus1_multiplexer_address">Bus1 (Wire1) I2C Multiplexer address (in dec):</label>
                                <input type="text" placeholder="default 0x70" value="%BUS1_MULTIPLEXER_ADDRESS%" name="bus1_multiplexer_address" id="bus1_multiplexer_address" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="enable_lcd">Enable display:</label>
                                <select id="enable_lcd" name="enable_lcd" class="pure-input-1-1">
                                    <option selected>%ENABLE_LCD%</option>
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <!-- <div class="pure-u-1 pure-u-md-1-2">
                                <label for="display_i2c_adddress">Display I2C address (in dec):</label>
                                <input type="text" placeholder="default 0x27" value="%DISPLAY_I2C_ADDRESS%" name="display_i2c_address" id="display_i2c_address" class="pure-input-3-4" />
                            </div> -->
                        </div>
                        <br>
                        <input type="submit" value="Save I2C settings" class="pure-button-primary button-xsmall pure-button">
                        <br>
                    </fieldset>
                </form>
            </p>
            <h4 class="content-subhead">Fan control settings, by MQTT subscribe or http request - %STATUS_FAN_CONTROL_CONFIG%</h4> 
            <p>
                <form class="pure-form pure-form-stacked" method="POST" action="/settings_fan">
                    <fieldset>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-1-1">
                                <label for="fan_control_mode">Fan control mode:</label>
                                <select id="fan_control_mode" name="fan_control_mode" id="fan_control_mode" class="pure-input-1-4">
                                    <option selected>%FAN_CONTROL_MODE%</option>
                                    <option>MQTT subscribe</option>
                                    <option>HTTP API</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="fan_control_mqtt_server">Fan control MQTT Server:</label>
                                <input type="text" placeholder="Fan control MQTT server ipdress or hostname" value="%FAN_CONTROL_MQTT_SERVER%" name="fan_control_mqtt_server" id="fan_control_mqtt_server" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="fan_control_mqtt_port">Fan control MQTT Server port:</label>
                                <input type="text" placeholder="default is 1883" value="%FAN_CONTROL_MQTT_PORT%" name="fan_control_mqtt_port" id="fan_control_mqtt_port" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="fan_control_mqtt_topic">Fan control MQTT publish topic:</label>
                                <input type="text" placeholder="MQTT topic" value="%FAN_CONTROL_MQTT_TOPIC%" name="fan_control_mqtt_topic" id="fan_control_mqtt_topic" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="fan_control_url_high_speed">Fan control URL high speed:</label>
                                <input type="text" placeholder="URL" value="%FAN_CONTROL_URL_HIGH_SPEED%" name="fan_control_url_high_speed" id="fan_control_url_high_speed" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="fan_control_url_medium_speed">Fan control URL medium speed:</label>
                                <input type="text" placeholder="URL" value="%FAN_CONTROL_URL_MEDIUM_SPEED%" name="fan_control_url_medium_speed" id="fan_control_url_medium_speed" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="fan_control_url_low_speed">Fan control URL low speed:</label>
                                <input type="text" placeholder="URL" value="%FAN_CONTROL_URL_LOW_SPEED%" name="fan_control_url_low_speed" id="fan_control_url_low_speed" class="pure-input-3-4" />
                            </div>
                        </div>
                        <br>
                        <input type="submit" value="Save fan control settings" class="pure-button-primary button-xsmall pure-button">
                        <br>
                    </fieldset>
                </form>
            </p>
            <h4 class="content-subhead">InfluxDB configuration - %STATUS_INFLUXDB_CONFIG% (requires ESP32 restart)</h4>
            <p>
                <form class="pure-form pure-form-stacked" method="POST" action="/settings_influxdb">
                    <fieldset>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-1-1">
                                <label for="enable_influxdb">Enable InfluxDB:</label>
                                <select id="enable_influxdb" name="enable_influxdb" id="enable_influxdb" class="pure-input-1-1">
                                    <option selected>%ENABLE_INFLUXDB%</option>
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="influxdb_url">InfluxDB URL:</label>
                                <input type="text" placeholder="http://ip-address:8086" value="%INFLUXDB_URL%" name="influxdb_url" id="influxdb_url" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="influxdb_org">InfluxDB org:</label>
                                <input type="text" placeholder="Fill in InfluxDB organisation" value="%INFLUXDB_ORG%" name="influxdb_org" id="influxdb_org" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="influxdb_bucket">InfluxDB bucket:</label>
                                <input type="text" placeholder="Fill in InfluxDB bucket name" value="%INFLUXDB_BUCKET%" name="influxdb_bucket" id="influxdb_bucket" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="influxdb_token">InfluxDB token:</label>
                                <input type="text" placeholder="Fill in InfluxDB token" value="%INFLUXDB_TOKEN%" name="influxdb_token" id="influxdb_token" class="pure-input-3-4" />
                            </div>
                        </div>
                        <br>
                        <input type="submit" value="Save InfluxDB settings" class="pure-button-primary button-ssmall pure-button">
                        <br>
                    </fieldset>
                </form>
            </p>
            <h4 class="content-subhead">Hardware I2C configuration - %STATUS_RTC_CONFIG% (requires ESP32 restart)</h4>   
            <p>
                <form class="pure-form pure-form-stacked" method="POST" action="/settings_rtc">
                    <fieldset>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="timezone">Timezone:</label>
                                <input type="text" placeholder="CET-1CEST,M3.5.0,M10.5.0/3" value="%TIMEZONE%" name="timezone" id="timezone" class="pure-input-3-4" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-2">
                                <label for="ntp_server">NTP server address:</label>
                                <input type="text" placeholder="e.g. pool.ntp.org" value="%NTP_SERVER%" name="ntp_server" id="ntp_server" class="pure-input-3-4" />
                            </div>

                        </div>
                        <br>
                        <input type="submit" value="Save RTC settings" class="pure-button-primary button-xsmall pure-button">
                        <br>
                    </fieldset>
                </form>
            </p>
            <p>Refer to <a href="https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html" target="_blank">GNU documentation on TZ-Variable</a> for more details on format.<br>For Europe/Amsterdam use: CET-1CEST,M3.5.0,M10.5.0/3</p>
        </div>
    </div>
     <div id="page_name">settings</div>

    <script src="ui.js"></script>
    <script src="websocket.js"></script>

  </body>
  </html>
)rawliteral";
