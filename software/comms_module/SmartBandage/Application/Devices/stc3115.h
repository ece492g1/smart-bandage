#ifndef MCP9808_H_
#define MCP9808_H_

#define STC3115_I2C_ADDRESS 0b1110000

#define STC3115_REG_MODE            0
#define STC3115_REG_MODE_VMODE      0
#define STC3115_REG_MODE_CLR_VM_ADJ 1
#define STC3115_REG_MODE_CLR_CC_ADJ 2
#define STC3115_REG_MODE_ALM_ENA    3
#define STC3115_REG_MODE_GG_RUN     4
#define STC3115_REG_MODE_FORCE_CC   5
#define STC3115_REG_MODE_FORCE_VM   6

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

#define STC3115_REG_VM_CNF_LSB 17
#define STC3115_REG_VM_CNF_MSB 18

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

// Struct for an STC3115 device. Note that the order in the struct
// corresponds to the order of the actual registers - meaning that this struct can
// be filled by directly reading/writing 64 bytes from/to the device.
typedef struct {
	uint8_t  mode;
	uint8_t  ctrl;
	uint16_t soc;
	uint16_t counter;
	uint16_t current;
	uint16_t voltage;
	uint8_t  temperature;
	uint8_t  cc_adj_high;
	uint8_t  vm_adj_high;
	uint16_t ocv;
	uint16_t cc_cnf;
	uint16_t vm_cnf;
	uint8_t  alarm_soc;
	uint8_t  alarm_voltage;
	uint8_t  current_thres;
	uint8_t  relax_count;
	uint8_t  relax_max;
	uint8_t  id;
	uint8_t  cc_adj_low;
	uint8_t  vm_adj_low;
	uint16_t acc_cc_adj;
	uint16_t acc_vm_adj;
	uint8_t  ramX[STC3115_NUM_RAM_REGISTERS];
	uint8_t  ocvX[STC3115_NUM_OCV_REGISTERS];
	uint8_t  Address;
} STC3115_DEVICE;

#endif
