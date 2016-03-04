#ifndef MCP9808_H_
#define MCP9808_H_

#include "hci_tl.h"
#include "../Board.h"
#include <ti/sysbios/knl/Semaphore.h>

#define STC3115_I2C_ADDRESS 0b1110000

#define STC3115_REG_MODE            0
#define STC3115_REG_MODE_VMODE      0
#define STC3115_REG_MODE_CLR_VM_ADJ 1
#define STC3115_REG_MODE_CLR_CC_ADJ 2
#define STC3115_REG_MODE_ALM_ENA    3
#define STC3115_REG_MODE_GG_RUN     4
#define STC3115_REG_MODE_FORCE_CC   5
#define STC3115_REG_MODE_FORCE_VM   6

#define STC3115_REG_MODE_VMODE_MIXED 0
#define STC3115_REG_MODE_VMODE_PSAVE 1

#define STC3115_REG_CTRL          1
#define STC3115_REG_CTRL_IO0DATA  0
#define STC3115_REG_CTRL_GG_RST   1
#define STC3115_REG_CTRL_GG_VM    2
#define STC3115_REG_CTRL_BATFAIL  3
#define STC3115_REG_CTRL_PORDET   4
#define STC3115_REG_CTRL_ALM_SOC  5
#define STC3115_REG_CTRL_ALM_VOLT 6

#define STC3115_REG_SOC_LSB 2
#define STC3115_REG_SOC_MSB 3

#define STC3115_REG_COUNTER_LSB 4
#define STC3115_REG_COUNTER_MSB 5

#define STC3115_REG_CURRENT_LSB 6
#define STC3115_REG_CURRENT_MSB 7

#define STC3115_REG_VOLTAGE_LSB 8
#define STC3115_REG_VOLTAGE_MSB 9

#define STC3115_REG_TEMPERATURE 10
#define STC3115_REG_CC_ADJ_HIGH 11
#define STC3115_REG_VM_ADJ_HIGH 12

#define STC3115_REG_OCV_LSB 13
#define STC3115_REG_OCV_MSB 14

#define STC3115_REG_CC_CNF_LSB 15
#define STC3115_REG_CC_CNF_MSB 16

#define STC3115_REG_CC_CNF_CONV_FACTOR 1000/49556

#define STC3115_REG_VM_CNF_LSB 17
#define STC3115_REG_VM_CNF_MSB 18

#define STC3115_REG_VM_CNF_CONV_FACTOR 100/97778

#define STC3115_REG_ALARM_SOC     19
#define STC3115_REG_ALARM_VOLTAGE 20
#define STC3115_REG_CURRENT_THRES 21
#define STC3115_REG_RELAX_COUNT   22
#define STC3115_REG_RELAX_MAX     23
#define STC3115_REG_ID            24
#define STC3115_REG_CC_ADJ_LOW    25
#define STC3115_REG_VM_ADJ_LOW    26

#define STC3115_ACC_CC_ADJ_LSB 27
#define STC3115_ACC_CC_ADJ_MSB 28

#define STC3115_ACC_VM_ADJ_LSB 29
#define STC3115_ACC_VM_ADJ_MSB 30

#define STC3115_REG_RAM0  32
#define STC3115_REG_RAM1  33
#define STC3115_REG_RAM2  34
#define STC3115_REG_RAM3  35
#define STC3115_REG_RAM4  36
#define STC3115_REG_RAM5  37
#define STC3115_REG_RAM6  38
#define STC3115_REG_RAM7  39
#define STC3115_REG_RAM8  40
#define STC3115_REG_RAM9  41
#define STC3115_REG_RAM10 42
#define STC3115_REG_RAM11 43
#define STC3115_REG_RAM12 44
#define STC3115_REG_RAM13 45
#define STC3115_REG_RAM14 46
#define STC3115_REG_RAM15 47

#define STC3115_REG_OCVTAB_START 48
#define STC3115_REG_OCVTAB_END   63

#define STC3115_ADDR_FIRST     0
#define STC3115_ADDR_LAST     63
#define STC3115_NUM_REGISTERS 64

#define STC3115_NUM_RAM_REGISTERS 16
#define STC3115_NUM_OCV_REGISTERS 16

#define STC3115_DEVICE_ID 0x14

//#define STC3115_WRITE_REG_COUNT(device) (uint8_t)(STC3115_READ_REG_COUNT(device) + 1)
//#define STC3115_READ_REG_COUNT(device) (uint8_t)((uint8*)&device->address - (uint8*)&device->mode)
//#define STC3115_READ_PTR(device) ((uint8_t*)&device->mode)
//#define STC3115_WRITE_PTR(device) ((uint8_t*)device)

#define STC3115_WRITE_REG_COUNT(device) (uint8_t)(STC3115_READ_REG_COUNT(device) + 1)
#define STC3115_READ_REG_COUNT(device) ((uint8_t)((uint8*)&stc3115_address(device) - (uint8*)&stc3115_mode(device))-1)
#define STC3115_READ_PTR(device) ((uint8_t*)&stc3115_mode(device))
#define STC3115_WRITE_PTR(device) ((uint8_t*)device)
//#define STC3115_SET_ADDR_PTR(device, addr) (device->address = addr << 8)
//#define STC3115

