#pragma once

typedef struct RSDPDescriptor {
 char Signature[8];
 uint8_t Checksum;
 char OEMID[6];
 uint8_t Revision;
 uint32_t RsdtAddress;
} __attribute__ ((packed)) RSDPDescriptor;

typedef struct RSDPDescriptor20 {
 RSDPDescriptor firstPart; 
 uint32_t Length;
 uint64_t XsdtAddress;
 uint8_t ExtendedChecksum;
 uint8_t reserved[3];
} __attribute__ ((packed)) RSDPDescriptor20;

typedef struct ACPISDTHeader {
  char Signature[4];
  uint32_t Length;
  uint8_t Revision;
  uint8_t Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorRevision;
} __attribute__((__packed__)) ACPITableHeader;

typedef struct ACPI_RSDT {
  ACPITableHeader h;
  uint32_t PointerToOtherSDT[];
} __attribute__((__packed__)) ACPI_RSDT;

typedef struct ACPI_XSDT {
  ACPITableHeader h;
  uint64_t PointerToOtherSDT[];
} __attribute__((__packed__)) ACPI_XSDT;

typedef struct
{
  uint64_t base_addr;
  uint16_t pci_segment_number;
  uint8_t pci_start_bus;
  uint8_t pci_end_bus;
  uint8_t Reserved[4];
} __attribute__((__packed__)) PCIeSpaceConfig;

typedef struct ACPI_MCFG {
  ACPITableHeader h;
  uint8_t Reserved[8];
  PCIeSpaceConfig spaces[];
} __attribute__((__packed__)) ACPI_MCFG;