/**
 * @file server_manager.h
 * @brief This header file contains the declaration of the ServerManager class, which is responsible for managing the server connection and handling server-related operations in the project. It includes functions for setting and testing the server, connecting to the default webserver, and handling user requests for updating the webserver information.
 * @author JGPS
 * @date 2025-03-09
 */

#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H
#include "config.h"


/**
 * @brief The ServerManager class is responsible for managing the server connection and handling server-related operations.
 */
class ServerManager{
    private:
    static String serverAdress;
    
    public:
    static String getServer();
    static bool testServer();
    static bool setServer(String);
    static bool handleUserWebserverUpdationRequest();
    static bool connectDefaultWebserver();
    
};

#endif