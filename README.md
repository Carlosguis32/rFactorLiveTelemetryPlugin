# rFactorLiveTelemetryPlugin

A plugin that sends through UDP every signal available from the simulator rFactor.

&nbsp;

## Usage

To use it, just copy the files **"TelemetryPlugin.dll"** and **"telemetryConfig.json"** into the folder **"Plugins"** inside the root folder of rFactor.

The project was build with Visual Studio.

In the file **"telemetryConfig.json"** you will found many parameters. The ones that can be modified are the **IP** (address at which the data will be sent), **port** (port at which the data will be sent) and **frequency** (maximum frequency to send the data) ones. The rest are purely informative and will have no effect on the code.

The **packet_3** will contain data of your driver, and the rest of the drivers in the race in case of multiple drivers in a session, so its size is variable. If 3 drivers are in the session, the packet size will be 428 \* 3 = 1284, and the first 428 bytes will correspond to your driver, the second 428 to the second driver, and so on.

The signals in the packets come on the same order as displayed. The structure is: **[size in bytes, type]**. The type **skip** means that those bytes are empty.

&nbsp;

# Technologies Used:

-   **Languages:**

    -   C++
