const char test2_html[] PROGMEM = R"rawliteral(
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
        </div>
        <div class="content">
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
                        <td id ="valve0_position"></td>
                        <td>6</td>
                        <td id="valve6_position"></td>
                    </tr>
                    <tr>
                        <td>1</td>
                        <td id ="valve1_position"></td>
                        <td>7</td>
                        <td id ="valve7_position"></td>
                    </tr>
                    <tr>
                        <td>2</td>
                        <td id ="valve2_position"></td>
                        <td>8</td>
                        <td id ="valve8_position"></td>
                    </tr>
                    <tr>
                        <td>3</td>
                        <td id ="valve3_position"></td>
                        <td>9</td>
                        <td id ="valve9_position"></td>
                    </tr>
                    <tr>
                        <td>4</td>
                        <td id ="valve4_position"></td>
                        <td>10</td>
                        <td id ="valve10_position"></td>
                    </tr>
                    <tr>
                        <td>5</td>
                        <td id ="valve5_position"></td>
                        <td>11</td>
                        <td id ="valve11_position"></td>
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
