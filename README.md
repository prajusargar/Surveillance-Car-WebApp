# Surveillance-Car-WebApp
Surveillance Car WebApp

### Project Simulation
https://wokwi.com/projects/395154549397872641

### Public Server View:
https://thingspeak.com/channels/2507816



### Write Api GET Requets URL:
https://api.thingspeak.com/update?api_key=X41Y0XE6GQ043KM1&field1=1&field2=12&field3=6&field4=10
https://api.thingspeak.com/update?api_key=X41Y0XE6GQ043KM1&field1=0&field2=22&field3=10&field4=30


Field 1 : Motion Detected (0 = No motion, 1 = motion)
Field 2 : Left  Side Object Distance (distance value in cm)
Field 3 : Front Side Object Distance (distance value in cm)
Field 4 : Right Side Object Distance (distance value in cm)


give html page code for 

### Read Api GET Requets URL:
    Field 1 : Motion - https://api.thingspeak.com/channels/2507816/fields/1.json?results=1
    Field 2 : Left   - https://api.thingspeak.com/channels/2507816/fields/2.json?results=1
    Field 3 : Front  - https://api.thingspeak.com/channels/2507816/fields/3.json?results=1
    Field 4 : Right  - https://api.thingspeak.com/channels/2507816/fields/4.json?results=1


Read Api Response Data:

{
"channel": {
"id": 2507816,
"name": "Surveillance Car",
"latitude": "0.0",
"longitude": "0.0",
"field1": "MotionDetected",
"field2": "LeftDist",
"field3": "FrontDist",
"field4": "RightDist",
"created_at": "2024-04-15T07:09:22Z",
"updated_at": "2024-04-15T07:09:42Z",
"last_entry_id": 2
},
"feeds": [
{
"created_at": "2024-04-15T07:42:20Z",
"entry_id": 2,
"field1": "0"
}
]
}

