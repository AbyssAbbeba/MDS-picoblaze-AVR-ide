
# Project name
PROJECT = EVK527-series4-hidio

# CPU architecture : {avr0|...|avr6}
# Parts : {at90usb646|at90usb647|at90usb1286|at90usb1287|at90usb162|at90usb82}
MCU = atmega32u4

# Source files
CSRCS = \
  ../../../lib_mcu/wdt/wdt_drv.c\
  ../main.c\
  ../../../modules/usb/device_chap9/usb_standard_request.c\
  ../../../modules/usb/usb_task.c\
  ../../../modules/scheduler/scheduler.c\
  ../../../lib_mcu/util/start_boot.c\
  ../usb_descriptors.c\
  ../../../modules/usb/device_chap9/usb_device_task.c\
  ../../../lib_mcu/usb/usb_drv.c\
  ../usb_specific_request.c\
  ../../../lib_mcu/power/power_drv.c\
  ../hid_task.c\

# Assembler source files
ASSRCS = \
  ../../../lib_mcu/flash/flash_drv.s\

