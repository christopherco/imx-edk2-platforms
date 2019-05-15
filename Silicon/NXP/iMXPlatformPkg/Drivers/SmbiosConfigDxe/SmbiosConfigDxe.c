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

EFI_STATUS
EFIAPI
GetSmbiosOverrideData (
  )
{
  CONST CHAR16 *DevicePathText;
  EFI_STATUS Status;

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