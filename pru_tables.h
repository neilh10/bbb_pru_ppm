/*
 * pru_tables.h/
 *
 */
struct channel_s {
	uint16_t ppm; //0-60*1000
	uint8_t pulse_width_0_5mS;
	uint8_t mode; //0=Continuous,1=singleshot
} ;


union channel_u {
    uint32_t sr_uint32;
    struct channel_s channel;
};

struct channels_s {
	union channel_u chn[8]; /*maps to SHARED_RAM space */
} ;
