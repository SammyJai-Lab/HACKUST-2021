import pyrebase

config = {
    "apiKey": "AIzaSyBdMgthiUTBi17UW4MhV9E7a3dQ8OUFQ3E",
    "authDomain": "fir-iot-85fdf.firebaseapp.com",
    "databaseURL": "https://fir-iot-85fdf-default-rtdb.firebaseio.com",
    "projectId": "fir-iot-85fdf",
    "storageBucket": "fir-iot-85fdf.appspot.com",
    "messagingSenderId": "724659573143",
    "appId": "1:724659573143:web:7d93a82b30b178c3f134d2",
    "measurementId": "G-XN3TMR9BT5"
}

root = "FirebaseIOT"

firebase=pyrebase.initialize_app(config)
db=firebase.database()

def reset():
    ButtonCount = db.child(root).child("ButtonCount").get().val()
    print("Button Count: " + str(ButtonCount))

    db.child(root).update({"ResetPending": 0})
    db.child(root).update({"ButtonCount": 0})
    db.child(root).update({"ledGreen": "0"})
    db.child(root).update({"ledRed": "0"})

    serveHistory = db.child(root).child("serveHistory").get()
    if (type(serveHistory.val()) != type(None)):
        for serve in serveHistory:
            print(str(serve.key()) + ":" + str(serve.val()))
            db.child(root).child("serveHistory").child(serve.key()).remove()

    foodDeliverHistory = db.child(root).child("foodDelivery").get()
    if (type(foodDeliverHistory.val()) != type(None)):
        for deliver in foodDeliverHistory:
            print(str(deliver.key()) + ":" + str(deliver.val()))
            db.child(root).child("foodDelivery").child(deliver.key()).remove()

def 