# bbb_pru_ppm
BeagleBone Black - using the PRU0 for generating a pulse per minute signal on output. That is a variable frequency, fixed pulse width. Range 0ppm to 6000ppm(60sec*1000pps)

Status/Testing
<latest-date>  
180626-1700: [4.14.49] rebuit and workking with P9_25 _26 - looks like shareRam comms issue from 3rd variable  
18061?: [4.9.78] P9_25 27 28 29 30 31, P8_11 12 working with shareRam[i][0-7] using <16bits> per [i]  
180616-1610: Channel7 P9_25 ppm_delay transferring sharedRam[i]0,1,2,3,4 using <32bits>- but not 5,onwards!!  
180616-1557: Channel7 P9_25 ppm_delay through struct through shared ram[0]; working.  
180616-1455: Basic Channel7 P9_25 ppm delay through ram working  
