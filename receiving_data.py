import struct
import socket

# Definir el formato de los datos, basándonos en la estructura C++
# 'f' representa float y 'B' representa unsigned char
DATA_FORMAT = 'f' * 111 + 'B' + 'f' * 6 + 'B' + 'f' * 10
DATA_SIZE = 636

def parse_data(data):
    try:
        # Calcular el número de elementos que se pueden desempaquetar
        num_elements = len(data) // struct.calcsize('f')
        # Ajustar el formato de los datos según el número de elementos disponibles
        adjusted_format = 'f' * num_elements
        unpacked_data = struct.unpack(adjusted_format, data)
        keys = [
            'clutchRPM', 'deltaTime', 'engineOilTemp', 'engineWaterTemp', 'lapStartET', 'lapDistance', 'bestS1', 'bestS2', 'bestLap', 'lastS1', 'lastS2', 'lastLap', 'currentS1', 'currentS2', 'numPitstops', 'numPenalties', 'localAccelX', 'localAccelY', 'localAccelZ', 'localRotX', 'localRotY', 'localRotZ', 'localRotAccelX', 'localRotAccelY', 'localRotAccelZ', 'localVelX', 'localVelY', 'localVelZ', 'oriXX', 'oriXY', 'oriXZ', 'oriYX', 'oriYY', 'oriYZ', 'oriZX', 'oriZY', 'oriZZ', 'posX', 'posY', 'posZ', 'steeringArmForce', 'trackName', 'vehicleName', 'driverName', 'speed', 'accelLat', 'accelLong', 'roll', 'pitch', 'rpm', 'maxRPM', 'lapIniTime', 'lapNumber', 'brake', 'clutch', 'steering', 'throttle', 'gear', 'brakeTempFL', 'brakeTempFR', 'brakeTempRL', 'brakeTempRR', 'gripFactorFL', 'gripFactorFR', 'gripFactorRL', 'gripFactorRR', 'lateralForceFL', 'lateralForceFR', 'lateralForceRL', 'lateralForceRR', 'pressureFL', 'pressureFR', 'pressureRL', 'pressureRR', 'rideHeightFL', 'rideHeightFR', 'rideHeightRL', 'rideHeightRR', 'rotationFL', 'rotationFR', 'rotationRL', 'rotationRR', 'shockDeflectionFL', 'shockDeflectionFR', 'shockDeflectionRL', 'shockDeflectionRR', 'tireTempFLI', 'tireTempFLM', 'tireTempFLO', 'tireTempFRI', 'tireTempFRM', 'tireTempFRO', 'tireTempRLI', 'tireTempRLM', 'tireTempRLO', 'tireTempRRI', 'tireTempRRM', 'tireTempRRO', 'tireLoadFL', 'tireLoadFR', 'tireLoadRL', 'tireLoadRR', 'tireWearFL', 'tireWearFR', 'tireWearRL', 'tireWearRR', 'detached', 'detachedFL', 'detachedFR', 'detachedRL', 'detachedRR', 'overheating', 'flatFL', 'flatFR', 'flatRL', 'flatRR', 'surfaceFL', 'surfaceFR', 'surfaceRL', 'surfaceRR', 'lastImpactET', 'lastImpactMagnitude', 'lastImpactPosX', 'lastImpactPosY', 'lastImpactPosZ', 'scheduledStops', 'fuel', 'isPlayer', 'whoIsInControl', 'isInPit', 'vehicleClass', 'deltaTimeBehind', 'deltaLapsBehind', 'deltaTimeLeader', 'deltaLapsLeader', 'numOfVehicles', 'terrainNameFL', 'terrainNameFR', 'terrainNameRL', 'terrainNameRR', 'finishStatus', 'position', 'maxLaps', 'endingTime', 'currentTime', 'gamePhase', 'yellowFlagState', 'startLight', 'numRedLights', 'playerName', 'cloudDarkness', 'rainSeverity', 'ambientTemp', 'trackTemp', 'windSpeedX', 'windSpeedY', 'windSpeedZ', 'onPathWetness', 'offPathWetness'
        ]
        return {key: value for key, value in zip(keys, unpacked_data)}
    except struct.error as e:
        print(f"Error desempaquetando datos: {e}")
        return {}

# Configurar la dirección IP y el puerto en el que escucharemos
HOST = '127.0.0.1'
PORT = 6000

# Crear un socket UDP
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((HOST, PORT))

print(f"Escuchando en {HOST}:{PORT}")

while True:
    data, addr = sock.recvfrom(1024)  # Leer hasta 1024 bytes para verificar tamaño
    print(f"Datos recibidos ({len(data)} bytes) de {addr}")
    print(len(data))
    print(DATA_SIZE)
    
    if len(data) <= DATA_SIZE:
        # Procesar los datos recibidos si el tamaño es correcto
        parsed_data = parse_data(data)
        print(f"Datos recibidos de {addr}:")
        print(parsed_data["steering"])
    else:
        print(f"Tamaño de datos incorrecto: {len(data)} bytes")