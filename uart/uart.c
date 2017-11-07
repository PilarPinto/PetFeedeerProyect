char uart_getchar()
{   
	while (! (uart0->ucr & UART_DR)) ;
	return uart0->rxtx;
}

void uart_putchar(char c)
{
	while (uart0->ucr & UART_BUSY) ;
	uart0->rxtx = c;
}

void uart_putstr(char *str)
{
	char *c = str;
	while(*c) {
		uart_putchar(*c);
		c++;
	}
}

/*******************************
 **** FUNCIONES PARA WIFI   ****
 *******************************/

void init_wifi(){ //configurar el modulo como estación con puerto 80
	//comando de prueba
	int c=0;
	while(c==0){
		uart_putstr(" AT+RST\r\n");
		c=ok();
	}
	
	msleep(200);
	c = 0;
	while(c==0){  //con el siguiente comando aceptamos que el modulo tenga multiples conexiones
		uart_putstr("AT+CIPMUX=1\r\n");
		c = ok();
	}
	msleep(10);
	c = 0;
	
	//Se configura el modulo como un servidor con puerto 80
	while (c==0){
		uart_putstr("AT+CIPSERVER=1,80\r\n");
		c = ok();
	}

	//configurar Servidor como un acces point
	//con esto nos podemos conectar desde cualquier dispositivo
	msleep(10);
	c=0;
	while(c==0){
		uart_putstr("AT+CWSAP=\"PetFeeder\",\"pet1234\",5,3\r\n");
		c=ok();
	}


	//inicio segmento para obtener dirección IP
	msleep(10);
	c=0;
	while(c==0){
		uart_putstr("AT+CIFSR\r\n");
		c=ok();
	}//fin de segmento
}


	//funciones con la secuencia para enviar datos a travez del periferico UART a el modulo ESP8266
void wifi_putchar(char a){
	int c = 0; 	  
	while(c == 0){
		uart_putstr("AT+CIPSEND=0,1\r\n");
		msleep(10);
		uart_putchar(a);
		c = ok();
		msleep(100);
	}
	c = 0; 	  
	while(c == 0){
		uart_putstr("AT+CIPCLOSE=0\r\n");
		c = ok();
	}
}

void wifi_putstr(char *a){
	int c = 0;   
	char *cc = a;
	int counter = 0;
	while(*cc) {
		uart_putchar(*cc);
		cc++;
		counter ++;
	}

	while(c == 0){
		uart_putstr("AT+CIPSEND=0,");
		uart_putchar(counter);
		uart_putstr("\r\n");
		msleep(10);
		uart_putstr(a);
		c = ok();
		msleep(100);
	}
	c = 0; 	  
	while(c == 0){
		uart_putstr("AT+CIPCLOSE=0\r\n");
		c = ok();
	}
}
	
char wifi_getchar(){
	char c='\n';
	int i=0;
	for(i=0;i<20;i++){
		c = uart_getchar();
		if (c ==':'){
			c = uart_getchar();
			return c;
		}
	}
	return '\n';
}
	//función para comprobar que el comando fue enviado correctamente.
int ok(){
	int i=0;
	char a;
	for(i=0;i<30;i++){
		a = uart_getchar();
		if(a=='K'){
			return 1;
		}
	}
	return 0;

}
