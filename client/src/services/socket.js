import { io } from 'socket.io-client';

// Replace with your backend server URL
const URL = 'https://sixsem-project-zxlt.onrender.com/'; 

export const socket = io(URL, {
  autoConnect: false // Set to false if you want to manually connect later
});