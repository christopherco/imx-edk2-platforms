/** @file
*
*  Copyright (c) 2019 Microsoft Corporation. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#include "SmbiosConfigDxe.h"

STATIC EFI_HANDLE mImageHandle = NULL;

EFI_STATUS
EFIAPI
GetSmbiosOverrideData (
  )
{
  CONST CHAR16 *DevicePathText;
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;
  EFI_FILE_PROTOCOL *File;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *Fs;
  EFI_DEVICE_PATH_PROTOCOL *IntermediateDevicePath;
  EFI_STATUS ExitStatus;
  EFI_HANDLE MediaHandle;
  EFI_FILE_HANDLE RootVolume;
  EFI_STATUS Status;
  

  DevicePathText = (CONST CHAR16 *) FixedPcdGetPtr(
    PcdSmbiosOverrideDevicePath);
  if ((DevicePathText == NULL) || (*DevicePathText == L'\0')) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_ERROR, "%a: PcdStorageMediaPartition is unspecified\n", __FUNCTION__));
    goto Exit;
  }

  DevicePath = ConvertTextToDevicePath (DevicePathText);
  if (!DevicePath) {
    DEBUG ((DEBUG_ERROR, "%a: Could not convert DevicePathText\n", __FUNCTION__));
    return FALSE;
  }

  IntermediateDevicePath = DevicePath;
  Status = gBS->LocateDevicePath(
    &gEfiSimpleFileSystemProtocolGuid,
    &IntermediateDevicePath,
    &MediaHandle);

  if (Status == EFI_NOT_FOUND) {
    Status = EFI_SUCCESS;
    DEBUG ((DEBUG_INFO,"%a: %s FAT partition is not ready yet\n", __FUNCTION__, DevicePathText));
    goto Exit;
  }

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: gBS->LocateDevicePath() failed. (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  Status = gBS->OpenProtocol(
    MediaHandle,
    &gEfiSimpleFileSystemProtocolGuid,
    (VOID **)&Fs,
    mImageHandle,
    NULL,
    EFI_OPEN_PROTOCOL_GET_PROTOCOL);

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: gBS->OpenProtocol() failed. (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  Status = Fs->OpenVolume(Fs, &RootVolume);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Fs->OpenVolume() failed. (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  Status = RootVolume->Open(
    RootVolume,
    &File,
    L"VarLog.txt",
    EFI_FILE_MODE_READ,
    0);

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: RootVolume->Open() failed. (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  Status = File->SetPosition(File, 0);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: File->SetPosition(0) failed. (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

Exit:

  if (File != NULL) {
    File->Flush(File);
    File->Close(File);
    File = NULL;
  }

  if (RootVolume != NULL) {
    RootVolume->Close(RootVolume);
    RootVolume = NULL;
  }

  if (MediaHandle != NULL) {
      ExitStatus = gBS->CloseProtocol(
      MediaHandle,
      &gEfiSimpleFileSystemProtocolGuid,
      mImageHandle,
      NULL);

    if (EFI_ERROR(ExitStatus)) {
      DEBUG ((DEBUG_ERROR, "%a: gBS->CloseProtocol() failed. (Status=%r)\n", __FUNCTION__, ExitStatus));
    }

    Fs = NULL;
  }


  if (DevicePath != NULL) {
    FreePool(DevicePath);
  }

  return Status;

}

BOOLEAN
CheckSmbiosOverridePresent (
  )
{
  return TRUE;
}

EFI_STATUS
EFIAPI
SetSmbiosOverridePresent (
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SmbiosConfigDxeInitialize (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  BOOLEAN Locked;
  EFI_STATUS Status;

  DEBUG ((DEBUG_ERROR, "%a: Enter\n", __FUNCTION__));

  // Check if an Smbios override config was previously set
  Locked = CheckSmbiosOverridePresent();
  if (Locked == TRUE) {
    Status = EFI_SUCCESS;
    goto Exit;
  }

  // Lock down config no matter the result
  SetSmbiosOverridePresent();

  // Read Smbios Override file from filesystem
  Status = GetSmbiosOverrideData();

  // Validate Smbios override data

  // Store override data in NV variable

Exit:

  return Status;
}
