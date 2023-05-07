import serial
import time
import os

desktop = os.path.join(os.path.join(os.environ['USERPROFILE']), 'Desktop')
path = desktop + '\\calibragem.txt'
data = list()

time.sleep(2)

try:
    ser = serial.Serial('COM5', 115200)
except:
    print("Erro ao abrir porta serial. ")
    time.sleep(2)
    exit()
    
def endCheck():
    b = ser.readline()
    string_n = b.decode()
    check = string_n.rstrip()
    return check

def leitura():
    b = ser.readline()
    string_n = b.decode()
    string = string_n.rstrip()
    flt = float(string)
    print(flt)
    data.append(flt)

def fechamento():
    fp.close()
    ser.close()

if __name__ == '__main__':
    while (ser.is_open == True and endCheck() != "n"):
        leitura()

    with open(path, 'w+') as fp:
        fp.writelines(data)
    fechamento()