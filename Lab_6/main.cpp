#include <iostream>
#include <libusb-1.0/libusb.h>
#include <cstdio>

const int serialNumBuffer = 33;

void PrintHead(){
    std::cout << "===================================\n"
                 "|* класс устройства\n"
                 "| | * идентификатор производителя\n"
                 "| |    | * идентификатор устройства\n"
                 "| |    |   | * количество интерфейсов\n"
                 "| |    |   |    | * серийный номер\n"
                 "+-+----+---+----+------------------------+\n";

}
void printDevices(libusb_device *dev){
    libusb_device_descriptor descriptor{}; // дескриптор устройства
    libusb_device_handle* handle = nullptr; // хэндл устройства
    unsigned char serialNum[256]; // строка для хранения серийного номера

    int x = libusb_get_device_descriptor(dev, &descriptor); // получить дескриптор
    if (x < 0){
        fprintf(stderr, "Ошибка: дескриптор устройства не получен, код: %d.\n", x);
        return;
    }
    libusb_config_descriptor *config;
    libusb_get_config_descriptor(dev, 0, &config);
    printf("%.2x %.4x %.3d %.4x ",
           descriptor.bDeviceClass,
           descriptor.idVendor,
           config->bNumInterfaces,
           descriptor.idProduct); ///здесь добавить инфы

    libusb_open(dev, &handle);
    bool c = false;
    if (handle && descriptor.iSerialNumber){
        libusb_get_string_descriptor_ascii(handle, descriptor.iSerialNumber, serialNum, sizeof(serialNum));
        printf("%s", serialNum);
    }else{
        c = true;
        printf("null");
    }


    auto* product = new uint8_t[serialNumBuffer];

    try{
        if (libusb_open(dev, &handle) < 0){
            std::cout << "error: not open" << std::endl;
            handle = nullptr;
        }

        std::cout << "   ";

        if (handle != nullptr && libusb_get_string_descriptor_ascii(handle, descriptor.iProduct, product, serialNumBuffer - 2) >= 0){
            product[serialNumBuffer - 1] = '\0';
            if (c){
                printf("        ");
            }
            std::cout << product;
        }

        libusb_close(handle);

    }catch (libusb_error &error){
        std::cout << "error: " << error << std::endl;
        std::cerr << error << std::endl;
    }
    std::cout << "\n";
}

int main(){
    libusb_device **devices; // указатель на указатель на устройство, используется для получения списка устройств
    libusb_context *context = nullptr; // контекст сессии libusb
    int x; // для возвращаемых значений
    size_t number; // число найденных USB-устройств

    x = libusb_init(&context); // инициализировать библиотеку libusb, открыть сессию работы с libusb
    if (x < 0){
        fprintf(stderr, "Ошибка: инициализация не выполнена, код: %d.\n", x);
        return 1;
    }

    number = libusb_get_device_list(context, &devices);
    if (number == 0){
        fprintf(stderr,"Ошибка: список USB устройств не получен. Код: %d\n", x);
        return 1;
    }

    PrintHead();

    for (size_t i = 0; i < number; i++){ // цикл перебора всех устройств
        printDevices(devices[i]); // печать параметров устройства
    }
    printf("===================================\n");

    // освободить память, выделенную функцией получения списка устройств
    libusb_free_device_list(devices, 1);

    // завершить работу с библиотекой libusb, закрыть сессию работы с libusb
    libusb_exit(context);
    return 0;
}
