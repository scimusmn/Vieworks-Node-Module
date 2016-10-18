#ifndef __GVCP_REGISTERS__
#define __GVCP_REGISTERS__



#define	GVSP_PIX_3BYTE_MASK 0x02000000
#define GVSP_PIX_2BYTE_MASK 0x01100000
#define GVSP_PIX_1BYTE_MASK 0x01000000

typedef enum
{
	NIC_TYPE_DRIVER_NONE = 0,
	NIC_TYPE_DRIVER_REALTEK = 1,
	NIC_TYPE_DRIVER_INTEL = 2,
	NIC_TYPE_DRIVER_SOCKET = 3,
	NIC_TYPE_DRIVER_FILTER = 4
}NIC_TYPES;

#define INTELINX_HEADER_SIZE 50

//----------------------------------------------------------------------------------
#define GIGE_VERSION 0x0000 //Version M R 4 Version of the GigE Standard with which the device is
//compliant
//The two most-significant bytes are allocated to the major
//number of the version, the two least-significant bytes for
//the minor number.
#define GIGE_DEVICE_MODE 0x0004 //Device Mode M R 4 Information about device mode of operation.
#define GIGE_DEVICE_MAC_ADDRESS_H 0x0008 //Device MAC address ?High
//(Network interface #0)
//M R 4 The two most-significant bytes of this area are reserved
//and will return 0. Upper 2 bytes of the MAC address are
//stored in the lower 2 significant bytes of this register.
#define GIGE_DEVICE_MAC_ADDRESS_L 0x000C //Device MAC address ?Low
//(Network interface #0)
//M R 4 Lower 4 bytes of the MAC address
#define GIGE_SUPPORTED_IP_CFG 0x0010 //Supported IP configuration
//(Network interface #0)
//M R 4 Bits can be OR-ed. All other bits are reserved and set to
//0. DHCP and LLA bits must be on.
#define GIGE_CURRENT_IP_CFG 0x0014 //Current IP configuration procedure
//(Network interface #0)
//M R/W 4 Bits can be OR-ed. LLA is always activated and is readonly.
//0x0018 Reserved - - -
#define GIGE_CURRENT_IP 0x0024 //Current IP address (Network
//interface #0)
//M R 4 Current IP address of this device.
//0x0028 Reserved - - -
#define GIGE_CURRENT_SUBNET_MASK 0x0034 //Current subnet mask (Network
//interface #0)
//M R 4 Current subnet mask used by this device.
//0x0038 Reserved - - -
#define GIGE_DEFAULT_GATEWAY 0x0044 //Current default Gateway (Network
//interface #0)
//M R 4 Current default gateway used by this device.
#define GIGE_MANUFACTURER_NAME 0x0048 //Manufacturer name M R 32 Provides the name of the manufacturer of the device.
//String using the format in “string character set? The
//string must be 0 terminated, and all bytes after the
//terminator must be 0.
#define GIGE_MODEL_NAME 0x0068 //Model name M R 32 Provides the model name of the device.
//String using the format in “string character set? The
//string must be 0 terminated, and all bytes after the
//terminator must be 0.
#define GIGE_DEVICE_VERSION 0x0088 //Device version M R 32 Provides the version of the device.
//String using the format in “string character set? The
//string must be 0 terminated, and all bytes after the
//terminator must be 0.
//GigE Vision Specification
//Draft 1.6 February 28, 2006 Page 144 of 198
//Address Name Support Type Length
//(bytes)
//Description
#define GIGE_MANUFACTURER_SPECIFIC_INFO 0x00A8 //Manufacturer specific information M R 48 Provides extended manufacturer information about the
//device.
//String using the format in “string character set? The
//string must be 0 terminated.
#define GIGE_SERIAL_NUMBER 0x00D8 //Serial number O R 16 When supported, this string contains the serial number of
//the device. It can be used to identify the device.
//This string is provided in the DISCOVERY_ACK
//message (set to all NULL when not supported).
//String using the format in “string character set? The
//string must be 0 terminated.
#define GIGE_USER_DEFINED_NAMED 0x00E8 //User-defined name O R/W 16 When supported, this string contains a user-programmable
//name to assign to the device. It can be used to identify the
//device.
//This string is provided in the DISCOVERY_ACK
//message (set to all NULL when not supported).
//String using the format in “string character set? The
//string must be 0 terminated.
//0x00F8 Reserved - - -
#define GIGE_FIRST_URL_XML 0x0200 //First choice of URL for XML
//device description file
//M R 512 String using the format in “string character set? The
//string must be NULL-terminated, and all bytes after the
//terminator must be 0.
//3 different schemes are supported to link to the XML
//device description. This string indicates XML file
//location.
//1) XML stores in non-volatile on-board memory. In this
//case, the device provides the address and the length where
//the XML file is stored.
//2) URL to device manufacturer. If file extension is .XML,
//then file is an uncompressed text file. If file extension is
//.ZIP, then file is compressed using ZIP.
//3) Filename of the XML device description file. This
//assumes the application uses a ‘default?folder where
//XML files are stored. If file extension is .XML, then file
//is an uncompressed text file. If file extension is .ZIP, then
//file is compressed using ZIP.
#define GIGE_SECOND_URL_XML 0x0400 //Second choice of URL for XML
//device description file
//M R 512 String using the format in “string character set? The
//string must be NULL-terminated, and all bytes after the
//terminator must be 0.
#define GIGE_NUMBER_INTERFACES 0x0600 //Number of network interfaces M R 4 Indicates the number of physical network interfaces on
//this device. A device must have at least one network
//interface.
//0x0604 Reserved - - -
#define GIGE_PERSISTENT_IP 0x064C //Persistent IP address (Network
//interface #0)
//O R/W 4 Only available if Persistent IP is supported by the device.
//0x0650 Reserved - - -
//GigE Vision Specification
//Draft 1.6 February 28, 2006 Page 145 of 198
//Address Name Support Type Length
//(bytes)
//Description
#define GIGE_PERSISTENT_SUBNET_MASK 0x065C //Persistent subnet mask (Network
//interface #0)
//O R/W 4 Only available if Persistent IP is supported by the device.
//0x0660 Reserved - - -
#define GIGE_PERSISTENT_DEFAULT_GATEWAY 0x066C //Persistent default gateway
//(Network interface #0)
//O R/W 4 Only available if Persistent IP is supported by the device.
//0x0670 Reserved - - -
/*
0x0680 MAC address ?High (Network
interface #1)
O R 4 The two most-significant bytes of this area are reserved
and will return 0. Upper 2 bytes of the MAC address are
stored in the lower 2 significant bytes of this register.
0x0684 MAC address ?Low (Network
interface #1)
O R 4 Lower 4 bytes of the MAC address
0x0688 Supported IP configuration
(Network interface #1)
O R 4 Bits can be OR-ed. All other bits are reserved and set to
0. DHCP and LLA bits must be on.
0x068C Current IP configuration procedure
(Network interface #1)
O R/W 4 Bits can be OR-ed. LLA is always activated and is readonly.
0x0690 Reserved - - -
0x069C Current IP address (Network
interface #1)
O R 4 Current IP address of this device.
0x06A0 Reserved - - -
0x06AC Current subnet mask (Network
interface #1)
O R 4 Current subnet mask used by this device.
0x06B0 Reserved - - -
0x06BC Current default gateway (Network
interface #1)
O R 4 Current default gateway used by this device.
0x06C0 Reserved - - -
0x06CC Persistent IP address (Network
interface #1)
O R/W 4 Only available if Persistent IP is supported by the device.
0x06D0 Reserved - - -
0x06DC Persistent subnet mask (Network
interface #1)
O R/W 4 Only available if Persistent IP is supported by the device.
0x06E0 Reserved - - -
0x06EC Persistent default gateway
(Network interface #1)
O R/W 4 Only available if Persistent IP is supported by the device.
0x06F0 Reserved - - -
0x0700 MAC address ?High (Network
interface #2)
O R 4 The two most-significant bytes of this area are reserved
and will return 0. Upper 2 bytes of the MAC address are
stored in the lower 2 significant bytes of this register.
0x0704 MAC address ?Low (Network
interface #2)
O R 4 Lower 4 bytes of the MAC address
GigE Vision Specification
Draft 1.6 February 28, 2006 Page 146 of 198
Address Name Support Type Length
(bytes)
Description
0x0708 Supported IP configuration
(Network interface #2)
O R 4 Bits can be OR-ed. All other bits are reserved and set to
0. DHCP and LLA bits must be on.
0x070C Current IP configuration procedure
(Network interface #2)
O R/W 4 Bits can be OR-ed. LLA is always activated and is readonly.
0x0710 Reserved - - -
0x071C Current IP address (Network
interface #2)
O R 4 Current IP address of this device.
0x0720 Reserved - - -
0x072C Current subnet mask (Network
interface #2)
O R 4 Current subnet mask used by this device.
0x0730 Reserved - - -
0x073C Current default gateway (Network
interface #2)
O R 4 Current default gateway used by this device.
0x0740 Reserved - - -
0x074C Persistent IP address (Network
interface #2)
O R/W 4 Only available if Persistent IP is supported by the device.
0x0750 Reserved - - -
0x075C Persistent subnet mask (Network
interface #2)
O R/W 4 Only available if Persistent IP is supported by the device.
0x0760 Reserved - - -
0x076C Persistent default gateway
(Network interface #2)
O R/W 4 Only available if Persistent IP is supported by the device.
0x0770 Reserved - - -
0x0780 MAC address ?High (Network
interface #3)
O R 4 The two most-significant bytes of this area are reserved
and will return 0. Upper 2 bytes of the MAC address are
stored in the lower 2 significant bytes of this register.
0x0784 MAC address ?Low (Network
interface #3)
O R 4 Lower 4 bytes of the MAC address
0x0788 Supported IP configuration
(Network interface #3)
O R 4 Bits can be OR-ed. All other bits are reserved and set to
0. DHCP and LLA bits must be on.
0x078C Current IP configuration procedure
(Network interface #3)
O R/W 4 Bits can be OR-ed. LLA is always activated and is readonly.
0x0790 Reserved - - -
0x079C Current IP address (Network
interface #3)
O R 4 Current IP address of this device.
0x07A0 Reserved - - -
0x07AC Current subnet mask (Network
interface #3)
O R 4 Current subnet mask used by this device.
0x07B0 Reserved - - -
GigE Vision Specification
Draft 1.6 February 28, 2006 Page 147 of 198
Address Name Support Type Length
(bytes)
Description
0x07BC Current default gateway (Network
interface #3)
O R 4 Current default gateway used by this device.
0x07C0 Reserved - - -
0x07CC Persistent IP address (Network
interface #3)
O R/W 4 Only available if Persistent IP is supported by the device.
0x07D0 Reserved - - -
0x07DC Subnet mask (Network interface
#3)
O R/W 4 Only available if Persistent IP is supported by the device.
0x07E0 Reserved - - -
0x07EC Default gateway (Network
interface #3)
O R/W 4 Only available if Persistent IP is supported by the device.
0x07F0 Reserved - - -
0x0900 Number of Message channels M R 4 Indicates the number of message channels supported by
this device. It can take two values: 0 or 1.
0x0904 Number of Stream channels M R 4 Indicates the number of stream channels supported by this
device. It can take any value from 1 to 512.
0x0908 Reserved - - -
*/
#define GIGE_GVCP_CAPABILITY 0x0934 //GVCP Capability M R 4 This is a capability register indicating which one of the
//non-mandatory GVCP commands are supported by this
//device.
#define GIGE_HEARTBEAT_TIMEOUT 0x0938 //Heartbeat timeout M R/W 4 In msec, default is 3000 msec. Internally, the heartbeat is
//rounded according to the clock used for heartbeat. The
//heartbeat timeout shall have a minimum precision of 100
//ms. The minimal value is 500 ms.
/*
0x093C Timestamp tick frequency ?High O R 4 64-bit value indicating the number of timestamp clock
tick in 1 second. This register holds the most significant
bytes.
Timestamp tick frequency is 0 if timestamp is not
supported.
0x0940 Timestamp tick frequency ?Low O R 4 64-bit value indicating the number of timestamp clock
tick in 1 second. This register holds the least significant
bytes.
Timestamp tick frequency is 0 if timestamp is not
supported.
0x0944 Timestamp control O W 4 Used to latch the current timestamp value. No need to
clear to 0.
0x0948 Timestamp value (latched) ?High O R 4 Latched value of the timestamp (most significant bytes)
0x094C Timestamp value (latched) ?Low O R 4 Latched value of the timestamp (least significant bytes)
0x0950 Reserved -
*/
#define GIGE_CCP 0x0A00 //CCP M R/W 4 Control Channel Privilege register.
//0x0A04 Reserved
#define GIGE_MCP 0x0B00 //MCP O R/W 4 Message Channel Port register.
//GigE Vision Specification
//Draft 1.6 February 28, 2006 Page 148 of 198
//Address Name Support Type Length
//(bytes)
//Description
//0x0B04 Reserved - - -
#define GIGE_MCDA 0x0B10 //MCDA O R/W 4 Message Channel Destination Address register.
#define GIGE_MCTT 0x0B14 //MCTT O R/W 4 Message Channel Transmission Timeout in ms
#define GIGE_MCRC 0x0B18 //MCRC O R/W 4 Message Channel Retry Count
//0x0B1C Reserved - -
#define GIGE_SCP0 0x0D00 //SCP0 M R/W 4 First Stream Channel Port register.
#define GIGE_SCPS0 0x0D04 //SCPS0 M R/W 4 First Stream Channel Packet Size register.
#define GIGE_SCPD0 0x0D08 //SCPD0 M R/W 4 First Stream Channel Packet Delay register.
//0x0D0C Reserved - - -
#define GIGE_SCDA0 0x0D18 //SCDA0 M R/W 4 First Stream Channel Destination Address register.
/*
0x0D1C Reserved - -
0x0D40 SCP1 O R/W 4 Second stream channel, if supported.
0x0D44 SCPS1 O R/W 4 Second stream channel, if supported.
0x0D48 SCPD1 O R/W 4 Second stream channel, if supported.
0x0D4C Reserved - - -
0x0D58 SCDA1 O R/W 4 Second stream channel, if supported.
0x0D5C Reserved - -
0x0D80 Other stream channels registers O R/W - Each stream channel is allocated a section of 64 bytes
(0x40). Only supported channels are available.
0x8CC0 SCP511 O R/W 4 512th stream channel, if supported.
0x8CC4 SCPS511 O R/W 4 512th stream channel, if supported.
0x8CC8 SCPD511 O R/W 4 512th stream channel, if supported.
0x8CCC Reserved - - -
0x8CD8 SCDA511 O R/W 4 512th stream channel, if supported.
0x8CDC Reserved - -
0xA000 Start of manufacturer-specific
register space
- - - Start of device-specific registers. These are not covered by
the specification.
*/

#define GIGE_GVCP_NB_STREAM_CHANNELS 0x0904

#define FLASH_OFFSET			0x40C00000
#define FPGA_RUNTIME_ZONE		0x881A0
#define NEW_FIRMWARE_FLAG		FLASH_OFFSET + 0x001800A0
#define FLASH_FPGA_COPY1_OFFSET	FLASH_OFFSET + 0x00008000  
#define FLASH_FPGA_COPY1_END	FLASH_OFFSET + 0x0007FFFF
#define FLASH_FPGA_COPY2_OFFSET	FLASH_OFFSET + 0x00080000  
#define FLASH_FPGA_COPY2_END	FLASH_OFFSET + 0x000FFFFF
#define MB_COPY1_OFFSET			FLASH_OFFSET + 0x00100000  
#define MB_COPY1_END			FLASH_OFFSET + 0x0013FFFF
#define MB_COPY2_OFFSET			FLASH_OFFSET + 0x00140000  
#define MB_COPY2_END			FLASH_OFFSET + 0x0017FFFF

#endif
