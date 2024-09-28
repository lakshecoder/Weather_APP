# Weather Forecast Application

A C++ application that fetches weather data from the OpenWeatherMap API and presents it in a user-friendly format. This project utilizes the `nlohmann/json` library for JSON parsing.

## Features

- Fetches current weather data based on a city name.
- Displays temperature, humidity, wind speed, and weather conditions.

## API Used

The application uses the **OpenWeatherMap API** to retrieve weather data. To use this application, you will need to obtain an API key.

### How to Obtain an API Key

1. Go to [OpenWeatherMap](https://openweathermap.org/).
2. Sign up for a free account.
3. After logging in, navigate to the API section and subscribe to the "Current Weather Data" service.
4. Once subscribed, you will receive an API key that you can use in the application.

## Getting Started

### Prerequisites

- A C++ compiler (e.g., g++, MSVC)
- [nlohmann/json](https://github.com/nlohmann/json) library (included in the project)
- Visual Studio (optional)

### Cloning the Project

You can clone the project using the following command:

```bash
git clone https://github.com/yourusername/weather-forecast.git
```
## Open in Visual Studio

After cloning the project, you can open it in Visual Studio using the provided solution file (`WeatherForecast.sln`).

## Compiling the Project

To compile the project, follow these steps:

1. Open the solution file in Visual Studio.
2. Build the solution (usually `Build > Build Solution` or press `Ctrl+Shift+B`).
3. Ensure there are no build errors.

## Building and Running the Application via Command Line

### Building the Project

To build the project using the command line, follow these steps:

1. **Open Command Prompt or Terminal**:
   - On Windows, press `Win + R`, type `cmd`, and press `Enter`.
   - On macOS or Linux, open the Terminal application.

2. **Navigate to the Project Directory**:
   Use the `cd` command to change the directory to where your project files are located. For example:
   ```bash
   cd path\to\your\project
	```
3. **Compile the Source Code**: 
   - Assuming you have a compatible C++ compiler installed (e.g., g++ for GCC), compile the source code using the following command:
   ```bash
   g++ -o WeatherForecast main.cpp -I/path/to/nlohmann/json
	```
## Running the Application

1. After building the project, navigate to the output directory (e.g., `Debug` or `Release` folder).
2. Run the executable file.
3. Follow the prompts to enter the city name for which you want to fetch the weather data.

## Output

The application will display the following information:

- **Temperature**: Current temperature in Celsius.
- **Feels Like**: How the temperature feels.
- **Humidity**: Humidity percentage.
- **Weather Description**: Description of the weather conditions.
- **Wind Speed**: Speed of the wind.
- **Visibility**: Visibility distance.
