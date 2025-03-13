from socket import *
import cv2
import time
from ultralytics import YOLO

# 소켓 연결 함수
def socketStart():
    try:
        clientSock = socket(AF_INET, SOCK_STREAM)
        clientSock.connect(('10.10.141.72', 5000))
        print("Connection established.")
        return clientSock
    except Exception as e:
        print(f"Socket connection failed: {e}")
        exit(1)

# 실시간 감지 및 서버 전송
def startCapturing(clientSock):
    try:
        model = YOLO('runs/detect/train/weights/best.pt')
        cap = cv2.VideoCapture(0)  # 웹캠 열기
        start_time = time.time()

        while cap.isOpened():
            ret, frame = cap.read()
            if not ret:
                print("Failed to capture frame.")
                break

            # 5초마다 감지 수행
            if time.time() - start_time >= 5:
                results = model(frame)
                detections = results[0].boxes.data

#{0: 'crutches', 1: 'person', 2: 'push_wheelchair', 3: 'walking_frame', 4: 'wheelchair'}

                if len(detections) > 0: #감지된 객체가 있다면
                    message = "1".encode("utf-8")
                else:
                    message = "0".encode("utf-8")
                clientSock.send(message)
                print(f"Sent: {message.decode()}")
                print("message")

                start_time = time.time()  # 타이머 리셋

            # 실시간 영상 확인 (옵션)
            cv2.imshow('Real-time Detection', frame)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

    except Exception as e:
        print(f"Error during capturing: {e}")

    finally:
        cap.release()
        clientSock.close()
        cv2.destroyAllWindows()
        print("Resources released and socket closed.")

# 실행
clientSock = socketStart()
startCapturing(clientSock)