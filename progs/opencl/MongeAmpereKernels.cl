
__kernel void MongeAmpere(__global float* u, __global float* xaxis, __global float* yaxis) {

    const int i = get_global_id(0);
    const int j = get_global_id(1);

    const int N= get_global_size(0);
    const int iOffset = j * N;

    const float dx = xaxis[1] - xaxis[0];

    const float dt = 0.00001;

    if((i ==0) || (j == 0) || (i == N-1) || (j == N-1)) {
        return;
    } else {

        float Dx, Dy, Dxx, Dyy, Dxy = 0.0;
        float rhoY, rhoX = 0.0;

        float milieu = 0.0;

        Dxx = 1/(dx*dx) * (u[i-1 + (j)*N] + u[i+1 + (j)*N] - 2* u[i + (j)*N]);
        Dyy = 1/(dx*dx) * (u[i + (j+1)*N] + u[i + (j-1)*N] - 2* u[i + (j)*N]);
        Dy = 1/(2*dx) * (u[i + (j+1)*N] - u[i + (j-1)*N] );
        Dx = 1/(2*dx) * (u[i+1 + (j)*N] - u[i-1 + (j)*N] );
        Dxy = 1/(4* dx *dx) * (u[i+1 + (j+1)*N] + u[i-1 + (j-1)*N] - u[i-1 + (j+1)*N] - u[i+1 + (j-1)*N]);

        milieu = u[N/2 + N * N/2];

        rhoY =((float) exp((float)( - ((Dx-0.3)*(Dx-0.3) + (Dy+0.3)*(Dy+0.3))/0.1))) + 0.1;
        rhoX =((float)  exp((float)( - ((xaxis[i]+0.3)* (xaxis[i]+0.3)+ (yaxis[j]-0.3)*(yaxis[j]-0.3))/0.1))) + 0.1;
    
        u[i+j*N] = u[i+j*N] + dt * (Dxx * Dyy - Dxy * Dxy  - rhoX / rhoY - milieu);
    
    }

}




