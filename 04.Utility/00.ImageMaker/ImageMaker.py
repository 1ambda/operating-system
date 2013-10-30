
import sys
import struct


def createDiskImage():
    fDiskImage = open('Disk.img', "wb+")
    return fDiskImage

def readBootLoader(fDiskImage):
    fBootLoader = open('00.BootLoader/BootLoader.bin', 'rb')
    
    # get size of bootloader
    fBootLoader.seek(0, 2)
    sizeOfBootLoader = fBootLoader.tell()
    
    # reset file descriptor
    fBootLoader.seek(0)
    
    fDiskImage.write(fBootLoader.read())
#    padding = struct.pack('b', 0) * (sizeOfBootLoader % 512)
    if ( sizeOfBootLoader % 512 ) != 0 :
        padding = (b'\x00') * (sizeOfBootLoader % 512)
        fDiskImage.write(padding)
    
    fBootLoader.close()
    
    
def readKernel(fDiskImage, Path):
    fKernel = open(Path, 'rb')
    
    fKernel.seek(0, 2)
    sizeOfKernel = fKernel.tell()
    fKernel.seek(0)
    
    fDiskImage.write(fKernel.read())
    if ( sizeOfKernel % 512 ) != 0 :
        for i in range(512 - (sizeOfKernel % 512)):
            fDiskImage.write(b'\x00')
        sizeOfKernel += (512 - (sizeOfKernel % 512))
        
    fKernel.close()
    
    return (sizeOfKernel // 512)

if __name__ == "__main__":
    fDiskImage = createDiskImage()
    readBootLoader(fDiskImage)
    k32sectorNumber = readKernel(fDiskImage, '01.Kernel32/Kernel32.bin')
    k64sectorNumber = readKernel(fDiskImage, '02.Kernel64/Kernel64.bin')
    
    print ("Sector Number :", k32sectorNumber)
    print ("Sector Number :", k64sectorNumber)
    
    fDiskImage.seek(5);
    fDiskImage.write(struct.pack('b', k32sectorNumber +  k64sectorNumber))
    fDiskImage.seek(7);
    fDiskImage.write(struct.pack('b', k32sectorNumber)) 
    
    fDiskImage.close()
    
    
    
    
    
    
    