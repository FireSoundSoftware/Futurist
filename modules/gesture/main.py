import cv2
import mediapipe as mp
import math
import numpy as np
import osascript  # pip install osascript

# solution APIs
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_hands = mp.solutions.hands

# macOS Volume Control (0-100)
minVol, maxVol = 0, 100
volBar, volPer = 400, 0

# Webcam Setup
wCam, hCam = 640, 480
cam = cv2.VideoCapture(0)
cam.set(3, wCam)
cam.set(4, hCam)

# Mediapipe Hand Landmark Model
with mp_hands.Hands(
        model_complexity=0,
        min_detection_confidence=0.5,
        min_tracking_confidence=0.5) as hands:

  while cam.isOpened():
    success, image = cam.read()

    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    results = hands.process(image)
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

    if results.multi_hand_landmarks:
      for hand_landmarks in results.multi_hand_landmarks:
        mp_drawing.draw_landmarks(
          image,
          hand_landmarks,
          mp_hands.HAND_CONNECTIONS,
          mp_drawing_styles.get_default_hand_landmarks_style(),
          mp_drawing_styles.get_default_hand_connections_style()
        )

    # Finding position of Hand landmarks
    lmList = []
    if results.multi_hand_landmarks:
      myHand = results.multi_hand_landmarks[0]
      for id, lm in enumerate(myHand.landmark):
        h, w, c = image.shape
        cx, cy = int(lm.x * w), int(lm.y * h)
        lmList.append([id, cx, cy])

        # Thumb и Index finger position
    if len(lmList) != 0:
      x1, y1 = lmList[4][1], lmList[4][2]  # Thumb tip
      x2, y2 = lmList[8][1], lmList[8][2]  # Index tip

      # Рисуем маркеры и линию
      cv2.circle(image, (x1, y1), 15, (255, 255, 255), cv2.FILLED)
      cv2.circle(image, (x2, y2), 15, (255, 255, 255), cv2.FILLED)
      cv2.line(image, (x1, y1), (x2, y2), (0, 255, 0), 3)

      length = math.hypot(x2 - x1, y2 - y1)
      if length < 50:
        cv2.line(image, (x1, y1), (x2, y2), (0, 0, 255), 3)

      # Интерполяция расстояния в громкость (0-100)
      vol = np.interp(length, [50, 220], [minVol, maxVol])
      volPer = np.interp(length, [50, 220], [0, 100])

      # Установка громкости через osascript (AppleScript)
      osascript.osascript(f'set volume output volume {int(vol)}')

      # Volume Bar
      volBar = np.interp(length, [50, 220], [400, 150])
      cv2.rectangle(image, (50, 150), (85, 400), (0, 255, 0), 3)
      cv2.rectangle(image, (50, int(volBar)), (85, 400), (255, 0, 0), cv2.FILLED)
      cv2.putText(image, f'{int(volPer)} %', (40, 450), cv2.FONT_HERSHEY_COMPLEX,
                  1, (255, 0, 0), 3)

    cv2.imshow('Hand Volume Control (macOS)', image)
    if cv2.waitKey(1) & 0xFF == ord('q'):
      break

cam.release()
cv2.destroyAllWindows()
