# Instructions
1. Get http://debian.beagleboard.org/images/bone-debian-9.4-lxqt-armhf-2018-06-17-4gb.img.xz
2. Testing with Linux beaglebone 4.9.78-ti-r94 #1 SMP PREEMPT Fri Jan 26 21:26:24 UTC 2018 armv7l GNU/Linux
3. 
2. Program a microSD card with that image using http://etcher.io
3. Boot it on a BeagleBone
4. Get connected to the Internet
5. Update for making PRU0 accessible
6. nano /boot/uEnv.txt
7. disable_uboot_overlay_audio=1
8. -----exit
9. On each powerup run pru/deploy.sh
10. (This executes config-pin px.yy pruout)


# Notes

*https://github.com/jadonk/bone101/blob/gh-pages/examples/extras/pru/bitflip.c  & bitflip.pru0c
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
