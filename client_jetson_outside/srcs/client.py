from socket import *
import cv2
import time
from ultralytics import YOLO

IP_ADDRESS = "10.10.141.42"
SOCK_NUM = 5000

# 소켓 연결 함수
def socketStart():
    try:
        clientSock = socket(AF_INET, SOCK_STREAM)
        clientSock.connect((IP_ADDRESS, SOCK_NUM))
        print("Connection established.")

        clientSock.send("jetsonOutside".encode("utf-8"))
        print("jetsonOutside sent to server")
        
        return clientSock
    except Exception as e:
        print(f"Socket connection failed: {e}")
        exit(1)

# 실시간 감지 및 서버 전송
def startCapturing(clientSock):
    try:
        model = YOLO('best.pt')
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

                # 감지 여부 전송 (1: 감지됨, 0: 감지 안 됨)
                # message = b"1" if len(detections) > 0 else b"0"
                # clientSock.send(message)
                # print(f"Sent: {message.decode()}")

                if len(detections) > 0:
                    for det in detections:
                        class_id = int(det[5])  # 클래스 ID는 6번째 열에 있음 (0부터 시작)
                        if class_id == 2 or class_id == 4:  # 휠체어(2) 또는 다른 특정 객체(4) 감지 시
                            message = "1".encode("utf-8")
                            break
                    else:
                        message = "0".encode("utf-8")
                else:
                    message = "0".encode("utf-8")

                clientSock.send(message)
                print(f"Sent: {message.decode()}")


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