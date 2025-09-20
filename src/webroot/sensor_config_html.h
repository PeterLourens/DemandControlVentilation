const char sensor_config_html[] = R"rawliteral(
<!doctype html>
<html lang="en">
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="The front-end for configuration of the open source demand controlled ventilation system">
    <title>Open Source demand driven ventilation system &ndash; Configuration and Monitoring</title>
    <link rel="icon" href="/favicon.ico" type="image/x-icon">
    <link rel="stylesheet" href="pure-min.css">
    <link rel="stylesheet" href="styles.css">
</head>
=======
=======
>>>>>>> Stashed changes

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description"
        content="The front-end for configuration of the open source demand controlled ventilation system">
    <title> Open Source demand driven ventilation system &ndash; Configuration and Monitoring</title>
    <link rel="icon" href="/favicon.ico" type="image/x-icon">
    <link rel="stylesheet" href="pure_min.css">
    <link rel="stylesheet" href="styles.css">
</head>

<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
<body>

    <div id="layout">
        <!-- Menu toggle -->
        <a href="#menu" id="menuLink" class="menu-link">
            <!-- Hamburger icon -->
            <span></span>
        </a>
<<<<<<< Updated upstream
    <div id="layout">
        <!-- Menu toggle -->
        <a href="#menu" id="menuLink" class="menu-link">
            <!-- Hamburger icon -->
            <span></span>
        </a>

<<<<<<< Updated upstream
    <div id="menu">
        <div class="pure-menu">
            <li class="pure-menu-heading">OSVentilation</li>
=======
        <div id="menu">
            <div class="pure-menu">
                <div class="pure-menu-heading">OSVentilation</div>
>>>>>>> Stashed changes
=======

        <div id="menu">
            <div class="pure-menu">
                <div class="pure-menu-heading">OSVentilation</div>
>>>>>>> Stashed changes

                <ul class="pure-menu-list">
                    <li class="pure-menu-item"><a href="/" class="pure-menu-link">Status</a></li>
                    <li class="pure-menu-item"><a href="/settings" class="pure-menu-link">Settings</a></li>
                    <li class="pure-menu-item"><a href="/valvecontrol" class="pure-menu-link">Valve Control</a></li>
                    <li class="pure-menu-item"><a href="/sensorconfig" class="pure-menu-link">Sensors</a></li>
                    <li class="pure-menu-item"><a href="/statemachine" class="pure-menu-link">Statemachine</a></li>
                    <li class="pure-menu-item"><a href="/web_serial" class="pure-menu-link">Webserial</a></li>
                </ul>
<<<<<<< Updated upstream
            </div>
        </div>
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
                <h2>Sensors</h2>
            </div>
<<<<<<< Updated upstream
            <div>
                <form method ="POST" action="/delete_sensor_config_file2">
                    <input type="submit" value="Delete sensor config file2" class="pure-button-primary button-xsmall pure-button">
                </form>
            </div>
            <p>
=======
=======
            </div>
        </div>

        <div id="main">
            <div class="header">
                <h2>Sensors</h2>
            </div>
>>>>>>> Stashed changes

            <div class="content">
                <h2 class="content-subhead">Sensor config</h2>
                <p>
                    In this section the sensors can be configured and written to a config file. For the sensors there
                    are two files, one for each I2C bus (Wire and Wire1). The values already store in the config file
                    are prefilled in the text boxes. The selection boxes indicate if these sensors should be included in
                    de the statemachine sensor evaluation.
                </p>
                <div>
                    <form method="POST" action="/delete_sensor_config_file1">
                        <input type="submit" value="Delete sensor config file1"
                            class="pure-button-primary button-xsmall pure-button">
                    </form>
                </div>
                <div>
                    <form method="POST" action="/delete_sensor_config_file2">
                        <input type="submit" value="Delete sensor config file2"
                            class="pure-button-primary button-xsmall pure-button">
                    </form>
                </div>

