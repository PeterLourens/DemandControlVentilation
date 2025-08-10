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
            <p>
                State statemachine: <span id="statemachine_state"></span><br>
                Fan speed: <span id="fanspeed"></span><br>
                System uptime: <span id="uptime"></span> minutes<br>
                Time and date: <span id="date_time"></span>
            </p>
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
                            <td id="valve0"></td>
                            <td>6</td>
                            <td id="valve6"></td>
                        </tr>
                        <tr>
                            <td>1</td>
                            <td id="valve1"></td>
                            <td>7</td>
                            <td id="valve7"></td>
                        </tr>
                        <tr>
                            <td>2</td>
                            <td id="valve2"></td>
                            <td>8</td>
                            <td id="valve8"></td>
                        </tr>
                        <tr>
                            <td>3</td>
                            <td id="valve3"></td>
                            <td>9</td>
                            <td id="valve9"></td>
                        </tr>
                        <tr>
                            <td>4</td>
                            <td id ="valve4"></td>
                            <td>10</td>
                            <td id ="valve10"></td>
                        </tr>
                        <tr>
                            <td>5</td>
                            <td id ="valve5"></td>
                            <td>11</td>
                            <td id ="valve11"></td>
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
                            <td id="bus0_sensor0_type"></td>
                            <td id="bus0_sensor0_temp"></td>
                            <td id="bus0_sensor0_hum"></td>
                            <td id="bus0_sensor0_co2"></td>
                            <td id="bus0_sensor0_valve"></td>
                            <td id="bus0_sensor0_location"></td>
                            <td id="bus0_sensor0_rhs"></td>
                            <td id="bus0_sensor0_co2s"></td>
                        </tr>
                        <tr>
                            <td>1</td>
                            <td id="bus0_sensor1_type"></td>
                            <td id="bus0_sensor1_temp"></td>
                            <td id="bus0_sensor1_hum"></td>
                            <td id="bus0_sensor1_co2"></td>
                            <td id="bus0_sensor1_valve"></td>
                            <td id="bus0_sensor1_location"></td>
                            <td id="bus0_sensor1_rhs"></td>
                            <td id="bus0_sensor1_co2s"></td>
                        </tr>
                        <tr>
                            <td>2</td>
                            <td id="bus0_sensor2_type"></td>
                            <td id="bus0_sensor2_temp"></td>
                            <td id="bus0_sensor2_hum"></td>
                            <td id="bus0_sensor2_co2"></td>
                            <td id="bus0_sensor2_valve"></td>
                            <td id="bus0_sensor2_location"></td>
                            <td id="bus0_sensor2_rhs"></td>
                            <td id="bus0_sensor2_co2s"></td>
                        </tr>
                        <tr>
                            <td>3</td>
                            <td id="bus0_sensor3_type"></td>
                            <td id="bus0_sensor3_temp"></td>
                            <td id="bus0_sensor3_hum"></td>
                            <td id="bus0_sensor3_co2"></td>
                            <td id="bus0_sensor3_valve"></td>
                            <td id="bus0_sensor3_location"></td>
                            <td id="bus0_sensor3_rhs"></td>
                            <td id="bus0_sensor3_co2s"></td>
                        </tr>
                        <tr>
                            <td>4</td>
                            <td id="bus0_sensor4_type"></td>
                            <td id="bus0_sensor4_temp"></td>
                            <td id="bus0_sensor4_hum"></td>
                            <td id="bus0_sensor4_co2"></td>
                            <td id="bus0_sensor4_valve"></td>
                            <td id="bus0_sensor4_location"></td>
                            <td id="bus0_sensor4_rhs"></td>
                            <td id="bus0_sensor4_co2s"></td>
                        </tr>
                        <tr>
                            <td>5</td>
                            <td id="bus0_sensor5_type"></td>
                            <td id="bus0_sensor5_temp"></td>
                            <td id="bus0_sensor5_hum"></td>
                            <td id="bus0_sensor5_co2"></td>
                            <td id="bus0_sensor5_valve"></td>
                            <td id="bus0_sensor5_location"></td>
                            <td id="bus0_sensor5_rhs"></td>
                            <td id="bus0_sensor5_co2s"></td>
                        </tr>
                        <tr>
                            <td>6</td>
                            <td id="bus0_sensor6_type"></td>
                            <td id="bus0_sensor6_temp"></td>
                            <td id="bus0_sensor6_hum"></td>
                            <td id="bus0_sensor6_co2"></td>
                            <td id="bus0_sensor6_valve"></td>
                            <td id="bus0_sensor6_location"></td>
                            <td id="bus0_sensor6_rhs"></td>
                            <td id="bus0_sensor6_co2s"></td>
                        </tr>
                        <tr>
                            <td>7</td>
                            <td id="bus0_sensor7_type"></td>
                            <td id="bus0_sensor7_temp"></td>
                            <td id="bus0_sensor7_hum"></td>
                            <td id="bus0_sensor7_co2"></td>
                            <td id="bus0_sensor7_valve"></td>
                            <td id="bus0_sensor7_location"></td>
                            <td id="bus0_sensor7_rhs"></td>
                            <td id="bus0_sensor7_co2s"></td>
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
                            <td id="bus1_sensor0_type"></td>
                            <td id="bus1_sensor0_temp"></td>
                            <td id="bus1_sensor0_hum"></td>
                            <td id="bus1_sensor0_co2"></td>
                            <td id="bus1_sensor0_valve"></td>
                            <td id="bus1_sensor0_location"></td>
                            <td id="bus1_sensor0_rhs"></td>
                            <td id="bus1_sensor0_co2s"></td>
                        </tr>
                        <tr>
                            <td>1</td>
                            <td id="bus1_sensor1_type"></td>
                            <td id="bus1_sensor1_temp"></td>
                            <td id="bus1_sensor1_hum"></td>
                            <td id="bus1_sensor1_co2"></td>
                            <td id="bus1_sensor1_valve"></td>
                            <td id="bus1_sensor1_location"></td>
                            <td id="bus1_sensor1_rhs"></td>
                            <td id="bus1_sensor1_co2s"></td>
                        </tr>
                        <tr>
                            <td>2</td>
                            <td id="bus1_sensor2_type"></td>
                            <td id="bus1_sensor2_temp"></td>
                            <td id="bus1_sensor2_hum"></td>
                            <td id="bus1_sensor2_co2"></td>
                            <td id="bus1_sensor2_valve"></td>
                            <td id="bus1_sensor2_location"></td>
                            <td id="bus1_sensor2_rhs"></td>
                            <td id="bus1_sensor2_co2s"></td>
                        </tr>
                        <tr>
                            <td>3</td>
                            <td id="bus1_sensor3_type"></td>
                            <td id="bus1_sensor3_temp"></td>
                            <td id="bus1_sensor3_hum"></td>
                            <td id="bus1_sensor3_co2"></td>
                            <td id="bus1_sensor3_valve"></td>
                            <td id="bus1_sensor3_location"></td>
                            <td id="bus1_sensor3_rhs"></td>
                            <td id="bus1_sensor3_co2s"></td>
                        </tr>
                        <tr>
                            <td>4</td>
                            <td id="bus1_sensor4_type"></td>
                            <td id="bus1_sensor4_temp"></td>
                            <td id="bus1_sensor4_hum"></td>
                            <td id="bus1_sensor4_co2"></td>
                            <td id="bus1_sensor4_valve"></td>
                            <td id="bus1_sensor4_location"></td>
                            <td id="bus1_sensor4_rhs"></td>
                            <td id="bus1_sensor4_co2s"></td>
                        </tr>
                        <tr>
                            <td>5</td>
                            <td id="bus1_sensor5_type"></td>
                            <td id="bus1_sensor5_temp"></td>
                            <td id="bus1_sensor5_hum"></td>
                            <td id="bus1_sensor5_co2"></td>
                            <td id="bus1_sensor5_valve"></td>
                            <td id="bus1_sensor5_location"></td>
                            <td id="bus1_sensor5_rhs"></td>
                            <td id="bus1_sensor5_co2s"></td>
                        </tr>
                        <tr>
                            <td>6</td>
                            <td id="bus1_sensor6_type"></td>
                            <td id="bus1_sensor6_temp"></td>
                            <td id="bus1_sensor6_hum"></td>
                            <td id="bus1_sensor6_co2"></td>
                            <td id="bus1_sensor6_valve"></td>
                            <td id="bus1_sensor6_location"></td>
                            <td id="bus1_sensor6_rhs"></td>
                            <td id="bus1_sensor6_co2s"></td>
                        </tr>
                        <tr>
                            <td>7</td>
                            <td id="bus1_sensor7_type"></td>
                            <td id="bus1_sensor7_temp"></td>
                            <td id="bus1_sensor7_hum"></td>
                            <td id="bus1_sensor7_co2"></td>
                            <td id="bus1_sensor7_valve"></td>
                            <td id="bus1_sensor7_location"></td>
                            <td id="bus1_sensor7_rhs"></td>
                            <td id="bus1_sensor7_co2s"></td>
                        </tr>
                    </tbody>
                </table>
            </p>
        </div>
    </div>

    <script src="ui.js"></script>
    <script src="websocket.js"></script>

  </body>
  </html>
)rawliteral";
