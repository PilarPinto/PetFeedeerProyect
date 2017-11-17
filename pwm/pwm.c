/***************************************************************************
 * PWM Functions
 */

void pwm_init(){
	pwm0->Period = (0x1E8480);
	pwm0->Duty = (0xEA60);
	msleep(0x5E8);
}

void pwm_stop(){
	pwm0->Period = (0);
	pwm0->Duty = (0);	
}
