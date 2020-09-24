import firebase_admin
from firebase_admin import credentials

cred = credentials.Certificate("C:\Users\ADMIN\Downloads/to/serviceAccountKey.json")
firebase_admin.initialize_app(cred)