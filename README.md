# MOTT

MOTT is an open source library for Infrared communication in Arduino devices.

As most Infrared protocols limit the ammount of information you can send and are complex I decided to create a simple but general protocol for Infrared communciation.

The protocol is designed to send chars over IR.

As ASCII chars are 7 bit I decided to make a char 7 bit for better speed performance. If in some moment 8 bit chars or bytes are needed for communication it can be easily changed.

From now on when I refer to a char I will refer to a 7 bit binary number.

The period of bit transmision can be set at will. All bit transmision will ocurre at the same rate.

Protocol starts by sending a start bit which consits of a 1 for the time set. After the start bit a char will be sent that contains the amount of chars of the payload. Finally, the payload is created by concateting all the message chars.


![image](https://user-images.githubusercontent.com/33181424/126815587-b76cd287-819d-4bac-b6ad-a3812a2422d8.png)

For example, for sending the message "B" over MOTT protocol at a rate of 1 bit/ms this will be the bit sequence:

| Start Bit | Payload Size | Payload |
|-----------|:------------:|---------|
| 1         | 0000001      | 1000010 |

The char 'B' is encoded with ASCII protocol.
Note that each bit takes 1ms so the message takes (1 + 7 + 7) * 1 ms = 15 ms.

For sending a multi char message "BC" this will be the bit sequence:

| Start Bit | Payload Size | Payload        |
|-----------|:------------:|----------------|
| 1         | 0000010      | 10000101000011 |

The time spent will be (1 + 7 + 2 * 7) * 1 ms = 22 ms.

As IR receptors need a modulated signal for interference and noise issues each bit needs to be coded with a carrier signal.

Common IR receptors use 38KHz carrier wave for modulation. MOTT library takes care of this modulation. If need, carrier frequency can be changed at will.

Most IR receptors need at least 10 carrier cycles for detecting a bit. For stable designs we recommend to not set bit transmission time to less than 400us.
