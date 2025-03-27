from socket import *
import cv2
import time
from ultralytics import YOLO

IP_ADDRESS = "10.10.141.72"
SOCK_NUM = 5000

# 클래스 ID와 라벨 매핑
CLASS_NAMES = {0: "Wheel", 1: "Wheel-chairs", 2: "handicap-people", 3: "people_wheelchair", 4: "wheel chair", 5: "wheelchair"}

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

            # # 5초마다 감지 수행
            # if time.time() - start_time >= 5:
            #     results = model(frame)
            #     detections = results[0].boxes.data

            #     wheelchair_detected = False  # 휠체어 감지 여부 플래그

            #     if len(detections) > 0:
            #         for det in detections:
            #             det = det.tolist()  # 리스트 변환
            #             if len(det) < 6:  # 데이터 구조 확인
            #                 print("Invalid detection format:", det)
            #                 continue  

            #             x1, y1, x2, y2, conf, class_id = det[:6]  # 필요한 6개 요소만 추출
            #             class_id = int(class_id)

            #             if class_id in CLASS_NAMES:  # 휠체어나 특정 객체일 경우
            #                 label = f"{CLASS_NAMES[class_id]} {conf:.2f}"
            #                 color = (0, 255, 0) if 0 <= class_id <= 5 else (255, 0, 0)  # 클래스에 따라 색상 변경

            #                 # 바운딩 박스 그리기 (정수 변환 필수!)
            #                 x1, y1, x2, y2 = map(int, [x1, y1, x2, y2])
            #                 cv2.rectangle(frame, (x1, y1), (x2, y2), color, 2)
            #                 cv2.putText(frame, label, (x1, max(y1 - 10, 20)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)

            #                 # 감지 플래그 설정
            #                 if 0 <= class_id <= 5:
            #                     wheelchair_detected = True

            #     message = "1".encode("utf-8") if wheelchair_detected else "0".encode("utf-8")
            #     clientSock.send(message)
            #     print(f"Sent: {message.decode()}")

            #     start_time = time.time()  # 타이머 리셋

            results = model(frame)
            detections = results[0].boxes.data

            wheelchair_detected = False  # 휠체어 감지 여부 플래그

            if len(detections) > 0:
                for det in detections:
                    det = det.tolist()  # 리스트 변환
                    if len(det) < 6:  # 데이터 구조 확인
                        print("Invalid detection format:", det)
                        continue  

                    x1, y1, x2, y2, conf, class_id = det[:6]  # 필요한 6개 요소만 추출
                    class_id = int(class_id)

                    if class_id in CLASS_NAMES:  # 휠체어나 특정 객체일 경우
                        label = f"{CLASS_NAMES[class_id]} {conf:.2f}"
                        color = (0, 255, 0) if 0 <= class_id <= 5 else (255, 0, 0)  # 클래스에 따라 색상 변경

                        # 바운딩 박스 그리기 (정수 변환 필수!)
                        if conf > 0.5:
                            x1, y1, x2, y2 = map(int, [x1, y1, x2, y2])
                            cv2.rectangle(frame, (x1, y1), (x2, y2), color, 2)
                            cv2.putText(frame, label, (x1, max(y1 - 10, 20)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)

                        # 감지 플래그 설정
                        if 0 <= class_id <= 5:
                            wheelchair_detected = True

            if time.time() - start_time >= 5:
                message = "1".encode("utf-8") if wheelchair_detected else "0".encode("utf-8")
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
