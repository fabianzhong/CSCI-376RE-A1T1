#define CL_USE_DEPRECATED_OPENCL_2_0_APIS // using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS // enable OpenCL exemptions

//C++ standard library  and STL headers
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

//OpenCL header, depending on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

// functions to handle errors
#include "error.h"

#define LENGTH 20

//Task 1 - Device Information
//Allow user to select CPU or GPU device
bool select_one_device(cl::Platform* platfm, cl::Device* dev) //function to allow user to choose device
{
	std::vector<cl::Platform> platforms;	// available platforms
	std::vector< std::vector<cl::Device> > platformDevices;	// devices available for each platform
	std::string outputString;				// string for output
	unsigned int i, j;								// counters

	try {
		// get the number of available OpenCL platforms
		cl::Platform::get(&platforms);

		std::cout << "Number of OpenCL platforms: " << platforms.size() << std::endl;

		// find and store the devices available to each platform
		//Search the system for all CPU or GPU devices for all available platform
		for (i = 0; i < platforms.size(); i++)
		{
			std::vector<cl::Device> devices;		// available devices

			// get all devices available to the platform
			platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &devices);

			// store available devices for the platform
			platformDevices.push_back(devices);
		}

		// display available platforms and devices
		std::cout << "--------------------" << std::endl;
		std::cout << "Available options:" << std::endl;

		// store options as platform and device indices
		std::vector< std::pair<int, int> > options;
		unsigned int optionCounter = 0;	// option counter

		// for all platforms
		for (i = 0; i < platforms.size(); i++)
		{
			// for all devices per platform
			for (j = 0; j < platformDevices[i].size(); j++)
			{
				// display options
				std::cout << "Option " << optionCounter << ": Platform - ";

				// platform vendor name
				outputString = platforms[i].getInfo<CL_PLATFORM_VENDOR>();
				std::cout << outputString << ", Device - ";

				// device name
				outputString = platformDevices[i][j].getInfo<CL_DEVICE_NAME>();
				std::cout << outputString << ", Type - ";

				//device type
				if (platformDevices[i][j].getInfo<CL_DEVICE_TYPE>() == CL_DEVICE_TYPE_CPU) {
					outputString = "CPU";
				}
				else if (platformDevices[i][j].getInfo<CL_DEVICE_TYPE>() == CL_DEVICE_TYPE_GPU) {
					outputString = "GPU";
				}
				std::cout << outputString << std::endl;

				// store option
				options.push_back(std::make_pair(i, j));
				optionCounter++; // increment option counter
			}
		}

		std::cout << "\n--------------------" << std::endl;
		std::cout << "Select a device: ";

		std::string inputString;
		unsigned int selectedOption;	// option that was selected

		std::getline(std::cin, inputString);
		std::istringstream stringStream(inputString);

		// check whether valid option was selected
		// check if input was an integer
		if (stringStream >> selectedOption)
		{
			char c;

			// check if there was anything after the integer
			if (!(stringStream >> c))
			{
				// check if valid option range
				if (selectedOption >= 0 && selectedOption < optionCounter)
				{
					// return the platform and device
					int platformNumber = options[selectedOption].first;
					int deviceNumber = options[selectedOption].second;

					*platfm = platforms[platformNumber];
					*dev = platformDevices[platformNumber][deviceNumber];

					return true;
				}
			}
		}
		// if invalid option selected
		std::cout << "\n--------------------" << std::endl;
		std::cout << "Invalid option." << std::endl;
	}
	// catch any OpenCL function errors
	catch (cl::Error e) {
		// call function to handle errors
		handle_error(e);
	}

	return false;
}

