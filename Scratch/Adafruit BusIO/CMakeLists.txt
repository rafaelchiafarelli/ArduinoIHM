# Adafruit Bus IO Library
# https://github.com/adafruit/BusIO
# MIT License

cmake_minimum_required(VERSION 3.5)

idf_component_register(SRCS "I2CDevice.cpp" "BusIO_Register.cpp" "SPIDevice.cpp" "GenericDevice.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES arduino-esp32)

project(BusIO)
