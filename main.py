import cv2
from picamera2 import Picamera2
from ultralytics import YOLO
import serial
import time

priority_order = ['TRECERE DE PIETONI', 'STOP', 'CEDEAZA TRECEREA', 'PRIORITATE']

class_map = {
    'STOP': '1',
    'TRECERE DE PIETONI': '3',
    'CEDEAZA TRECEREA': '2',
    'PRIORITATE': '4'
}

ser = serial.Serial('/dev/ttyUSB0', 9600)
time.sleep(2)

model = YOLO("best.pt")

picam2 = Picamera2()
picam2.preview_configuration.main.size = (640, 480)
picam2.preview_configuration.main.format = "RGB888"
picam2.configure("preview")
picam2.start()

cv2.namedWindow("Live Camera", cv2.WINDOW_NORMAL)
cv2.resizeWindow("Live Camera", 960, 720)

while True:
    frame = picam2.capture_array()
    results = model(frame)[0]
    annotated_frame = results.plot()

    detected_signs = []

    if results.boxes:
        for box in results.boxes:
            conf = float(box.conf[0])
            if conf < 0.6:
                continue

            cls_id = int(box.cls[0])
            class_name = model.names[cls_id]

            if class_name in class_map:
                detected_signs.append(class_name)

    if detected_signs:
        detected_signs.sort(key=lambda x: priority_order.index(x))
        selected = detected_signs[0]
        code = class_map[selected]
        print(f"Detected: {selected} (priority) ? Sent: {code}")
        ser.write(code.encode())
    else:
        ser.write(b' ')  

    cv2.imshow("Live Camera", annotated_frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()
picam2.stop()
