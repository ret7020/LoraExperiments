import websocket
import logging
import json
from termcolor import colored

logging.basicConfig(
    level=logging.DEBUG,
    format='%(asctime)s.%(msecs)03d: %(message)s',
    datefmt='%H:%M:%S',
)

stats = {"last10RSSI": 0,
         "last10Loss": 0,
         "packetCnt": 0,
         "totalLoss": 0,
         "lastId": -1}

def parsePacket(packet):
    global stats
    loss = None
    parsed = json.loads(packet)
    packetCheck = colored("OK", "green") if parsed["PacketCheck"] else colored("NO", "red")
    if parsed['RSSI'] == 0: parsed['RSSI'] = 255
    if stats["lastId"] == -1: stats["lastId"] = parsed["PacketId"]
    else: loss = parsed["PacketId"] - stats["lastId"] - 1
    if loss:
        logging.info(colored(f"Lost cnt: {loss}", "red"))
    logging.info(f"{packetCheck} RSSI: {parsed['RSSI']} {parsed['PacketId']}")
    stats["lastId"] = parsed["PacketId"]


ws = websocket.WebSocketApp("ws://192.168.4.1/ws",
                            on_open=lambda x: None,
                            on_message=lambda _, x: parsePacket(x),
                            on_close=lambda x: None)
 
ws.run_forever()
