const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const bodyParser = require('body-parser')
const express = require('express')
const app = express()
const portName = '/dev/cu.usbmodemB0818499CFF02';

const port = new SerialPort({
  path: portName,
  baudRate: 9600, 

});

mainData = "";

const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));

// Open the connection and register an event listener
port.on('open', () => {
  console.log('Serial Port Open');
  port.set({ dtr: true, rts: true }, (err) => {
    if (err) console.error('Error setting signals:', err.message);
    else console.log('Signals set: Data flow should start...');
  });
});

app.use(bodyParser.urlencoded())
app.use(bodyParser.json())

parser.on('data', (data) => {
        mainData = data;
});

// this gets current ultrasonic sensor value
app.get('/', (req, res) => {
    res.send(mainData);
})


/*
/:[number] => [0,9]
smaller # => larger speed
0 => 100% => slowest speed
*/


app.get("/:setTimeIntervalInbetween", (req, res) => {
    console.log("accessed");
    let time_inteval_btw = req.params.setTimeIntervalInbetween;
    console.log(time_inteval_btw);
    
    port.write(time_inteval_btw);
    
})

port.on('error', (err) => {
  console.error('Error:', err.message);
});

app.listen(3000);