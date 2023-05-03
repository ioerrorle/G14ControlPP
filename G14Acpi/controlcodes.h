#ifndef CONTROLCODES_H
#define CONTROLCODES_H

#include <cstdint>
namespace G14AcpiCodes
{

enum class ControlCode : uint32_t
{
    FIRST_EVENT_SUBSCRIBE = 0x222400,
    EVENT_SUBSCRIBE = 0x222408,
    ATK_ACPI = 0x22240C,
};

enum class Function : uint32_t
{
    BSTS = 0x53545342,
    DSTS = 0x53545344,
    OSVR = 0x5256534f,
    INIT = 0x54494e49,
    DEVS = 0x53564544,
    WDOG = 0x474f4457, //useless, does nothing in G14
    SFUN = 0x4e554653, //useless, does nothing
};

enum class CommandCode : uint32_t
{
    DEVID_KBD_BACKLIGHT = 0x00050011, //need to be set 0x02 for g14
    DEVID_BATTERY_MAX_CHARGE = 0x00120057, //setting max power charge
    DEVID_POWER_POLICY = 0x00120075, //setting armoury crate plan
    DEVID_POWER_SOURCE_TYPE = 0x0012006C, //getting current power source
    DEVID_CPU_FAN_TACH = 0x00110013, //getting current rpm
    DEVID_GPU_FAN_TACH = 0x00110014, // getting current rpm
    DEVID_CPU_FAN_CURVE = 0x00110024, // set fan curve/get default fan curve (GDFC + powerplan)
    DEVID_GPU_FAN_CURVE = 0x00110025, // set fan curve/get default fan curve (GDFC + powerplan)
    x00060023 = 0x00060023,
    x00060024 = 0x00060024,
    x00060026 = 0x00060026,
    x00110022 = 0x00110022,
    x00110023 = 0x00110023,
    x00110026 = 0x00110026,
    x00110027 = 0x00110027,
    /* Notifications for 0x00100021
     * If (IIA1 == 0x6C)) // Fn + F11
       {
           ^^PCI0.SBRG.EC0._Q0A () // Notify (SLPB: "Sleep Button")
           Return (Zero)
       }

       If ((IIA1 == 0x88)) // Fn + F12
       {
           ^^PCI0.SBRG.EC0._Q0B () // Notify (ASHS: "Asus Hardware something?", this kills RF)
           Return (Zero)
       }

       If ((IIA1 == 0xC5)) // Fn + Arrow Down
       {
           ^^PCI0.SBRG.EC0.KBLD () // nop
           Return (Zero)
       }

       If ((IIA1 == 0xC4)) // Fn + Arrow Up
       {
           ^^PCI0.SBRG.EC0.KBLU () // nop
           Return (Zero)
       }

       If ((IIA1 == 0x10)) // Fn + F7
       {
           ^^PCI0.SBRG.EC0._Q0E () // Hardware control: decrease internal LCD brightness
           Return (Zero)
       }

       If ((IIA1 == 0x20)) // Fn + F8
       {
           ^^PCI0.SBRG.EC0._Q0F () // Hardware control: increase internal LCD brightness
           Return (Zero)
       }

       If ((IIA1 == 0x35)) // 53, Not sure what this is
       {
           ^^PCI0.SBRG.EC0._Q10 () // Noify WMI?
           Return (Zero)
       }

       If ((IIA1 == 0x6B)) // Fn + F10
       {
           ^^PCI0.SBRG.EC0._Q12 () // Notify WMI?
           Return (Zero)
       }

       If ((IIA1 == 0x8A)) // Fn + V
       {
           ^^PCI0.SBRG.EC0._Q6B () // Notify WMI?
           Return (Zero)
       }

       If ((IIA1 == 0x38)) // ROG Key
       {
           ^^PCI0.SBRG.EC0._Q6B () // Notify WMI?
           Return (Zero)
       }

       If ((IIA1 == 0xAE)) // Fn + F5
       {
           IANE (IIA1) // Notify 255...?
           Return (Zero)
       }

       If ((IIA1 == 0x7C)) // Microphone mute toggle
       {
           IANE (IIA1) // Notify 255...?
           Return (Zero)
       }

       If ((IIA1 == 0x9E)) // Fn + C
       {
           IANE (IIA1) // Notify 255...?
           Return (Zero)
       }

       If ((IIA1 == 0xA8)) // 168, Not sure what this is
       {
           IANE (IIA1)
           Return (Zero)
       }

       If ((IIA1 == 0xA9)) // 169, Not sure what this is
       {
           IANE (IIA1)
           Return (Zero)
       }

       If ((IIA1 == 0xAA)) // 170, Not sure what this is
       {
           IANE (IIA1)
           Return (Zero)
       }

       If ((IIA1 == 0xAB)) // 171, Not sure what this is
       {
           IANE (IIA1)
           Return (Zero)
       }

       Return (One)
       */
    WMI_NOTIFY = 0x00100021
};
}

#endif // CONTROLCODES_H
