#pragma once
#include <stdint.h>
#include <LibC/stdbool.h>
#include <Disk/GenericDiskController.h>
#include <Disk/defs.h>
#include <PCI/Device.h>
#include <PCI/PCI.h>

#define AHCI_MAXIMUM_COMMAND_SLOTS 32
#define AHCI_MAXIMUM_PORTS 32

#define AHCI_HBA_PxIS_TFES   (1 << 30)       /* TFES - Task File Error Status */
#define AHCI_ABAR_BASE 0x24

#define HBA_PORT_IPM_ACTIVE 1
#define HBA_PORT_DET_PRESENT 3

#define	SATA_SIG_ATA	0x00000101	// SATA drive
#define	SATA_SIG_ATAPI	0xEB140101	// SATAPI drive
#define	SATA_SIG_SEMB	0xC33C0101	// Enclosure management bridge
#define	SATA_SIG_PM	    0x96690101	// Port multiplier 

namespace AHCI
{
    typedef enum
    {
        FIS_TYPE_REG_H2D	= 0x27,	// Register FIS - host to device
        FIS_TYPE_REG_D2H	= 0x34,	// Register FIS - device to host
        FIS_TYPE_DMA_ACT	= 0x39,	// DMA activate FIS - device to host
        FIS_TYPE_DMA_SETUP	= 0x41,	// DMA setup FIS - bidirectional
        FIS_TYPE_DATA		= 0x46,	// Data FIS - bidirectional
        FIS_TYPE_BIST		= 0x58,	// BIST activate FIS - bidirectional
        FIS_TYPE_PIO_SETUP	= 0x5F,	// PIO setup FIS - device to host
        FIS_TYPE_DEV_BITS	= 0xA1,	// Set device bits FIS - device to host
    } FIS_TYPE;

    typedef enum
    {
        AHCI_SUCCESS		= 0x42,	// Operation ended. Success
        AHCI_FATAL_ERROR	= 0x02,	// Operation failed. Fatal Error!
        AHCI_NONFATAL_ERROR = 0x39,	// Operation ended. Error occured.
        AHCI_REQUEST_RESET	= 0x05,	// Operation failed. Reset requested.
        AHCI_BIOS_CONTROL	= 0x06,	// Operation failed. BIOS took control of HBA
        AHCI_DMA_CONGESTION	= 0x58,	// Operation failed. DMA Engines are working
    } AHCI_REPORT_TYPE;

    typedef struct AHCI_REPORT_INFO {
        uint32_t type;
        void *FIS;
        uint8_t FIStype;
        uint8_t ATA_errorcode;
        bool CntlrRequireReset;

        uint32_t payload_length;
        uint8_t data[];

    } AHCI_REPORT_INFO;

    typedef struct FIS_HEADER {
        uint8_t  fis_type;
        uint8_t  attrs;
    } __attribute__((__packed__)) FIS_HEADER;

    typedef struct tagFIS_REG_H2D
    {
        // DWORD 0
        FIS_HEADER h;
    
        uint8_t  command;	// Command register
        uint8_t  featurel;	// Feature register, 7:0
    
        // DWORD 1
        uint8_t  lba0;		// LBA low register, 7:0
        uint8_t  lba1;		// LBA mid register, 15:8
        uint8_t  lba2;		// LBA high register, 23:16
        uint8_t  device;		// Device register
    
        // DWORD 2
        uint8_t  lba3;		// LBA register, 31:24
        uint8_t  lba4;		// LBA register, 39:32
        uint8_t  lba5;		// LBA register, 47:40
        uint8_t  featureh;	// Feature register, 15:8
    
        // DWORD 3
        uint8_t  countl;		// Count register, 7:0
        uint8_t  counth;		// Count register, 15:8
        uint8_t  icc;		// Isochronous command completion
        uint8_t  control;	// Control register
    
        // DWORD 4
        uint8_t  rsv1[4];	// Reserved
    } __attribute__((__packed__)) FIS_REG_H2D;

