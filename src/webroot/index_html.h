const char index_html[] PROGMEM = R"rawliteral(
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
            <h2>System Status</h2>
        </div>

        <div class="content">
            <h2 class="content-subhead">General</h2>
            <p>State statemachine: %STATEMACHINE_STATE% </p>
            <p>Fan speed: %FANSPEED% </p>
            <p>System uptime: %UPTIME% minutes</p>
            <p>Time and date: %FORMATTEDTIME% </p>
            <p></p>
            <h2 class="content-subhead">Valve positions</h2>
            <p>
                <table class="pure-table pure-table-bordered">
                    <thead>
                        <tr>
                            <th>Valve</th>
                            <th>position</th>
                            <th>Valve</th>
                            <th>Position</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr>
                            <td>0</td>
                            <td>%VALVE0_POS%</td>
                            <td>6</td>
                            <td>%VALVE6_POS%</td>
                        </tr>
                        <tr>
                            <td>1</td>
                            <td>%VALVE1_POS%</td>
                            <td>7</td>
                            <td>%VALVE7_POS%</td>
                        </tr>
                        <tr>
                            <td>2</td>
                            <td>%VALVE2_POS%</td>
                            <td>8</td>
                            <td>%VALVE8_POS%</td>
                        </tr>
                        <tr>
                            <td>3</td>
                            <td>%VALVE3_POS%</td>
                            <td>9</td>
                            <td>%VALVE9_POS%</td>
                        </tr>
                        <tr>
                            <td>4</td>
                            <td>%VALVE4_POS%</td>
                            <td>10</td>
                            <td>%VALVE10_POS%</td>
                        </tr>
                        <tr>
                            <td>5</td>
                            <td>%VALVE5_POS%</td>
                            <td>11</td>
                            <td>%VALVE11_POS%</td>
                        </tr>
                    </tbody>
                </table>
            </p>
            <h2 class="content-subhead">Sensor readings</h2>
            <p>Bus0</p>
            <p>
                <table class="pure-table pure-table-bordered">
                    <thead>
                        <tr>
                            <th>Nr</th>
                            <th>Type</th>
                            <th>Temp</th>
                            <th>RH</th>
                            <th>CO2</th>
                            <th>Sensor loc.</th>
                            <th>Room name</th>
                            <th>RH Ctrl</th>
                            <th>CO2 Ctrl</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr>
                            <td>0</td>
                            <td>%BUS0_SENSOR0_TYPE%</td>
                            <td>%BUS0_SENSOR0_TEMP%</td>
                            <td>%BUS0_SENSOR0_HUM%</td>
                            <td>%BUS0_SENSOR0_CO2%</td>
                            <td>%BUS0_SENSOR0_VALVE%</td>
                            <td>%BUS0_SENSOR0_LOCATION%</td>
                            <td>%BUS0_SENSOR0_RHS%</td>
                            <td>%BUS0_SENSOR0_CO2S%</td>
                        </tr>
                        <tr>
                            <td>1</td>
                            <td>%BUS0_SENSOR1_TYPE%</td>
                            <td>%BUS0_SENSOR1_TEMP%</td>
                            <td>%BUS0_SENSOR1_HUM%</td>
                            <td>%BUS0_SENSOR1_CO2%</td>
                            <td>%BUS0_SENSOR1_VALVE%</td>
                            <td>%BUS0_SENSOR1_LOCATION%</td>
                            <td>%BUS0_SENSOR1_RHS%</td>
                            <td>%BUS0_SENSOR1_CO2S%</td>
                        </tr>
                        <tr>
                            <td>2</td>
                            <td>%BUS0_SENSOR2_TYPE%</td>
                            <td>%BUS0_SENSOR2_TEMP%</td>
                            <td>%BUS0_SENSOR2_HUM%</td>
                            <td>%BUS0_SENSOR2_CO2%</td>
                            <td>%BUS0_SENSOR2_VALVE%</td>
                            <td>%BUS0_SENSOR2_LOCATION%</td>
                            <td>%BUS0_SENSOR2_RHS%</td>
                            <td>%BUS0_SENSOR2_CO2S%</td>
                        </tr>
                        <tr>
                            <td>3</td>
                            <td>%BUS0_SENSOR3_TYPE%</td>
                            <td>%BUS0_SENSOR3_TEMP%</td>
                            <td>%BUS0_SENSOR3_HUM%</td>
                            <td>%BUS0_SENSOR3_CO2%</td>
                            <td>%BUS0_SENSOR3_VALVE%</td>
                            <td>%BUS0_SENSOR3_LOCATION%</td>
                            <td>%BUS0_SENSOR3_RHS%</td>
                            <td>%BUS0_SENSOR3_CO2S%</td>
                        </tr>
                        <tr>
                            <td>4</td>
                            <td>%BUS0_SENSOR4_TYPE%</td>
                            <td>%BUS0_SENSOR4_TEMP%</td>
                            <td>%BUS0_SENSOR4_HUM%</td>
                            <td>%BUS0_SENSOR4_CO2%</td>
                            <td>%BUS0_SENSOR4_VALVE%</td>
                            <td>%BUS0_SENSOR4_LOCATION%</td>
                            <td>%BUS0_SENSOR4_RHS%</td>
                            <td>%BUS0_SENSOR4_CO2S%</td>
                        </tr>
                        <tr>
                            <td>5</td>
                            <td>%BUS0_SENSOR5_TYPE%</td>
                            <td>%BUS0_SENSOR5_TEMP%</td>
                            <td>%BUS0_SENSOR5_HUM%</td>
                            <td>%BUS0_SENSOR5_CO2%</td>
                            <td>%BUS0_SENSOR5_VALVE%</td>
                            <td>%BUS0_SENSOR5_LOCATION%</td>
                            <td>%BUS0_SENSOR5_RHS%</td>
                            <td>%BUS0_SENSOR5_CO2S%</td>
                        </tr>
                        <tr>
                            <td>6</td>
                            <td>%BUS0_SENSOR6_TYPE%</td>
                            <td>%BUS0_SENSOR6_TEMP%</td>
                            <td>%BUS0_SENSOR6_HUM%</td>
                            <td>%BUS0_SENSOR6_CO2%</td>
                            <td>%BUS0_SENSOR6_VALVE%</td>
                            <td>%BUS0_SENSOR6_LOCATION%</td>
                            <td>%BUS0_SENSOR6_RHS%</td>
                            <td>%BUS0_SENSOR6_CO2S%</td>
                        </tr>
                        <tr>
                            <td>7</td>
                            <td>%BUS0_SENSOR7_TYPE%</td>
                            <td>%BUS0_SENSOR7_TEMP%</td>
                            <td>%BUS0_SENSOR7_HUM%</td>
                            <td>%BUS0_SENSOR7_CO2%</td>
                            <td>%BUS0_SENSOR7_VALVE%</td>
                            <td>%BUS0_SENSOR7_LOCATION%</td>
                            <td>%BUS0_SENSOR7_RHS%</td>
                            <td>%BUS0_SENSOR7_CO2S%</td>
                        </tr>
                    </tbody>
                </table>
            </p>
            <p>Bus1</p>
            <p>
                <table class="pure-table pure-table-bordered">
                    <thead>
                        <tr>
                            <th>Nr</th>
                            <th>Type</th>
                            <th>Temp</th>
                            <th>RH</th>
                            <th>CO2</th>
                            <th>Sensor loc.</th>
                            <th>Room name</th>
                            <th>RH Ctrl</th>
                            <th>CO2 Ctrl</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr>
                            <td>0</td>
                            <td>%BUS1_SENSOR0_TYPE%</td>
                            <td>%BUS1_SENSOR0_TEMP%</td>
                            <td>%BUS1_SENSOR0_HUM%</td>
                            <td>%BUS1_SENSOR0_CO2%</td>
                            <td>%BUS1_SENSOR0_VALVE%</td>
                            <td>%BUS1_SENSOR0_LOCATION%</td>
                            <td>%BUS1_SENSOR0_RHS%</td>
                            <td>%BUS1_SENSOR0_CO2S%</td>
                        </tr>
                        <tr>
                            <td>1</td>
                            <td>%BUS1_SENSOR1_TYPE%</td>
                            <td>%BUS1_SENSOR1_TEMP%</td>
                            <td>%BUS1_SENSOR1_HUM%</td>
                            <td>%BUS1_SENSOR1_CO2%</td>
                            <td>%BUS1_SENSOR1_VALVE%</td>
                            <td>%BUS1_SENSOR1_LOCATION%</td>
                            <td>%BUS1_SENSOR1_RHS%</td>
                            <td>%BUS1_SENSOR1_CO2S%</td>
                        </tr>
                        <tr>
                            <td>2</td>
                            <td>%BUS1_SENSOR2_TYPE%</td>
                            <td>%BUS1_SENSOR2_TEMP%</td>
                            <td>%BUS1_SENSOR2_HUM%</td>
                            <td>%BUS1_SENSOR2_CO2%</td>
                            <td>%BUS1_SENSOR2_VALVE%</td>
                            <td>%BUS1_SENSOR2_LOCATION%</td>
                            <td>%BUS1_SENSOR2_RHS%</td>
                            <td>%BUS1_SENSOR2_CO2S%</td>
                        </tr>
                        <tr>
                            <td>3</td>
                            <td>%BUS1_SENSOR3_TYPE%</td>
                            <td>%BUS1_SENSOR3_TEMP%</td>
                            <td>%BUS1_SENSOR3_HUM%</td>
                            <td>%BUS1_SENSOR3_CO2%</td>
                            <td>%BUS1_SENSOR3_VALVE%</td>
                            <td>%BUS1_SENSOR3_LOCATION%</td>
                            <td>%BUS1_SENSOR3_RHS%</td>
                            <td>%BUS1_SENSOR3_CO2S%</td>
                        </tr>
                        <tr>
                            <td>4</td>
                            <td>%BUS1_SENSOR4_TYPE%</td>
                            <td>%BUS1_SENSOR4_TEMP%</td>
                            <td>%BUS1_SENSOR4_HUM%</td>
                            <td>%BUS1_SENSOR4_CO2%</td>
                            <td>%BUS1_SENSOR4_VALVE%</td>
                            <td>%BUS1_SENSOR4_LOCATION%</td>
                            <td>%BUS1_SENSOR4_RHS%</td>
                            <td>%BUS1_SENSOR4_CO2S%</td>
                        </tr>
                        <tr>
                            <td>5</td>
                            <td>%BUS1_SENSOR5_TYPE%</td>
                            <td>%BUS1_SENSOR5_TEMP%</td>
                            <td>%BUS1_SENSOR5_HUM%</td>
                            <td>%BUS1_SENSOR5_CO2%</td>
                            <td>%BUS1_SENSOR5_VALVE%</td>
                            <td>%BUS1_SENSOR5_LOCATION%</td>
                            <td>%BUS1_SENSOR5_RHS%</td>
                            <td>%BUS1_SENSOR5_CO2S%</td>
                        </tr>
                        <tr>
                            <td>6</td>
                            <td>%BUS1_SENSOR6_TYPE%</td>
                            <td>%BUS1_SENSOR6_TEMP%</td>
                            <td>%BUS1_SENSOR6_HUM%</td>
                            <td>%BUS1_SENSOR6_CO2%</td>
                            <td>%BUS1_SENSOR6_VALVE%</td>
                            <td>%BUS1_SENSOR6_LOCATION%</td>
                            <td>%BUS1_SENSOR6_RHS%</td>
                            <td>%BUS1_SENSOR6_CO2S%</td>
                        </tr>
                        <tr>
                            <td>7</td>
                            <td>%BUS1_SENSOR7_TYPE%</td>
                            <td>%BUS1_SENSOR7_TEMP%</td>
                            <td>%BUS1_SENSOR7_HUM%</td>
                            <td>%BUS1_SENSOR7_CO2%</td>
                            <td>%BUS1_SENSOR7_VALVE%</td>
                            <td>%BUS1_SENSOR7_LOCATION%</td>
                            <td>%BUS1_SENSOR7_RHS%</td>
                            <td>%BUS1_SENSOR7_CO2S%</td>
                        </tr>
                    </tbody>
                </table>
            </p>
        </div>
    </div>

    <script src="ui.js"></script>

  </body>
  </html>
)rawliteral";
  