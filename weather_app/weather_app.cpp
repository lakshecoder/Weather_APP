#include <iostream>
#include <windows.h>
#include <winhttp.h>
#include <string>
#include "..\weather_app\nlohmann\json.hpp"
#include <chrono>
#include <thread>

#pragma comment(lib, "winhttp.lib")

using json = nlohmann::json;

// Your OpenWeatherMap API key (replace with your actual API key)
const std::wstring API_KEY = L"2c6933958c0d9b404ecac4792d475f93";

// Helper function to convert std::string to std::wstring
std::wstring stringToWstring(const std::string& str) {
    return std::wstring(str.begin(), str.end());
}

// Helper function to convert std::wstring to std::string
std::string wstringToString(const std::wstring& wstr) {
    return std::string(wstr.begin(), wstr.end());
}

// Function to fetch data using WinHTTP
std::string fetchData(const std::wstring& api_path) {
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    BOOL  bResults = FALSE;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    std::string responseData = "";

    // Open an HTTP session
    hSession = WinHttpOpen(L"WeatherApp/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    // Specify the server and API endpoint
    if (hSession) {
        hConnect = WinHttpConnect(hSession, L"api.openweathermap.org", INTERNET_DEFAULT_HTTP_PORT, 0);
    }

    // Create an HTTP request
    if (hConnect) {
        hRequest = WinHttpOpenRequest(hConnect, L"GET", api_path.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    }

    // Send the request
    if (hRequest) {
        bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
    }

    // Receive the response
    if (bResults) {
        bResults = WinHttpReceiveResponse(hRequest, NULL);
    }

    // Read the data
    if (bResults) {
        do {
            dwSize = 0;
            if (WinHttpQueryDataAvailable(hRequest, &dwSize)) {
                pszOutBuffer = new char[dwSize + 1];
                ZeroMemory(pszOutBuffer, dwSize + 1);

                if (WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                    responseData.append(pszOutBuffer, dwDownloaded);
                }
                delete[] pszOutBuffer;
            }
        } while (dwSize > 0);
    }

    // Clean up handles
    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);

    return responseData;
}

// Function to get the city's coordinates (latitude and longitude)
std::pair<double, double> getCityCoordinates(const std::wstring& city) {
    std::wstring api_path = L"/data/2.5/weather?q=" + city + L"&appid=" + API_KEY;
    std::string responseData = fetchData(api_path);

    try {
        auto jsonData = json::parse(responseData);
        double lat = jsonData["coord"]["lat"];
        double lon = jsonData["coord"]["lon"];
        return { lat, lon };
    }
    catch (json::parse_error& e) {
        std::cerr << "JSON Parse Error: " << e.what() << std::endl;
        return { 0.0, 0.0 };  // Invalid coordinates
    }
}

// Function to display the 7-day weather forecast
void display7DayForecast(double lat, double lon) {
    std::wstring api_path = L"/data/2.5/forecast?lat=" + std::to_wstring(lat) + L"&lon=" + std::to_wstring(lon) + L"&exclude=current,minutely,hourly,alerts&appid=" + API_KEY + L"&units=metric";
   // std::wcout << "----------------------------------------"<<api_path << std::endl;
    std::string responseData = fetchData(api_path);

   
    try {
        auto jsonData = json::parse(responseData);
        std::cout << "Weather Forecast Data:\n";
        for (const auto& entry : jsonData["list"]) {
            long dt = entry["dt"];
            double temp = entry["main"]["temp"];
            double feels_like = entry["main"]["feels_like"];
            double temp_min = entry["main"]["temp_min"];
            double temp_max = entry["main"]["temp_max"];
            int pressure = entry["main"]["pressure"];
            int humidity = entry["main"]["humidity"];
            std::string weather_main = entry["weather"][0]["main"];
            std::string weather_description = entry["weather"][0]["description"];
            double wind_speed = entry["wind"]["speed"];
            int clouds_all = entry["clouds"]["all"];
            std::string dt_txt = entry["dt_txt"];

            // Display extracted fields
                std::cout << "Temperature: " << temp << " °C\n"
                << "Feels Like: " << feels_like << " °C\n"
                << "Min Temperature: " << temp_min << " °C\n"
                << "Max Temperature: " << temp_max << " °C\n"
                << "Pressure: " << pressure << " hPa\n"
                << "Humidity: " << humidity << " %\n"
                << "Weather: " << weather_main << " (" << weather_description << ")\n"
                << "Wind Speed: " << wind_speed << " m/s\n"
                << "Cloud Cover: " << clouds_all << " %\n"
                << "Date and Time (Formatted): " << dt_txt << "\n\n";;
        }
    }
    catch (json::parse_error& e) {
        std::cerr << "JSON Parse Error: " << e.what() << std::endl;
    }
}

// Function to display current weather
void displayCurrentWeather(double lat, double lon) {
    std::wstring api_path = L"/data/2.5/weather?lat=" + std::to_wstring(lat) + L"&lon=" + std::to_wstring(lon) + L"&appid=" + API_KEY + L"&units=metric";
    std::string weatherData = fetchData(api_path);

    try {
        auto jsonData = json::parse(weatherData);

        std::string cityName = jsonData["name"];
        float temperature = jsonData["main"]["temp"];
        int humidity = jsonData["main"]["humidity"];
        std::string description = jsonData["weather"][0]["description"];

        std::cout << "Weather in " << cityName << ":\n";
        std::cout << "Temperature: " << temperature << "°C\n";
        std::cout << "Humidity: " << humidity << "%\n";
        std::cout << "Description: " << description << "\n";
        std::cout << "----------------------------------------" << std::endl;

    }
    catch (json::parse_error& e) {
        std::cerr << "JSON Parse Error: " << e.what() << std::endl;
    }
}

int main() {
    std::string city;

    std::cout << "Enter the city name: ";
    std::getline(std::cin, city);

    std::wstring wcity = stringToWstring(city);

    // Get city's coordinates
    std::pair<double, double> cityCoordinates = getCityCoordinates(wcity);
    double lat = cityCoordinates.first;
    double lon = cityCoordinates.second;

    // Display 7-day weather forecast
    if (lat != 0.0 && lon != 0.0) {
        display7DayForecast(lat, lon);

        // Start refreshing current weather every 10 seconds
        while (true) {
            displayCurrentWeather(lat, lon);

            // Wait for 10 seconds before refreshing
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }
    else {
        std::cerr << "Error: Unable to fetch weather data.\n";
    }

    return 0;
}
