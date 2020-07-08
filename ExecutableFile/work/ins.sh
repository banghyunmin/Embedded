insmod fpga_led_driver.ko
insmod fpga_dip_switch_driver.ko
insmod fpga_fnd_driver.ko
insmod fpga_text_lcd_driver.ko
insmod fpga_dot_driver.ko
insmod fpga_push_switch_driver.ko
insmod fpga_bz_driver.ko
insmod fpga_motor_driver.ko

mknod /dev/fpga_led c 260 1
mknod /dev/fpga_dip_switch c 266 1
mknod /dev/fpga_fnd c 261 1
mknod /dev/fpga_text_lcd c 263 1
mknod /dev/fpga_dot c 262 1
mknod /dev/fpga_push_switch c 265 1
mknod /dev/fpga_buzzer c 264 1
mknod /dev/fpga_step_motor c 267 1