<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
                <form class="pure-form pure-form-stacked" method="POST" action="/sensorconfig1">
                    <fieldset>
                        <legend>I2C bus 1 (wire) - Sensor 0 - 7</legend>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire_sensor0_type">Type:</label>
                                <select id="wire_sensor0_type" name="wire_sensor0_type" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>DHT20</option>
                                    <option>AHT20</option>
                                    <option>SCD40</option>
                                    <option>SCD41</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire_sensor0_valve">Sensor location:</label>
                                <select id="wire_sensor0_valve" name="wire_sensor0_valve" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>Fan inlet</option>
                                    <option>valve0</option>
                                    <option>valve1</option>
                                    <option>valve2</option>
                                    <option>valve3</option>
                                    <option>valve4</option>
                                    <option>valve5</option>
                                    <option>valve6</option>
                                    <option>valve7</option>
                                    <option>valve8</option>
                                    <option>valve9</option>
                                    <option>valve10</option>
                                    <option>valve11</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-8-24">
                                <label for="wire_sensor0_location">Room name:</label>
                                <input type="text" placeholder="e.g. bathroom" name="wire_sensor0_location"
                                    id="wire_sensor0_location" class="pure-input-3-4">
<<<<<<< Updated upstream
                                <input type="text" placeholder="e.g. bathroom" name="wire_sensor0_location"
                                    id="wire_sensor0_location" class="pure-input-3-4">
=======
>>>>>>> Stashed changes
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire_sensor0_rh">RH:</label>
                                <select id="wire_sensor0_rh" name="wire_sensor0_rh" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire_sensor0_co2">CO2:</label>
                                <select id="wire_sensor0_co2" name="wire_sensor0_co2" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire_sensor1_type">Type:</label>
                                <select id="wire_sensor1_type" name="wire_sensor1_type" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>DHT20</option>
                                    <option>AHT20</option>
                                    <option>SCD40</option>
                                    <option>SCD41</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire_sensor1_valve">Sensor location:</label>
                                <select id="wire_sensor1_valve" name="wire_sensor1_valve" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>Fan inlet</option>
                                    <option>valve0</option>
                                    <option>valve1</option>
                                    <option>valve2</option>
                                    <option>valve3</option>
                                    <option>valve4</option>
                                    <option>valve5</option>
                                    <option>valve6</option>
                                    <option>valve7</option>
                                    <option>valve8</option>
                                    <option>valve9</option>
                                    <option>valve10</option>
                                    <option>valve11</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-8-24">
                                <label for="wire_sensor1_location">Room name:</label>
                                <input type="text" placeholder="e.g. bathroom" name="wire_sensor1_location"
                                    id="wire_sensor1_location" class="pure-input-3-4">
<<<<<<< Updated upstream
                                <input type="text" placeholder="e.g. bathroom" name="wire_sensor1_location"
                                    id="wire_sensor1_location" class="pure-input-3-4">
=======
>>>>>>> Stashed changes
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire_sensor1_rh">RH:</label>
                                <select id="wire_sensor1_rh" name="wire_sensor1_rh" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire_sensor1_co2">CO2:</label>
                                <select id="wire_sensor1_co2" name="wire_sensor1_co2" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire_sensor2_type">Type:</label>
                                <select id="wire_sensor2_type" name="wire_sensor2_type" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>DHT20</option>
                                    <option>AHT20</option>
                                    <option>SCD40</option>
                                    <option>SCD41</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire_sensor2_valve">Sensor location:</label>
                                <select id="wire_sensor2_valve" name="wire_sensor2_valve" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>Fan inlet</option>
                                    <option>valve0</option>
                                    <option>valve1</option>
                                    <option>valve2</option>
                                    <option>valve3</option>
                                    <option>valve4</option>
                                    <option>valve5</option>
                                    <option>valve6</option>
                                    <option>valve7</option>
                                    <option>valve8</option>
                                    <option>valve9</option>
                                    <option>valve10</option>
                                    <option>valve11</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-8-24">
                                <label for="wire_sensor2_location">Room name:</label>
                                <input type="text" placeholder="e.g. bathroom" name="wire_sensor2_location"
                                    id="wire_sensor2_location" class="pure-input-3-4">
<<<<<<< Updated upstream
                                <input type="text" placeholder="e.g. bathroom" name="wire_sensor2_location"
                                    id="wire_sensor2_location" class="pure-input-3-4">
