import websocket
import logging

logging.basicConfig(
    level=logging.DEBUG,
    format='%(asctime)s.%(msecs)03d %(levelname)s: %(message)s',
    datefmt='%Y-%m-%d %H:%M:%S',
)

ws = websocket.WebSocketApp("ws://192.168.4.1/ws",
                            on_open=lambda x: None,
                            on_message=lambda _, x: logging.info(x),
                            on_close=lambda x: None)
 
ws.run_forever()
