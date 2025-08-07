const char statemachine_html[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="The front-end for configuration of the open source demand controlled ventilation system">
    <title>Open Source demand driven ventilation system &ndash; Configuration and Monitoring</title>
    <link rel="stylesheet" href="pure-min.css">
    <link rel="stylesheet" href="grids-responsive-min.css">
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
            <h2>Statemachine</h2>
        </div>

        <div class="content">  
            <h4 class="content-subhead">General settings statemachine - %STATUS_STATEMACHINE_CONFIG%</h4>
            <p>
                <form class="pure-form pure-form-stacked" method="POST" action="/settings_statemachine">
                    <fieldset>
                        <div class="pure-g">
                            
                        </div>
                    </fieldset>
                </form>
            </p>
            <h4 class="content-subhead">Day state valve settings - %STATUS_STATE_DAY_CONFIG%</h4>
            <p>
                <form class="pure-form pure-form-stacked" method="POST" action="/settings_valve_day">
                    <fieldset>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-1-3">
                                <label for="enable_state_day">State:</label>
                                <select id="enable_state_day" name="enable_state_day" id="enable_state_day" class="pure-input-1-1">
                                    <option selected>%ENABLE_STATE_DAY%</option>
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-3">
                                <label for="state_day_fanspeed">Fan speed:</label>
                                <select id="state_day_fanspeed" name="state_day_fanspeed" id="state_day_fanspeed" class="pure-input-1-1">
                                    <option selected>%STATE_DAY_FANSPEED%</option>
                                    <option>Low</option>
                                    <option>Medium</option>
                                    <option>High</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-3">
                                <label for="name_state_day">Name:</label>
                                <input type="text" value="%NAME_STATE_DAY%" name="name_state_day" id="name_state_day" class="pure-input-1-1" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve0_position_day">Valve0:</label>
                                <input type="number" value="%VALVE0_POSITION_DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve0_position_day" id="valve0_position_day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve1_position_day">Valve1:</label>
                                <input type="number" value="%VALVE1_POSITION_DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve1_position_day" id="valve1_position_day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve2_position_day">Valve2:</label>
                                <input type="number" value="%VALVE2_POSITION_DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve2_position_day" id="valve0_position_day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve3_position_day">Valve3:</label>
                                <input type="number" value="%VALVE3_POSITION_DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve3_position_day" id="valve3_position_day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve4_position_day">Valve4:</label>
                                <input type="number" value="%VALVE4_POSITION_DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve4_position_day" id="valve4_position_day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve5_position_day">Valve5:</label>
                                <input type="number" value="%VALVE5_POSITION_DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve5_position_day" id="valve5_position_day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve6_position_day">Valve6:</label>
                                <input type="number" value="%VALVE6_POSITION_DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve6_position_day" id="valve6_position_day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve7_position_day">Valve7:</label>
                                <input type="number" value="%VALVE7_POSITION_DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve7_position_day" id="valve7_position_day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve8_position_day">Valve8:</label>
                                <input type="number" value="%VALVE8_POSITION_DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve8_position_day" id="valve8_position_day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve9_position_day">Valve9:</label>
                                <input type="number" value="%VALVE9_POSITION_DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve9_position_day" id="valve9_position_day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve10_position_day">Valve10:</label>
                                <input type="number" value="%VALVE10_POSITION_DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve10_position_day" id="valve10_position_day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve11_position_day">Valve11:</label>
                                <input type="number" value="%VALVE11_POSITION_DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve11_position_day" id="valve11_position_day" />
                            </div>
                        </div>
                        <br>
                        <input type="submit" value="Store day valve position" class="button-xsmall pure-button-primary pure-button">
                    </fieldset>
                </form>
            </p>
            <h4 class="content-subhead">Night state valve settings - %STATUS_STATE_NIGHT_CONFIG%</h4>
            <p>
                <form class="pure-form pure-form-stacked" method="POST" action="/settings_valve_night">
                    <fieldset>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-1-3">
                                <label for="enable_state_night">State:</label>
                                <select id="enable_state_night" name="enable_state_night" id="enable_state_night" class="pure-input-1-1">
                                    <option selected>%ENABLE_STATE_NIGHT%</option>
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-3">
                                <label for="state_night_fanspeed">Fan speed:</label>
                                <select id="state_night_fanspeed" name="state_night_fanspeed" id="state_night_fanspeed" class="pure-input-1-1">
                                    <option selected>%STATE_NIGHT_FANSPEED%</option>
                                    <option>Low</option>
                                    <option>Medium</option>
                                    <option>High</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-3">
                                <label for="name_state_night">Name:</label>
                                <input type="text" value="%NAME_STATE_NIGHT%" name="name_state_night" id="name_state_night" class="pure-input-1-1" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve0_position_night">Valve0:</label>
                                <input type="number" value="%VALVE0_POSITION_NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve0_position_night" id="valve0_position_night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve1_position_night">Valve1:</label>
                                <input type="number" value="%VALVE1_POSITION_NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve1_position_night" id="valve1_position_night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve2_position_night">Valve2:</label>
                                <input type="number" value="%VALVE2_POSITION_NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve2_position_night" id="valve2_position_night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve3_position_night">Valve3:</label>
                                <input type="number" value="%VALVE3_POSITION_NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve3_position_night" id="valve3_position_night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve4_position_night">Valve4:</label>
                                <input type="number" value="%VALVE4_POSITION_NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve4_position_night" id="valve4_position_night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve5_position_night">Valve5:</label>
                                <input type="number" value="%VALVE5_POSITION_NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve5_position_night" id="valve5_position_night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve6_position_night">Valve6:</label>
                                <input type="number" value="%VALVE6_POSITION_NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve6_position_night" id="valve6_position_night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve7_position_night">Valve7:</label>
                                <input type="number" value="%VALVE7_POSITION_NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve7_position_night" id="valve7_position_night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve8_position_night">Valve8:</label>
                                <input type="number" value="%VALVE8_POSITION_NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve8_position_night" id="valve8_position_night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve9_position_night">Valve9:</label>
                                <input type="number"value="%VALVE9_POSITION_NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve9_position_night" id="valve9_position_night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve10_position_night">Valve10:</label>
                                <input type="number" value="%VALVE10_POSITION_NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve10_position_night" id="valve10_position_night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve11_position_night">Valve11:</label>
                                <input type="number" value="%VALVE11_POSITION_NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve11_position_night" id="valve11_position_night" />
                            </div>
                        </div>
                        <br>
                        <input type="submit" value="Store night valve position" class="button-xsmall pure-button-primary pure-button">
                    </fieldset>
                </form>
            </p>
            <h4 class="content-subhead">High CO2 day state valve settings - %STATUS_STATE_HIGHCO2DAY_CONFIG%</h4>
            <p>
                <form class="pure-form pure-form-stacked" method="POST" action="/settings_valve_highco2day">
                    <fieldset>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="enable_state_highco2day">State:</label>
                                <select id="enable_state_highco2day" name="enable_state_highco2day" id="enable_state_highco2day" class="pure-input-1-1">
                                    <option selected>%ENABLE_STATE_HIGHCO2DAY%</option>
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="state_highco2day_fanspeed">Fan speed:</label>
                                <select id="state_highco2day_fanspeed" name="state_highco2day_fanspeed" id="state_highco2day_fanspeed" class="pure-input-1-1">
                                    <option selected>%STATE_HIGHCO2DAY_FANSPEED%</option>
                                    <option>Low</option>
                                    <option>Medium</option>
                                    <option>High</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="name_state_highco2day">Name:</label>
                                <input type="text" value="%NAME_STATE_HIGHCO2DAY%" name="name_state_highco2day" id="name_state_highco2day" class="pure-input-1-1" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6"></div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="co2_low_state_highco2day">CO2 Low level:</label>
                                <input type="number" value="%CO2_LOW_STATE_HIGHCO2DAY%" value="700" min="500" name="co2_low_state_highco2day" id="co2_low_state_highco2day" class="pure-input-1-2" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="co2_high_state_highco2day">CO2 High level:</label>
                                <input type="number" value="%CO2_HIGH_STATE_HIGHCO2DAY%" value="1000" min="500" name="co2_high_state_highco2day" id="co2_high_state_highco2day" class="pure-input-1-2" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve0_position_highco2day">Valve0:</label>
                                <input type="number" value="%VALVE0_POSITION_HIGHCO2DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve0_position_highco2day" id="valve0_position_highco2day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve1_position_highco2day">Valve1:</label>
                                <input type="number" value="%VALVE1_POSITION_HIGHCO2DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve1_position_highco2day" id="valve1_position_highco2day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve2_position_highco2day">Valve2:</label>
                                <input type="number" value="%VALVE2_POSITION_HIGHCO2DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve2_position_highco2day" id="valve2_position_highco2day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve3_position_highco2day">Valve3:</label>
                                <input type="number" value="%VALVE3_POSITION_HIGHCO2DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve3_position_highco2day" id="valve3_position_highco2day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve4_position_highco2day">Valve4:</label>
                                <input type="number" value="%VALVE4_POSITION_HIGHCO2DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve4_position_highco2day" id="valve4_position_highco2day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve5_position_highco2day">Valve5:</label>
                                <input type="number" value="%VALVE5_POSITION_HIGHCO2DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve5_position_highco2day" id="valve5_position_highco2day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve6_position_highco2day">Valve6:</label>
                                <input type="number" value="%VALVE6_POSITION_HIGHCO2DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve6_position_highco2day" id="valve6_position_highco2day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve7_position_highco2day">Valve7:</label>
                                <input type="number" value="%VALVE7_POSITION_HIGHCO2DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve7_position_highco2day" id="valve7_position_highco2day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve8_position_highco2day">Valve8:</label>
                                <input type="number" value="%VALVE8_POSITION_HIGHCO2DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve8_position_highco2day" id="valve8_position_highco2day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve9_position_highco2day">Valve9:</label>
                                <input type="number" value="%VALVE9_POSITION_HIGHCO2DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve9_position_highco2day" id="valve9_position_highco2day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve10_position_highco2day">Valve10:</label>
                                <input type="number" value="%VALVE10_POSITION_HIGHCO2DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve10_position_highco2day" id="valve10_position_highco2day" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve11_position_highco2day">Valve11:</label>
                                <input type="number" value="%VALVE11_POSITION_HIGHCO2DAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve11_position_highco2day" id="valve11_position_highco2day" />
                            </div>
                        </div>
                        <br>
                        <input type="submit" value="Store highco2day valve position" class="button-xsmall pure-button-primary pure-button">
                    </fieldset>
                </form>
            </p>
            <h4 class="content-subhead">High CO2 night state valve settings - %STATUS_STATE_HIGHCO2NIGHT_CONFIG%</h4>
            <p>
                <form class="pure-form pure-form-stacked" method="POST" action="/settings_valve_highco2night">
                    <fieldset>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="enable_state_highco2night">State:</label>
                                <select id="enable_state_highco2night" name="enable_state_highco2night" id="enable_state_highco2night" class="pure-input-1-1">
                                    <option selected>%ENABLE_STATE_HIGHCO2NIGHT%</option>
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="state_highco2night_fanspeed">Fan speed:</label>
                                <select id="state_highco2night_fanspeed" name="state_highco2night_fanspeed" id="state_highco2night_fanspeed" class="pure-input-1-1">
                                    <option selected>%STATE_HIGHCO2NIGHT_FANSPEED%</option>
                                    <option>Low</option>
                                    <option>Medium</option>
                                    <option>High</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="name_state_highco2night">Name:</label>
                                <input type="text" value="%NAME_STATE_HIGHCO2NIGHT%" name="name_state_highco2night" id="name_state_highco2night" class="pure-input-1-1" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6"></div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="co2_low_state_highco2night">CO2 Low level:</label>
                                <input type="number" value="%CO2_LOW_STATE_HIGHCO2NIGHT%" value="700" min="500" name="co2_low_state_highco2night" id="co2_low_state_highco2night" class="pure-input-1-2" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="co2_high_state_highco2night">CO2 High level:</label>
                                <input type="number" value="%CO2_HIGH_STATE_HIGHCO2NIGHT%" value="1000" min="500" name="co2_high_state_highco2night" id="co2_high_state_highco2night" class="pure-input-1-2" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve0_position_highco2night">Valve0:</label>
                                <input type="number" value="%VALVE0_POSITION_HIGHCO2NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve0_position_highco2night" id="valve0_position_highco2night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve1_position_highco2night">Valve1:</label>
                                <input type="number" value="%VALVE1_POSITION_HIGHCO2NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve1_position_highco2night" id="valve1_position_highco2night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve2_position_highco2night">Valve2:</label>
                                <input type="number" value="%VALVE2_POSITION_HIGHCO2NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve2_position_highco2night" id="valve2_position_highco2night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve3_position_highco2night">Valve3:</label>
                                <input type="number" value="%VALVE3_POSITION_HIGHCO2NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve3_position_highco2night" id="valve3_position_highco2night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve4_position_highco2night">Valve4:</label>
                                <input type="number" value="%VALVE4_POSITION_HIGHCO2NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve4_position_highco2night" id="valve4_position_highco2night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve5_position_highco2night">Valve5:</label>
                                <input type="number" value="%VALVE5_POSITION_HIGHCO2NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve5_position_highco2night" id="valve5_position_highco2night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve6_position_highco2night">Valve6:</label>
                                <input type="number" value="%VALVE6_POSITION_HIGHCO2NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve6_position_highco2night" id="valve6_position_highco2night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve7_position_highco2night">Valve7:</label>
                                <input type="number" value="%VALVE7_POSITION_HIGHCO2NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve7_position_highco2night" id="valve7_position_highco2night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve8_position_highco2night">Valve8:</label>
                                <input type="number" value="%VALVE8_POSITION_HIGHCO2NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve8_position_highco2night" id="valve8_position_highco2night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve9_position_highco2night">Valve9:</label>
                                <input type="number" value="%VALVE9_POSITION_HIGHCO2NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve9_position_highco2night" id="valve9_position_highco2night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve10_position_highco2night">Valve10:</label>
                                <input type="number" value="%VALVE10_POSITION_HIGHCO2NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve10_position_highco2night" id="valve10_position_highco2night" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve11_position_highco2night">Valve11:</label>
                                <input type="number" value="%VALVE11_POSITION_HIGHCO2NIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve11_position_highco2night" id="valve11_position_highco2night" />
                            </div>
                        </div>
                        <br>
                        <input type="submit" value="Store highco2night valve position" class="button-xsmall pure-button-primary pure-button">
                    </fieldset>
                </form>
            </p>
            <h4 class="content-subhead">High humidity day state valve settings - %STATUS_STATE_HIGHRHDAY_CONFIG%</h4>
            <p>
                <form class="pure-form pure-form-stacked" method="POST" action="/settings_valve_highrhday">
                    <fieldset>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="enable_state_highrhday">State:</label>
                                <select id="enable_state_highrhday" name="enable_state_highrhday" id="enable_state_highrhday" class="pure-input-1-1">
                                    <option selected>%ENABLE_STATE_HIGHRHDAY%</option>
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="state_highrhday_fanspeed">Fan speed:</label>
                                <select id="state_highhrday_fanspeed" name="state_highrhday_fanspeed" id="state_highrhday_fanspeed" class="pure-input-1-1">
                                    <option selected>%STATE_HIGHRHDAY_FANSPEED%</option>
                                    <option>Low</option>
                                    <option>Medium</option>
                                    <option>High</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="name_state_highrhday">Name:</label>
                                <input type="text" value="%NAME_STATE_HIGHRHDAY%" name="name_state_highrhday" id="name_state_highrhday" class="pure-input-1-1" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6"></div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="rh_low_state_highrhday">RH Low level:</label>
                                <input type="number" value="%RH_LOW_STATE_HIGHRHDAY%" value="75" max="100" min="0" name="rh_low_state_highrhday" id="rh_low_state_highrhday" class="pure-input-1-2" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="rh_high_state_highrhday">RH High level:</label>
                                <input type="number" value="%RH_HIGH_STATE_HIGHRHDAY%" value="85" max="100" min="0" name="rh_high_state_highrhday" id="rh_high_state_highrhday" class="pure-input-1-2" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve0_position_highrhday">Valve0:</label>
                                <input type="number" value="%VALVE0_POSITION_HIGHRHDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve0_position_highrhday" id="valve0_position_highrhday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve1_position_highrhday">Valve1:</label>
                                <input type="number" value="%VALVE1_POSITION_HIGHRHDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve1_position_highrhday" id="valve1_position_highrhday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve2_position_highrhday">Valve2:</label>
                                <input type="number" value="%VALVE2_POSITION_HIGHRHDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve2_position_highrhday" id="valve2_position_highrhday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve3_position_highrhday">Valve3:</label>
                                <input type="number" value="%VALVE3_POSITION_HIGHRHDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve3_position_highrhday" id="valve3_position_highrhday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve4_position_highrhday">Valve4:</label>
                                <input type="number" value="%VALVE4_POSITION_HIGHRHDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve4_position_highrhday" id="valve4_position_highrhday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve5_position_highrhday">Valve5:</label>
                                <input type="number" value="%VALVE5_POSITION_HIGHRHDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve5_position_highrhday" id="valve5_position_highrhday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve6_position_highrhday">Valve6:</label>
                                <input type="number" value="%VALVE6_POSITION_HIGHRHDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve6_position_highrhday" id="valve6_position_highrhday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve7_position_highrhday">Valve7:</label>
                                <input type="number" value="%VALVE7_POSITION_HIGHRHDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve7_position_highrhday" id="valve7_position_highrhday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve8_position_highrhday">Valve8:</label>
                                <input type="number" value="%VALVE8_POSITION_HIGHRHDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve8_position_highrhday" id="valve8_position_highrhday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve9_position_highrhday">Valve9:</label>
                                <input type="number" value="%VALVE9_POSITION_HIGHRHDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve9_position_highrhday" id="valve9_position_highrhday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve10_position_highrhday">Valve10:</label>
                                <input type="number" value="%VALVE10_POSITION_HIGHRHDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve10_position_highrhday" id="valve10_position_highrhday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve11_position_highrhday">Valve11:</label>
                                <input type="number" value="%VALVE11_POSITION_HIGHRHDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve11_position_highrhday" id="valve11_position_highrhday" />
                            </div>
                        </div>
                        <br>
                        <input type="submit" value="Store highrhday valve position" class="button-xsmall pure-button-primary pure-button">
                    </fieldset>
                </form>
            </p>
            <h4 class="content-subhead">High humidity night state valve settings - %STATUS_STATE_HIGHRHNIGHT_CONFIG%</h4>
            <p>
                <form class="pure-form pure-form-stacked" method="POST" action="/settings_valve_highrhnight">
                    <fieldset>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="enable_state_highrhnight">State:</label>
                                <select id="enable_state_highrhnight" name="enable_state_highrhnight" id="enable_state_highrhnight" class="pure-input-1-1">
                                    <option selected>%ENABLE_STATE_HIGHRHNIGHT%</option>
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="state_highrhnight_fanspeed">Fan speed:</label>
                                <select id="state_highrhnight_fanspeed" name="state_highrhnight_fanspeed" id="state_highrhnight_fanspeed" class="pure-input-1-1">
                                    <option selected>%STATE_HIGHRHNIGHT_FANSPEED%</option>
                                    <option>Low</option>
                                    <option>Medium</option>
                                    <option>High</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="name_state_highrhnight">Name:</label>
                                <input type="text" value="%NAME_STATE_HIGHRHNIGHT%" name="name_state_highrhnight" id="name_state_highrhnight" class="pure-input-1-1" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6"></div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="rh_low_state_highrhnight">RH Low level:</label>
                                <input type="number" value="%RH_LOW_STATE_HIGHRHNIGHT%" value="75" max="100" min="0" name="rh_low_state_highrhnight" id="rh_low_state_highrhnight" class="pure-input-1-2" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="rh_high_state_highrhnight">RH High level:</label>
                                <input type="number" value="%RH_HIGH_STATE_HIGHRHNIGHT%" value="85" max="100" min="0" name="rh_high_state_highrhnight" id="rh_high_state_highrhnight" class="pure-input-1-2" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve0_position_highrhnight">Valve0:</label>
                                <input type="number" value="%VALVE0_POSITION_HIGHRHNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve0_position_highrhnight" id="valve0_position_highrhnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve1_position_highrhnight">Valve1:</label>
                                <input type="number" value="%VALVE1_POSITION_HIGHRHNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve1_position_highrhnight" id="valve1_position_highrhnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve2_position_highrhnight">Valve2:</label>
                                <input type="number" value="%VALVE2_POSITION_HIGHRHNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve2_position_highrhnight" id="valve2_position_highrhnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve3_position_highrhnight">Valve3:</label>
                                <input type="number" value="%VALVE3_POSITION_HIGHRHNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve3_position_highrhnight" id="valve3_position_highrhnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve4_position_highrhnight">Valve4:</label>
                                <input type="number" value="%VALVE4_POSITION_HIGHRHNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve4_position_highrhnight" id="valve4_position_highrhnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve5_position_highrhnight">Valve5:</label>
                                <input type="number" value="%VALVE5_POSITION_HIGHRHNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve5_position_highrhnight" id="valve5_position_highrhnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve6_position_highrhnight">Valve6:</label>
                                <input type="number" value="%VALVE6_POSITION_HIGHRHNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve6_position_highrhnight" id="valve6_position_highrhnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve7_position_highrhnight">Valve7:</label>
                                <input type="number" value="%VALVE7_POSITION_HIGHRHNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve7_position_highrhnight" id="valve7_position_highrhnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve8_position_highrhnight">Valve8:</label>
                                <input type="number" value="%VALVE8_POSITION_HIGHRHNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve8_position_highrhnight" id="valve8_position_highrhnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve9_position_highrhnight">Valve9:</label>
                                <input type="number" value="%VALVE9_POSITION_HIGHRHNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve9_position_highrhnight" id="valve9_position_highrhnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve10_position_highrhnight">Valve10:</label>
                                <input type="number" value="%VALVE10_POSITION_HIGHRHNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve10_position_highrhnight" id="valve10_position_highrhnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve11_position_highrhnight">Valve11:</label>
                                <input type="number" value="%VALVE11_POSITION_HIGHRHNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve11_position_highrhnight" id="valve11_position_highrhnight" />
                            </div>
                        </div>
                        <br>
                        <input type="submit" value="Store highrhnight valve position" class="button-xsmall pure-button-primary pure-button">
                    </fieldset>
                </form>
            </p>
            <h4 class="content-subhead">Cooking valve settings - %STATUS_STATE_COOKING_CONFIG%</h4>
            <p>
                <form class="pure-form pure-form-stacked" method="POST" action="/settings_valve_cooking">
                    <fieldset>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-1-3">
                                <label for="enable_state_cooking">State:</label>
                                <select id="enable_state_cooking" name="enable_state_cooking" id="enable_state_cooking" class="pure-input-1-1">
                                    <option selected>%ENABLE_STATE_COOKING%</option>
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-3">
                                <label for="state_cooking_fanspeed">Fan speed:</label>
                                <select id="state_cooking_fanspeed" name="state_cooking_fanspeed" id="state_cooking_fanspeed" class="pure-input-1-1">
                                    <option selected>%STATE_COOKING_FANSPEED%</option>
                                    <option>Low</option>
                                    <option>Medium</option>
                                    <option>High</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-3">
                                <label for="name_state_cooking">Name:</label>
                                <input type="text" value="%NAME_STATE_COOKING%" name="name_state_cooking" id="name_state_cooking" class="pure-input-1-1" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-4">
                                <label for="start_hour_state_cooking">Cooking start hour:</label>
                                <input type="number" value="%START_HOUR_STATE_COOKING%" value="17" min="0" max="23" name="start_hour_state_cooking" id="start_hour_state_cooking" class="pure-input-1-1" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-4">
                                <label for="start_min_state_cooking">Cooking start minutes:</label>
                                <input type="number" value="%START_MIN_STATE_COOKING%" value="20" min="0" max="59" name="start_min_state_cooking" id="start_min_state_cooking" class="pure-input-1-1" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-4">
                                <label for="stop_hour_state_cooking">Cooking stop hour:</label>
                                <input type="number" value="%STOP_HOUR_STATE_COOKING%" value="18" min="0" max="23" name="stop_hour_state_cooking" id="stop_hour_state_cooking" class="pure-input-1-1" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-4">
                                <label for="stop_min_state_cooking">Cooking stop minutes:</label>
                                <input type="number" value="%STOP_MIN_STATE_COOKING%" value="20" min="0" max="59" name="stop_min_state_cooking" id="stop_min_state_cooking" class="pure-input-1-1" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve0_position_cooking">Valve0:</label>
                                <input type="number" value="%VALVE0_POSITION_COOKING%" value="0" min="0" max="24" class="pure-input-1-1" name="valve0_position_cooking" id="valve0_position_cooking" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve1_position_cooking">Valve1:</label>
                                <input type="number" value="%VALVE1_POSITION_COOKING%" value="0" min="0" max="24" class="pure-input-1-1" name="valve1_position_cooking" id="valve1_position_cooking" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve2_position_cooking">Valve2:</label>
                                <input type="number" value="%VALVE2_POSITION_COOKING%" value="0" min="0" max="24" class="pure-input-1-1" name="valve2_position_cooking" id="valve2_position_cooking" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve3_position_cooking">Valve3:</label>
                                <input type="number" value="%VALVE3_POSITION_COOKING%" value="0" min="0" max="24" class="pure-input-1-1" name="valve3_position_cooking" id="valve3_position_cooking" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve4_position_cooking">Valve4:</label>
                                <input type="number" value="%VALVE4_POSITION_COOKING%" value="0" min="0" max="24" class="pure-input-1-1" name="valve4_position_cooking" id="valve4_position_cooking" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve5_position_cooking">Valve5:</label>
                                <input type="number" value="%VALVE5_POSITION_COOKING%" value="0" min="0" max="24" class="pure-input-1-1" name="valve5_position_cooking" id="valve5_position_cooking" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve6_position_cooking">Valve6:</label>
                                <input type="number" value="%VALVE6_POSITION_COOKING%" value="0" min="0" max="24" class="pure-input-1-1" name="valve6_position_cooking" id="valve6_position_cooking" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve7_position_cooking">Valve7:</label>
                                <input type="number" value="%VALVE7_POSITION_COOKING%" value="0" min="0" max="24" class="pure-input-1-1" name="valve7_position_cooking" id="valve7_position_cooking" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve8_position_cooking">Valve8:</label>
                                <input type="number" value="%VALVE8_POSITION_COOKING%" value="0" min="0" max="24" class="pure-input-1-1" name="valve8_position_cooking" id="valve8_position_cooking" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve9_position_cooking">Valve9:</label>
                                <input type="number" value="%VALVE9_POSITION_COOKING%" value="0" min="0" max="24" class="pure-input-1-1" name="valve9_position_cooking" id="valve9_position_cooking" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve10_position_cooking">Valve10:</label>
                                <input type="number" value="%VALVE10_POSITION_COOKING%" value="0" min="0" max="24" class="pure-input-1-1" name="valve10_position_cooking" id="valve10_position_cooking" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve11_position_cooking">Valve11:</label>
                                <input type="number" value="%VALVE11_POSITION_COOKING%" value="0" min="0" max="24" class="pure-input-1-1" name="valve11_position_cooking" id="valve11_position_cooking" />
                            </div>
                        </div>
                        <br>
                        <input type="submit" value="Store cooking valve position" class="button-xsmall pure-button-primary pure-button">
                    </fieldset>
                </form>
            </p>
            <h4 class="content-subhead">Valve cycling day state valve settings - %STATUS_STATE_CYCLINGDAY_CONFIG%</h4>
            <p>
                <form class="pure-form pure-form-stacked" method="POST" action="/settings_valve_cyclingday">
                    <fieldset>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-1-3">
                                <label for="enable_state_cyclingday">State:</label>
                                <select id="enable_state_cyclingday" name="enable_state_cyclingday" id="enable_state_cyclingday" class="pure-input-1-1">
                                    <option selected>%ENABLE_STATE_CYCLINGDAY%</option>
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-3">
                                <label for="state_cyclingday_fanspeed">Fan speed:</label>
                                <select id="state_cyclingday_fanspeed" name="state_cyclingday_fanspeed" id="state_cyclingday_fanspeed" class="pure-input-1-1">
                                    <option selected>%STATE_CYCLINGDAY_FANSPEED%</option>
                                    <option>Low</option>
                                    <option>Medium</option>
                                    <option>High</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-3">
                                <label for="name_state_cyclingday">Name:</label>
                                <input type="text" value="%NAME_STATE_CYCLINGDAY%" name="name_state_cyclingday" id="name_state_cyclingday" class="pure-input-1-1" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve0_position_cyclingday">Valve0:</label>
                                <input type="number" value="%VALVE0_POSITION_CYCLINGDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve0_position_cyclingday" id="valve0_position_cyclingday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve1_position_cyclingday">Valve1:</label>
                                <input type="number" value="%VALVE1_POSITION_CYCLINGDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve1_position_cyclingday" id="valve1_position_cyclingday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve2_position_cyclingday">Valve2:</label>
                                <input type="number" value="%VALVE2_POSITION_CYCLINGDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve2_position_cyclingday" id="valve2_position_cyclingday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve3_position_cyclingday">Valve3:</label>
                                <input type="number" value="%VALVE3_POSITION_CYCLINGDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve3_position_cyclingday" id="valve3_position_cyclingday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve4_position_cyclingday">Valve4:</label>
                                <input type="number" value="%VALVE4_POSITION_CYCLINGDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve4_position_cyclingday" id="valve4_position_cyclingday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve5_position_cyclingday">Valve5:</label>
                                <input type="number" value="%VALVE5_POSITION_CYCLINGDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve5_position_cyclingday" id="valve5_position_cyclingday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve6_position_cyclingday">Valve6:</label>
                                <input type="number" value="%VALVE6_POSITION_CYCLINGDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve6_position_cyclingday" id="valve6_position_cyclingday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve7_position_cyclingday">Valve7:</label>
                                <input type="number" value="%VALVE7_POSITION_CYCLINGDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve7_position_cyclingday" id="valve7_position_cyclingday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve8_position_cyclingday">Valve8:</label>
                                <input type="number" value="%VALVE8_POSITION_CYCLINGDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve8_position_cyclingday" id="valve8_position_cyclingday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve9_position_cyclingday">Valve9:</label>
                                <input type="number" value="%VALVE9_POSITION_CYCLINGDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve9_position_cyclingday" id="valve9_position_cyclingday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve10_position_cyclingday">Valve10:</label>
                                <input type="number" value="%VALVE10_POSITION_CYCLINGDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve10_position_cyclingday" id="valve10_position_cyclingday" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve11_position_cyclingday">Valve11:</label>
                                <input type="number" value="%VALVE11_POSITION_CYCLINGDAY%" value="0" min="0" max="24" class="pure-input-1-1" name="valve11_position_cyclingday" id="valve11_position_cyclingday" />
                            </div>
                        </div>
                        <br>
                        <input type="submit" value="Store cyclingday valve position" class="button-xsmall pure-button-primary pure-button">
                    </fieldset>
                </form>
            </p>
            <h4 class="content-subhead">Valve cycling night state valve settings - %STATUS_STATE_CYCLINGNIGHT_CONFIG%</h4>
            <p>
                <form class="pure-form pure-form-stacked" method="POST" action="/settings_valve_cyclingnight">
                    <fieldset>
                        <div class="pure-g">
                            <div class="pure-u-1 pure-u-md-1-3">
                                <label for="enable_state_cyclingnight">State:</label>
                                <select id="enable_state_cyclingnight" name="enable_state_cyclingnight" id="enable_state_cyclingnight" class="pure-input-1-1">
                                    <option selected>%ENABLE_STATE_CYCLINGNIGHT%</option>
                                    <option>On</option>
                                    <option>Off</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-3">
                                <label for="state_cyclingnight_fanspeed">Fan speed:</label>
                                <select id="state_cyclingnight_fanspeed" name="state_cyclingnight_fanspeed" id="state_cyclingnight_fanspeed" class="pure-input-1-1">
                                    <option selected>%STATE_CYCLINGNIGHT_FANSPEED%</option>
                                    <option>Low</option>
                                    <option>Medium</option>
                                    <option>High</option>
                                </select>
                            </div>
                            <div class="pure-u-1 pure-u-md-1-3">
                                <label for="name_state_cyclingnight">Name:</label>
                                <input type="text" value="%NAME_STATE_CYCLINGNIGHT%" name="name_state_cyclingnight" id="name_state_cyclingnight" class="pure-input-1-1" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve0_position_cyclingnight">Valve0:</label>
                                <input type="number" value="%VALVE0_POSITION_CYCLINGNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve0_position_cyclingnight" id="valve0_position_cyclingnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve1_position_cyclingnight">Valve1:</label>
                                <input type="number" value="%VALVE1_POSITION_CYCLINGNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve1_position_cyclingnight" id="valve1_position_cyclingnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve2_position_cyclingnight">Valve2:</label>
                                <input type="number" value="%VALVE2_POSITION_CYCLINGNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve2_position_cyclingnight" id="valve2_position_cyclingnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve3_position_cyclingnight">Valve3:</label>
                                <input type="number" value="%VALVE3_POSITION_CYCLINGNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve3_position_cyclingnight" id="valve3_position_cyclingnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve4_position_cyclingnight">Valve4:</label>
                                <input type="number" value="%VALVE4_POSITION_CYCLINGNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve4_position_cyclingnight" id="valve4_position_cyclingnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve5_position_cyclingnight">Valve5:</label>
                                <input type="number" value="%VALVE5_POSITION_CYCLINGNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve5_position_cyclingnight" id="valve5_position_cyclingnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve6_position_cyclingnight">Valve6:</label>
                                <input type="number" value="%VALVE6_POSITION_CYCLINGNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve6_position_cyclingnight" id="valve6_position_cyclingnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve7_position_cyclingnight">Valve7:</label>
                                <input type="number" value="%VALVE7_POSITION_CYCLINGNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve7_position_cyclingnight" id="valve7_position_cyclingnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve8_position_cyclingnight">Valve8:</label>
                                <input type="number" value="%VALVE8_POSITION_CYCLINGNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve8_position_cyclingnight" id="valve8_position_cyclingnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve9_position_cyclingnight">Valve9:</label>
                                <input type="number" value="%VALVE9_POSITION_CYCLINGNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve9_position_cyclingnight" id="valve9_position_cyclingnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve10_position_cyclingnight">Valve10:</label>
                                <input type="number" value="%VALVE10_POSITION_CYCLINGNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve10_position_cyclingnight" id="valve10_position_cyclingnight" />
                            </div>
                            <div class="pure-u-1 pure-u-md-1-6">
                                <label for="valve11_position_cyclingnight">Valve11:</label>
                                <input type="number" value="%VALVE11_POSITION_CYCLINGNIGHT%" value="0" min="0" max="24" class="pure-input-1-1" name="valve11_position_cyclingnight" id="cyclingnight_valve11_position_cyclingnight" />
                            </div>
                        </div>
                        <br>
                        <input type="submit" value="Store cyclingnight valve position" class="button-xsmall pure-button-primary pure-button">
                    </fieldset>
                </form>
            </p>
        </div>
    </div>

    <script src="ui.js"></script>

  </body>
  </html>
)rawliteral";