import cv2
from picamera2 import Picamera2
import time

# Initializeaza camera
picam2 = Picamera2()
picam2.preview_configuration.main.size = (1280, 720)  # rezolutie decenta pentru preview
picam2.preview_configuration.main.format = "RGB888"
picam2.configure("preview")
picam2.start()

# Fereastra pentru preview
cv2.namedWindow("Live Camera", cv2.WINDOW_NORMAL)
cv2.resizeWindow("Live Camera", 1280, 720)

# Preview + posibilitatea de a salva imaginea la apasarea tastei 's'
while True:
    frame = picam2.capture_array()
    cv2.imshow("Live Camera", frame)

    key = cv2.waitKey(1) & 0xFF
    if key == ord('s'):
        filename = f"poza_{int(time.time())}.jpg"
        picam2.capture_file(filename)
        print(f"Poza salvata: {filename}")
    elif key == ord('q'):
        break

cv2.destroyAllWindows()
picam2.stop()