void displayDeviceInfo(cl::Platform plat, cl::Device dev) {
	//Based on Choice Display
	//Display Platform Information
	std::cout << "--------------------" << std::endl;
	std::cout << "Platform Information" << std::endl;
	std::cout << "--------------------" << std::endl;

	//Platform that support chosen device
	// platform vendor name
	std::string outputString;
	outputString = plat.getInfo<CL_PLATFORM_VENDOR>();
	std::cout << "Platform Vendor - ";
	std::cout << outputString << std::endl;

	//Display Platform Name
	outputString = plat.getInfo<CL_PLATFORM_NAME>();
	std::cout << "Platform Name - ";
	std::cout << outputString << std::endl;

	std::cout << std::endl;

	//Display Device Information
	std::cout << "--------------------" << std::endl;
	std::cout << "Device Information" << std::endl;
	std::cout << "--------------------" << std::endl;
	//Display Device Name (CL_DEVICE_NAME)
	std::cout << "Device Name - ";
	std::cout << dev.getInfo<CL_DEVICE_NAME>() << std::endl;

	//Display Device Type (CL_DEVICE_TYPE – either CL_DEVICE_TYPE _CPU or CL_DEVICE_TYPE_GPU)
	std::cout << "Device Type - ";

	cl_device_type type;
	type = dev.getInfo<CL_DEVICE_TYPE>();
	if (type == CL_DEVICE_TYPE_CPU) {
		std::cout << "CPU";
	}
	else if (type == CL_DEVICE_TYPE_GPU) {
		std::cout << "GPU";
	}

	std::cout << std::endl;

	//Number of Compute Units (CL_DEVICE_MAX_COMPUTE_UNITS)
	unsigned outputInt;
	outputInt = dev.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
	std::cout << "Max Compute Units - ";
	std::cout << outputInt << std::endl;

	//Maximum work group size (CL_DEVICE_MAX_WORK_GROUP_SIZE)
	size_t outputSizeT;
	outputSizeT = dev.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
	std::cout << "Max Work Group Size - ";
	std::cout << outputSizeT << std::endl;

	//Max work item dimensions(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS)
	outputInt = dev.getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>();
	std::cout << "Max Work Item Dimensions - ";
	std::cout << outputInt << std::endl;

	//Max work item sizes (CL_DEVICE_MAX_WORK_ITEM_SIZES)
	std::vector<size_t> outputVecSizeT;
	outputVecSizeT = dev.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
	std::cout << "Max Work Item Sizes" << std::endl;
	for (int i = 0; i < outputVecSizeT.size(); i++) {
		std::cout << outputVecSizeT[i] << " ";
	}
	std::cout << std::endl;

	//Global memory size (CL_DEVICE_GLOBAL_MEM_SIZE)
	unsigned long  outputLong;
	outputLong = dev.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
	std::cout << "Global Memory Size - ";
	std::cout << outputLong << std::endl;

	//Local memory size (CL_DEVICE_LOCAL_MEM_SIZE)	
	outputLong = dev.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();
	std::cout << "Local Memory Size - ";
	std::cout << outputLong << std::endl;
}

