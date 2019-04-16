/** @file
*
*  Copyright (c) 2017, Linaro, Ltd. All rights reserved.
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

#include <PiDxe.h>

#include <libfdt.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/MemoryAllocationLib.h>

/**
  Return a pool allocated copy of the DTB image that is appropriate for
  booting the current platform via DT.

  @param[out]   Dtb                   Pointer to the DTB copy
  @param[out]   DtbSize               Size of the DTB copy

  @retval       EFI_SUCCESS           Operation completed successfully
  @retval       EFI_NOT_FOUND         No suitable DTB image could be located
  @retval       EFI_OUT_OF_RESOURCES  No pool memory available

**/
EFI_STATUS
EFIAPI
DtPlatformLoadDtb (
  OUT   VOID        **Dtb,
  OUT   UINTN       *DtbSize
  )
{
  EFI_STATUS                        Status;
  VOID                              *OrigDtb;
  VOID                              *CopyDtb;
  UINTN                             OrigDtbSize;
  UINTN                             CopyDtbSize;
  INT32                             Rc;

  Status = GetSectionFromAnyFv (&gDtPlatformDefaultDtbFileGuid,
             EFI_SECTION_RAW, 0, &OrigDtb, &OrigDtbSize);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  CopyDtbSize = OrigDtbSize;
  CopyDtb = AllocatePool (CopyDtbSize);
  if (CopyDtb == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Rc = fdt_open_into (OrigDtb, CopyDtb, CopyDtbSize);
  if (Rc < 0) {
    DEBUG ((DEBUG_ERROR, "%a: fdt_open_into () failed: %a\n", __FUNCTION__,
      fdt_strerror (Rc)));
    return EFI_NOT_FOUND;
  }
  
  *Dtb = CopyDtb;
  *DtbSize = CopyDtbSize;

  return EFI_SUCCESS;
}
