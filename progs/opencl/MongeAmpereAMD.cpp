
// For clarity,error checking has been omitted.

#include <CL/cl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>

#define SUCCESS 0
#define FAILURE 1

using namespace std;

const int NPOINTS = 30;
const int XMIN = -1;
const int yMIN = -1;
const int XMAX = 1;
const int YMAX = 1;



/* convert the kernel file into a string */
int convertToString(const char *filename, std::string& s)
{
	size_t size;
	char*  str;
	std::fstream f(filename, (std::fstream::in | std::fstream::binary));

	if(f.is_open())
	{
		size_t fileSize;
		f.seekg(0, std::fstream::end);
		size = fileSize = (size_t)f.tellg();
		f.seekg(0, std::fstream::beg);
		str = new char[size+1];
		if(!str)
		{
			f.close();
			return 0;
		}

		f.read(str, fileSize);
		f.close();
		str[size] = '\0';
		s = str;
		delete[] str;
		return 0;
	}
	cout<<"Error: failed to open file\n:"<<filename<<endl;
	return FAILURE;
}

void generateaxis(cl_double * axis) {
    for (int i = 0; i < NPOINTS; i++) {
       axis[i] = i * (XMAX - XMIN)/(NPOINTS-1) + XMIN; 
    }
}

void checkstatus(cl_int st) {
	if (st!= CL_SUCCESS)
	{
		cout << "Error!" << endl;
	}
}

int main(int argc, char* argv[])
{

	/*Step1: Getting platforms and choose an available one.*/
	cl_uint numPlatforms;	//the NO. of platforms
	cl_platform_id platform = NULL;	//the chosen platform
	cl_int	status = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (status != CL_SUCCESS)
	{
		cout << "Error: Getting platforms!" << endl;
		return FAILURE;
	}

	/*For clarity, choose the first available platform. */
	if(numPlatforms > 0)
	{
		cl_platform_id* platforms = (cl_platform_id* )malloc(numPlatforms* sizeof(cl_platform_id));
		status = clGetPlatformIDs(numPlatforms, platforms, NULL);
		platform = platforms[0];
		free(platforms);
	}

	/*Step 2:Query the platform and choose the first GPU device if has one.Otherwise use the CPU as device.*/
	cl_uint				numDevices = 0;
	cl_device_id        *devices;
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);	
    checkstatus(status);



if (numDevices == 0)	//no GPU available.
	{
		cout << "No GPU device available." << endl;
		cout << "Choose CPU as default device." << endl;
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 0, NULL, &numDevices);	
		devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, numDevices, devices, NULL);
	}
	else
	{
		devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices, devices, NULL);
        checkstatus(status);
	}
	

	/*Step 3: Create context.*/
	cl_context context = clCreateContext(NULL,1, devices,NULL,NULL,NULL);
	
	/*Step 4: Creating command queue associate with the context.*/
	cl_command_queue commandQueue = clCreateCommandQueueWithProperties(context, devices[0], 0, NULL);

    cout << "pouet\n";
	/*Step 5: Create program object */
	const char *filename = "MongeAmpere_Kernels.cl";
	string sourceStr;
	status = convertToString(filename, sourceStr);
	const char *source = sourceStr.c_str();
	size_t sourceSize[] = {strlen(source)};
	cl_program program = clCreateProgramWithSource(context, 1, &source, sourceSize, NULL);
	
	/*Step 6: Build program. */
	status=clBuildProgram(program, 1,devices,NULL,NULL,NULL);

	/*Step 7: Initial input,output for the host and create memory objects for the kernel*/
//	const char* input = "GdkknVnqkc";
    
//	size_t strlength = strlen(input);
//	char *output = (char*) malloc(strlength + 1);

    cout << "pouet\n";
    double * xaxis = (double *) malloc(sizeof(double) * NPOINTS);
    double * yaxis = (double *) malloc(sizeof(double) * NPOINTS);

    generateaxis(xaxis);
    generateaxis(yaxis);

	cl_mem xAxis = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, NPOINTS* sizeof(cl_double),(void *) xaxis, NULL);
	cl_mem yAxis = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, NPOINTS * sizeof(cl_double),(void *) yaxis, NULL);
	cl_mem uBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, NPOINTS * NPOINTS* sizeof(cl_double), NULL, NULL);
    cout << "pouet\n";
	/*Step 8: Create kernel object */
	cl_kernel kernelident = clCreateKernel(program,"identinit", NULL);
	//cl_kernel kernelrhoX = clCreateKernel(program,"rhoX", NULL);
	//cl_kernel kernelrhoY = clCreateKernel(program,"rhoY", NULL);

	/*Step 9: Sets Kernel arguments.*/
	status = clSetKernelArg(kernelident, 0, sizeof(cl_mem), (void *)&uBuffer);
	status = clSetKernelArg(kernelident, 1, sizeof(cl_mem), (void *)&xAxis);
	status = clSetKernelArg(kernelident, 2, sizeof(cl_mem), (void *)&yAxis);
	
	/*Step 10: Running the kernel.*/
	size_t global_work_size[1] = {NPOINTS * NPOINTS};
	status = clEnqueueNDRangeKernel(commandQueue, kernelident, 1, NULL, global_work_size, NULL, 0, NULL, NULL);

	/*Step 11: Read the cout put back to host memory.*/
	//status = clEnqueueReadBuffer(commandQueue, outputBuffer, CL_TRUE, 0, strlength * sizeof(char), output, 0, NULL, NULL);
	
	/*Step 12: Clean the resources.*/
	status = clReleaseKernel(kernelident);				//Release kernel.
	status = clReleaseProgram(program);				//Release the program object.
	status = clReleaseMemObject(uBuffer);		//Release mem object.
	status = clReleaseMemObject(xAxis);
	status = clReleaseMemObject(yAxis);
	status = clReleaseCommandQueue(commandQueue);	//Release  Command queue.
	status = clReleaseContext(context);				//Release context.

    free(xaxis);
    free(yaxis);


	return SUCCESS;
}
