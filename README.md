# bbb_pru_ppm
BeagleBone Black - using the PRU0 for generating a pulse per minute signal on output. That is a variable frequency, fixed pulse width. Range 0ppm to 6000ppm(60sec*1000pps)

To setup from a new BBB    
$sudo nano /boot/uEnv.txt  #change to be as follows  
disable_uboot_overlay_audio=1  
cmdline=coherent_pool=1M net.ifnames=0 quiet **cape_universaln=enable**  
uboot_overlay_pru=/lib/firmware/AM335X-PRU-RPROC-4-14-TI-00A0.dtbo  
\*save & exit*  
$sudo reboot now  
Create SSH and store in github->settings  
$mkdir git; cd git; git clone git@github.com:neilh10/bbb_pru_ppm.git github_bbb_pru_ppm   
$config-pin P9_25 pruout     # repeat 27 28 29 30 31 P8_11 P8_12  
$config-pin -q P9_25  
Shortcut is  
$cd bbb_pru_ppm/pru  
$./deploy.sh  
$sudo make clean  
$cd ..  
$./deploy.sh  

Status/Testing  
*latest-date-frst*  
180626-1700: [4.14.49] rebuit and workking with P9_25 _26 - looks like shareRam comms issue from 3rd variable  
18061?: [4.9.78] P9_25 27 28 29 30 31, P8_11 12 working with shareRam[i][0-7] using <16bits> per [i]  
180616-1610: Channel7 P9_25 ppm_delay transferring sharedRam[i]0,1,2,3,4 using <32bits>- but not 5,onwards!!  
180616-1557: Channel7 P9_25 ppm_delay through struct through shared ram[0]; working.  
180616-1455: Basic Channel7 P9_25 ppm delay through ram working  