=======
>>>>>>> Stashed changes
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire_sensor2_rh">RH:</label>
                                <select id="wire_sensor2_rh" name="wire_sensor2_rh" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire_sensor2_co2">CO2:</label>
                                <select id="wire_sensor2_co2" name="wire_sensor2_co2" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire_sensor3_type">Type:</label>
                                <select id="wire_sensor3_type" name="wire_sensor3_type" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>DHT20</option>
                                    <option>AHT20</option>
                                    <option>SCD40</option>
                                    <option>SCD41</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire_sensor3_valve">Sensor location:</label>
                                <select id="wire_sensor3_valve" name="wire_sensor3_valve" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>Fan inlet</option>
                                    <option>valve0</option>
                                    <option>valve1</option>
                                    <option>valve2</option>
                                    <option>valve3</option>
                                    <option>valve4</option>
                                    <option>valve5</option>
                                    <option>valve6</option>
                                    <option>valve7</option>
                                    <option>valve8</option>
                                    <option>valve9</option>
                                    <option>valve10</option>
                                    <option>valve11</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-8-24">
                                <label for="wire_sensor3_location">Room name:</label>
                                <input type="text" placeholder="e.g. bathroom" name="wire_sensor3_location"
                                    id="wire_sensor3_location" class="pure-input-3-4">
<<<<<<< Updated upstream
                                <input type="text" placeholder="e.g. bathroom" name="wire_sensor3_location"
                                    id="wire_sensor3_location" class="pure-input-3-4">
=======
>>>>>>> Stashed changes
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire_sensor3_rh">RH:</label>
                                <select id="wire_sensor3_rh" name="wire_sensor3_rh" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire_sensor3_co2">CO2:</label>
                                <select id="wire_sensor3_co2" name="wire_sensor3_co2" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire_sensor4_type">Type:</label>
                                <select id="wire_sensor4_type" name="wire_sensor4_type" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>DHT20</option>
                                    <option>AHT20</option>
                                    <option>SCD40</option>
                                    <option>SCD41</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire_sensor4_valve">Sensor location:</label>
                                <select id="wire_sensor4_valve" name="wire_sensor4_valve" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>Fan inlet</option>
                                    <option>valve0</option>
                                    <option>valve1</option>
                                    <option>valve2</option>
                                    <option>valve3</option>
                                    <option>valve4</option>
                                    <option>valve5</option>
                                    <option>valve6</option>
                                    <option>valve7</option>
                                    <option>valve8</option>
                                    <option>valve9</option>
                                    <option>valve10</option>
                                    <option>valve11</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-8-24">
                                <label for="wire_sensor4_location">Room name:</label>
                                <input type="text" placeholder="e.g. bathroom" name="wire_sensor4_location"
                                    id="wire_sensor4_location" class="pure-input-3-4">
<<<<<<< Updated upstream
                                <input type="text" placeholder="e.g. bathroom" name="wire_sensor4_location"
                                    id="wire_sensor4_location" class="pure-input-3-4">
=======
>>>>>>> Stashed changes
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire_sensor4_rh">RH:</label>
                                <select id="wire_sensor4_rh" name="wire_sensor4_rh" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire_sensor4_co2">CO2:</label>
                                <select id="wire_sensor4_co2" name="wire_sensor4_co2" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire_sensor5_type">Type:</label>
                                <select id="wire_sensor5_type" name="wire_sensor5_type" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>DHT20</option>
                                    <option>AHT20</option>
                                    <option>SCD40</option>
                                    <option>SCD41</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire_sensor5_valve">Sensor location:</label>
                                <select id="wire_sensor5_valve" name="wire_sensor5_valve" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>Fan inlet</option>
                                    <option>valve0</option>
                                    <option>valve1</option>
                                    <option>valve2</option>
                                    <option>valve3</option>
                                    <option>valve4</option>
                                    <option>valve5</option>
                                    <option>valve6</option>
                                    <option>valve7</option>
                                    <option>valve8</option>
                                    <option>valve9</option>
                                    <option>valve10</option>
                                    <option>valve11</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-8-24">
                                <label for="wire_sensor5_location">Room name:</label>
                                <input type="text" placeholder="e.g. bathroom" name="wire_sensor5_location"
                                    id="wire_sensor5_location" class="pure-input-3-4">
<<<<<<< Updated upstream
                                <input type="text" placeholder="e.g. bathroom" name="wire_sensor5_location"
                                    id="wire_sensor5_location" class="pure-input-3-4">
