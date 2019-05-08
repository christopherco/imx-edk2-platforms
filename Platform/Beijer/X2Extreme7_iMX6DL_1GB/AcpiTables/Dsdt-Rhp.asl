/** @file
*
*  iMX6 DualLite Resource Hub Proxy
*
*  Copyright (c) 2018 Microsoft Corporation. All rights reserved.
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

Device (RHPX)
{
  Name (_HID, "MSFT8000")
  Name (_CID, "MSFT8000")
  Name (_UID, 1)

  Name (_CRS, ResourceTemplate() {
    // Index 0
    SPISerialBus (            // SCKL
                              // MOSI
                              // MISO
                              // CE0
      0,                      // Device selection (CE0)
      PolarityLow,            // Device selection polarity
      FourWireMode,           // wiremode
      8,                      // databit len
      ControllerInitiated,    // slave mode
      4000000,                // connection speed
      ClockPolarityLow,       // clock polarity
      ClockPhaseFirst,        // clock phase
      "\\_SB.SPI1",           // ResourceSource: SPI bus controller name
      0,                      // ResourceSourceIndex
                              // Resource usage
                              // DescriptorName: creates name for
                              //   offset of resource descriptor
    )                         // Vendor Data
  })
  Name (_DSD, Package() {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package() {
      // SPI 1
      Package (2) { "bus-SPI-SPI1", Package() { 0 } }, // Index 0
      Package (2) { "SPI1-MinClockInHz", 115 },        // 115 Hz
      Package (2) { "SPI1-MaxClockInHz", 12000000 },   // 12 MHz
      // Data bit length
      Package (2) { "SPI1-SupportedDataBitLengths", Package() { 8, 16, 32 } },
    }
  })
}

