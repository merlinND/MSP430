@REM This batch file has been generated by the IAR Embedded Workbench
@REM C-SPY Debugger, as an aid to preparing a command line for running
@REM the cspybat command line utility using the appropriate settings.
@REM
@REM You can launch cspybat by typing the name of this batch file followed
@REM by the name of the debug file (usually an ELF/DWARF or UBROF file).
@REM Note that this file is generated every time a new debug session
@REM is initialized, so you may want to move or rename the file before
@REM making changes.
@REM 


"C:\Program Files\IAR Systems\Embedded Workbench 6.0 Kickstart\common\bin\cspybat" "C:\Program Files\IAR Systems\Embedded Workbench 6.0 Kickstart\430\bin\430proc.dll" "C:\Program Files\IAR Systems\Embedded Workbench 6.0 Kickstart\430\bin\430fet.dll"  %1 --plugin "C:\Program Files\IAR Systems\Embedded Workbench 6.0 Kickstart\430\bin\430bat.dll" --backend -B "--hardware_multiplier" "16" "--hwmult_type" "1" "-d" "fet" "--erase_main_and_info" "--derivative" "Generic MSP430 device" "--protocol" "4wire" "--port" "Automatic" "--connection" "ti_usb" "--settlingtime=0" 

