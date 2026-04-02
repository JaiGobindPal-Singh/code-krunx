/**
 * Calculate the magnitude of acceleration from its components
 * @param {number} ax - Acceleration in x-direction (m/s²)
 * @param {number} ay - Acceleration in y-direction (m/s²)
 * @param {number} az - Acceleration in z-direction (m/s²)
 * @returns {number} - Magnitude of acceleration (m/s²)
 */
export const calculateAccelerationMagnitude = (ax, ay, az) => {
    return Math.sqrt(ax * ax + ay * ay + az * az);
};

/**
 * Calculate acceleration components
 * @param {number} ax - Acceleration in x-direction (m/s²)
 * @param {number} ay - Acceleration in y-direction (m/s²)
 * @param {number} az - Acceleration in z-direction (m/s²)
 * @returns {Object} - Object containing magnitude and components
 */
export const calculateAcceleration = (ax, ay, az) => {
    const magnitude = calculateAccelerationMagnitude(ax, ay, az);
    
    return {
        ax,
        ay,
        az,
        magnitude
    };
};