=======
>>>>>>> Stashed changes
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire_sensor5_rh">RH:</label>
                                <select id="wire_sensor5_rh" name="wire_sensor5_rh" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire_sensor5_co2">CO2:</label>
                                <select id="wire_sensor5_co2" name="wire_sensor5_co2" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire_sensor6_type">Type:</label>
                                <select id="wire_sensor6_type" name="wire_sensor6_type" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>DHT20</option>
                                    <option>AHT20</option>
                                    <option>SCD40</option>
                                    <option>SCD41</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire_sensor6_valve">Sensor location:</label>
                                <select id="wire_sensor6_valve" name="wire_sensor6_valve" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>Fan inlet</option>
                                    <option>valve0</option>
                                    <option>valve1</option>
                                    <option>valve2</option>
                                    <option>valve3</option>
                                    <option>valve4</option>
                                    <option>valve5</option>
                                    <option>valve6</option>
                                    <option>valve7</option>
                                    <option>valve8</option>
                                    <option>valve9</option>
                                    <option>valve10</option>
                                    <option>valve11</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-8-24">
                                <label for="wire_sensor6_location">Room name:</label>
                                <input type="text" placeholder="e.g. bathroom" name="wire_sensor6_location"
                                    id="wire_sensor6_location" class="pure-input-3-4">
<<<<<<< Updated upstream
                                <input type="text" placeholder="e.g. bathroom" name="wire_sensor6_location"
                                    id="wire_sensor6_location" class="pure-input-3-4">
=======
>>>>>>> Stashed changes
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire_sensor6_rh">RH:</label>
                                <select id="wire_sensor6_rh" name="wire_sensor6_rh" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire_sensor6_co2">CO2:</label>
                                <select id="wire_sensor6_co2" name="wire_sensor6_co2" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire_sensor7_type">Type:</label>
                                <select id="wire_sensor7_type" name="wire_sensor7_type" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>DHT20</option>
                                    <option>AHT20</option>
                                    <option>SCD40</option>
                                    <option>SCD41</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire_sensor7_valve">Sensor location:</label>
                                <select id="wire_sensor7_valve" name="wire_sensor7_valve" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>Fan inlet</option>
                                    <option>valve0</option>
                                    <option>valve1</option>
                                    <option>valve2</option>
                                    <option>valve3</option>
                                    <option>valve4</option>
                                    <option>valve5</option>
                                    <option>valve6</option>
                                    <option>valve7</option>
                                    <option>valve8</option>
                                    <option>valve9</option>
                                    <option>valve10</option>
                                    <option>valve11</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-8-24">
                                <label for="wire_sensor7_location">Room name:</label>
                                <input type="text" placeholder="e.g. bathroom" name="wire_sensor7_location"
                                    id="wire_sensor7_location" class="pure-input-3-4">
<<<<<<< Updated upstream
                                <input type="text" placeholder="e.g. bathroom" name="wire_sensor7_location"
                                    id="wire_sensor7_location" class="pure-input-3-4">
=======
>>>>>>> Stashed changes
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire_sensor7_rh">RH:</label>
                                <select id="wire_sensor7_rh" name="wire_sensor7_rh" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire_sensor7_co2">CO2:</label>
                                <select id="wire_sensor7_co2" name="wire_sensor7_co2" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                        </div>
                        <br><br>
                        <input type="submit" value="Save sensor configuration"
                            class="pure-button-primary button-xsmall pure-button">
<<<<<<< Updated upstream
                        <input type="submit" value="Save sensor configuration"
                            class="pure-button-primary button-xsmall pure-button">
                    </fieldset>
                </form>
<<<<<<< Updated upstream
            </p>
            <p>
=======


>>>>>>> Stashed changes
=======
                    </fieldset>
                </form>


>>>>>>> Stashed changes
                <form class="pure-form pure-form-stacked" method="POST" action="/sensorconfig2">
                    <fieldset>
                        <legend>I2C bus 2 (wire1) - Sensor 0 - 7</legend>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire1_sensor0_type">Type:</label>
                                <select id="wire1_sensor0_type" name="wire1_sensor0_type" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>DHT20</option>
                                    <option>AHT20</option>
                                    <option>SCD40</option>
                                    <option>SCD41</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire1_sensor0_valve">Sensor location:</label>
                                <select id="wire1_sensor0_valve" name="wire1_sensor0_valve" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>Fan inlet</option>
                                    <option>valve0</option>
                                    <option>valve1</option>
                                    <option>valve2</option>
                                    <option>valve3</option>
                                    <option>valve4</option>
                                    <option>valve5</option>
                                    <option>valve6</option>
                                    <option>valve7</option>
                                    <option>valve8</option>
                                    <option>valve9</option>
                                    <option>valve10</option>
                                    <option>valve11</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-8-24">
                                <label for="wire1_sensor0_location">Room name:</label>
                                <input type="text" placeholder="e.g. bathroom" name="wire1_sensor0_location"
                                    id="wire1_sensor0_location" class="pure-input-3-4">
