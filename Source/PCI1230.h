#ifndef __PCI1230_H__
#define __PCI1230_H__

#define PCI1230_API __declspec(dllimport) unsigned int __stdcall

//maximum boards supported in one system
#define MAX_BOARD	16

//API return code
typedef enum _PCI1230RETURN_CODE
{
    PCI1230Success = 0x400,
    PCI1230ApiFailed,
    PCI1230InvalidParam,
    PCI1230DevNotFind
} PCI1230RETURN_CODE;


    PCI1230_API Pci1230Open(unsigned int boardid);
    PCI1230_API Pci1230Close(unsigned int boardid);

    PCI1230_API Pci1230Read(unsigned int boardid, unsigned int* pData);
    PCI1230_API Pci1230Write(unsigned int boardid, unsigned int Data);

    PCI1230_API Pci1230ReadDiBit(unsigned int boardid, unsigned int bit, unsigned int* pData);
    PCI1230_API Pci1230WriteDoBit(unsigned int boardid, unsigned int bit, unsigned int Data);
    PCI1230_API Pci1230ReadDoBit(unsigned int boardid, unsigned int bit, unsigned int* pData);


#endif