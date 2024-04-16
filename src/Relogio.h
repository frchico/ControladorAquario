#ifndef RELOGIO_H
#define RELOGIO_H

#include <ESP8266WiFi.h>
#include <ezTime.h>
#include <sunset.h>
// struct DateTime_t {
//   uint16_t year;
//   uint8_t month;
//   uint8_t day;
//   uint8_t hour;
//   uint8_t minute;
//   uint8_t second;
// };

class Relogio {
	private:
	// Define NTP Client to get time
		Timezone timezone;
		//Week Days
		String weekDays[7]={"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sáb"};

		//Month names
		static const String months[12];// = {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"};
		String format2digits(int number) {
			if (number < 10) {
				return "0" + String(number);
			}
			return String(number);
		}
		SunSet sun;
		double latitude = 0;
		double longitude = 0;
		int tz = -3;

	public:
		static const String* getMonths() {
			return Relogio::months;
		}
		bool needUpdate(){
			return timeStatus() != timeStatus_t::timeSet;
		}
		void updateTime(){
			if ( needUpdate() ){
				updateNTP();
			}
		}
		void setup(
				String location = "America/Maceio", 
				double lat = -10.9095, 
				double lon = -37.0748, 
				int tz = -3 ){
			if (WiFi.status() != WL_CONNECTED) {
				Serial.println("error ao conectar...");
				return;
			}
			waitForSync();
			timezone.setLocation(location);
			// timezone.setPosix("GMT-3");
			Serial.println("Time synchronized");
			setInterval(60);
			latitude = lat;
			longitude = lon;
			this->tz = tz;
			if ( latitude == 0 || longitude == 0 ){
				latitude = -10.9095;
				longitude = -37.0748;
				this->tz = timezone.getOffset(timezone.now());
			}
			sun.setPosition(latitude, longitude, tz);
		}


		static String formatarDataHora(unsigned long totalSeconds, String formato = "%02u:%02u:%02u"){
			// Converter para horas, minutos e segundos
			unsigned int hours = totalSeconds / 3600;
			unsigned int minutes = (totalSeconds % 3600) / 60;
			unsigned int seconds = totalSeconds % 60;
			
			// Formatar em uma string com o formato HH:mm:ss
			char buffer[9]; // Suficiente para HH:mm:ss\0
			snprintf(buffer, sizeof(buffer), formato.c_str(), hours, minutes, seconds);
			String timeString = String(buffer);
			
			// Imprimir a string resultante
			// Serial.println(timeString);
			return timeString;
			
		}
		
		/**
		 * Retorna um "DataTime" style. Ver https://github.com/ropg/ezTime?tab=readme-ov-file#breaktime
		*/
		tmElements_t getTime() {
			tmElements_t tm;
			breakTime(timezone.now(), tm);

			return tm;
		}
		tmElements_t getTime(time_t tempo ) {
			tmElements_t tm;
			breakTime(tempo, tm);
			return tm;
		}

		String formatarDataHora(tmElements_t dataHora) {
			
			return dateTime(makeTime(dataHora), "d/m/Y H:i:s");
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

		tmElements_t getSunrise(){
		// You should always set the date to be accurate
			sun.setCurrentDate(year(), month(), day());
			// If you have daylight savings time, make sure you set the timezone appropriately as well
			sun.setTZOffset(tz);
			time_t tempoUTC = sun.calcSunriseUTC();
			time_t tempo = timezone.tzTime(tempoUTC, UTC_TIME);
			return getTime(tempo);

		}
		tmElements_t getSunset(){
		// You should always set the date to be accurate
			sun.setCurrentDate(year(), month(), day());
			// If you have daylight savings time, make sure you set the timezone appropriately as well
			sun.setTZOffset(tz);
			time_t tempoUTC = sun.calcSunsetUTC();
			time_t tempo = timezone.tzTime(tempoUTC, UTC_TIME);
			return getTime(tempo);
			int moonphase = sun.moonPhase(std::time(nullptr));
		}
		
		int getMoonPhase(){
			return sun.moonPhase();
		}
};
const String Relogio::months[12] = {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"};
#endif