/*
 * pru_tables.h/
 *
 */
#define CHNL_TOT 8
struct channel_s {
	uint16_t ppm; //0-60*1000
	//uint8_t pulse_width_0_5mS;
	//uint8_t mode; //0=Continuous,1=singleshot
} ;


union channel_u {
    uint16_t sr; //shared ram
    struct channel_s channel;
};

struct channels_s {
	union channel_u chn[CHNL_TOT]; /*maps to SHARED_RAM space */
} ;
