"%QEMU_PATH%\qemu.exe" -L "%QEMU_PATH%\bios" -kernel "..\kernel\bzImage" -initrd rootfs.img.gz -append "root=/dev/ram rdinit=/sbin/init vga=0x311 noapic"

:: virtual netcard 'my-tap' use 
:: -net nic -net tap,ifname=my-tap

:: setup ip
:: see http://namefame.blog.163.com/blog/static/17405234220114411498994/
:: see http://www.0x61.com/forum/busybox-linux-f110/setting-static-ip-t1267327.html
:: ipconfig eth0 up
:: ipconfig eth0 down


:: setup static ip
:: ifconfig eth0 192.168.200.11
:: ifconfig eth0 
:: ping 192.168.200.10
