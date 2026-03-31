#!/usr/bin/env python3
"""VL53L1X GD32E230 Realistic Simulator
Simulates the actual microcontroller behavior and VL53L1X sensor"""

import serial
import serial.tools.list_ports
import time
import threading
import random
import sys

class VL53L1X_Simulator:
    def __init__(self):
        self.running = False
        self.serial_port = None
        self.baudrate = 115200
        self.distance = 0
        self.sensor_connected = True
        self.initialized = False
        
        # VL53L1X simulation parameters
        self.min_distance = 50        # 50mm minimum
        self.max_distance = 4000      # 4000mm maximum
        self.noise_level = 5          # ±5mm noise
        self.measurement_rate = 1.0   # 1Hz
        
    def find_serial_port(self):
        """Find available serial port"""
        ports = serial.tools.list_ports.comports()
        available_ports = []
        
        print("Available serial ports:")
        for i, port in enumerate(ports):
            print(f"  {i}: {port.device} - {port.description}")
            available_ports.append(port.device)
        
        if not available_ports:
            print("No serial ports found!")
            return None
            
        # Auto-select first available port
        return available_ports[0]
    
    def simulate_vl53l1x_measurement(self):
        """Simulate realistic VL53L1X distance measurement"""
        if not self.sensor_connected:
            return 0
            
        # Simulate realistic distance patterns
        base_distance = 1000  # 1 meter base
        
        # Add some realistic variation
        variation = random.uniform(-200, 200)
        self.distance = base_distance + variation
        
        # Add measurement noise
        noise = random.uniform(-self.noise_level, self.noise_level)
        self.distance += noise
        
        # Clamp to realistic range
        self.distance = max(self.min_distance, min(self.max_distance, self.distance))
        
        return int(self.distance)
    
    def send_uart_message(self, message):
        """Send UART message as if from GD32E230"""
        if self.serial_port and self.serial_port.is_open:
            try:
                self.serial_port.write(message.encode('utf-8'))
                print(f"Sent: {message.strip()}")
            except Exception as e:
                print(f"Error sending: {e}")
    
    def simulate_boot_sequence(self):
        """Simulate GD32E230 boot and VL53L1X initialization"""
        messages = [
            "VL53L1X GD32E230 Implementation Ready\r\n",
            "Initializing VL53L1X sensor...\r\n"
        ]
        
        for msg in messages:
            self.send_uart_message(msg)
            time.sleep(0.5)
        
        # Simulate sensor detection
        if self.sensor_connected:
            self.send_uart_message("VL53L1X initialized successfully!\r\n")
            self.send_uart_message("Starting distance measurements...\r\n")
            self.initialized = True
        else:
            self.send_uart_message("VL53L1X initialization failed!\r\n")
            self.send_uart_message("Using dummy distance values\r\n")
        
        time.sleep(1)
    
    def simulate_measurement_loop(self):
        """Simulate continuous distance measurements"""
        while self.running:
            if self.initialized:
                # Get simulated measurement
                distance = self.simulate_vl53l1x_measurement()
                
                # Send distance message
                if distance > 0:
                    self.send_uart_message(f"Distance: {distance} mm\r\n")
                else:
                    self.send_uart_message("No data ready\r\n")
            else:
                # Dummy measurements
                dummy_distance = 1234
                self.send_uart_message(f"Distance (dummy): {dummy_distance} mm\r\n")
            
            time.sleep(self.measurement_rate)
    
    def start_simulation(self):
        """Start the simulation"""
        print("=== VL53L1X GD32E230 Realistic Simulator ===")
        print()
        
        # Find and connect to serial port
        port = self.find_serial_port()
        if not port:
            print("No serial port available. Using console output.")
            self.run_console_simulation()
            return
        
        try:
            self.serial_port = serial.Serial(port, self.baudrate, timeout=1)
            print(f"Connected to {port} at {self.baudrate} baud")
            print()
            
            self.running = True
            
            # Start boot sequence
            boot_thread = threading.Thread(target=self.simulate_boot_sequence)
            boot_thread.start()
            
            # Wait for boot to complete
            time.sleep(3)
            
            # Start measurement loop
            measurement_thread = threading.Thread(target=self.simulate_measurement_loop)
            measurement_thread.start()
            
            print("Simulation running... Press Ctrl+C to stop")
            print()
            
            # Keep running until interrupted
            try:
                while self.running:
                    time.sleep(1)
            except KeyboardInterrupt:
                print("\nStopping simulation...")
                self.running = False
                
        except Exception as e:
            print(f"Error: {e}")
        finally:
            if self.serial_port and self.serial_port.is_open:
                self.serial_port.close()
                print("Serial port closed")
    
    def run_console_simulation(self):
        """Run simulation in console when no serial port available"""
        print("=== Console Simulation Mode ===")
        print()
        
        self.running = True
        
        # Boot sequence
        print("VL53L1X GD32E230 Implementation Ready")
        print("Initializing VL53L1X sensor...")
        time.sleep(0.5)
        print("VL53L1X initialized successfully!")
        print("Starting distance measurements...")
        time.sleep(1)
        
        # Measurement loop
        print()
        print("=== Distance Measurements ===")
        
        try:
            while self.running:
                distance = self.simulate_vl53l1x_measurement()
                print(f"Distance: {distance} mm")
                time.sleep(self.measurement_rate)
                
        except KeyboardInterrupt:
            print("\nStopping simulation...")
            self.running = False

def main():
    simulator = VL53L1X_Simulator()
    
    print("VL53L1X GD32E230 Realistic Simulator")
    print("=====================================")
    print()
    print("This simulator mimics the actual GD32E230 microcontroller")
    print("running the VL53L1X distance measurement program.")
    print()
    print("Features:")
    print("- Realistic boot sequence")
    print("- VL53L1X sensor initialization")
    print("- Continuous distance measurements")
    print("- Realistic measurement noise and variation")
    print("- UART output at 115200 baud")
    print()
    
    # Ask user for simulation mode
    print("Simulation Options:")
    print("1. Sensor connected (real measurements)")
    print("2. Sensor disconnected (dummy values)")
    
    try:
        choice = input("Select mode (1 or 2): ").strip()
        if choice == "2":
            simulator.sensor_connected = False
            print("Mode: Sensor disconnected (dummy values)")
        else:
            print("Mode: Sensor connected (real measurements)")
    except:
        print("Default: Sensor connected (real measurements)")
    
    print()
    print("Starting simulation...")
    print()
    
    simulator.start_simulation()

if __name__ == "__main__":
    main()
