# UART Communication: Pico-to-Pico LED Control Protocol

## Overview

This project demonstrates custom UART communication between two Raspberry Pi Pico boards. The transmitter (Pico A) sends a framed 4-byte packet to the receiver (Pico B). If the packet matches a specific format, Pico B lights its onboard LED. Otherwise, it blinks rapidly to indicate an invalid or misaligned message.

This project highlights:

- Raw UART communication

- Protocol design with start-of-packet detection

- Packet validation via XOR checksum

- Noise handling and receiver resynchronization

---

### Packet Structure

- [0] CMD (Start Marker):  0x01
- [1] DATA1:               0xFF
- [2] DATA2:               0x1C
- [3] CHECKSUM:            XOR of bytes [0], [1], and [2]

### Valid Packet Example

- 0x01  0xFF  0x1C  0xE2
// 0x01 ^ 0xFF ^ 0x1C = 0xE2

---

### Transmitter Behavior (Pico A)

- Sends the valid 4-byte packet.

- Waits 10 seconds.

- Sends a fake (invalid) packet for testing.

### Code Behavior:

uint8_t TX[4] = {0x01, 0xFF, 0x1C, 0xE2};
uint8_t FakeTX[4] = {0x31, 0x21, 0x3C, 0xE2};

---

### Receiver Behavior (Pico B)

- Listens on UART.

- Waits for a start marker (0x01).

- Once detected, reads the next 3 bytes to complete the packet.

- Verifies the checksum.

- If valid:

  - Turns the onboard LED ON permanently.

- If invalid:

  - Blinks the LED rapidly (100ms on/off).

### Key Features:

- Real-time synchronization: Scans for 0x01 to stay aligned.

- Checksum validation: Ensures data wasn't corrupted.

- Visual feedback: Clean indicator for success/failure.

----

### Edge Cases Handled

- Stray Bytes or Reboots: Receiver ignores invalid start bytes (e.g. 0x00 caused by TX shutdown).

- Misaligned Reads: Resyncs using 0x01 start byte.

- Fake Packets: Valid checksum but invalid data are rejected.

----

### Limitations & Future Improvements

- No timeout or watchdog for partial packets.

- Data bytes are hardcoded — future version could make the protocol dynamic.

- No ACK/NACK feedback from RX to TX.

- Could add CRC for stronger validation.
