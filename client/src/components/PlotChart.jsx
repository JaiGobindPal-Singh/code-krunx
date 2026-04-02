import {Chart as ChartJS} from 'chart.js/auto';
import { Line } from 'react-chartjs-2';
const PlotChart = ({ sensorData }) => {
    //sample data
    const data = sensorData.map((item)=>(
        {
            readTime: (item.readTime/1000).toFixed(2),
            magnitude: item.magnitude,
        }
    ))


    return (
        <div className='w-full h-full'>
            <Line
                data={{
                    labels: data.map((item) => item.readTime),
                    datasets: [
                        {
                            label: 'Magnitude',
                            data: data.map((item) => item.magnitude),
                            borderColor: 'rgb(75, 192, 192)',
                            backgroundColor: 'rgba(75, 192, 192, 0.2)',
                        },
                    ],
                }}
                options={{
                    responsive: true,
                    maintainAspectRatio: false,
                    scales: {
                        x: {
                            border: {
                                display: true,
                                color: '#e5e7eb',
                            },
                            ticks: {
                                color: '#e5e7eb',
                            },
                        },
                        y: {
                            border: {
                                display: true,
                                color: '#e5e7eb',
                            },
                            ticks: {
                                color: '#e5e7eb',
                            },
                        },
                    },
                }}
            />
        </div>
    )
}

export default PlotChart
