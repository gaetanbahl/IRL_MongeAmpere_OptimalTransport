#ifndef __linux__
#include "stdafx.h"
#else
#include <string.h>
#endif

#include <iostream>
#include "basic.hpp"
#include "cmdparser.hpp"
#include "oclobject.hpp"
#include "utils.h"
#include <math.h>

using namespace std;

const int NPOINTS = 30;
const float XMIN = -1;
const float YMIN = -1;
const float XMAX = 1;
const float YMAX = 1;
const int NITER = 5000;
//const float DT = 0.0001;


void generateaxis(cl_float * axis) {
    for (int i = 0; i < NPOINTS; i++) {
       axis[i] = i * (XMAX - XMIN)/(NPOINTS-1) + XMIN; 
    }
}

void initubuffer(cl_float * ubuffer, float * xaxis, float * yaxis) {

    int iOffset;
    for (int i = 0; i < NPOINTS; i++) {
        for (int j = 0; j < NPOINTS; j++) {
            iOffset = j * NPOINTS;

            ubuffer[iOffset + i] = (pow(xaxis[i],2) + pow(yaxis[j],2))/2;
        }
    }
}

float ExecuteMongeAmpereKernel(cl_float* xaxis, cl_float* yaxis, cl_float* ubuffer, OpenCLBasic& ocl, OpenCLProgramOneKernel& executable)
{
    float perf_start;
    float perf_stop;

    cl_int err = CL_SUCCESS;

    cl_mem cl_xaxis =
        clCreateBuffer
        (
            ocl.context,
            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
            NPOINTS * sizeof(cl_float),
            xaxis,
            &err
        );
    SAMPLE_CHECK_ERRORS(err);
    if (cl_xaxis == (cl_mem)0)
        throw Error("Failed to create XAXIS Buffer!");
    cl_mem cl_yaxis =
        clCreateBuffer
        (
            ocl.context,
            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
            NPOINTS * sizeof(cl_float),
            yaxis,
            &err
        );
    SAMPLE_CHECK_ERRORS(err);
    if (cl_yaxis == (cl_mem)0)
        throw Error("Failed to create YAXIS Buffer!");

    cl_mem cl_ubuffer=
        clCreateBuffer
        (
            ocl.context,
            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
            NPOINTS * NPOINTS * sizeof(cl_float),
            ubuffer,
            &err
        );
    SAMPLE_CHECK_ERRORS(err);
    if (cl_yaxis == (cl_mem)0)
        throw Error("Failed to create U Buffer!");



    err = clSetKernelArg(executable.kernel, 0, sizeof(cl_mem), (void *) &cl_ubuffer);
    SAMPLE_CHECK_ERRORS(err);
    err = clSetKernelArg(executable.kernel, 1, sizeof(cl_mem), (void *) &cl_xaxis);
    SAMPLE_CHECK_ERRORS(err);
    err = clSetKernelArg(executable.kernel, 2, sizeof(cl_mem), (void *) &cl_yaxis);
    SAMPLE_CHECK_ERRORS(err);
    //err = clSetKernelArg(executable.kernel, 3, sizeof(float), DT);
    //SAMPLE_CHECK_ERRORS(err);

    size_t global_work_size[2] = { (size_t)NPOINTS, (size_t)NPOINTS};

    // execute kernel
    perf_start=time_stamp();

    cl_event event;
    for (int t = 0; t < NITER; t ++) {
    err = clEnqueueNDRangeKernel(ocl.queue, executable.kernel, 2, NULL, global_work_size, NULL, 0, NULL, &event);
    SAMPLE_CHECK_ERRORS(err);
    cout << "iteration : " << t << "\n";
    clWaitForEvents(1, &event);

    }

    err = clFinish(ocl.queue);
    SAMPLE_CHECK_ERRORS(err);
    perf_stop=time_stamp();

    void* tmp_ptr = NULL;
    tmp_ptr = clEnqueueMapBuffer(ocl.queue, cl_ubuffer, true, CL_MAP_READ, 0, sizeof(cl_float) * NPOINTS * NPOINTS, 0, NULL, NULL, NULL);
    if(tmp_ptr!=ubuffer)
    {
        throw Error("clEnqueueMapBuffer failed to return original pointer");
    }

    err = clFinish(ocl.queue);
    SAMPLE_CHECK_ERRORS(err);

    err = clEnqueueUnmapMemObject(ocl.queue, cl_ubuffer, tmp_ptr, 0, NULL, NULL);
    SAMPLE_CHECK_ERRORS(err);

    err = clReleaseMemObject(cl_xaxis);
    SAMPLE_CHECK_ERRORS(err);
    err = clReleaseMemObject(cl_yaxis);
    SAMPLE_CHECK_ERRORS(err);
    err = clReleaseMemObject(cl_ubuffer);
    SAMPLE_CHECK_ERRORS(err);

    for (int i = 0; i < NPOINTS; i++) {
        for (int j = 0; j < NPOINTS; j++) {
            cout << ubuffer[i*NPOINTS+j] << " ";
        }
        cout << "\n";
    }

    // retrieve perf. counter frequency
    return (float)(perf_stop - perf_start);
}

int main (int argc, const char** argv)
{
    //return code
    int ret = EXIT_SUCCESS;
    // pointer to the HOST buffers
    cl_float* xaxis= NULL;
    cl_float* yaxis= NULL;
    cl_float* ubuffer= NULL;

    try
    {
        // Define and parse command-line arguments.
        CmdParserCommon cmdparser(argc, argv);

        cmdparser.parse();

        // Immediatly exit if user wanted to see the usage information only.
        if(cmdparser.help.isSet())
        {
            return EXIT_SUCCESS;
        }

        int width = NPOINTS;
        int height = NPOINTS;

        // Create the necessary OpenCL objects up to device queue.
        OpenCLBasic oclobjects(
            cmdparser.platform.getValue(),
            cmdparser.device_type.getValue(),
            cmdparser.device.getValue()
        );

        // Build kernel
        OpenCLProgramOneKernel executable(oclobjects,L"MongeAmpere_Kernels.cl","","MongeAmpere");


        xaxis = (cl_float*)malloc(NPOINTS * sizeof(float));
        yaxis = (cl_float*)malloc(NPOINTS * sizeof(float));
        ubuffer = (cl_float*)malloc(sizeof(cl_float) * width * height);

        if(!(xaxis && yaxis&& ubuffer))
        {
            throw Error("Could not allocate buffers on the HOST!");
        }

        generateaxis(xaxis);
        generateaxis(yaxis);
        initubuffer(ubuffer, xaxis, yaxis);

        //printf("Executing OpenCL kernel...\n");
        float ocl_time = ExecuteMongeAmpereKernel(xaxis, yaxis, ubuffer, oclobjects, executable);
        //printf("NDRange perf. counter time %f ms.\n", ocl_time*1000);

    }
    catch(const CmdParser::Error& error)
    {
        cerr
            << "[ ERROR ] In command line: " << error.what() << "\n"
            << "Run " << argv[0] << " -h for usage info.\n";
        ret = EXIT_FAILURE;
    }
    catch(const Error& error)
    {
        cerr << "[ ERROR ] Sample application specific error: " << error.what() << "\n";
        ret = EXIT_FAILURE;
    }
    catch(const exception& error)
    {
        cerr << "[ ERROR ] " << error.what() << "\n";
        ret = EXIT_FAILURE;
    }
    catch(...)
    {
        cerr << "[ ERROR ] Unknown/internal error happened.\n";
        ret = EXIT_FAILURE;
    }

    free( ubuffer);
    free( xaxis);
    free( yaxis);

    return ret;
}

