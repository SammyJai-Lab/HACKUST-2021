# HACKUST 2021
<img src="/Media/icon.png" height="100px" width="100px" >

## **Theme**: Food and Beverage / Catering Technology

### Background

>What does the food and beverage industry look like in the next normal? While dine-in is expected to rebound, the habit of eating out of store is here to stay. Whether it is ordering online, picking up from stores or delivery, how do we make every customer experience remain great or even better every single time?

### The Problem Statement

>**You are to develop a viable solution to meet customers’ expectation of hygiene or even go beyond that. And to inject agility into a caterer’s operations and leverage data to enable fast response to shifts between dine-in and out-of-store orders.**

---

## Our Solution
### Analytical sprightly

Our solution to the problem is an IoT node made by Node MCU made to collect dining data more precisely and aim to help managers gain more information about the operations of the restaurant.

### The main module of the node
<img src="/Media/MainModuleV2.png">
<img src="/Media/MainModuleV2WithNote.jpg">

The main module of the node is made using a NodeMcu V3, two leds and a button held inside a 3d printed case.

### Sample installation
<img src="/Media/SampleInstallation.png">

Other than the main module, the node also includes an RFID module which can be installed at the side of the dining table and connected to the main module by a cable.

### Circuit Diagram
<img src="/Media/Circuit.png">

This is the circuit diagram of the node.

### Firebase Example
<img src="/Media/DBExample.png">

Nodes are connected to a Realtime Database of Firebase. In this example, the document FirebaseIOT contains all the data of one node.

| Key          | Type   | Value      | Description                                                                          |
|--------------|--------|------------|--------------------------------------------------------------------------------------|
| ResetPending | Int    | 0 or 1     | Shows whether the node is pending for a reset                                        |
| buttonState  | Int    | 0 or 1     | Showing whether the button is pressed                                                |
| foodDelivery | Array  | -          | An array of UID of waiters showing the food delivery history of the particular table |
| ledGreen     | string | "0" or "1" | Showing the state of the green led on the node                                       |
| ledRed       | string | "0" or "1" | Showing the state of the red led on the node                                         |
| serveHistory | Array  | -          | An array of UID of waiters showing the serve history of the particular table         |
| serveState   | Int    | 0 or 1     | Showing whether the table needs to be served                                         |
---

### Data collected
By using this node, we can collect a handful of dining data. 
Which include:
* History of serving (with timestamp)
* History of food delivery (with timestamp)
* Dining time of customers
* Time needed for serving (Time from the button pressed to waiter register their tag)

### Sample usage
Data collected can be used for:
* Estimating the waiting time
> By making a more accurate waiting time avaliable to the customers, customers could decide whether they want to dine-in or order a take-away which can potentially make restaurants less crowded and improve hygiene
* Monitoring the utilization of staffs
> By collecting data of staffs and doing simple analysis, we can get the utilization of staffs and managers can make use of this data to prevent shortage or surplus
* Enabling fast response to shifts between dine-in and out-of-store orders
> Knowing the utilization of staffs and customer's dining information, managers can decide how much resources should be allocated to dine-in services and out-of-store services respectively. And this enables a fast response to shifts between dine-in and out-of-store orders




<img height = 15> 

# Libraries used

### Node:
* ESP8266WiFi.h "http://arduino.esp8266.com/stable/package_esp8266com_index.json"
* FirebaseESP8266.h
* SPI.h
* MFRC522.h

### Backend:
* pyrebase
```bash
pip install pyrebase
```
* eel
```bash
pip install eel
```

---
## License & copyright

Licensed under the [GNU AFFERO GENERAL PUBLIC LICENSE](License)