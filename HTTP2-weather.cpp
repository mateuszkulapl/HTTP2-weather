// HTTP2-weather.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma comment(lib, "libcurl_imp.lib")
#pragma comment(lib, "jsoncpp.lib")

#include <curl/curl.h>
#include <json/json.h>

#include <iostream>
#include <string>


static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    std::string* mem = (std::string*)userp;

    char* cont = (char*)contents;
    (*mem).append(cont, realsize);
    return realsize;
}


int main(void)
{
    CURL* curl;
    CURLcode res;

    Json::Reader reader;
    Json::Value js;
    std::string response;



    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://danepubliczne.imgw.pl/api/data/synop/id/12560");

        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);//podazanie za przekierowaniami
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);//obsluga "zapisu" danych
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        response.clear();


        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);


        if (reader.parse(response, js))
        {
            //std::cout << js.toStyledString() << std::endl; // Wyświetl całość w postaci tekstu
            //std::cout << "a: " << js.get("a", 0).asInt() << std::endl; // Pole "a"
            std::cout << "cisnienie: " << js.get("cisnienie", "null") << std::endl;
            std::cout << "data_pomiaru: " << js.get("data_pomiaru", "null") << std::endl;
            std::cout << "godzina_pomiaru: " << js.get("godzina_pomiaru", "null") << std::endl;
            std::cout << "id_stacji: " << js.get("id_stacji", "null") << std::endl;
            std::cout << "kierunek_wiatru: " << js.get("kierunek_wiatru", "null") << std::endl;
            std::cout << "predkosc_wiatru: " << js.get("predkosc_wiatru", "null") << std::endl;
            std::cout << "suma_opadu: " << js.get("suma_opadu", "null") << std::endl;
            std::cout << "temperatura: " << js.get("temperatura", "null") << std::endl;
            std::cout << "stacja: " << js.get("stacja", "null") << std::endl;
            std::cout << "wilgotnosc_wzgledna: " << js.get("wilgotnosc_wzgledna", "null") << std::endl;
        }



    }
    return 0;
}