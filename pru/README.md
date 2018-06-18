# Instructions
1. Grab https://rcn-ee.com/rootfs/bb.org/testing/2017-06-11/stretch-iot/bone-debian-stretch-iot-armhf-2017-06-11-4gb.img.xz
2. Testing with Linux beaglebone 4.9.78-ti-r94 #1 SMP PREEMPT Fri Jan 26 21:26:24 UTC 2018 armv7l GNU/Linux
3. 
2. Program a microSD card with that image using http://etcher.io
3. Boot it on a BeagleBone
4. Get connected to the Internet
5. Run the following in the shell
```sh
cd /var/lib/cloud9
git clone https://gist.github.com/jadonk/2ecf864e1b3f250bad82c0eae12b7b64
cd 2ecf864e1b3f250bad82c0eae12b7b64
make
echo none > /sys/class/leds/beaglebone\:green\:usr0/trigger
sudo config-pin overlay cape-universala
sudo config-pin p9.30 pruout
sudo make run
```

You'll see USR0 blinking 5 times a second (toggles every 100ms). Modify hello-pru.c as desired
and enjoy!

# Notes

* sudo perl /opt/scripts/device/bone/show-pins.pl -v
* config-pin --help
* Bug tracker: http://bugs.elinux.org/projects/debian-image-releases
* http://elinux.org/EBC_Exercise_30_PRU_via_remoteproc_and_RPMsg
* http://elinux.org/EBC_Exercise_11b_gpio_via_mmap
* https://docs.google.com/presentation/d/1yMuyQwkYKU48LeMYnQj4sspnsbXf9niojWe_jr4BWjw/edit?usp=sharing
* http://processors.wiki.ti.com/images/3/34/Sitara_boot_camp_pru-module1-hw-overview.pdf
* http://processors.wiki.ti.com/index.php/PRU_Assembly_Instructions
* http://theduchy.ualr.edu/?p=996
* http://processors.wiki.ti.com/index.php/PRU_Projects

# Version

Debian Stretch BeagleBoard.org BeagleBone IoT Image

```sh
git:/opt/scripts/:[09ae22ec483e5483c6ae7f0ca7bbbabf864b06af]
eeprom:[A335BNLTBP00yywwBP000000]
dogtag:[BeagleBoard.org Debian Image 2017-06-11]
bootloader:[microSD-(push-button)]:[/dev/mmcblk0]:[U-Boot 2017.05-00002-ga302d6e48b]
kernel:[4.4.68-ti-r108]
nodejs:[v6.11.0]
uboot_overlay_options:[enable_uboot_overlays=1]
uboot_overlay_options:[uboot_overlay_pru=/lib/firmware/AM335X-PRU-RPROC-4-4-TI-00A0.dtbo]
uboot_overlay_options:[enable_uboot_cape_universal=1]
```