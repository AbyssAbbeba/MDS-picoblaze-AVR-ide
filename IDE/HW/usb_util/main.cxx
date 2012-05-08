#include <iostream>
#include <libusb-1.0/libusb.h>

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
	for ( int i = 0; i < numberOfDevices; i++ ) {
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
			return 1;
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
			return 1;
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

		// Free the configuration descriptor obtained from libusb_get_active_config_descriptor() or libusb_get_config_descriptor().
		libusb_free_config_descriptor(config);

// 		libusb_ref_device(dev);

		// Open the device and obtain a device handle.
		std::cout << "\tOpening the device:" << std::endl;
		libusb_device_handle * handle;
		switch ( libusb_open(dev, &handle) ) {
			case 0: // success
				std::cout << "\t\tSuccess." << std::endl;
				static const int strLen = 256;
				unsigned char string[strLen];

// 				libusb_claim_interface(handle, 0);
   
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

// 				libusb_release_interface(handle, 0);

				// Close the device handle.
				libusb_close(handle);
				break;
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

// 		libusb_unref_device(dev);
	}

	// Frees a list of devices previously discovered using libusb_get_device_list().
	libusb_free_device_list(listOfDevices, 1);

	// Deinitialize libusb.
	libusb_exit(libusbSession);
	return 0;
}
