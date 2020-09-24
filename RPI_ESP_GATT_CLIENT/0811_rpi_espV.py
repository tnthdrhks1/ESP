import struct
from bluepy.btle import Scanner, DefaultDelegate, UUID, Peripheral

#TARGET_UUID = "4d6fc88bbe756698da486866a36ec78e"
TARGET_UUID = "4c0002154d6fc88bbe756698da486866a36ec78e0001000100"
target_dev = None
UART_SERVICE_UUID = UUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b")
uart_service = None
UART_WRITE_UUID = UUID("beb5483e-36e1-4688-b7f5-ea07361b26a8")
write_char = None
UART_READ_UUID = UUID("beb5483e-36e1-4688-b7f5-ea07361b26fa")
read_char = None
read_handle = None
read_cccd = None
#global phk
direction= None
global data 
#data = []
#global direction_data
data1=[]
testvalue=[]
resp=[]
#############################################
# Define scan callback
#############################################
class ScanDelegate(DefaultDelegate):
    
    def __init__(self):
        DefaultDelegate.__init__(self)
        print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")

    def handleDiscovery(self, dev, isNewDev, isNewData):
        if isNewDev:
            print("Discovered device %s" % dev.addr)
        elif isNewData:
            print("Received new data from %s", dev.addr)
        print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
#############################################
# Define notification callback
#############################################
class NotifyDelegate(DefaultDelegate):
    global direction_data
#    global data
    #Constructor (run once on startup)  
    def __init__(self, params):
        DefaultDelegate.__init__(self)
        print("###########################################")
    #func is caled on notifications
    def handleNotification(self, cHandle, data):
        print("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$")
        #global data
        global phk
        phk = data
        
        print("Notification : " + data.decode("utf-8"))
        print("data", data)
        if data == 'left':
             print("leftside")
             #direction_data = '1'
             
        #direction1 = direction     
        #data1 = data     
        #testvalue = 'asibal'
        return data, phk#data1, #data, testvalue, resp
        
#############################################
# Main starts here
#############################################
scanner = Scanner().withDelegate(ScanDelegate())
devices = scanner.scan(10.0)

for dev in devices:
    print("Device %s (%s), RSSI=%d dB" % (dev.addr, dev.addrType, dev.rssi))

    for (adtype, desc, value) in dev.getScanData():
        # Check iBeacon UUID
        # 255 is manufacturer data (1  is Flags, 9 is Name)
        if adtype is 255 and TARGET_UUID in value:
            target_dev = dev
            print("  +--- found target device!!")
        print("  (AD Type=%d) %s = %s" % (adtype, desc, value))
print("1111111111111111111111111111111111111111111")
if target_dev is not None:
    #############################################
    # Connect
    #############################################
    print("Connecting...")
    print(" ")
    p = Peripheral(target_dev.addr, target_dev.addrType)
    print("2222222222222222222222222222222222222222")
    try:
        # Set notify callback
        p.setDelegate( NotifyDelegate(p) )

        #############################################
        # For debug
        #############################################
        services=p.getServices()
        # displays all services
        for service in services:
            print(service)
            # displays characteristics in this service
            chList = service.getCharacteristics()
            print("-------------------------------------------------------")
            print("Handle   UUID                                Properties")
            print("-------------------------------------------------------")
            for ch in chList:
                print("  0x"+ format(ch.getHandle(),'02X')  +"   "+str(ch.uuid) +" " + ch.propertiesToString())
            print("-------------------------------------------------------")
            print(" ")
            print("333333333333333333333333333333")
        #############################################
        # Set up characteristics
        #############################################
        uart_service = p.getServiceByUUID(UART_SERVICE_UUID)
        write_char = uart_service.getCharacteristics(UART_WRITE_UUID)[0]
        read_char = uart_service.getCharacteristics(UART_READ_UUID)[0]
        print("444444444444444444444444444444444444444444444444444")
        read_handle = read_char.getHandle()
        # Search and get the read-Characteristics "property" 
        # (UUID-0x2902 CCC-Client Characteristic Configuration))
        # which is located in a handle in the range defined by the boundries of the Service
        for desriptor in p.getDescriptors(read_handle, 0xFFFF):  # The handle range should be read from the services 
            if (desriptor.uuid == 0x2902):                   #      but is not done due to a Bluez/BluePy bug :(     
                print("Client Characteristic Configuration found at handle 0x"+ format(desriptor.handle, "02X"))
                read_cccd = desriptor.handle
                p.writeCharacteristic(read_cccd, struct.pack('<bb', 0x01, 0x00))
        print("##1",data1)
        #print("##2", data)
        print("##3",resp)
        print("5555555555555555555555555555555555")
        
        #############################################
        # BLE message loop
        #############################################
        while 1:
            #global direction_data
            #global data
            #print(data1)
            #print(data)
            #print(testvalue)
            #print("direction while")
            #print("#dr1 %s #d1 %s" %(direction1, data1))
            #print("#0" , direction_data)
            #print("#0.5", data)
            #print("#1" , resp)
            #print("%1.5", phk)
            print("66666666666666666666666666666666666666666666666666")
            if p.waitForNotifications(5.0):
                #print("#2" , resp)
                #print("#3" , data)
                #print("#4" , direction_data)
                print("%5", phk)
                #handleNotification() #was called
            #    print("#dr2 %s #d2 %s" %(direction1, data1))
                if phk == 'left':
                    print("sidong huga")
                    write_char.write(str.encode("huga"))
                
                print("777777777777777777777777777777777777")
                continue
            #if direction == 'left':
            #    print("left OK")     
            # p.writeCharacteristic(hButtonCCC, struct.pack('<bb', 0x01, 0x00))
            write_char.write(str.encode("hello~"))
            write_char.write(str.encode("poweron"))
            print("88888888888888888888888888888888888888888888888888888888888888")
    finally:
        p.disconnect()

else:
    print("No matching device found...")


print("Close app")
