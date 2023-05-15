#pragma once
typedef struct
{
    char JumpBoot[3];
    char FileSystemName[8];
    char MustBeZero[53];
    long int PartitionOffset;
    long int VolumeLength;
    int FatOffset;
    int FatLength;
    int ClusterHeapOffset;
    int ClusterCount;
    int FirstClusterOfRootDirectory;
    int VolumeSerialNumber;
    short int FileSystemRevision;
    short int VolumeFlags;
    unsigned char BytesPerSectorShift;
    unsigned char SectorsPerClusterShift;
    unsigned char NumberOfFats;
    unsigned char DriveSelect;
    unsigned char PercentInUse;
    unsigned char Reserved[7];
    unsigned char BootCode[390];
    short int BootSignature;
    unsigned char ExcessSpace;
} Main_Boot;

// This structure contains all of the information about a single disk // image file
typedef struct
{
    // There are the disk image
    Main_Boot *M_Boot;
    Main_Boot *B_Boot;
    void *FAT;
    void *Data;
    // To identify the file
    char *fileName;
    int fd; // The file descriptor
    // For convenience, these values are extracted from the // image
    int SectorSize;
    int FileLength;
} fileInfo;