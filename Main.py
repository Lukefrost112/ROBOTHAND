import cvzone.HandTrackingModule
import cvzone.SerialModule
import cv2


cap = cv2.VideoCapture(0)
detector = cvzone.HandTrackingModule.HandDetector(maxHands=1, detectionCon=0.7)
mySerial = cvzone.SerialModule.SerialObject("COM5", 9600, 1)

while True:
    success, img = cap.read()
    if success:
        hands, img = detector.findHands(img)  # hands will contain the information about detected hands

        # If hands are detected
        if hands:
            # Assuming maxHands=1, so we take the first hand detected
            hand = hands[0]  # hand is a dictionary
            fingers = detector.fingersUp(hand)  # Now passing the correct argument
            print(fingers)
            mySerial.sendData(fingers)
    else:
        print("Failed to capture image from camera.")

    cv2.imshow("Image", img)
    if cv2.waitKey(1) & 0xFF == ord('q'):  # Press 'q' to exit
        break

cap.release()
cv2.destroyAllWindows()
