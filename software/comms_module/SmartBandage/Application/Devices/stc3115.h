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

#define STC3115_REG_ACC_CC_ADJ_LSB 27
#define STC3115_REG_ACC_CC_ADJ_MSB 28

#define STC3115_REG_ACC_VM_ADJ_LSB 29
#define STC3115_REG_ACC_VM_ADJ_MSB 30

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

#define STC3115_WRITE_REG_COUNT(device) (uint8_t)(STC3115_READ_REG_COUNT(device) + 1)
#define STC3115_READ_REG_COUNT(device) ((uint8_t)((uint8*)&stc3115_address(device) - (uint8*)&stc3115_mode(device))-1)
#define STC3115_READ_PTR(device) ((uint8_t*)&stc3115_mode(device))
#define STC3115_WRITE_PTR(device) ((uint8_t*)device)

#define STC3115_RW_REG_ACCESSOR8(device, reg) device[reg + 1]
#define STC3115_RO_REG_ACCESSOR8(device, reg) ((uint8_t) *(device + reg + 1))

#define STC3115_RW_REG_ACCESSOR16(device, reg_lsb) *((uint16_t*) &device[reg_lsb + 1])
#define STC3115_RO_REG_ACCESSOR16(device, reg_lsb) ((uint16_t) *(device + reg_lsb + 1))

// The STC3115 device is defined as a 66 byte array so that all 64 registers from the
// device may be read and written in a single I2C operation. Therefore instead of accessing
// struct members, use the below stc3115_*() macros to access specific values within the
// device array. These macros will produce readings of the right type (8 or 16 bit) and will
// only allow writing to writeable registers. RO registers cannot be assigned using these macros.

#define stc3115_devAddrPointer(device) device[0]
#define stc3115_mode(device) STC3115_RW_REG_ACCESSOR8(device, STC3115_REG_MODE)
#define stc3115_ctrl(device) STC3115_RW_REG_ACCESSOR8(device, STC3115_REG_CTRL)
#define stc3115_soc(device)  STC3115_RW_REG_ACCESSOR16(device, STC3115_REG_SOC_LSB)
#define stc3115_counter(device) STC3115_RO_REG_ACCESSOR16(device, STC3115_REG_COUNTER_LSB)
#define stc3115_current(device) STC3115_RO_REG_ACCESSOR16(device, STC3115_REG_CURRENT_LSB)
#define stc3115_voltage(device) STC3115_RO_REG_ACCESSOR16(device, STC3115_REG_VOLTAGE_LSB)
#define stc3115_temperature(device) STC3115_RO_REG_ACCESSOR8(device, STC3115_REG_TEMPERATURE)
#define stc3115_cc_adj_high(device) STC3115_RO_REG_ACCESSOR8(device, STC3115_REG_CC_ADJ_HIGH)
#define stc3115_vm_adj_high(device) STC3115_RO_REG_ACCESSOR8(device, STC3115_REG_VM_ADJ_HIGH)
#define stc3115_ocv(device) STC3115_RW_REG_ACCESSOR16(device, STC3115_REG_OCV_LSB)
#define stc3115_cc_cnf(device) STC3115_RW_REG_ACCESSOR16(device, STC3115_REG_CC_CNF_LSB)
#define stc3115_vm_cnf(device) STC3115_RW_REG_ACCESSOR16(device, STC3115_REG_VM_CNF_LSB)
#define stc3115_alarm_soc(device) STC3115_RW_REG_ACCESSOR8(device, STC3115_REG_ALARM_SOC)
#define stc3115_alarm_voltage(device) STC3115_RW_REG_ACCESSOR8(device, STC3115_REG_ALARM_VOLTAGE)
#define stc3115_current_thres(device) STC3115_RW_REG_ACCESSOR8(device, STC3115_REG_CURRENT_THRES)
#define stc3115_relax_count(device) STC3115_RO_REG_ACCESSOR8(device, STC3115_REG_RELAX_COUNT)
#define stc3115_relax_max(device) STC3115_RO_REG_ACCESSOR8(device, STC3115_REG_RELAX_MAX)
#define stc3115_id(device) STC3115_RO_REG_ACCESSOR8(device, STC3115_REG_ID)
#define stc3115_cc_adj_low(device) STC3115_RO_REG_ACCESSOR8(device, STC3115_REG_CC_ADJ_LOW)
#define stc3115_vm_adj_low(device) STC3115_RO_REG_ACCESSOR8(device, STC3115_REG_VM_ADJ_LOW)
#define stc3115_acc_cc_adj(device) STC3115_RO_REG_ACCESSOR16(device, STC3115_REG_ACC_CC_ADJ_LSB)
#define stc3115_acc_vm_adj(device) STC3115_RO_REG_ACCESSOR16(device, STC3115_REG_ACC_VM_ADJ_LSB)
#define stc3115_ramX(device) (*((uint8_t (*)[STC3115_NUM_RAM_REGISTERS]) &device[STC3115_REG_RAM0 + 1]))
#define stc3115_ocvX(device) (*((uint8_t (*)[STC3115_NUM_OCV_REGISTERS]) &device[STC3115_REG_OCVTAB_START + 1]))
#define stc3115_address(device) device[STC3115_DEV_SIZE - 1]

#define STC3115_DEV_SIZE (STC3115_NUM_REGISTERS + 1 + 1)
typedef uint8_t STC3115_DEVICE_HANDLE[STC3115_DEV_SIZE];

SB_Error stc3115_init(STC3115_DEVICE_HANDLE device, Semaphore_Handle *semaphore);
SB_Error stc3115_readInfo(STC3115_DEVICE_HANDLE device, Semaphore_Handle *semaphore);
SB_Error stc3115_configure(STC3115_DEVICE_HANDLE device, Semaphore_Handle *semaphore, uint16_t rsense, uint16_t battImpedance, uint16_t battCapacity);
inline uint16_t stc3115_convertedVoltage(STC3115_DEVICE_HANDLE device);
inline uint16_t stc3115_convertedTemp(STC3115_DEVICE_HANDLE device);

#endif
