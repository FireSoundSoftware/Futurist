import cv2
import mediapipe as mp
import math
import numpy as np
import osascript  # pip install osascript

from mediapipe.tasks import python
from mediapipe.tasks.python import vision
import time


class GestureDetector:
  def __init__(self):

# solution APIs
    self.mp_drawing = mp.solutions.drawing_utils
    self.mp_drawing_styles = mp.solutions.drawing_styles
    self.mp_hands = mp.solutions.hands

    # macOS Volume Control (0-100)
    self.minVol, self.maxVol = 0, 100
    self.volBar, self.volPer = 400, 0
    self.volPer = 0
    self.length = 0
    self.volBar = 0

    # Webcam Setup
    self.wCam, self.hCam = 640, 480
    self.cam = cv2.VideoCapture(0)
    self.cam.set(3, self.wCam)
    self.cam.set(4, self.hCam)

    self.BaseOptions = mp.tasks.BaseOptions
    self.GestureRecognizer = mp.tasks.vision.GestureRecognizer
    self.GestureRecognizerOptions = mp.tasks.vision.GestureRecognizerOptions
    self.VisionRunningMode = mp.tasks.vision.RunningMode

    self.current_gesture = None


    model_path = 'model/gesture_recognizer.task'
    self.options = self.GestureRecognizerOptions(
        base_options=self.BaseOptions(model_asset_path=model_path),
        running_mode=self.VisionRunningMode.LIVE_STREAM,
        result_callback=self.print_result)

    self.recognizer = self.GestureRecognizer.create_from_options(self.options)

# Mediapipe Hand Landmark Model
    self.hands = self.mp_hands.Hands(
            model_complexity=0,
            min_detection_confidence=0.5,
            min_tracking_confidence=0.5) 
    self.prev_time = 0

  def print_result(self, result, output_image, timestamp_ms):
    if result.gestures and len(result.gestures) > 0:
        # Берем первый жест (самый уверенный)
        gesture_list = result.gestures[0]
        if gesture_list:
            top_gesture = gesture_list[0]  # Первый Category в списке
            self.current_gesture = top_gesture
            #print(f"Жест: {top_gesture.category_name} (score: {top_gesture.score:.2f})")

  def run(self):
    while self.cam.isOpened():
      success, image = self.cam.read()

      image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

      mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=image)
      results = self.hands.process(image)

      current_time = int(time.time() * 1000)  # ms, строго растет
      if current_time > self.prev_time:
        self.recognizer.recognize_async(mp_image, current_time)
        self.prev_time = current_time
        print("Current gesture", self.current_gesture)
      image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

      if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
          self.mp_drawing.draw_landmarks(
            image,
            hand_landmarks,
            self.mp_hands.HAND_CONNECTIONS,
            self.mp_drawing_styles.get_default_hand_landmarks_style(),
            self.mp_drawing_styles.get_default_hand_connections_style()
          )

      # Finding position of Hand landmarks
      if self.current_gesture is not None:
        if self.current_gesture.category_name == "Pointing_Up":
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

            self.length = math.hypot(x2 - x1, y2 - y1)
            if self.length < 50:
              cv2.line(image, (x1, y1), (x2, y2), (0, 0, 255), 3)

            # Интерполяция расстояния в громкость (0-100)
            self.volPer = np.interp(self.length, [50, 220], [0, 100])



            # Volume Bar
            self.volBar = np.interp(self.length, [50, 220], [400, 150])
      cv2.rectangle(image, (50, 150), (85, 400), (0, 255, 0), 3)
      cv2.rectangle(image, (50, int(self.volBar)), (85, 400), (255, 0, 0), cv2.FILLED)
      cv2.putText(image, f'{int(self.volPer)} %', (40, 450), cv2.FONT_HERSHEY_COMPLEX,
                  1, (255, 0, 0), 3)

      cv2.imshow('Hand Volume Control (macOS)', image)
      if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    self.cam.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    d = GestureDetector()
    d.run()
