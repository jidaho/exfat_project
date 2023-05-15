#include <stdio.h>
#include <string.h>
#include "extfat.h"
#include "DiffCheck.h"

void DiffCheck(Main_Boot *MB, Main_Boot *BB)
{
    if (MB->JumpBoot[0] != BB->JumpBoot[0])
    {
        fprintf(stdout, "Verifying in JumpBoot[0]: Main:%c, Backup:%c\n", MB->JumpBoot[0], BB->JumpBoot[0]);
    }
    if (MB->JumpBoot[1] != BB->JumpBoot[1])
    {
        fprintf(stdout, "Verifying in JumpBoot[1]: Main:%c, Backup:%c\n", MB->JumpBoot[1], BB->JumpBoot[1]);
    }
    if (MB->JumpBoot[2] != BB->JumpBoot[2])
    {
        fprintf(stdout, "Verifying in JumpBoot[2]: Main:%c, Backup:%c\n", MB->JumpBoot[2], BB->JumpBoot[2]);
    }
    if (memcmp(MB->FileSystemName, BB->FileSystemName, sizeof(MB->FileSystemName)) != 0)
    {
        fprintf(stdout, "Verifying in FileSystemName: Main:%s, Backup:%s\n", MB->FileSystemName, BB->FileSystemName);
    }
    if (memcmp(MB->MustBeZero, BB->MustBeZero, sizeof(MB->MustBeZero)) != 0)
    {
        fprintf(stdout, "Verifying in MustBeZero: Main:\"%s\", Backup:\"%s\"\n", MB->MustBeZero, BB->MustBeZero);
    }
    if (MB->PartitionOffset != BB->PartitionOffset)
    {
        fprintf(stdout, "Verifying in PartitionOffset: Main:%ld, Backup:%ld\n", MB->PartitionOffset, BB->PartitionOffset);
    }
    if (MB->VolumeLength != BB->VolumeLength)
    {
        fprintf(stdout, "Verifying in VolumeLength: Main:%ld, Backup:%ld\n", MB->VolumeLength, BB->VolumeLength);
    }
    if (MB->FatOffset != BB->FatOffset)
    {
        fprintf(stdout, "Verifying in FatOffset: Main:%d, Backup:%d\n", MB->FatOffset, BB->FatOffset);
    }
    if (MB->FatLength != BB->FatLength)
    {
        fprintf(stdout, "Verifying in FatLength: Main:%d, Backup:%d\n", MB->FatLength, BB->FatLength);
    }
    if (MB->ClusterHeapOffset != BB->ClusterHeapOffset)
    {
        fprintf(stdout, "Verifying in ClusterHeapOffset: Main:%d, Backup:%d\n", MB->ClusterHeapOffset, BB->ClusterHeapOffset);
    }
    if (MB->ClusterCount != BB->ClusterCount)
    {
        fprintf(stdout, "Verifying in ClusterCount: Main:%d, Backup:%d\n", MB->ClusterCount, BB->ClusterCount);
    }
    if (MB->FirstClusterOfRootDirectory != BB->FirstClusterOfRootDirectory)
    {
        fprintf(stdout, "Verifying in FirstClusterOfRootDirectory: Main:%d, Backup:%d\n", MB->FirstClusterOfRootDirectory, BB->FirstClusterOfRootDirectory);
    }
    if (MB->VolumeSerialNumber != BB->VolumeSerialNumber)
    {
        fprintf(stdout, "Verifying in VolumeSerialNumber: Main:%d, Backup:%d\n", MB->VolumeSerialNumber, BB->VolumeSerialNumber);
    }
    if (MB->FileSystemRevision != BB->FileSystemRevision)
    {
        fprintf(stdout, "Verifying in FileSystemRevision: Main:%d, Backup:%d\n", MB->FileSystemRevision, BB->FileSystemRevision);
    }
    if (MB->VolumeFlags != BB->VolumeFlags)
    {
        fprintf(stdout, "Verifying in VolumeFlags: Main:%d, Backup:%d\n", MB->VolumeFlags, BB->VolumeFlags);
    }
    if (MB->BytesPerSectorShift != BB->BytesPerSectorShift)
    {
        fprintf(stdout, "Verifying in BytesPerSectorShift: Main:%c, Backup:%c\n", MB->BytesPerSectorShift, BB->BytesPerSectorShift);
    }
    if (MB->SectorsPerClusterShift != BB->SectorsPerClusterShift)
    {
        fprintf(stdout, "Verifying in SectorsPerClusterShift: Main:%c, Backup:%c\n", MB->SectorsPerClusterShift, BB->SectorsPerClusterShift);
    }
    if (MB->NumberOfFats != BB->NumberOfFats)
    {
        fprintf(stdout, "Verifying in NumberOfFats: Main:%c, Backup:%c\n", MB->NumberOfFats, BB->NumberOfFats);
    }
    if (MB->DriveSelect != BB->DriveSelect)
    {
        fprintf(stdout, "Verifying in DriveSelect: Main:%c, Backup:%c\n", MB->DriveSelect, BB->DriveSelect);
    }
    if (MB->PercentInUse != BB->PercentInUse)
    {
        fprintf(stdout, "Verifying in PercentInUse: Main:%c, Backup:%c\n", MB->PercentInUse, BB->PercentInUse);
    }
    if (memcmp(MB->Reserved, BB->Reserved, sizeof(MB->Reserved)) != 0)
    {
        fprintf(stdout, "Verifying in Reserved: Main:%s, Backup:%s\n", MB->Reserved, BB->Reserved);
    }

    if (memcmp(MB->BootCode, BB->BootCode, sizeof(MB->BootCode)) != 0)
    {
        fprintf(stdout, "Verifying in BootCode: Main:%s, Backup:%s\n", MB->BootCode, BB->BootCode);
    }
    if (MB->BootSignature != BB->BootSignature)
    {
        fprintf(stdout, "Verifying in BootSignature: Main:%d, Backup:%d\n", MB->BootSignature, BB->BootSignature);
    }
    if (MB->ExcessSpace != BB->ExcessSpace)
    {
        fprintf(stdout, "Verifying in ExcessSpace: Main:%d, Backup:%d\n", MB->ExcessSpace, BB->ExcessSpace);
    }
}
