from tkinter import Pack
import cv2
from serial_connection import stm_serial_handle
from serial_connection import Packet
from simple_ui import read_only_ui
from rp_recognition import registration_plates_recognition
from db_wrapper import db_wrapper
import time

vid = cv2.VideoCapture(0) #get video
report_generator = read_only_ui()
database_wrapper = db_wrapper()
millis = time.time() * 1000

def print_reports(reports : list) -> None:
    print("Ilość samochodów w danym roku:")
    for entries, year in reports[0]:
        print(f"{year}: {entries}")

    print("Ilość samochodów w danym miesiącu w roku 2022:")
    for entries, month in reports[1]:
        print(f"{month}: {entries}")
    
    print("Ilość prób dostępu w tym dniu:")
    for entries in reports[2]:
        print(f"{entries}")

    print("Samochody, które mogą wjechać:")
    for approved in reports[3]:
        print(f"{approved}")

    print("Ilość samochodów, które wjechały na parking:")
    for entries in reports[4]:
        print(f"{entries}")

def main():
    stm_serial_handle.write(Packet(0, "close").get_packet()) #close the gate at start

    while(True):
        ret, frame = vid.read() #read video from camera
        recognise = registration_plates_recognition(frame)
        cv2.imshow('frame', frame) #show frame to be recognised
        plate = recognise.recognise() #try to recognise registration plate
        if(plate != None): #if plate`s been recognised
            print(plate)
        
        if(plate != None):
            database_wrapper.insert_log(plate)

        if(plate != None and database_wrapper.check_if_rp_exists(plate) >= 1):
            stm_serial_handle.write(Packet(0, "open").get_packet())
        
        else:
            stm_serial_handle.write(Packet(0, "close").get_packet())

        if(time.time() * 1000 - millis > 5000): #get reports every 5 seconds
            reports = report_generator.get_reports()
            print_reports(reports)
            millis = time.time() * 1000

        if cv2.waitKey(1) & 0xFF == ord('q'): #end program if user pressed q
            break

    vid.release() #end the program
    cv2.destroyAllWindows()


main()