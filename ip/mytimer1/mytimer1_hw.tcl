package require -exact sopc 9.1

set_module_property NAME mytimer1
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property GROUP "Jalex"
set_module_property DISPLAY_NAME mytimer1
set_module_property TOP_LEVEL_HDL_FILE main.vhd
set_module_property TOP_LEVEL_HDL_MODULE mytimer2
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property ANALYZE_HDL TRUE

add_file main.vhd {SYNTHESIS SIMULATION}

add_interface avalon_slave avalon end
set_interface_property avalon_slave addressAlignment NATIVE
set_interface_property avalon_slave associatedClock clock_sink
set_interface_property avalon_slave burstOnBurstBoundariesOnly false
set_interface_property avalon_slave explicitAddressSpan 0
set_interface_property avalon_slave holdTime 0
set_interface_property avalon_slave isMemoryDevice false
set_interface_property avalon_slave isNonVolatileStorage false
set_interface_property avalon_slave linewrapBursts false
set_interface_property avalon_slave maximumPendingReadTransactions 0
set_interface_property avalon_slave printableDevice false
set_interface_property avalon_slave readLatency 0
set_interface_property avalon_slave readWaitStates 2
set_interface_property avalon_slave readWaitTime 2
set_interface_property avalon_slave setupTime 0
set_interface_property avalon_slave timingUnits Cycles
set_interface_property avalon_slave writeWaitTime 0

set_interface_property avalon_slave ASSOCIATED_CLOCK clock_sink
set_interface_property avalon_slave ENABLED true

add_interface_port avalon_slave s_read read Input 1
add_interface_port avalon_slave s_cs_n chipselect_n Input 1
add_interface_port avalon_slave s_readdata readdata Output 32
add_interface_port avalon_slave s_write write Input 1
add_interface_port avalon_slave s_writedata writedata Input 32

add_interface clock_sink clock end

set_interface_property clock_sink ENABLED true

add_interface_port clock_sink clk clk Input 1
add_interface_port clock_sink reset_n reset_n Input 1

add_interface interrupt_sender interrupt end
set_interface_property interrupt_sender associatedAddressablePoint avalon_slave

set_interface_property interrupt_sender ASSOCIATED_CLOCK clock_sink
set_interface_property interrupt_sender ENABLED true

add_interface_port interrupt_sender irq irq Output 1



