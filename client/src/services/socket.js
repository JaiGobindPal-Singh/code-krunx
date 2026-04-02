import { io } from 'socket.io-client';

// Replace with your backend server URL
const URL = 'http://localhost:3000'; 

export const socket = io(URL, {
  autoConnect: false // Set to false if you want to manually connect later
});