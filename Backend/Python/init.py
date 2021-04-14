import firebase_admin
from firebase_admin import credentials
from firebase_admin import db


firebaseRTURL = "https://fir-iot-85fdf-default-rtdb.firebaseio.com/"
firebaseRTAuth = "sG8YYCucVC9Xjd0503QDNCJybTLKBAFPpANTTwN0"

firebase = firebase.FirebaseApplication(firebaseRTURL, None)

result = firebase.get('/FirebaseIOT/Nodes/','')
print(result)