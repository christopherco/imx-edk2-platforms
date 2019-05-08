/** @file
*
*  iMX6 DualLite SPI Controllers
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

Device (SPI1)
{
  Name (_HID, "NXP0105")
  Name (_UID, 0x1)
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x02008000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 63 }

    // CS0 (PAD_KEY_ROW1) GPIO4_IO09
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 105 }
    // CS1 (PAD_KEY_COL2) GPIO4_IO10
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 106 }

    // MISO: PAD_KEY_COL1 (GPIO4_IO08) Alt2
    // MOSI: PAD_KEY_ROW0 (GPIO4_IO07) Alt2
    // SCLK: PAD_KEY_COL0 (GPIO4_IO06) Alt2
    // MsftFunctionConfig (Exclusive, PullDown, IMX_ALT2, "\\_SB.GPIO", 0,
    //                     ResourceConsumer, ) { 104, 103, 102 }
    //
    // MsftFunctionConfig (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) { Pin List }
    VendorLong () {
      MSFT_UUID,            // Vendor UUID (MSFT UUID)
      MSFT_FUNCTION_CONFIG, // Resource Identifier (MSFT Function Config)
      0x1f,0x00,            // Length (0xF + sizeof(PinList) + sizeof(ResourceName))
      0x01,                 // Revision (0x1)
      RESOURCECONSUMER_EXCLUSIVE, // Flags (Arg5 | Arg0: ResourceConsumer | Exclusive)
      PULL_DOWN,            // Pin configuration (Arg1: PullDown)
      IMX_ALT0,0x00,        // Function Number (Arg2: IMX_ALT0)
      PIN_TABLE_OFFSET,     // Pin Table Offset (0x12)
      0x00,                 // Resource Source Index (Arg4: 0)
      0x18,0x00,            // Resource Source Name Offset (0x12 + sizeof(PinList))
      0x22,0x00,            // Vendor Data Offset (0x12 + sizeof(PinList) + sizeof(ResourceName))
      0x00,0x00,            // Vendor Data Length (sizeof(Arg6) = 0)
      0x68,0x00,0x67,0x00,0x66,0x00,  // Pin List (104, 103, 102)
      SB_GPIO               // Resource Name (Arg3: \_SB.GPIO in ASCII)
    }
  })
}

Device(SPBT)
{
  Name(_HID, "SPB0000")
  Name(_UID, 1)
  Method(_CRS, 0x0, NotSerialized)
  {
    Name (RBUF, ResourceTemplate ()
    {
      SPISerialBus (          // SCLK
                              // MOSI
                              // MISO
                              // CE0
        0,                    // Device selection (CE0)
        PolarityLow,          // Device selection polarity
        FourWireMode,         // wiremode
        8,                    // databit len
        ControllerInitiated,  // slave mode
        4000000,              // connection speed
        ClockPolarityLow,     // clock polarity
        ClockPhaseFirst,      // clock phase
        "\\_SB.SPI1",         // ResourceSource: SPI bus controller name
        0,                    // ResourceSourceIndex
                              // Resource usage
                              // DescriptorName: creates name for offset of resource descriptor
      )                       // Vendor Data
    
      GpioInt(Level, ActiveHigh, Exclusive, PullDown, 0, "\\_SB.GPIO") {7}
    })
    Return(RBUF)
  }
}

