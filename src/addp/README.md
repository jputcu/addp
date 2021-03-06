Protocol
========
http://qbeukes.blogspot.com/2009/11/advanced-digi-discovery-protocol_21.html

ADDP is a proprietary protocol developed by and it's copyrighted owned
by Digi International Inc. Use of the protocol is limited to their products
and their terms. I released this purely because I noticed a need for
implementing it in your own non C and non Windows applications for managing
your own Digi devices. For more information see http://www.digi.com/.

Summary
-------
The ADDP protocol is a UDP multicast protocol, doing broadcasts on 224.0.5.128
port 2362. This is the default configuration, though it is possible
to disable this for a given Digi device, or change the port.

All communications are in the form of request/response, and there exists
4 communication types which give a total of 8 packets. They are Discovery,
Static Configuration, DHCP Network Configuration and Restart.

All packets consists of a header and a payload. The header format is
common throughout all packet types, where the payload is common on response
packets but unique for each request packet type.

~~~~
+--------------+
| magic        |                  +-------------+
+--------------+                / | field type  |
| packet type  |               /  +-------------+
+--------------+    +-------+ /   | field size  |
| payload size |  / | field |     +-------------+
+--------------+ /  +-------+ \   |             |
|              |    | field |  \  | field value |
| payload      |    +-------+   \ |             |
|              |    | field |     +-------------+
+--------------+ \  +-------+
                  \ | ...   |
                    +-------+
~~~~

The Header
----------
The header starts with the magic bytes "DIGI", ie. 0x44,49,47,49. This
is followed by a 2 byte packet type identifier (see the tables at the
end of this document). After the identifier is a 2 byte short, which
indicates the length/size of the payload.

The Payload
-----------
As explain, the payloads of all request packets are unique to the type
of packet, where the response packets have a common format. Response
packets contain information pertaining to the request. This information
is given in a variable amount of fields, each field being identified
by a 1 byte identifier. These identifiers are shared across all response
packets, each response sending only the information relevant to the
request.

So a field takes the form of a 1 byte field type ID (see the tables
at the end of this document), then a 1 byte field length followed the
the field's data. The field length byte indicates how many data bytes
need to be read. This will always be at least one. Take for example
the IP address field. It's type id is 0x02, and being an IP address
it will be 4 bytes long. If the IP address was 10.0.0.5, then the field
would have the following value: 0x02,04,0a,00,00,05. Response packets
always have the MAC address field set, and is the MAC address of the
device the packet originated from. You use this to pair responses with
requests.

When a request packet had an invalid format, even though the MAC address
was valid an it's obvious the packet was meant for a given device, it
will be ignored and no response packet will be sent. Response packets
are only sent when the request packet had a 100% valid structure. Failure
responses are sent only for failure after the packet was parsed successfully.

So to parse the payload, you just read the fields in a serial manner
until the complete payload has been consumed. Not all responses of the
same type will always have the same fields. So it's important to read
the payload based on the size specified in the header. Further, depending
on the field type, an absent field could mean that it's the default
value or irrelevant to the type of packet. For example, in the restart
response packet the IP address field won't be set, where the absense
of the RealPort number field in a discovery response could indicate
it's disabled.

When transferring MAC addresses or IP addresses, they are sent as the
raw binary values. Since each component of both a MAC address and IP
address is a number in the range 0-255 they are each represented as
a byte. This is actually what a MAC address and IP address is. Even
though we write an IP address as: 10.0.0.9, it's actually a 4 byte values
of: 0x0a,00,00,09. Same goes for a MAC address, which is written as
comma separated hexadecimal bytes. When transferred, the host byte order
is used, so you read the IP address in the byte order it appears in
the packet (which is the same as the example just given).

Numbers are always either 1, 2 or 4 bytes. For 1 byte you just take
the integer value of the byte, and for 2 or 4 byte you interpret it
with Little Endian. The 2 byte shorts can be calculated with: "((b[0]
& 0xff) <<>

Strings are never sent in request packets, though received in a number
of response packet fields. The length of the string would be the field
size, and it has no terminator. So to read the string value simply copy
all of the field data into a character array/string. Characters are
always 1 byte ASCII.

