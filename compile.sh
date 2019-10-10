make all

DRIVE_PARAMS="-hda vm.img"
DEBUG_PARAMS="-debugcon vc --no-reboot --no-shutdown"
BOOTSYSTEM_PARAMS="-boot d --enable-kvm -smp 2"
MONITOR_PARAMS="-monitor stdio"
CPU_PARAMS="-cpu Nehalem"
MACHINE_TYPE="-machine $1"
MEMORY_ALLOCATION="-m 1024M"
GRAPHICS_PARAMS="-vga std"
USB_DEVICE="-device qemu-xhci"
SOUND_DEVICE="-device AC97"
NETWORK_DEVICE="-device rtl8139"
DEVICES_LIST="$GRAPHICS_PARAMS $DRIVE_PARAMS $USB_DEVICE $SOUND_DEVICE"

qemu-system-x86_64 $MACHINE_TYPE \
                $MEMORY_ALLOCATION \
                $DEVICES_LIST \
                $CPU_PARAMS \
                $MONITOR_PARAMS \
                $BOOTSYSTEM_PARAMS \
                $DEBUG_PARAMS
echo ""