    typedef struct tagFIS_REG_D2H
    {
        // DWORD 0
        FIS_HEADER h;
    
        uint8_t  status;      // Status register
        uint8_t  error;       // Error register
    
        // DWORD 1
        uint8_t  lba0;        // LBA low register, 7:0
        uint8_t  lba1;        // LBA mid register, 15:8
        uint8_t  lba2;        // LBA high register, 23:16
        uint8_t  device;      // Device register
    
        // DWORD 2
        uint8_t  lba3;        // LBA register, 31:24
        uint8_t  lba4;        // LBA register, 39:32
        uint8_t  lba5;        // LBA register, 47:40
        uint8_t  rsv2;        // Reserved
    
        // DWORD 3
        uint8_t  countl;      // Count register, 7:0
        uint8_t  counth;      // Count register, 15:8
        uint8_t  rsv3[2];     // Reserved
    
        // DWORD 4
        uint8_t  rsv4[4];     // Reserved
    } __attribute__((__packed__)) FIS_REG_D2H;

    typedef struct tagFIS_DATA
    {
        // DWORD 0
        FIS_HEADER h;
    
        uint8_t  rsv1[2];	// Reserved
    
        // DWORD 1 ~ N
        uint32_t data[1];	// Payload
    } __attribute__((__packed__)) FIS_DATA;

    typedef struct tagFIS_PIO_SETUP
    {
        // DWORD 0
        FIS_HEADER h;

        uint8_t  status;		// Status register
        uint8_t  error;		// Error register
    
        // DWORD 1
        uint8_t  lba0;		// LBA low register, 7:0
        uint8_t  lba1;		// LBA mid register, 15:8
        uint8_t  lba2;		// LBA high register, 23:16
        uint8_t  device;		// Device register
    
        // DWORD 2
        uint8_t  lba3;		// LBA register, 31:24
        uint8_t  lba4;		// LBA register, 39:32
        uint8_t  lba5;		// LBA register, 47:40
        uint8_t  rsv2;		// Reserved
    
        // DWORD 3
        uint8_t  countl;		// Count register, 7:0
        uint8_t  counth;		// Count register, 15:8
        uint8_t  rsv3;		// Reserved
        uint8_t  e_status;	// New value of status register
    
        // DWORD 4
        uint16_t tc;		// Transfer count
        uint8_t  rsv4[2];	// Reserved
    } __attribute__((__packed__)) FIS_PIO_SETUP;

    typedef struct tagFIS_DMA_SETUP
    {
        // DWORD 0
        FIS_HEADER h;
    
        uint8_t  rsved[2];       // Reserved
    
        //DWORD 1&2
    
        uint32_t DMAbufferID;    // DMA Buffer Identifier. Used to Identify DMA buffer in host memory. SATA Spec says host specific and not in Spec. Trying AHCI spec might work.
        uint32_t DMAbufferID2;    // DMA Buffer Identifier. Used to Identify DMA buffer in host memory. SATA Spec says host specific and not in Spec. Trying AHCI spec might work.
        //DWORD 3
        uint32_t rsvd;           //More reserved
        //DWORD 4
        uint32_t DMAbufOffset;   //Byte offset into buffer. First 2 bits must be 0
    
        //DWORD 5
        uint32_t TransferCount;  //Number of bytes to transfer. Bit 0 must be 0
        //DWORD 6
        uint32_t resvd;          //Reserved
    
    } __attribute__((__packed__)) FIS_DMA_SETUP;


    typedef volatile struct tagHBA_PORT
    {
        uint32_t clb;		// 0x00, command list base address, 1K-byte aligned
        uint32_t clbu;		// 0x04, command list base address upper 32 bits
        uint32_t fb;		// 0x08, FIS base address, 256-byte aligned
        uint32_t fbu;		// 0x0C, FIS base address upper 32 bits
        uint32_t is;		// 0x10, interrupt status
        uint32_t ie;		// 0x14, interrupt enable
        uint32_t cmd;		// 0x18, command and status
        uint32_t rsv0;		// 0x1C, Reserved
        uint32_t tfd;		// 0x20, task file data
        uint32_t sig;		// 0x24, signature
        uint32_t ssts;		// 0x28, SATA status (SCR0:SStatus)
        uint32_t sctl;		// 0x2C, SATA control (SCR2:SControl)
        uint32_t serr;		// 0x30, SATA error (SCR1:SError)
        uint32_t sact;		// 0x34, SATA active (SCR3:SActive)
        uint32_t ci;		// 0x38, command issue
        uint32_t sntf;		// 0x3C, SATA notification (SCR4:SNotification)
        uint32_t fbs;		// 0x40, FIS-based switch control
        uint32_t rsv1[11];	// 0x44 ~ 0x6F, Reserved
        uint32_t vendor[4];	// 0x70 ~ 0x7F, vendor specific
    } __attribute__((__packed__)) HBA_PORT;

