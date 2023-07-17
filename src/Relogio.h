#ifndef RELOGIO_H
#define RELOGIO_H

#include <ESP8266WiFi.h>
#include <ezTime.h>

struct DateTime_t {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
};

class Relogio {
	private:
	// Define NTP Client to get time
		Timezone timezone;
		//Week Days
		String weekDays[7]={"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sáb"};

		//Month names
		String months[12]={"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"};
		String format2digits(int number) {
			if (number < 10) {
				return "0" + String(number);
			}
			return String(number);
		}

	public:
		
		bool needUpdate(){
			return timeStatus() != timeStatus_t::timeSet;
		}
		void updateTime(){
			if ( needUpdate() ){
				updateNTP();
			}
		}
		void setup() {
			if (WiFi.status() != WL_CONNECTED) {
				Serial.println("error ao conectar...");
				return;
			}
			waitForSync();
			timezone.setLocation("America/Maceio");
			// timezone.setPosix("GMT-3");
			Serial.println("Time synchronized");
			setInterval(60);
		}
		

		DateTime_t getTime() {
			DateTime_t dateTime;
			// time_t epochTime = now();
			// struct tm *ptm = gmtime(&epochTime);

			// dateTime.year = timezone.year;
			// dateTime.month = timezone.month;
			// dateTime.day = timezone.day;
			// dateTime.hour = timezone.hour;
			// dateTime.minute = timezone.minute;
			// dateTime.second = timezone.second;

			return dateTime;
		}
		String formatarDataHora(DateTime_t dateTime) {
			
			return String(dateTime.year) + "-" + format2digits(dateTime.month) + "-" + format2digits(dateTime.day) + " " +
				format2digits(dateTime.hour) + ":" + format2digits(dateTime.minute) + ":" + format2digits(dateTime.second);
		}

		String formatarDataHora(String formato = "d-m-Y H:i:s") {
			updateTime();
			return timezone.dateTime(formato);
		}

		Timezone& getTimezone() {
			return timezone;
		}
	
		String getDataHora(){
			return formatarDataHora(getTime()); 
		}
};

#endif