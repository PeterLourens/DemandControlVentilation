const char test_html[] PROGMEM = R"rawliteral(
<div class="header">
    <h2>System Status</h2>
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
)rawliteral";