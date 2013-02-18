#include <iostream>
#include <libusb-1.0/libusb.h>
#include <cstring>

int main() {
    std::cout << "libusb-1.0" << std::endl;

    // Structure representing a libusb session
    libusb_context * libusbSession;

    // Initialize libusb, this function must be called before calling any other libusb function.
    if ( 0 != libusb_init(&libusbSession) ) {
        std::cout << "libusb_init failed!" << std::endl;
        return 1;
    }

    // Set message verbosity to level 3:
    // informational messages are printed to stdout, warning and error messages are printed to stderr
    libusb_set_debug(libusbSession, 3);

    // Generate a list of USB devices currently attached to the system.
    libusb_device ** listOfDevices;
    size_t numberOfDevices = libusb_get_device_list(libusbSession, &listOfDevices);
    if ( numberOfDevices < 0 ) {
        std::cout << "numberOfDevices < 0" << std::endl;
        return 1;
    }

    // Iterate over the detected devices
    for ( int i = 0; i < numberOfDevices; i++ )
    {
        libusb_device * dev = listOfDevices[i];
        std::cout << "Device " << (i+1) << "/" << numberOfDevices << ":" << std::endl;

        // Get the number of the bus that a device is connected to.
        std::cout << "\tlibusb_get_bus_number(dev): " << (int)libusb_get_bus_number(dev) << std::endl;

        // Get the address of the device on the bus it is connected to.
        std::cout << "\tlibusb_get_device_address(dev): " << (int)libusb_get_device_address(dev) << std::endl;

        // Get the USB device descriptor for a given device. 
        libusb_device_descriptor desc;
        if ( 0 != libusb_get_device_descriptor(dev, &desc) ) {
            std::cout << "libusb_get_device_descriptor(dev, desc) failed!" << std::endl;
            break;
        }

        if ( 0x03eb != desc.idVendor || 0x2016 != desc.idProduct )
        {
            continue;
        }

        std::cout << "\tdevice descriptor:" << std::endl;
        // Size of this descriptor (in bytes)
        std::cout << "\t\tdesc.bLength = " << int(desc.bLength) << std::endl;
        // Descriptor type.
        std::cout << "\t\tdesc.bDescriptorType = " << int(desc.bDescriptorType) << std::endl;
        // USB specification release number in binary-coded decimal.
        std::cout << "\t\tdesc.bcdUSB = " << int(desc.bcdUSB) << std::endl;
        // USB-IF class code for the device.
        std::cout << "\t\tdesc.bDeviceClass = " << int(desc.bDeviceClass) << std::endl;
        // USB-IF subclass code for the device, qualified by the bDeviceClass value.
        std::cout << "\t\tdesc.bDeviceSubClass = " << int(desc.bDeviceSubClass) << std::endl;
        // USB-IF protocol code for the device, qualified by the bDeviceClass and bDeviceSubClass values.
        std::cout << "\t\tdesc.bDeviceProtocol = " << int(desc.bDeviceProtocol) << std::endl;
        // Maximum packet size for endpoint 0.
        std::cout << "\t\tdesc.bMaxPacketSize0 = " << int(desc.bMaxPacketSize0) << std::endl;
        // USB-IF vendor ID.
        std::cout << "\t\tdesc.idVendor = " << int(desc.idVendor) << std::endl;
        // USB-IF product ID.
        std::cout << "\t\tdesc.idProduct = " << int(desc.idProduct) << std::endl;
        // Device release number in binary-coded decimal.
        std::cout << "\t\tdesc.bcdDevice = " << int(desc.bcdDevice) << std::endl;
        // Index of string descriptor describing manufacturer.
        std::cout << "\t\tdesc.iManufacturer = " << int(desc.iManufacturer) << std::endl;
        // Index of string descriptor describing product.
        std::cout << "\t\tdesc.iProduct = " << int(desc.iProduct) << std::endl;
        // Index of string descriptor containing device serial number.
        std::cout << "\t\tdesc.iSerialNumber = " << int(desc.iSerialNumber) << std::endl;
        // Number of possible configurations.
        std::cout << "\t\tdesc.bNumConfigurations = " << int(desc.bNumConfigurations) << std::endl;

        // Get the USB configuration descriptor for the currently active configuration.
        libusb_config_descriptor * config;
        if ( 0 != libusb_get_active_config_descriptor(dev, &config) ) {
            std::cout << "libusb_get_active_config_descriptor(dev, &config) failed!" << std::endl;
            break;
        }
        std::cout << "\tdevice active configuration:" << std::endl;
        // Size of this descriptor (in bytes)
        std::cout << "\t\tconfig.bLength = " << int(config->bLength) << std::endl;
        // Descriptor type.
        std::cout << "\t\tconfig.bDescriptorType = " << int(config->bDescriptorType) << std::endl;
        // Total length of data returned for this configuration.
        std::cout << "\t\tconfig.wTotalLength = " << int(config->wTotalLength) << std::endl;
        // Number of interfaces supported by this configuration.
        std::cout << "\t\tconfig.bNumInterfaces = " << int(config->bNumInterfaces) << std::endl;
        // Identifier value for this configuration.
        std::cout << "\t\tconfig.bConfigurationValue = " << int(config->bConfigurationValue) << std::endl;
        // Index of string descriptor describing this configuration.
        std::cout << "\t\tconfig.iConfiguration = " << int(config->iConfiguration) << std::endl;
        // Configuration characteristics.
        std::cout << "\t\tconfig.bmAttributes = " << int(config->bmAttributes) << std::endl;
        // Maximum power consumption of the USB device from this bus in this configuration when the device is fully opreation.
        std::cout << "\t\tconfig.MaxPower = " << int(config->MaxPower) << std::endl;
        // Array of interfaces supported by this configuration.
        std::cout << "\t\tArray of interfaces supported by this configuration:" << std::endl;
        std::cout << "\t\t\tconfig->interface->num_altsetting = " << config->interface->num_altsetting << std::endl;
        // Extra descriptors.
        std::cout << "\t\tconfig.extra = " << "(value hidden)"/*config->extra*/ << std::endl;
        // Length of the extra descriptors, in bytes.
        std::cout << "\t\tconfig.extra_length = " << int(config->extra_length) << std::endl;

        std::cout << "\t\tinterface descriptors:" << std::endl;
        for ( int intIndex = 0; intIndex < config->bNumInterfaces; intIndex++ )
        {
            std::cout << "\t\t\tInterface #" << intIndex << ":" << std::endl;
            for ( int cfgIndex = 0; cfgIndex < config->interface[intIndex].num_altsetting; cfgIndex++ )
            {
                std::cout << "\t\t\t\tAlt. cfg. #" << cfgIndex << ":" << std::endl;
                const libusb_interface_descriptor & interface_descriptor = config->interface[intIndex].altsetting[cfgIndex];

                // Size of this descriptor (in bytes)
                std::cout << "\t\t\t\t\tbLength = " << (int)interface_descriptor.bLength << std::endl;
                // Descriptor type.
                std::cout << "\t\t\t\t\tbDescriptorType = " << (int)interface_descriptor.bDescriptorType << std::endl;
                // Number of this interface.
                std::cout << "\t\t\t\t\tbInterfaceNumber = " << (int)interface_descriptor.bInterfaceNumber << std::endl;
                // Value used to select this alternate setting for this interface.
                std::cout << "\t\t\t\t\tbAlternateSetting = " << (int)interface_descriptor.bAlternateSetting << std::endl;
                // Number of endpoints used by this interface (excluding the control endpoint).
                std::cout << "\t\t\t\t\tbNumEndpoints = " << (int)interface_descriptor.bNumEndpoints << std::endl;
                // USB-IF class code for this interface.
                std::cout << "\t\t\t\t\tbInterfaceClass = " << (int)interface_descriptor.bInterfaceClass << std::endl;
                // USB-IF subclass code for this interface, qualified by the bInterfaceClass value.
                std::cout << "\t\t\t\t\tbInterfaceSubClass = " << (int)interface_descriptor.bInterfaceSubClass << std::endl;
                // USB-IF protocol code for this interface, qualified by the bInterfaceClass and bInterfaceSubClass values.
                std::cout << "\t\t\t\t\tbInterfaceProtocol = " << (int)interface_descriptor.bInterfaceProtocol << std::endl;
                // Index of string descriptor describing this interface.
                std::cout << "\t\t\t\t\tiInterface = " << (int)interface_descriptor.iInterface << std::endl;

                for ( int epIndex = 0; epIndex < interface_descriptor.bNumEndpoints; epIndex++ )
                {
                    std::cout << "\t\t\t\t\tEnpoint #" << epIndex << ":" << std::endl;
                    // Size of this descriptor (in bytes)
                    std::cout << "\t\t\t\t\t\tbLength = " << (int)interface_descriptor.endpoint[epIndex].bLength << std::endl;
                    // Descriptor type.
                    std::cout << "\t\t\t\t\t\tbDescriptorType = " << (int)interface_descriptor.endpoint[epIndex].bDescriptorType << std::endl;
                    // The address of the endpoint described by this descriptor.
                    std::cout << "\t\t\t\t\t\tbEndpointAddress = "
                              << (int)interface_descriptor.endpoint[epIndex].bEndpointAddress
                              << " (direction: " << ((interface_descriptor.endpoint[epIndex].bEndpointAddress & LIBUSB_ENDPOINT_IN) ? "IN" : "OUT") << ")" << std::endl;
                    // Attributes which apply to the endpoint when it is configured using the bConfigurationValue.
                    std::cout << "\t\t\t\t\t\tbmAttributes = " << (int)interface_descriptor.endpoint[epIndex].bmAttributes << std::endl;
                    // Maximum packet size this endpoint is capable of sending/receiving.
                    std::cout << "\t\t\t\t\t\twMaxPacketSize = " << (int)interface_descriptor.endpoint[epIndex].wMaxPacketSize << std::endl;
                    // Interval for polling endpoint for data transfers.
                    std::cout << "\t\t\t\t\t\tbbInterval = " << (int)interface_descriptor.endpoint[epIndex].bInterval << std::endl;
                    // For audio devices only: the rate at which synchronization feedback is provided.
                    std::cout << "\t\t\t\t\t\tbRefresh = " << (int)interface_descriptor.endpoint[epIndex].bRefresh << std::endl;
                    // For audio devices only: the address if the synch endpoint. 
                    std::cout << "\t\t\t\t\t\tbSynchAddress = " << (int)interface_descriptor.endpoint[epIndex].bSynchAddress << std::endl;
                }
            }
        }

        // Free the configuration descriptor obtained from libusb_get_active_config_descriptor() or libusb_get_config_descriptor().
        libusb_free_config_descriptor(config);

        libusb_ref_device(dev);

        // Open the device and obtain a device handle.
        std::cout << "\tOpening the device:" << std::endl;

        int length = 60;
        unsigned char * data = new unsigned char [ length ];
        memcpy(data, "0123456789001234567890123456789012345678901234567890123456789", length);
        int transferred = -1;
        const int interface = 0;
        const unsigned char endpoint = 2;

        libusb_device_handle * handle;
        switch ( libusb_open(dev, &handle) )
        {
            case 0: // success
            {
//                 std::cout << "\t\tlibusb_reset_device(handle)." << std::endl;
//                 libusb_reset_device(handle);
                std::cout << "\t\tSuccess." << std::endl;
                static const int strLen = 256;
                unsigned char string[strLen];

                std::cout << "\t\tstring descriptors:" << std::endl;
                if ( 0 < libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, string, strLen) ) {
                    std::cout << "\t\t\t1/4 [iManufacturer] = " << string << std::endl;
                }
                if ( 0 < libusb_get_string_descriptor_ascii(handle, desc.iProduct, string, strLen) ) {
                    std::cout << "\t\t\t2/4 [iProduct] = " << string << std::endl;
                }
                if ( 0 < libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, string, strLen) ) {
                    std::cout << "\t\t\t3/4 [iSerialNumber] = " << string << std::endl;
                }
                if ( 0 < libusb_get_string_descriptor_ascii(handle, config->iConfiguration, string, strLen) ) {
                    std::cout << "\t\t\t4/4 [iConfiguration] = " << string << std::endl;
                }

                std::cout << "\t\tIs a kernel driver active?" << std::endl;
                switch ( libusb_kernel_driver_active(handle, interface) )
                {
                    case 0: // if no kernel driver is active
                        std::cout << "\t\tINFO: no kernel driver is active" << std::endl;
                        break;
                    case 1: // if a kernel driver is active
                        std::cout << "\t\tINFO: a kernel driver is active --> DETACHING." << std::endl;
                        switch ( libusb_detach_kernel_driver(handle, interface) )
                        {
                            case 0: // on success
                                std::cout << "\t\t\tDetachment ok." << std::endl;
                                break;
                            case LIBUSB_ERROR_NOT_FOUND: // if no kernel driver was active
                                std::cout << "\t\tFAILURE: if no kernel driver was active." << std::endl;
                                break;
                            case LIBUSB_ERROR_INVALID_PARAM: // if the interface does not exist
                                std::cout << "\t\tFAILURE: if the interface does not exist." << std::endl;
                                break;
                            case LIBUSB_ERROR_NO_DEVICE: // if the device has been disconnected
                                std::cout << "\t\tFAILURE: if the device has been disconnected." << std::endl;
                                break;
                            case LIBUSB_ERROR_NOT_SUPPORTED: // on platforms where the functionality is not available
                                std::cout << "\t\tFAILURE: on platforms where the functionality is not available." << std::endl;
                                break;
                            default: // other failure
                                std::cout << "\t\tFAILURE: other failure." << std::endl;
                                break;
                        }
                        break;
                    case LIBUSB_ERROR_NO_DEVICE: // if the device has been disconnected
                        std::cout << "\t\tINFO: the device has been disconnected." << std::endl;
                        break;
                    case LIBUSB_ERROR_NOT_SUPPORTED: // on platforms where the functionality is not available
                        std::cout << "\t\tINFO: on platforms where the functionality is not available." << std::endl;
                        break;
                    default: // other failure
                        std::cout << "\t\tFAILURE: other failure." << std::endl;
                        break;
                }

                switch ( libusb_claim_interface(handle, interface) )
                {
                    case 0: // success
                    {
                        int maxPacketSize = libusb_get_max_packet_size(dev, endpoint);
                        switch ( maxPacketSize )
                        {
                            case LIBUSB_ERROR_NOT_FOUND: // if the endpoint does not exist
                                std::cout << "\t\tFAILURE: the endpoint does not exist." << std::endl;
                                break;
                            case LIBUSB_ERROR_OTHER: // on other failure
                                std::cout << "\t\tFAILURE: other failure." << std::endl;
                                break;
                            default:
                                std::cout << "\t\t\tmaxPacketSize = " << maxPacketSize << std::endl;
                        }
                        switch ( libusb_interrupt_transfer (handle, endpoint, data, length, &transferred, 100) )
                        {
                            case 0: // on success (and populates transferred)
                                std::cout << ">>> SUCCESS <<<" << std::endl;
                                break;
                            case LIBUSB_ERROR_TIMEOUT: // the transfer timed out
                                std::cout << "\t\tFAILURE: transfer timed out." << std::endl;
                                break;
                            case LIBUSB_ERROR_PIPE: // if the endpoint halted
                                std::cout << "\t\tFAILURE: the endpoint halted." << std::endl;
                                break;
                            case LIBUSB_ERROR_OVERFLOW: // if the device offered more data, see Packets and overflows
                                std::cout << "\t\tFAILURE: the device offered more data, see Packets and overflows." << std::endl;
                                break;
                            case LIBUSB_ERROR_NO_DEVICE: // if the device has been disconnected
                                std::cout << "\t\tFAILURE: the device has been disconnected." << std::endl;
                                break;
                            case LIBUSB_ERROR_IO: // code on other error
                                std::cout << "\t\tFAILURE: code on other error." << std::endl;
                                break;
                            default: // other failure
                                std::cout << "\t\tFAILURE: other failure." << std::endl;
                                break;
                        }
                        switch ( libusb_interrupt_transfer (handle, 129, data, length, &transferred, 100) )
                        {
                            case 0: // on success (and populates transferred)
                                std::cout << ">>> SUCCESS <<<" << std::endl;
                                break;
                            case LIBUSB_ERROR_TIMEOUT: // the transfer timed out
                                std::cout << "\t\tFAILURE: transfer timed out." << std::endl;
                                break;
                            case LIBUSB_ERROR_PIPE: // if the endpoint halted
                                std::cout << "\t\tFAILURE: the endpoint halted." << std::endl;
                                break;
                            case LIBUSB_ERROR_OVERFLOW: // if the device offered more data, see Packets and overflows
                                std::cout << "\t\tFAILURE: the device offered more data, see Packets and overflows." << std::endl;
                                break;
                            case LIBUSB_ERROR_NO_DEVICE: // if the device has been disconnected
                                std::cout << "\t\tFAILURE: the device has been disconnected." << std::endl;
                                break;
                            case LIBUSB_ERROR_IO: // code on other error
                                std::cout << "\t\tFAILURE: code on other error." << std::endl;
                                break;
                            default: // other failure
                                std::cout << "\t\tFAILURE: other failure." << std::endl;
                                break;
                        }

                        std::cout << "\t\tlibusb_release_interface(handle, interface)" << std::endl;
                        libusb_release_interface(handle, interface);
                        break;
                    }
                    case LIBUSB_ERROR_NOT_FOUND: // the requested interface does not exist
                        std::cout << "\t\tFAILURE: the requested interface does not exist." << std::endl;
                        break;
                    case LIBUSB_ERROR_BUSY: // another program or driver has claimed the interface
                        std::cout << "\t\tFAILURE: another program or driver has claimed the interface." << std::endl;
                        break;
                    case LIBUSB_ERROR_NO_DEVICE: // the device has been disconnected
                        std::cout << "\t\tFAILURE: the device has been disconnected." << std::endl;
                        break;
                    case LIBUSB_ERROR_IO: // code on other failure
                        std::cout << "\t\tFAILURE: code on other failure." << std::endl;
                        break;
                    default: // other failure
                        std::cout << "\t\tFAILURE: other failure." << std::endl;
                        break;
                }

                // Close the device handle.
                libusb_close(handle);
                break;
            }
            case LIBUSB_ERROR_NO_MEM: // memory allocation failure
                std::cout << "\t\tFAILURE: memory allocation failure." << std::endl;
                break;
            case LIBUSB_ERROR_ACCESS: // the user has insufficient permissions
                std::cout << "\t\tFAILURE: the user has insufficient permissions." << std::endl;
                break;
            case LIBUSB_ERROR_NO_DEVICE: // the device has been disconnected
                std::cout << "\t\tFAILURE: the device has been disconnected." << std::endl;
                break;
            default: // other failure
                std::cout << "\t\tFAILURE: other failure." << std::endl;
                break;
        }

        std::cout << "\tlibusb_unref_device(dev)" << std::endl;
        libusb_unref_device(dev);
    }

    // Frees a list of devices previously discovered using libusb_get_device_list().
    libusb_free_device_list(listOfDevices, 1);

    // Deinitialize libusb.
    libusb_exit(libusbSession);
    return 0;
}
