import struct, array, time, io, fcntl

# Classe para simplificar o acesso ao I2C
class i2c_device:

    # construtor
    def __init__(self, addr):
        # salva o endereco
        self.addr = addr

        # seleciona o i2c conforme a versao do Raspberry Pi
        self.revision = ([l[12:-1] for l in open('/proc/cpuinfo','r').readlines() if l[:8]=="Revision"]+['0000'])[0]
        self.busNumber = 1 if int(self.revision, 16) >= 4 else 0
            
        # inicia o acesso ao i2c
        self.fd = None
        self.open(self.busNumber)

    # escreve de valor de 16 bits (sem sinal) em um registrador
    def writeReg(self, reg, val):
        data = [ reg, val >> 8, val & 0xFF ]
        buf = bytearray(data)
        self.fd.write(buf)

    # leitura de valor de 16 bits (sem sinal) de um registrador
    def readReg(self, reg):
        data = [ reg ]
        buf = bytearray(data)
        self.fd.write(buf)
        time.sleep(0.0625)
        data = self.fd.read(2)
        buf = array.array('B', data)
        ret = (buf[0] << 8) | buf[1]
        return ret

    # dispara leitura de temperatura ou umidade
    def access(self, reg):
        data = [ reg ]
        buf = bytearray(data)
        self.fd.write(buf)
        
    # pega o resultado de uma leitura
    def readResult(self):
        data = self.fd.read(2)
        buf = array.array('B', data)
        ret = (buf[0] << 8) | buf[1]
        return ret

    def open(self, bus):
        self.fd = io.open("/dev/i2c-{}".format(bus), "r+b", buffering=0)
        I2C_SLAVE = 0x0703
        fcntl.ioctl(self.fd, I2C_SLAVE, self.addr)

    def close(self):
        if self.fd:
            self.fd.close()
            self.fd = None

# Classe para acesso ao HDC1080
class HDC1080(object):
    # construtor
    def __init__(self, addr=0x40):
        # Constantes
        self.REG_TEMP = 0
        self.REG_UMID = 1
        self.REG_CONF = 2
        self.CONFIG = 0 # ler separado temp/umid, resolucao alta
        # Guarda o endereco
        self.addr = addr

    # Iniciacao para uso com with
    def __enter__(self):
        self.init()
        return self

    # limpeza para uso com with
    def __exit__(self, exc_type, exc_val, exc_tb):
        self.device.close()

    # iniciacao
    def init(self):
        # Inicia device
        self.device = i2c_device(self.addr)
        # espera acabar reset
        while((self.device.readReg(self.REG_CONF) & 0x8000) != 0):
            time.sleep(0.1)
        # ler separado temperatura e umidade
        self.device.writeReg(self.REG_CONF, self.CONFIG)

    # le a temperatura
    def readTemp(self):
        # dispara a leitura
        self.device.access(self.REG_TEMP)
        # aguarda
        time.sleep(0.05)
        # le o resultado
        val = self.device.readResult()
        val = ((val * 1650) >> 16) - 400
        return val / 10.0

    # le a umidade
    def readUmid(self):
        # dispara a leitura
        self.device.access(self.REG_UMID)
        # aguarda
        time.sleep(0.05)
        # le o resultado
        val = self.device.readResult()
        return (val * 100) >> 16

# Teste simples
if __name__ == "__main__":
    try:
        with HDC1080() as sensor:
            while(True):
                    print sensor.readTemp(), sensor.readUmid()
                    time.sleep(10)
    except KeyboardInterrupt:
        exit(1)
