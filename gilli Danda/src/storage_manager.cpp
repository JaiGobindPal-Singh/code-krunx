#include "storage_manager.h"

//definition of the static variable of the class
Preferences StorageManager::storage;

/**
 * @brief This function retrieves a value from the ESP32's non-volatile storage (NVS) based on the provided key.
 * @param {char*} key - The key for which the value needs to be retrieved from storage.
 * @return {String} - The value associated with the provided key. If the key is not found, it returns "-1".
 */
String StorageManager::getValueFromStorage(const char* key){
    StorageManager::storage.begin("settings", true);
    String val = StorageManager::storage.getString(key, "-1");
    StorageManager::storage.end();
    return val;
}

/**
 * @brief This function saves a key-value pair to the ESP32's non-volatile storage (NVS).
 * @param {char*} key - The key under which the value will be stored.
 * @param {String} value - The value to be stored associated with the provided key.
 * @return {void} - This function does not return any value.
 */
void StorageManager::saveKeyValueToStorage(const char* key, String value){
    StorageManager::storage.begin("settings", false);
    StorageManager::storage.putString(key, value);
    StorageManager::storage.end();
}