Request Packet Authentication
The Configuration and Restart request packets require authentication
and a target. This is the last 11 bytes of each of these packets. The
target is a 6 byte MAC address, and is the MAC address of the target
Digi unit.

The authentication data seems to always be static byte sequence: 0x04,64,62,70,73.
This is a 1 byte length with the value of "4", and the character sequence
"dbps" (which is also the Digi Connect ME default root password). Even
when changing your device's root password, this will remain the same.
It's definitely used for authentication, as sending different values
will result in an "authentication error" response.

Discovery/Information Request
-----------------------------
The Discovery request packet is the simplest of all of them. It has
the packet type 0x0001 and a 6 byte payload which is a MAC address.
The MAC address in the payload is for the device which you request information
from. So only the device whose MAC address matches the one in the packet
will respond to this request. If you want all devices to respond, use
the broadcast MAC address ff:ff:ff:ff:ff:ff.

This sample Discovery Request packet will request responses from all
DIGIs on the network which has ADDP activated on port 2362. To request
information from Digis listening on a different ADDP port, change the
destination port of the UDP packet.

~~~~
Data (14 bytes):
0000   44 49 47 49 00 01 00 06 ff ff ff ff ff ff        DIGI..........

Breaks down as:
            44 49 47 49  DIGI       - Magic
                  00 01  ..         - Packet type (0001)
                  00 06  ..         - Payload size (6 bytes)
      ff ff ff ff ff ff  ......     - Target MAC
~~~~

Discovery/Information Response
------------------------------
If a Digi unit receives the discovery request packet and it's meant
for either all Digis or itself (based on the request packet's target
MAC address as described above), it will respond with a response packet
containing details of it's configuration, such as IP address, subnet
mask, number of serial ports, etc.

This packet has the id 0x0002 and is probably the largest of all the
packets. See the tables at the end of this e-mail for all possible fields.
The following example is a typical packet you might receive in response
to a discovery request.

~~~~
Data (104 bytes):
0000   44 49 47 49 00 02 00 60 01 06 00 40 9d 31 a9 0a  DIGI...`...@.1..
0010   02 04 0a 00 00 e7 03 04 ff ff ff 00 0b 04 0a 00  ................
0020   00 01 0d 0f 44 69 67 69 20 43 6f 6e 6e 65 63 74  ....Digi Connect
0030   20 4d 45 10 01 00 07 01 00 08 1e 56 65 72 73 69   ME........Versi
0040   6f 6e 20 38 32 30 30 30 38 35 36 5f 46 36 20 30  on 82000856_F6
0
0050   37 2f 32 31 2f 32 30 30 36 0e 04 00 00 03 03 13  7/21/2006.......
0060   04 00 00 04 03 12 01 01                          ........

Breaks down as:
            44 49 47 49  DIGI       - Magic
                  00 02  ..         - Packet type (0002)
                  00 60  .`         - Payload size (96 bytes)
01 06 00 40 9d 31 a9 0a  ...@.1..   - Mac address: 00:40:9D:31:A9:0A
      02 04 0a 00 00 e7  ......     - IP Address: 10.0.0.231
      03 04 ff ff ff 00  ......     - Subnet Mask: 255.255.255.0
      0b 04 0a 00 00 01  ......     - Gateway Address: 10.0.0.1
                     0d
0f 44 69 67 69 20 43 6f  .Digi Co
6e 6e 65 63 74 20 4d 45  nnect ME   - Device Name: Digi Connect ME
               10 01 00  ...        - DHCP Disabled
               07 01 00  ...        - Unknown - never seen a non-0x00
value.
08 1e 56 65 72 73 69 6f  ..Versio
6e 20 38 32 30 30 30 38  n 820008
35 36 5f 46 36 20 30 37  56_F6 07
2f 32 31 2f 32 30 30 36  /21/2006   - Firmware: Version 82000856_F6
07/21/2006
      0e 04 00 00 03 03  ......     - Real Port: 771
      13 04 00 00 04 03  ......     - Encrypted Real Port: 1027
               12 01 01  .          - Serial port count: 1
~~~~

Static Network Configuration Request Packet
-------------------------------------------
The Static Configuration Request packets have the packet type of 0x0003
and are used to reconfigure the network settings of the Digi unit. This
consists of 3 values to be set: IP address, subnet mask and default
gateway.

