set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_CROSS_COMPILING 1)

set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_AR         avr-ar)
set(CMAKE_OBJCOPY    avr-objcopy)
set(CMAKE_SIZE       avr-size)

set(F_CPU 16000000UL)
set(MCU atmega328p)

set(CMAKE_C_FLAGS_INIT
	"-mmcu=${MCU} -DF_CPU=${F_CPU} -Os -Wall -Wextra"
)
