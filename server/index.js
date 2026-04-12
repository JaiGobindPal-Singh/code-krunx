import express from 'express';
import { initializeSocket } from './socket.js';
import cors from 'cors';
import http from 'http';


const app = express();
const server = http.createServer(app);
const PORT = 3000;
const io = initializeSocket(server);
app.use(express.static('public')); //serve static files from public directory
app.use(express.json());
app.use(cors({
    origin: 'https://sixsem-project-zxlt.onrender.com/',
}));


//simple get request endpoint to test server
app.get('/', (req, res) => {
    res.sendFile('index.html', { root: './public' });
});

//post request endpoint to receive data
app.post('/sensor-data', (req, res) => {
    const receivedData = req.body;
    io.emit('sensor-data', receivedData); //emit data to frontend

    res.status(200).json({ message: 'Data received successfully'});
});

app.post('/reset-data', (req, res) => {
    io.emit('reset-data'); //emit reset event to frontend
    console.log('Reset event received');
    res.status(200).json({ message: 'Reset event emitted successfully'});
});

//additional test endpoint
app.get('/test-endpoint', (req, res) => {
    res.status(200).send('Test endpoint is working!');
});


//server and socket.io initialization
server.listen(PORT,'0.0.0.0', () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});