The 3 values are added in the mentioned order to the beginning of the
payload. This gives a 12 byte sequence, which is then followed the MAC
address of the device to be configured and the authentication data.
This results in a total payload size of 23 bytes, which is a 31 byte
packet.

The following sample would be to reconfigure the device with MAC address
'00:40:9D:31:A9:0A', giving it the IP address: 10.0.0.9, subnet mask:
255.255.255.0 and gateway: 10.0.0.1. Since these fields are static and
always in this order, to remove the gateway you would send a gateway
value of 0.0.0.0.

~~~~
Data (31 bytes):
0000   44 49 47 49 00 03 00 17 0a 00 00 09 ff ff ff 00  DIGI............
0010   0a 00 00 01 00 40 9d 31 a9 0a 04 64 62 70 73     .....@.1...dbps

Break down as:
            44 49 47 49  DIGI       - Magic
                  00 03  ..         - Packet type (0003)
                  00 17  .`         - Payload size (23 bytes)
            0a 00 00 09  ....       - New IP Address: 10.0.0.9
            ff ff ff 00  ....       - New Subnet Mask: 255.255.255.0
            0a 00 00 01  ....       - New Gateway: 10.0.0.1
      00 40 9d 31 a9 0a  .@.1..     - Target MAC Address: 00:40:9D:31:A9:0A
         04 64 62 70 73  .dbps      - Authentication Data
~~~~

Configuration Response Packet
-----------------------------
The configuration response packet is sent to indicate whether the configuration
request was successful and if not to give a reason for failure by means
of an error code and message. This packet has the id 0x0004 and uses
the standard response packet format. The success is indicated by the
result flag, result message, error code and configuration error code
fields. See the tables at the end of this document for details on these
fields.

Here is a sample packet for the above configuration request, indicating
the request succeeded.

~~~~
Data (44 bytes):
0000   44 49 47 49 00 04 00 24 0a 01 00 09 14 4f 70 65  DIGI...(.....Ope
0010   72 61 74 69 6f 6e 20 53 75 63 63 65 73 73 66 75  ration Successfu
0020   6c 11 01 00 01 06 00 40 9d 31 a9 0a              l......@.1..


Break down as:
            44 49 47 49  DIGI       - Magic
                  00 04  ..         - Packet type (0004)
                  00 24  .(         - Payload size (36 bytes)
               0a 01 00  ...        - Result flag: Success
      09 14 4f 70 65 72  ..Oper
61 74 69 6f 6e 20 53 75  ation Su
63 63 65 73 73 66 75 6c  ccessful   - Result Message: Operation Successful
               11 01 00  ...        - Error code: None
01 06 00 40 9d 31 a9 0a  ...@.1..   - Mac Address: 00:40:9D:31:A9:0A
~~~~

Here is a sample packet for a configuration request that failed because
an invalid IP address was sent (0.0.0.0).

~~~~
Data (37 bytes):
0000   44 49 47 49 00 04 00 1d 0a 01 ff 09 0d 49 6e 76  DIGI.........Inv
0010   61 6c 69 64 20 76 61 6c 75 65 11 01 03 01 06 00  alid value......
0020   40 9d 31 a3 a5                                   @.1..

Break down as:
            44 49 47 49  DIGI       - Magic
                  00 04  ..         - Packet type (0004)
                  00 1d  .(         - Payload size (29 bytes)
               0a 01 ff  ...        - Result flag: Error
   09 0d 49 6e 76 61 6c  ..Inval
69 64 20 76 61 6c 75 65  id value   - Result Message: Invalid value
               11 01 03  ...        - Error code: 0x03
01 06 00 40 9d 31 a3 a5  ...@.1..   - Mac Address: 00:40:9D:31:A3:A5
~~~~

DHCP Network Configuration Request
----------------------------------
The DHCP Configuration Request packets have the packet type of 0x0007
and are used to reconfigure the network of the Digi unit so it requests
it's IP address from a DHCP server. If a DHCP server doesn't respond
it will take on a default IP address (the 169 IPs you see often when
a device can't find an IP address).

The packet is very simply, in that it payload simply has a 1 byte boolean
to enable/disable DHCP followed by the target MAC and then the authentication
data. This will enabled DHCP. If you send a 0x00 for the enable byte
then DHCP will be enabled, and the current network settings will be
untouched. If you send 0x00 DHCP will be disabled and any previous network
settings will be restored.

The following sample would be to reconfigure the device with MAC address
'00:40:9D:31:A3:A5', instructing it to automatically get it's IP address
via DHCP.

~~~~
Data (20 bytes):
0000   44 49 47 49 00 07 00 0c 01 00 40 9d 31 a3 a5 04  DIGI......@.1...
0010   64 62 70 73                                      dbps

Break down as:
            44 49 47 49  DIGI       - Magic
                  00 07  ..         - Packet type (0003)
                  00 0c  ..         - Payload size (12 bytes)
                     01  ....       - Enable DHCP
      00 40 9d 31 a3 a5  .@.1..     - Target MAC Address: 00:40:9D:31:A3:A5
         04 64 62 70 73  .dbps      - Authentication Data
~~~~

DHCP Network Configuration Response
-----------------------------------
This packet is the same as the Static Network Configuration response,
except it has the packet type identifier 0x0008. Here is a sample response
for the DHCP request packet sample above:

~~~~
Data (44 bytes):
0000   44 49 47 49 00 08 00 24 0a 01 00 09 14 4f 70 65  DIGI...$.....Ope
0010   72 61 74 69 6f 6e 20 53 75 63 63 65 73 73 66 75  ration Successfu
0020   6c 11 01 00 01 06 00 40 9d 31 a3 a5              l......@.1..

Break down as:
            44 49 47 49  DIGI       - Magic
                  00 08  ..         - Packet type (0008)
                  00 24  .(         - Payload size (36 bytes)
               0a 01 00  ...        - Result flag: Success
      09 14 4f 70 65 72  ..Oper
61 74 69 6f 6e 20 53 75  ation Su
63 63 65 73 73 66 75 6c  ccessful   - Result Message: Operation Successful
               11 01 00  ...        - Error code: None
01 06 00 40 9d 31 a3 a5  ...@.1..   - Mac Address: 00:40:9D:31:A3:A5
~~~~

Restart Request Packet
----------------------
Th restart request is used to instruct a device to restart itself. This
is useful in some cases, and needed to apply certain configuration changes,
specifically the network configuration done with the Configuration packets.

This packet has the packet type id of 0x0005 and has a target MAC followed
by the authentication data as it's payload. This gives a payload length
of 11 bytes and a total packet size of 19 bytes. The following is a
sample packet that restarts the device with MAC '00:40:9D:31:A9:0A'.

~~~~
Data (19 bytes):
0000   44 49 47 49 00 05 00 0b 00 40 9d 31 a9 0a 04 64  DIGI.....@.1...d
0010   62 70 73                                         bps

Break down as:
            44 49 47 49  DIGI       - Magic
                  00 05  ..         - Packet type (0005)
                  00 0b  .`         - Payload size (11 bytes)
      00 40 9d 31 a9 0a  ......     - Target MAC: 00:40:9D:31:A9:0A
         04 64 62 70 73  .dbps      - Authentication Data
