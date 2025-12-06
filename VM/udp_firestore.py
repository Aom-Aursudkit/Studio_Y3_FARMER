import socket
import firebase_admin
from firebase_admin import credentials, firestore

# --- CONFIGURATION ---
UDP_IP = "0.0.0.0"  # Listen on all interfaces
UDP_PORT = 5005  # Port to listen on
SERVICE_ACCOUNT_FILE = "farmerassist-b8ddd-firebase-adminsdk-fbsvc-26d9563fb1.json"
COLLECTION_NAME = "nodes"  # Firestore collection name
DOCUMENT_NAME = "node_001"  # Existing document name
FIELD_NAME = "waterLevel"

# --- SETUP UDP SERVER ---
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
print(f"Listening for UDP messages on port {UDP_PORT}...")

# --- SETUP FIRESTORE ---
cred = credentials.Certificate(SERVICE_ACCOUNT_FILE)
firebase_admin.initialize_app(cred)
db = firestore.client()

# --- MAIN LOOP ---
while True:
    data, addr = sock.recvfrom(1024)  # Buffer size
    message = 50.00 - float(data.decode()) / 10
    print(f"Received from {addr}: {message}")

    # Add/update the existing document
    try:
        doc_ref = db.collection(COLLECTION_NAME).document(DOCUMENT_NAME)
        # Here we update the document by adding a new field or overwriting an existing one
        doc_ref.update({FIELD_NAME: message})
        print(f"Updated Firestore document: {DOCUMENT_NAME}")
    except Exception as e:
        print(f"Error updating Firestore: {e}")