int main(void) {
	cl::Platform platform;		//device's platform
	cl::Device device;			//device used
	cl::Context context;		//context for the device
	std::string outputString;	//string for output
	unsigned int i;				//counter
	cl::Kernel kernel;
	cl::CommandQueue queue;

	//Input Datas
	std::vector <cl_float> floatDataA(LENGTH);
	std::vector <cl_float> floatDataB(LENGTH);
	std::vector <cl_float> floatDataC(LENGTH);

	for (int i = 0; i < LENGTH; i++) {
		floatDataA[i] = i + 1.0f;			// set values from 1.0 to LENGTH.0
		floatDataB[i] = -1.0f * (i + 1);	// set values from -1.0 to -LENGTH.0
		floatDataC[i] = i * 1.0f;			// set values from 0 to LENGTH-1
	}

	//Output
	std::vector <cl_float> floatOutput(LENGTH);

	//buffers
	cl::Buffer floatBufferA;
	cl::Buffer floatBufferB;
	cl::Buffer floatBufferC;

	try {
		//call function for user to select OpenCL device
		if (!select_one_device(&platform, &device)) {	//function will return false if no device selected
			quit_program("Device not selected");		//quit program if no device selected
		}

		displayDeviceInfo(platform, device);

		//Based on selected device, Create
		//Context
		context = cl::Context(device); //create context from chosen device

		std::cout << "\nDevice in the context:" << std::endl;

		std::vector<cl::Device> contextDevices = context.getInfo<CL_CONTEXT_DEVICES>();	//check device in the context

		//output names of device in the context
		for (i = 0; i < contextDevices.size(); i++) {
			outputString = contextDevices[i].getInfo<CL_DEVICE_NAME>();
			std::cout << " Device " << ": " << outputString << std::endl;
		}

		//Read program source code from task1.cl and display program build log
		// open input file stream to .cl file
		std::ifstream programFile("task1.cl");

		// check whether file was opened
		if (!programFile.is_open())
		{
			quit_program("File not found.");
		}

		// create program string and load contents from the file
		std::string programString(std::istreambuf_iterator<char>(programFile), (std::istreambuf_iterator<char>()));

		// output contents of the file
		std::cout << "--------------------" << std::endl;
		std::cout << "Contents of program string: " << std::endl;
		std::cout << programString << std::endl;
		std::cout << "--------------------" << std::endl;

		// create program source from one input string
		cl::Program::Sources source(1, std::make_pair(programString.c_str(), programString.length() + 1));
		// create program from source and context
		cl::Program program(context, source);

		// try to build program
		try {
			// build the program for the devices in the context
			program.build(contextDevices);

			std::cout << "Program build: Successful" << std::endl;
			std::cout << "--------------------" << std::endl;

			//output build log
			// output build log for all devices in context
			for (unsigned int i = 0; i < contextDevices.size(); i++)
			{
				// get device name
				outputString = contextDevices[i].getInfo<CL_DEVICE_NAME>();
				std::string build_log = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[i]);

				std::cout << "Device - " << outputString << ", build log:" << std::endl;
				std::cout << build_log << std::endl << "--------------------" << std::endl;
			}

			//Create kernels from the program and display all kernel names
			std::vector<cl::Kernel> allKernels;		// container for all kernels
			program.createKernels(&allKernels);
			kernel = cl::Kernel(program, "add");

			//Find and display num of kernels in the program
			std::cout << "Total num of Kernels: " << allKernels.size() << std::endl;

			std::cout << "--------------------" << std::endl;
			std::cout << "Kernel Names" << std::endl;

			// output kernel name for each index
			for (i = 0; i < allKernels.size(); i++) {
				outputString = allKernels[i].getInfo<CL_KERNEL_FUNCTION_NAME>();
				std::cout << "Kernel " << i << ": " << outputString << std::endl;
			}
			std::cout << "--------------------" << std::endl;

			//Command Queue
			queue = cl::CommandQueue(context, device); //create command queue using context and device

			//Create Buffers
			floatBufferA = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(cl_float) * LENGTH
				, &floatDataA[0]);
			floatBufferB = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * LENGTH, &floatDataB[0]);
			floatBufferC = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * LENGTH, &floatDataC[0]);

			// enqueue command to write from host to device memory (host -> buffer -> device)
			//transfer data from floatDataB to floatBufferB
			//queue.enqueueWriteBuffer(floatBufferB, CL_TRUE, 0, sizeof(cl_float) * LENGTH, &floatDataB[0]);

			//Set Kernel Arguments
			kernel.setArg(0, floatBufferA);
			kernel.setArg(1, floatBufferC);
			kernel.setArg(2, floatBufferB);

			// enqueue kernel for execution
			queue.enqueueTask(kernel);

			std::cout << "Kernel enqueued." << std::endl;
			std::cout << "--------------------" << std::endl;

			// enqueue command to read from device to host memory (device -> buffer -> host)
			queue.enqueueReadBuffer(floatBufferA, CL_TRUE, 0, sizeof(cl_float) * LENGTH, &floatOutput[0]);

			// output contents
			std::cout << "\nContents of floatBufferA: " << std::endl;

			for (int i = 0; i < floatOutput.size() / 10; i++) {
				for (int j = 0; j < 10; j++) {
					std::cout << floatOutput[j + i * 10] << "\t";
				}
				std::cout << std::endl;
			}

			// enqueue command to read from device to host memory (device -> buffer -> host)
			queue.enqueueReadBuffer(floatBufferB, CL_TRUE, 0, sizeof(cl_float) * LENGTH, &floatOutput[0]);

			// output contents
			std::cout << "\nContents of floatBufferB: " << std::endl;

			for (int i = 0; i < floatOutput.size() / 10; i++) {
				for (int j = 0; j < 10; j++) {
					std::cout << floatOutput[j + i * 10] << "\t";
				}
				std::cout << std::endl;
			}

			// enqueue command to read from device to host memory (device -> buffer -> host)
			queue.enqueueReadBuffer(floatBufferC, CL_TRUE, 0, sizeof(cl_float) * LENGTH, &floatOutput[0]);

			// output contents
			std::cout << "\nContents of floatBufferC: " << std::endl;

			for (int i = 0; i < floatOutput.size() / 10; i++) {
				for (int j = 0; j < 10; j++) {
					std::cout << floatOutput[j + i * 10] << "\t";
				}
				std::cout << std::endl;
			}
		}
		catch (cl::Error e) {
			// if failed to build program
			if (e.err() == CL_BUILD_PROGRAM_FAILURE)
			{
				// output program build log
				std::cout << e.what() << ": Failed to build program." << std::endl;

				// output build log for all devices in context
				for (unsigned int i = 0; i < contextDevices.size(); i++)
				{
					// get device name
					outputString = contextDevices[i].getInfo<CL_DEVICE_NAME>();
					std::string build_log = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[i]);

					std::cout << "Device - " << outputString << ", build log:" << std::endl;
					std::cout << build_log << "--------------------" << std::endl;
				}
			}
			else
			{
				throw e;
			}
		}
	}
	catch (cl::Error e) {		//catch any OpenCL function errors
		//call function to handle errors
		handle_error(e);
	}

#ifdef _WIN32
	// wait for a keypress on Windows OS before exiting
	std::cout << "\npress a key to quit...";
	std::cin.ignore();
#endif

	return 0;
}