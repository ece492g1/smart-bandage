# Communications Module Software

## Setup
In order to run the communications module you will need Code Composer Studio, TI RTOS, and Tools for CC26xx Simplelink MCU installed.

On the first run you will need to create the `TI_DIR` variable. To do this open CCS and go to Window->Preferences->Code Composer Studio->Build->Variables and create a variable with name `TI_DIR`. The type will be `Directory` and the value will be the root path of your TI tools installation directory. This is probably `C:\ti`. Create the variable and load the project.

Set your workspace to be the `software/comms_module` directory. Then select File->Import in CCS. The import source is `Code Composer Studio->CCS Projects` and the search0-directory is your workspace directory. In the discovered projects import both `SmartBandage` and `SmartBandageBLEStack`.