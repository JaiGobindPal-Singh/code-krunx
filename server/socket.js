import { Server } from "socket.io";

export function initializeSocket(server) {
    const io = new Server(server,{
        cors: {
            origin: "*",
            methods: ["GET", "POST"]
            }
        }
    );
    io.on('connection', (socket) => {
        console.log('A user connected:', socket.id);
        
        // Socket-level events must be inside the connection handler
        socket.on('disconnect', () => {
            console.log('A user disconnected:', socket.id);
        });
        
        socket.on("message", (data) => {
            console.log("Message received:", data);
            // Echo back or broadcast
            socket.emit('message', data); // Send back to sender
            // io.emit('message', data); // Broadcast to all
        });
    });
    
    return io;
}
