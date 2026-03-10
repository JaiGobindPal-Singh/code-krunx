/**
 * @file storage_manager.h
 * @brief This header file defines the StorageManager class, which provides methods for managing data storage on an ESP32 device using the Preferences library. The class includes functions to retrieve values from storage and save key-value pairs to storage. The implementation of these methods is expected to be in the corresponding source file (storage_manager.cpp).
 * @author JGPS
 * @date 2025-03-10
 */

#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H
#include <Preferences.h>
class StorageManager{
    private:
    static Preferences storage;
    public:
    static String getValueFromStorage(const char*);
    static void saveKeyValueToStorage(const char*, String);
};

#endif