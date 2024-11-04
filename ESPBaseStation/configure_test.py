import websocket
import json

ESP_IP = "192.168.1.20"


def command_waiter():
    while 1:
        cmd = input("cmd")
        ws.send(json.dumps({"action": 2})),


ws = websocket.WebSocketApp(f"ws://{ESP_IP}/ws",
                            on_open=lambda x: command_waiter(),
                            on_close=lambda x: None)

ws.run_forever()