~~~~

Restart Response Packet
-----------------------
After you send a restart request, the device will respond, reporting
it's result. If successful the device will immediately initiate a restart.
This packet has the packet type id 0x0006. As with all the other response
packets it uses the field based packet format. The following sample
packet is a restart response indicating success of the previous restart
request sample.

~~~~
Data (44 bytes):
0000   44 49 47 49 00 06 00 24 0a 01 00 09 14 4f 70 65  DIGI...$.....Ope
0010   72 61 74 69 6f 6e 20 53 75 63 63 65 73 73 66 75  ration Successfu
0020   6c 11 01 00 01 06 00 40 9d 31 a9 0a              l......@.1..

Break down as:
            44 49 47 49  DIGI       - Magic
                  00 06  ..         - Packet type (0006)
                  00 24  .(         - Payload size (36 bytes)
               0a 01 00  ...        - Result flag: Success
      09 14 4f 70 65 72  ..Oper
61 74 69 6f 6e 20 53 75  ation Su
63 63 65 73 73 66 75 6c  ccessful   - Result Message: Operation Successful
               11 01 00  ...        - Error code: 0x00
01 06 00 40 9d 31 a9 0a  ...@.1..   - Mac Address: 00:40:9D:31:A9:0A
~~~~

Tables of Codes
---------------
The following tables summarize the different types and codes used in
the protocol.

### Packet Types

* 0x0001: Discovery Request
* 0x0002: Discovery Response
* 0x0003: Static Network Configuration Request
* 0x0004: Static Network Configuration Response
* 0x0005: Reboot Request
* 0x0006: Reboot Response
* 0x0007: DHCP Network Configuration Request
* 0x0008: DHCP Network Configuration Response

### Field Types

* 0x01: 6 byte MAC address
* 0x02: 4 byte IP address
* 0x03: 4 byte Netmask
* 0x04: String Network Name
* 0x05: UNSEEN
* 0x06: UNSEEN
* 0x07: 1 byte - UNKNOWN - seen in discovery responses
* 0x08: String Firmware
* 0x09: String Result message
* 0x0a: 1 byte Result flag - see "Result Flags"
* 0x0b: 4 byte IP Gateway
* 0x0c: 2 byte Configuration error code - see "Configuration Errors"
* 0x0d: String device name
* 0x0e: 4 byte Real Port number
* 0x0f: 4 byte IP address. Purpose unknown.
* 0x10: 1 byte DHCP Enabled boolean flag.
* 0x11: 1 byte Error code
* 0x12: 1 byte Serial Port Count
* 0x13: 4 byte Encrypted Real Port number

### Error codes (Field 0x11)

* 0x00: Success
* 0x01: Authentication Failure
* 0x03: Invalid Value
* 0x06: Unable to save value

### Result flags (Field 0x0a)

* 0x00: Success
* 0xff: Error

### Configuration Errors (0x0c)

* 0x0000: No error.
* 0x0001: Digi in different subnet than sender

### Boolean flags

* 0x01: True/Enabled
* 0x00: False/Disabled


Some General Notes
------------------
There might be more packet types.
There are more error codes which I haven't seen, and thus haven't documented.
Fields types 0x05 and 0x06 haven't been seen, so I'm not sure what they're for.
I haven't determined field 0x07. I haven't even seen a non 0x00 value for it.
It's also possible that fields above 0x13 exist.
Re. fields 0x0e and 0x13 being 4 bytes. Ports can only be 2 bytes (0
to 65535). Why they made it 4 bytes is a mystery, so it could be possible
that they store extra information in the first 2 bytes.

~~~~
Posted by Quintin at 21:57
Labels: addp, advanced digi discovery protocol, communication, digi
connect me, digi device discovery, electronics, ethernet, microcontrollers,
programming, protocols, reverse engineering, serial, utilities
4 comments:

 rick said...
Thanks! Found this info extremely helpful. I had the need to add this
functionality to a C Sharp based application. Wireshark did the trick
as well. My magic key is "DVKT" but most everything else is the same.
Also addp.hh (from netos74 folders) yielded the answers to your missing
questions: Field x05 = Domain, Field x06 = HWType, x07 = HWRev, x0f
= DNS IP, x14 = Version ID, x15 = Vendor GUID. It also shows the authentication
piece to be a byte indicating length, immediately followed by the encrypted
password. Example (spaces and line feed added for clarity)

44564b54 //Vendor ID: DVKT
0005 //Message ID
002 //Payload size.
004010221 //MAC Address
10 fc3ba0d3f1aef2bac239a84b0c7ad504 //Password
14 02 0200 //Field 14-Version
15 10 bf6db409c83d44a3a36d21797d2f73f9 //Field 15-Vendor GUIDDNS IP,
x14 = Version ID, x15 = Vendor GUID. It also shows the authentication
piece to be a byte indicating length, immediately followed by the encrypted
password. Example (spaces and line feed added for clarity)

44564b54 //Vendor ID: DVKT
0005 //Message ID
002 //Payload size.
004010221 //MAC Address
10 fc3ba0d3f1aef2bac239a84b0c7ad504 //Password
14 02 0200 //Field 14-Version
15 10 bf6db409c83d44a3a36d21797d2f73f9 //Field 15-Vendor GUID
~~~~
