import smbus
from time import sleep

# Classe para simplificar o acesso ao I2C
class i2c_device:

    # construtor
    def __init__(self, addr):
        # salva o endereco
        self.addr = addr

        # seleciona o i2c conforme a versao do Raspberry Pi
        self.revision = ([l[12:-1] for l in open('/proc/cpuinfo','r').readlines() if l[:8]=="Revision"]+['0000'])[0]
        self.bus = smbus.SMBus(1 if int(self.revision, 16) >= 4 else 0)

    # escreve de valor de 16 bits (sem sinal)
    def writeU16(self, reg, val):
        bytes = [ val >> 8, val & 0xFF ]
        self.bus.write_i2c_block_data(self.addr, reg, bytes)

    # leitura de valor de 16 bits (sem sinal)
    def readU16(self, reg):
        bytes = self.bus.read_i2c_block_data(self.addr, reg, 2)
        ret = (bytes[0] << 8) | bytes[1]
        return ret

# Classe para acesso ao LM75A
class LM75A:

    # construtor
    def __init__(self, addr=0x48):
        # Inicia device
        self.device = i2c_device(addr)
        # Constantes
        self.TEMP = 0
        self.CONF = 1
        self.THYST = 2
        self.TOS = 3

    # iniciacao
    def init(self, tmax, tmin):
        self.device.writeU16(self.TOS, self.codtemp(tmax))
        self.device.writeU16(self.THYST, self.codtemp(tmin))

    # codifica temperatura para escrita em TOS e THYST
    def codtemp(self, temp):
        return int(temp / 0.125)*32

    # le a temperatura
    def read(self):
        val = self.device.readU16(self.TEMP)
        val = val / 32
        return val * 0.125

# Teste simples
if __name__ == "__main__":
    sensor = LM75A()
    sensor.init(24.5, 22)
    while(True):
        try:
            print sensor.read()
            sleep(3)
        except KeyboardInterrupt:
            exit(1)
