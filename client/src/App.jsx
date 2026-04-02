// src/App.js
import { useState, useEffect } from 'react';
import clsx from 'clsx';

import { socket } from './services/socket.js';
import PlotChart from './components/PlotChart.jsx';
function App() {
  const [isConnected, setIsConnected] = useState(false);
  const [sensorData, setSensorData] = useState([]);
  // const [averageSensorData, setAverageSensorData] = useState([]);
  useEffect(() => {
    
      
    //function to handle incoming sensor data and convert to m/s^2
    function handleSensorData(data) {
      if (!data || !data.sensorData) return;     // Validate incoming data
      setSensorData((prevData) => [...prevData, ...(data.sensorData)]);
      
      console.log('Received sensor data:', data);
    }

    function resetSensorData() {
      setSensorData([]);
      console.log('Sensor data reset');
    }

    socket.on("sensor-data", handleSensorData);
    socket.on("reset-data", resetSensorData);

    // Cleanup function to remove the listener when the component unmounts
    return () => {
      socket.off("sensor-data", handleSensorData);
      socket.off("reset-data", resetSensorData);
    };

  }, []);
  useEffect(() => {
    // connecting to socket server
    socket.connect();

    function onConnect() {
      setIsConnected(true);
      console.log('Connected to server');
    }

    function onDisconnect() {
      setIsConnected(false);
      console.log('Disconnected from server');
    }

    // Subscribe to events
    socket.on('connect', onConnect);
    socket.on('disconnect', onDisconnect);

    // Cleanup function: unsubscribe and disconnect when the component unmounts
    return () => {
      socket.off('connect', onConnect);
      socket.off('disconnect', onDisconnect);
      socket.disconnect();
    };
  }, []); // Empty dependency array ensures this runs once when the component mounts


  function createChart(data){
    return (
      <main  className="h-screen w-full mt-2 flex flex-col items-center mb-5" >
              <div className="bg-gray-800 p-4 rounded shadow h-full w-full flex flex-col items-center gap-2">
                <h1 id='graphead' className='text-white text-2xl underline font-semibold'>Accelaration Graph Strike</h1>
                <div className='border bg-gray-900 border-white h-full w-full p-5 flex flex-col items-center justify-center'>
                  <PlotChart sensorData={data} />
                </div>
              </div>
            
            </main>
    )
  }
  return (
    <div className='bg-gray-200 h-screen max-w-screen min-w-screen overflow-x-hidden'>
      <nav className={clsx("bg-gray-700", { "text-green-400": isConnected }, { "text-red-500": !isConnected })}>{isConnected ? 'Connected' : 'Disconnected'}</nav>

      {
        sensorData.length === 0 ? (
          <div className='flex flex-col items-center justify-center min-h-48 min-w-screen'>
            <h2 className='text-gray-700 text-2xl font-semibold'>Waiting for sensor data...</h2>
          </div>
        ) :
          (
            <div className='w-full h-full'>
              {createChart(sensorData)}

              {/* Not required for now */}
              {/* smoothened data to 25 frames per second
              {createChart(fourFrameAverageSensorData)} */}
            </div>
            
          )
      }
    </div>
  );
}

export default App;
