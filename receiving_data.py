import socket
import struct
from dataclasses import dataclass
from typing import List

@dataclass
class TelemVect3:
    x: float
    y: float
    z: float

@dataclass
class TelemInfoV2:
    mDeltaTime: float
    mLapNumber: int
    mLapStartET: float
    mVehicleName: str
    mTrackName: str
    mPos: TelemVect3
    mLocalVel: TelemVect3
    mLocalAccel: TelemVect3

def receive_telemetry():
    UDP_IP = "127.0.0.1"
    UDP_PORT = 6000

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))

    print(f"Listening for telemetry data on {UDP_IP}:{UDP_PORT}")

    while True:
        data, addr = sock.recvfrom(2048)
        print(f"Received {len(data)} bytes of data")
        print(f"First 100 bytes: {data[:100].hex()}")
        try:
            telemetry = parse_telemetry(data)
            if telemetry:
                print_telemetry(telemetry)
        except Exception as e:
            print(f"Error processing telemetry data: {e}")
            print("Continuing to next packet...")

def parse_telemetry(data):
    # The format string for the data
    fmt = "<64s 64s 100x f I f 9f"  # 64-char string, 64-char string, 100 bytes padding, float, unsigned int, float, 9 floats for vectors
    
    try:
        unpacked = struct.unpack_from(fmt, data)
        
        track_name = unpacked[0].split(b'\x00')[0].decode('ascii', errors='ignore')
        vehicle_name = unpacked[1].split(b'\x00')[0].decode('ascii', errors='ignore')
        
        return TelemInfoV2(
            mTrackName=track_name,
            mVehicleName=vehicle_name,
            mDeltaTime=unpacked[2],
            mLapNumber=unpacked[3],
            mLapStartET=unpacked[4],
            mPos=TelemVect3(*unpacked[5:8]),
            mLocalVel=TelemVect3(*unpacked[8:11]),
            mLocalAccel=TelemVect3(*unpacked[11:14])
        )
    except struct.error as e:
        print(f"Error unpacking data: {e}")
        return None

def print_telemetry(telemetry):
    if telemetry is None:
        print("No telemetry data to print")
        return
    print(f"Track Name: {telemetry.mTrackName}")
    print(f"Vehicle Name: {telemetry.mVehicleName}")
    print(f"Delta Time: {telemetry.mDeltaTime:.6f}")
    print(f"Lap Number: {telemetry.mLapNumber}")
    print(f"Lap Start ET: {telemetry.mLapStartET:.2f}")
    print(f"Position: X={telemetry.mPos.x:.2f}, Y={telemetry.mPos.y:.2f}, Z={telemetry.mPos.z:.2f}")
    print(f"Local Velocity: X={telemetry.mLocalVel.x:.2f}, Y={telemetry.mLocalVel.y:.2f}, Z={telemetry.mLocalVel.z:.2f}")
    print(f"Local Acceleration: X={telemetry.mLocalAccel.x:.2f}, Y={telemetry.mLocalAccel.y:.2f}, Z={telemetry.mLocalAccel.z:.2f}")
    print("-" * 50)

if __name__ == "__main__":
    receive_telemetry()