<<<<<<< Updated upstream
                                <input type="text" placeholder="e.g. bathroom" name="wire1_sensor0_location"
                                    id="wire1_sensor0_location" class="pure-input-3-4">
=======
>>>>>>> Stashed changes
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire1_sensor0_rh">RH:</label>
                                <select id="wire1_sensor0_rh" name="wire1_sensor0_rh" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire1_sensor0_co2">CO2:</label>
                                <select id="wire1_sensor0_co2" name="wire1_sensor0_co2" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire1_sensor1_type">Type:</label>
                                <select id="wire1_sensor1_type" name="wire1_sensor1_type" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>DHT20</option>
                                    <option>AHT20</option>
                                    <option>SCD40</option>
                                    <option>SCD41</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire1_sensor1_valve">Sensor location:</label>
                                <select id="wire1_sensor1_valve" name="wire1_sensor1_valve" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>Fan inlet</option>
                                    <option>valve0</option>
                                    <option>valve1</option>
                                    <option>valve2</option>
                                    <option>valve3</option>
                                    <option>valve4</option>
                                    <option>valve5</option>
                                    <option>valve6</option>
                                    <option>valve7</option>
                                    <option>valve8</option>
                                    <option>valve9</option>
                                    <option>valve10</option>
                                    <option>valve11</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-8-24">
                                <label for="wire1_sensor1_location">Room name:</label>
                                <input type="text" placeholder="e.g. bathroom" name="wire1_sensor1_location"
                                    id="wire1_sensor1_location" class="pure-input-3-4">
<<<<<<< Updated upstream
                                <input type="text" placeholder="e.g. bathroom" name="wire1_sensor1_location"
                                    id="wire1_sensor1_location" class="pure-input-3-4">
=======
>>>>>>> Stashed changes
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire1_sensor1_rh">RH:</label>
                                <select id="wire1_sensor1_rh" name="wire1_sensor1_rh" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire1_sensor1_co2">CO2:</label>
                                <select id="wire1_sensor1_co2" name="wire1_sensor1_co2" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire1_sensor2_type">Type:</label>
                                <select id="wire1_sensor2_type" name="wire1_sensor2_type" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>DHT20</option>
                                    <option>AHT20</option>
                                    <option>SCD40</option>
                                    <option>SCD41</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire1_sensor2_valve">Sensor location:</label>
                                <select id="wire1_sensor2_valve" name="wire1_sensor2_valve" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>Fan inlet</option>
                                    <option>valve0</option>
                                    <option>valve1</option>
                                    <option>valve2</option>
                                    <option>valve3</option>
                                    <option>valve4</option>
                                    <option>valve5</option>
                                    <option>valve6</option>
                                    <option>valve7</option>
                                    <option>valve8</option>
                                    <option>valve9</option>
                                    <option>valve10</option>
                                    <option>valve11</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-8-24">
                                <label for="wire1_sensor2_location">Room name:</label>
                                <input type="text" placeholder="e.g. bathroom" name="wire1_sensor2_location"
                                    id="wire1_sensor2_location" class="pure-input-3-4">
<<<<<<< Updated upstream
                                <input type="text" placeholder="e.g. bathroom" name="wire1_sensor2_location"
                                    id="wire1_sensor2_location" class="pure-input-3-4">
