// HTTP2-weather.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma comment(lib, "libcurl_imp.lib")
#pragma comment(lib, "jsoncpp.lib")

#include <curl/curl.h>
#include <json/json.h>

#include <iostream>
#include <string>
#include "key.h"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    std::string* mem = (std::string*)userp;

    char* cont = (char*)contents;
    (*mem).append(cont, realsize);
    return realsize;
}
std::string timeStampToHReadble(const time_t rawtime)
{
    struct tm dt;
    char buffer[30];
    localtime_s(&dt , &rawtime);
    strftime(buffer, sizeof(buffer), "%m%d%H%M%y", &dt);
    return std::string(buffer);
}

int main(void)
{
    CURL* curl;
   

    Json::Reader reader;
    Json::Value js;
    std::string response;

    setlocale(LC_CTYPE, "Polish");
    curl = curl_easy_init();
    if (curl) {


        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);//podazanie za przekierowaniami
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);//obsluga "zapisu" danych
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        std::string city = "Gliwice";
        std::cout << "Podaj miasto" << std::endl;
        std::cin >> city;
            response.clear();
            std::string url = "http://api.openweathermap.org/data/2.5/weather/?q=" + city + "&appid=" + APIKEY::openWeatherMap + "&lang=pl&units=metric";
            const char* urlC = url.c_str();

            curl_easy_setopt(curl, CURLOPT_URL, urlC);

            /* Perform the request, res will get the return code */
            CURLcode res = curl_easy_perform(curl);
            /* Check for errors */
            if (res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

            /* always cleanup */
            curl_easy_cleanup(curl);


            if (reader.parse(response, js))
            {

                int code;
                code = js.get("cod", 0).asInt();
                if (code == 200)
                {
                    /* std::cout << "code: " << code << std::endl;*/
                    //std::cout << js.toStyledString() << std::endl; // Wyświetl całość w postaci tekstu

                     //const Json::Value data_mainweather = js["main"];
                     //const Json::Value data_main = js["main"];
                     //const Json::Value data_wind = js["main"];
                    std::cout << "Miasto: " << js.get("name", "brak danych").asString() << std::endl;
                    std::cout << "Temperatura: " << round(js.get("main", "brak danych")["temp"].asDouble() * 100) / 100 << "°C" << std::endl;
                    std::cout << "Temperatura odczuwalna: " << round(js.get("main", "brak danych")["feels_like"].asDouble() * 100) / 100 << "°C" << std::endl;
                    std::cout << "Cisnienie: " << (js.get("main", "brak danych")["pressure"].asInt()) << "hpa" << std::endl;
                    std::cout << "Wilgotnosc: " << js.get("main", "brak danych")["humidity"] << "%" << std::endl;
                    std::cout << "Predkosc wiatru: " << round(js.get("wind", "brak danych")["speed"].asDouble() * 100) / 100 << "m/s" << std::endl;
                    std::cout << "Kierunek wiatru: " << js.get("wind", "brak danych")["deg"] << "°" << std::endl;
                    std::cout << "Zachmurzenie: " << js.get("clouds", "brak danych")["all"] << "%" << std::endl;

                }
                else
                {
                    std::cout << "Error code: " << code << " : " << js.get("message", "brak danych") << std::endl;
                }
            }


    }
    return 0;
}