    typedef volatile struct tagHBA_MEM
    {
        // 0x00 - 0x2B, Generic Host Control
        uint32_t cap;		// 0x00, Host capability
        uint32_t ghc;		// 0x04, Global host control
        uint32_t is;		// 0x08, Interrupt status
        uint32_t pi;		// 0x0C, Port implemented
        uint32_t vs;		// 0x10, Version
        uint32_t ccc_ctl;	// 0x14, Command completion coalescing control
        uint32_t ccc_pts;	// 0x18, Command completion coalescing ports
        uint32_t em_loc;		// 0x1C, Enclosure management location
        uint32_t em_ctl;		// 0x20, Enclosure management control
        uint32_t cap2;		// 0x24, Host capabilities extended
        uint32_t bohc;		// 0x28, BIOS/OS handoff control and status
    
        // 0x2C - 0x9F, Reserved
        uint8_t  rsv[0xA0-0x2C];
    
        // 0xA0 - 0xFF, Vendor specific registers
        uint8_t  vendor[0x100-0xA0];
    
        // 0x100 - 0x10FF, Port control registers
        HBA_PORT	ports[1];	// 1 ~ 32
    } __attribute__((__packed__)) HBA_MEM;
    
    typedef struct tagHBA_CMD_HEADER
    {
        // DW0
        uint8_t command; // Command FIS length in DWORDS, 2 ~ 16 | ATAPI | Write, 1: H2D, 0: D2H | Prefetchable
        uint8_t flags; // Reset | BIST | Clear busy upon R_OK | Reserved | Port Multiplier port

        uint16_t prdtl;	// Physical region descriptor table length in entries
    
        // DW1
        volatile uint32_t prdbc; // Physical region descriptor byte count transferred
    
        // DW2, 3
        uint32_t ctba;		// Command table descriptor base address
        uint32_t ctbau;		// Command table descriptor base address upper 32 bits
    
        // DW4 - 7
        uint32_t rsv1[4];	// Reserved
    } __attribute__((__packed__)) HBA_CMD_HEADER;

    typedef struct tagHBA_PRDT_ENTRY
    {
        uint32_t dba;		// Data base address
        uint32_t dbau;		// Data base address upper 32 bits
        uint32_t rsv0;		// Reserved
    
        // DW3
        uint32_t info;		// Byte count, 4M max | Reserved | Interrupt on completion
    } __attribute__((__packed__)) HBA_PRDT_ENTRY;

    typedef struct tagHBA_CMD_TBL
    {
	    // 0x00
	    uint8_t  cfis[64];	// Command FIS
 
	    // 0x40
	    uint8_t  acmd[16];	// ATAPI command, 12 or 16 bytes
 
	    // 0x50
	    uint8_t  rsv[48];	// Reserved
 
	    // 0x80
	    HBA_PRDT_ENTRY	prdt_entry[1];	// Physical region descriptor table entries, 0 ~ 65535
    } __attribute__((__packed__)) HBA_CMD_TBL;
}

class AHCIController : public GenericDiskController {

public:
    AHCIController(PCI::Device& device,PCI::Access& access);
    ~AHCIController();
    bool probe_port_connected(uint32_t port) override;
    bool read(uint8_t transfer_mode,uint8_t commandset,uint32_t port_number,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size) override;
    uint16_t get_controller_type() override;
    uint16_t get_logical_sector_size(uint32_t port) override;
private:
    void initialize(PCI::Device& device,PCI::Access& access);
    bool read_ata(uint8_t port_number,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size);
    bool ata_identify(uint8_t port_number,uint16_t* buf);
    bool atapi_identify(uint8_t port_number,uint16_t* buf);
    
    AHCI::HBA_PORT& get_port(uint8_t port);
    AHCI::HBA_CMD_HEADER& get_cmd_header(AHCI::HBA_PORT& port,int slot);
    AHCI::HBA_CMD_TBL& get_cmd_table(AHCI::HBA_CMD_HEADER& cmd_header);
    int find_freeslot(AHCI::HBA_PORT& port);
    ATA_IDENTIFY_DATA m_cached_identify_data;
    AHCI::HBA_MEM& get_hba();
    AHCI::HBA_MEM* m_hba;
};