=======
>>>>>>> Stashed changes
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire1_sensor2_rh">RH:</label>
                                <select id="wire1_sensor2_rh" name="wire1_sensor2_rh" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire1_sensor2_co2">CO2:</label>
                                <select id="wire1_sensor2_co2" name="wire1_sensor2_co2" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire1_sensor3_type">Type:</label>
                                <select id="wire1_sensor3_type" name="wire1_sensor3_type" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>DHT20</option>
                                    <option>AHT20</option>
                                    <option>SCD40</option>
                                    <option>SCD41</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire1_sensor3_valve">Sensor location:</label>
                                <select id="wire1_sensor3_valve" name="wire1_sensor3_valve" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>Fan inlet</option>
                                    <option>valve0</option>
                                    <option>valve1</option>
                                    <option>valve2</option>
                                    <option>valve3</option>
                                    <option>valve4</option>
                                    <option>valve5</option>
                                    <option>valve6</option>
                                    <option>valve7</option>
                                    <option>valve8</option>
                                    <option>valve9</option>
                                    <option>valve10</option>
                                    <option>valve11</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-8-24">
                                <label for="wire1_sensor3_location">Room name:</label>
                                <input type="text" placeholder="e.g. bathroom" name="wire1_sensor3_location"
                                    id="wire1_sensor3_location" class="pure-input-3-4">
<<<<<<< Updated upstream
                                <input type="text" placeholder="e.g. bathroom" name="wire1_sensor3_location"
                                    id="wire1_sensor3_location" class="pure-input-3-4">
=======
>>>>>>> Stashed changes
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire1_sensor3_rh">RH:</label>
                                <select id="wire1_sensor3_rh" name="wire1_sensor3_rh" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire1_sensor3_co2">CO2:</label>
                                <select id="wire1_sensor3_co2" name="wire1_sensor3_co2" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire1_sensor4_type">Type:</label>
                                <select id="wire1_sensor4_type" name="wire1_sensor4_type" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>DHT20</option>
                                    <option>AHT20</option>
                                    <option>SCD40</option>
                                    <option>SCD41</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire1_sensor4_valve">Sensor location:</label>
                                <select id="wire1_sensor4_valve" name="wire1_sensor4_valve" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>Fan inlet</option>
                                    <option>valve0</option>
                                    <option>valve1</option>
                                    <option>valve2</option>
                                    <option>valve3</option>
                                    <option>valve4</option>
                                    <option>valve5</option>
                                    <option>valve6</option>
                                    <option>valve7</option>
                                    <option>valve8</option>
                                    <option>valve9</option>
                                    <option>valve10</option>
                                    <option>valve11</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-8-24">
                                <label for="wire1_sensor4_location">Room name:</label>
                                <input type="text" placeholder="e.g. bathroom" name="wire1_sensor4_location"
                                    id="wire1_sensor4_location" class="pure-input-3-4">
<<<<<<< Updated upstream
                                <input type="text" placeholder="e.g. bathroom" name="wire1_sensor4_location"
                                    id="wire1_sensor4_location" class="pure-input-3-4">
=======
>>>>>>> Stashed changes
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire1_sensor4_rh">RH:</label>
                                <select id="wire1_sensor4_rh" name="wire1_sensor4_rh" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire1_sensor4_co2">CO2:</label>
                                <select id="wire1_sensor4_co2" name="wire1_sensor4_co2" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire1_sensor5_type">Type:</label>
                                <select id="wire1_sensor5_type" name="wire1_sensor5_type" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>DHT20</option>
                                    <option>AHT20</option>
                                    <option>SCD40</option>
                                    <option>SCD41</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire1_sensor5_valve">Sensor location:</label>
                                <select id="wire1_sensor5_valve" name="wire1_sensor5_valve" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>Fan inlet</option>
                                    <option>valve0</option>
                                    <option>valve1</option>
                                    <option>valve2</option>
                                    <option>valve3</option>
                                    <option>valve4</option>
                                    <option>valve5</option>
                                    <option>valve6</option>
                                    <option>valve7</option>
                                    <option>valve8</option>
                                    <option>valve9</option>
                                    <option>valve10</option>
                                    <option>valve11</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-8-24">
                                <label for="wire1_sensor5_location">Room name:</label>
                                <input type="text" placeholder="e.g. bathroom" name="wire1_sensor5_location"
                                    id="wire1_sensor5_location" class="pure-input-3-4">
<<<<<<< Updated upstream
                                <input type="text" placeholder="e.g. bathroom" name="wire1_sensor5_location"
                                    id="wire1_sensor5_location" class="pure-input-3-4">