typedef enum {
	BIG_ENDIAN,
	LITTLE_ENDIAN
} STC3115_ENDIANNESS;

typedef enum {
	STC3115_BATT_110mAh,
	STC3115_BATT_1000mAh,
} STC3115_BATT_CONFIG;

// Struct for an STC3115 device. Note that the order in the struct
// corresponds to the order of the actual registers - meaning that this struct can
// be filled by directly reading/writing 64 bytes from/to the device.
//typedef struct {
//	uint16_t  devAddrPointer;
//	uint8_t  mode;
//	uint8_t  ctrl;
//	uint16_t soc;
//	uint16_t counter;
//	int16_t current;
//	int16_t voltage;
//	int8_t  temperature;
//	uint8_t  cc_adj_high;
//	uint8_t  vm_adj_high;
//	uint16_t ocv;
//	uint16_t cc_cnf;
//	uint16_t vm_cnf;
//	uint8_t  alarm_soc;
//	uint8_t  alarm_voltage;
//	uint8_t  current_thres;
//	uint8_t  relax_count;
//	uint8_t  relax_max;
//	uint8_t  id;
//	uint8_t  cc_adj_low;
//	uint8_t  vm_adj_low;
//	uint16_t acc_cc_adj;
//	uint16_t acc_vm_adj;
//	uint8_t  ramX[STC3115_NUM_RAM_REGISTERS];
//	uint8_t  ocvX[STC3115_NUM_OCV_REGISTERS];
//	uint8_t  address;
//	STC3115_ENDIANNESS  endianness;
//} STC3115_DEVICE;

//typedef struct {
#define stc3115_devAddrPointer(device) device[0]
#define stc3115_mode(device) device[1]
#define stc3115_ctrl(device) device[2]
#define stc3115_soc(device) ((uint16_t) device[3])
#define stc3115_counter(device) ((uint16_t device[5])
#define stc3115_current(device) ((int16_t) *(device + 7))
#define stc3115_voltage(device) *((int16_t*) &device[9])
#define stc3115_temperature(device) *((int8_t*) &device[11])
#define stc3115_cc_adj_high(device) device[12]
#define stc3115_vm_adj_high(device) device[13]
#define stc3115_ocv(device) ((uint16_t) *(device + 14))
#define stc3115_cc_cnf(device) *((uint16_t*) (&device[16]))
#define stc3115_vm_cnf(device) *((uint16_t*) (&device[18]))
#define stc3115_alarm_soc(device) ((uint8_t) *(device + 20))
#define stc3115_alarm_voltage(device) ((uint8_t) *(device + 21))
#define stc3115_current_thres(device) ((uint8_t) *(device + 22))
#define stc3115_relax_count(device) ((uint8_t) *(device + 23))
#define stc3115_relax_max(device) ((uint8_t) *(device + 24))
#define stc3115_id(device) ((uint8_t) *(device + 25))
#define stc3115_cc_adj_low(device) ((uint8_t) *(device + 26))
#define stc3115_vm_adj_low(device) ((uint8_t) *(device + 27))
#define stc3115_acc_cc_adj(device) ((uint16_t) *(device + 28))
#define stc3115_acc_vm_adj(device) ((uint16_t) *(device + 29))
#define stc3115_ramX(device) ((uint8_t*) &device[33]);
#define stc3115_ocvX(device) ((uint8_t*) &device[49]);
#define stc3115_address(device) device[STC3115_DEV_SIZE - 2]
#define stc3115_endianness(device) device[STC3115_DEV_SIZE - 1]
//} STC3115_DEVICE;

typedef uint8_t* STC3115_DEVICE_HANDLE
#define STC3115_DEV_SIZE (STC3115_NUM_REGISTERS + 1 + 1 + 1)
#define STC3115_DEV_DECL(name) uint8 name[STC3115_DEV_SIZE]
;
SB_Error stc3115_init(STC3115_DEVICE_HANDLE device, Semaphore_Handle *semaphore);
SB_Error stc3115_readInfo(STC3115_DEVICE_HANDLE device, Semaphore_Handle *semaphore);
SB_Error stc3115_configure(STC3115_DEVICE_HANDLE device, Semaphore_Handle *semaphore, uint16_t rsense, uint16_t battImpedance, uint16_t battCapacity);
uint16_t stc3115_deviceVoltage(STC3115_DEVICE_HANDLE device);
inline uint16_t stc3115_deviceTemp(STC3115_DEVICE_HANDLE device);
void stc3115_preTransfer(STC3115_DEVICE_HANDLE device, STC3115_ENDIANNESS desiredEndianness);
void stc3115_postTransfer(STC3115_DEVICE_HANDLE device, STC3115_ENDIANNESS desiredEndianness);

#endif
