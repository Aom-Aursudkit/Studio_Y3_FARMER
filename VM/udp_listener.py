import socket
import requests
import json

# --- CONFIGURATION ---
UDP_IP = "0.0.0.0"  # Listen on all interfaces
UDP_PORT = 5005  # Choose any port you want
FIREBASE_URL = "https://ais-nb-bc95-test-99137-default-rtdb.asia-southeast1.firebasedatabase.app/messages.json"  # Replace with your Firebase URL

# --- SETUP UDP SERVER ---
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
print(f"Listening for UDP messages on port {UDP_PORT}...")

while True:
    data, addr = sock.recvfrom(1024)  # buffer size 1024 bytes
    message = data.decode()
    print(f"Received from {addr}: {message}")

    # Prepare JSON payload for Firebase
    payload = {"message": message}

    try:
        response = requests.post(FIREBASE_URL, json=payload)
        print(f"Sent to Firebase, status: {response.status_code}")
    except Exception as e:
        print(f"Error sending to Firebase: {e}")