=======
>>>>>>> Stashed changes
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire1_sensor5_rh">RH:</label>
                                <select id="wire1_sensor5_rh" name="wire1_sensor5_rh" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire1_sensor5_co2">CO2:</label>
                                <select id="wire1_sensor5_co2" name="wire1_sensor5_co2" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire1_sensor6_type">Type:</label>
                                <select id="wire1_sensor6_type" name="wire1_sensor6_type" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>DHT20</option>
                                    <option>AHT20</option>
                                    <option>SCD40</option>
                                    <option>SCD41</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire1_sensor6_valve">Sensor location:</label>
                                <select id="wire1_sensor6_valve" name="wire1_sensor6_valve" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>Fan inlet</option>
                                    <option>valve0</option>
                                    <option>valve1</option>
                                    <option>valve2</option>
                                    <option>valve3</option>
                                    <option>valve4</option>
                                    <option>valve5</option>
                                    <option>valve6</option>
                                    <option>valve7</option>
                                    <option>valve8</option>
                                    <option>valve9</option>
                                    <option>valve10</option>
                                    <option>valve11</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-8-24">
                                <label for="wire1_sensor6_location">Room name:</label>
                                <input type="text" placeholder="e.g. bathroom" name="wire1_sensor6_location"
                                    id="wire1_sensor6_location" class="pure-input-3-4">
<<<<<<< Updated upstream
                                <input type="text" placeholder="e.g. bathroom" name="wire1_sensor6_location"
                                    id="wire1_sensor6_location" class="pure-input-3-4">
=======
>>>>>>> Stashed changes
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire1_sensor6_rh">RH:</label>
                                <select id="wire1_sensor6_rh" name="wire1_sensor6_rh" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire1_sensor6_co2">CO2:</label>
                                <select id="wire1_sensor6_co2" name="wire1_sensor6_co2" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire1_sensor7_type">Type:</label>
                                <select id="wire1_sensor7_type" name="wire1_sensor7_type" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>DHT20</option>
                                    <option>AHT20</option>
                                    <option>SCD40</option>
                                    <option>SCD41</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-5-24">
                                <label for="wire1_sensor7_valve">Sensor location:</label>
                                <select id="wire1_sensor7_valve" name="wire1_sensor7_valve" class="pure-input-3-4">
                                    <option>None</option>
                                    <option>Fan inlet</option>
                                    <option>valve0</option>
                                    <option>valve1</option>
                                    <option>valve2</option>
                                    <option>valve3</option>
                                    <option>valve4</option>
                                    <option>valve5</option>
                                    <option>valve6</option>
                                    <option>valve7</option>
                                    <option>valve8</option>
                                    <option>valve9</option>
                                    <option>valve10</option>
                                    <option>valve11</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-8-24">
                                <label for="wire1_sensor7_location">Room name:</label>
                                <input type="text" placeholder="e.g. bathroom" name="wire1_sensor7_location"
                                    id="wire1_sensor7_location" class="pure-input-3-4">
<<<<<<< Updated upstream
                                <input type="text" placeholder="e.g. bathroom" name="wire1_sensor7_location"
                                    id="wire1_sensor7_location" class="pure-input-3-4">
=======
>>>>>>> Stashed changes
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire1_sensor7_rh">RH:</label>
                                <select id="wire1_sensor7_rh" name="wire1_sensor7_rh" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-3-24">
                                <label for="wire1_sensor7_co2">CO2:</label>
                                <select id="wire1_sensor7_co2" name="wire1_sensor7_co2" class="pure-input-3-4">
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                        </div>
                        <br><br>
                        <input type="submit" value="Save sensor configuration"
                            class="pure-button-primary button-xsmall pure-button">
<<<<<<< Updated upstream
                        <input type="submit" value="Save sensor configuration"
                            class="pure-button-primary button-xsmall pure-button">
                    </fieldset>
                </form>
<<<<<<< Updated upstream
            </p>

=======
            </div>
>>>>>>> Stashed changes
        </div>
        <div id="page_name">sensor_config</div>
    </div>
<<<<<<< Updated upstream
    <div id="page_name">sensor_config</div>

    <script src="ui.js"></script>
    <script src="websocket.js"></script>

  </body>
  </html>
  )rawliteral";
=======
=======
                    </fieldset>
                </form>
            </div>
        </div>
        <div id="page_name">sensor_config</div>
    </div>
>>>>>>> Stashed changes
    <script src="ui.js"></script>
    <script src="websocket.js"></script>

</body>

</html>
)rawliteral";
>>>>>>> Stashed changes
