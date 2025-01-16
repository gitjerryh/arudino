from cvzone.HandTrackingModule import HandDetector
import cvzone
from pynput.keyboard import Controller
import cv2
import mediapipe as mp
import math
import serial
import time
import logging

logging.basicConfig(level=logging.DEBUG)

# mediapipe library requires us to provide a "confidence" value that determines how strictly it must check for hands.
detector = HandDetector(detectionCon=0.8)

# 初始化串口
try:
    ser = serial.Serial('COM4', 115200, timeout=1)
    time.sleep(2)  # 等待串口稳定
    logging.info("Serial port initialized")
except Exception as e:
    logging.error(f"Serial port error: {e}")
    exit()

# the following three lines are to help us change the colors of our finger tips and line joining them in mediapipe library
mp_drawing = mp.solutions.drawing_utils
hand_mpDraw = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands




'''
# # # # useless part

finalText = ""
keys = [["fan"]]
keyboard = Controller()
class Button():
    def __init__(self, pos, text, size=[150, 100]):
        self.pos = pos
        self.size = size
        self.text = text
def drawAll(img, buttonList):
    for button in buttonList:
        x, y = button.pos
        w, h = button.size
        cvzone.cornerRect(img, (button.pos[0], button.pos[1], button.size[0], button.size[1]),20, rt=0)
        cv2.rectangle(img, button.pos, (x + w, y + h), (255, 0, 255), cv2.FILLED)
        cv2.putText(img, button.text, (x + 20, y + 65), cv2.FONT_HERSHEY_PLAIN, 4, (255, 255, 255), 4)
    return img
def on_change(val):
    pass  # 空函数,仅用于满足createTrackbar的要求

buttonList = []
for i in range(len(keys)):
    for j, key in enumerate(keys[i]):
        buttonList.append(Button([100 * j + 50, 100 * i + 50], key))
'''


def motor(cap):
    with mp_hands.Hands(
            min_detection_confidence=0.7,
            min_tracking_confidence=0.7) as hands:
        try:
            time.sleep(1)
            pinch_count = 0
            distance = 1000
            last_angle = -1
            
            while cap.isOpened():
                success, image = cap.read()
                if not success:
                    continue
                
                image = cv2.flip(image, 1)
                image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
                image.flags.writeable = False
                results = hands.process(image)
                image.flags.writeable = True
                image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
                
                if results.multi_hand_landmarks:
                    for hand_landmarks in results.multi_hand_landmarks:
                        lmList = []
                        for id, lm in enumerate(hand_landmarks.landmark):
                            h, w, c = image.shape
                            cx, cy = int(lm.x * w), int(lm.y * h)
                            lmList.append([id, cx, cy])
                            
                            # 添加手指关键点标识
                            if id in [4, 8, 12, 16, 20]:  # 拇指、食指、中指、无名指、小指的指尖
                                cv2.circle(image, (cx, cy), 12, (255, 0, 0), cv2.FILLED)  # 蓝色圆点
                                cv2.putText(image, str(id), (cx-10, cy-10), 
                                          cv2.FONT_HERSHEY_PLAIN, 2, (255, 255, 255), 2)  # 添加标号
                        
                        if len(lmList) >= 13:
                            # 绘制拇指和食指之间的控制线
                            cv2.line(image, (lmList[4][1], lmList[4][2]), 
                                   (lmList[8][1], lmList[8][2]), (0, 255, 0), 3)  # 绿色线
                            
                            # 计算拇指和食指之间的距离用于控制舵机
                            angle = int(math.hypot(lmList[8][1] - lmList[4][1], 
                                                 lmList[8][2] - lmList[4][2]))
                            
                            # 在控制线中点显示距离
                            cx = (lmList[4][1] + lmList[8][1]) // 2
                            cy = (lmList[4][2] + lmList[8][2]) // 2
                            cv2.circle(image, (cx, cy), 8, (0, 255, 0), cv2.FILLED)
                            cv2.putText(image, f'Dist: {angle}', (cx+10, cy), 
                                      cv2.FONT_HERSHEY_PLAIN, 2, (0, 255, 0), 2)
                            
                            # 映射到舵机角度
                            angle = min(180, max(0, int(angle / 2)))
                            
                            # 显示舵机角度
                            cv2.putText(image, f'Angle: {angle}', (50, 50), 
                                      cv2.FONT_HERSHEY_PLAIN, 2, (255, 0, 255), 2)
                            
                            if abs(angle - last_angle) > 5:
                                try:
                                    ser.write(f"{angle}\n".encode())
                                    logging.info(f"Sending angle: {angle}")
                                    last_angle = angle
                                except Exception as e:
                                    logging.error(f"Servo control error: {e}")
                            
                            # 绘制食指和中指之间的退出检测线
                            distance = int(math.hypot(lmList[12][1] - lmList[8][1], 
                                                    lmList[12][2] - lmList[8][2]))
                            cv2.line(image, (lmList[8][1], lmList[8][2]), 
                                   (lmList[12][1], lmList[12][2]), (255, 0, 0), 3)  # 蓝色线
                            
                            # 显示退出检测距离
                            cx = (lmList[8][1] + lmList[12][1]) // 2
                            cy = (lmList[8][2] + lmList[12][2]) // 2
                            cv2.putText(image, f'Exit: {distance}', (cx+10, cy), 
                                      cv2.FONT_HERSHEY_PLAIN, 2, (255, 0, 0), 2)
                
                cv2.imshow('Hand Gesture Control', image)
                key = cv2.waitKey(1) & 0xFF
                
                if key == 27:
                    logging.info("ESC pressed, exiting...")
                    break
                
                if results.multi_hand_landmarks and distance < 20:
                    pinch_count += 1
                    logging.debug(f"Pinch count: {pinch_count}")
                else:
                    pinch_count = 0
                
                if pinch_count > 3:
                    logging.info("Fingers pinched consistently, exiting...")
                    break
                    
        finally:
            logging.info("Cleaning up...")
            cap.release()
            cv2.destroyAllWindows()
            ser.close()

def main_fun():
    cap = cv2.VideoCapture(0)
    motor(cap)

if __name__ == "__main__":
    main_fun()
