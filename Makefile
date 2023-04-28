.PHONY: config configure_from_env

CONFIG_FILE = "./nv7-smart-led-controls/secrets.h"

configure:

	WIFI_SSID_NAME="$$(read -p 'WiFi SSID: ' ssid && echo $$ssid)" \
	WIFI_PASSWORD="$$(read -p 'WiFi Password: ' pass && echo $$pass)" \
	LED_PIN="$$(read -p 'LED Pin number: ' number && echo $$number)" \
	BUTTON_PIN="$$(read -p 'Button Pin number: ' number && echo $$number)" \
	MQTT_ADDRESS="$$(read -p 'MQTT Broker address: ' address && echo $$address)" \
	make configure_from_env

configure_from_env:
	echo "Generating $(CONFIG_FILE)"
	$(shell [ -e $(CONFIG_FILE) ] && rm $(CONFIG_FILE))
	@echo "#define WIFI_SECRET_SSID \"$(WIFI_SSID_NAME)\"" >> $(CONFIG_FILE)
	@echo "#define WIFI_SECRET_PASSWORD \"$(WIFI_PASSWORD)\"" >> $(CONFIG_FILE)
	@echo "#define MQTT_ADDRESS \"$(MQTT_ADDRESS)\"" >> $(CONFIG_FILE)
	@echo "#define LED_PIN $(LED_PIN)" >> $(CONFIG_FILE)
	@echo "#define BUTTON_PIN $(BUTTON_PIN)" >> $(CONFIG_FILE)