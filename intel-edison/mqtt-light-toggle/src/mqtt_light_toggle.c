#include "mraa.h"

#include <stdio.h>
#include <unistd.h>

#include <mosquitto.h>

int lightStatus = 0;

int toggleLight()
{
	// select onboard LED pin based on the platform type
	// create a GPIO object from MRAA using it
	mraa_platform_t platform = mraa_get_platform_type();
	mraa_gpio_context d_pin = NULL;
	switch (platform) {
		case MRAA_INTEL_GALILEO_GEN1:
			d_pin = mraa_gpio_init_raw(3);
			break;
		case MRAA_INTEL_GALILEO_GEN2:
			d_pin = mraa_gpio_init(13);
			break ;
		case MRAA_INTEL_EDISON_FAB_C:
			d_pin = mraa_gpio_init(13);
			break;
		default:
			fprintf(stderr, "Unsupported platform, exiting");
			return MRAA_ERROR_INVALID_PLATFORM;
	}
	if (d_pin == NULL) {
		fprintf(stderr, "MRAA couldn't initialize GPIO, exiting");
		return MRAA_ERROR_UNSPECIFIED;
	}

	// set the pin as output
	if (mraa_gpio_dir(d_pin, MRAA_GPIO_OUT) != MRAA_SUCCESS) {
		fprintf(stderr, "Can't set digital pin as output, exiting");
		return MRAA_ERROR_UNSPECIFIED;
	};


	if (lightStatus == 0)
	{
		lightStatus = 1;
		mraa_gpio_write(d_pin, 1);
	}
	else
	{
		lightStatus = 0;
		mraa_gpio_write(d_pin, 0);
	}

	return MRAA_SUCCESS;
}


void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
	if(message->payloadlen){
		printf("%s %s\n", message->topic, message->payload);
		toggleLight();
	}else{
		printf("%s (null)\n", message->topic);
	}
	fflush(stdout);
}

int main(int argc, char *argv[])
{
	printf("Start\n");
	char id[30];
	int i;
	char *host = "test.mosquitto.org";
	int port = 1883;
	int keepalive = 60;
	bool clean_session = true;
	struct mosquitto *mosq = NULL;
	
	mosquitto_lib_init();

	mosq = mosquitto_new(id, clean_session, NULL);
	if(!mosq){
		printf("Error:Out of memory\n");
		return 1;
	}

	mosquitto_message_callback_set(mosq, my_message_callback);

	if(mosquitto_connect(mosq, host, port, keepalive)){
		printf("Error:Unable to connect\n");
	}

	printf("Connect\n");

	mosquitto_subscribe(mosq, NULL, "testtopic1", 2);

	while(!mosquitto_loop(mosq, -1, 1)){

	}


	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;

}
