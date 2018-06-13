# Build instructions:

### Dependencies:
  - Boost 1.62+

## Windows:

Windows builds made by us are available here: https://github.com/litecoinz-project/zhminer/releases

Download and install:
- [CUDA SDK](https://developer.nvidia.com/cuda-downloads) (if not needed remove **USE_CUDA_TROMP** from **zhminer** Preprocessor definitions under Properties > C/C++ > Preprocessor)
- Visual Studio 2013 Community: https://www.visualstudio.com/en-us/news/releasenotes/vs2013-community-vs
- [Visual Studio Update 5](https://www.microsoft.com/en-us/download/details.aspx?id=48129) installed
- 64 bit version only

Open **zhminer.sln** under **zhminer/zhminer.sln** and build. You will have to build ReleaseSSE2 cpu_tromp project first, then Release7.5 cuda_tromp project, then select Release and build all.

### Enabled solvers: 
  - USE_CPU_TROMP
  - USE_CUDA_TROMP

If you don't wan't to build with all solvlers you can go to **zhminer Properties > C/C++ > Preprocessor > Preprocessor Definitions** and remove the solver you don't need.

## Linux
Work in progress.
Working solvers CPU_TROMP, CUDA_TROMP

### General instructions:
  - Install CUDA SDK v8 (make sure you have cuda libraries in **LD_LIBRARY_PATH** and cuda toolkit bins in **PATH**)
    - example on Ubuntu:
    - LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/cuda-8.0/lib64:/usr/local/cuda-8.0/lib64/stubs"
    - PATH="$PATH:/usr/local/cuda-8.0/"
    - PATH="$PATH:/usr/local/cuda-8.0/bin"

  - Use Boost 1.62+ (if it is not available from the repos you will have to download and build it yourself)
  - CMake v3.5 (if it is not available from the repos you will have to download and build it yourself)
  - Currently support only static building (CUDA_TROMP is enabled by default, check **CMakeLists.txt** in **zhminer** root folder)
  - If not on Ubuntu make sure you have **fasm** installed and accessible in **PATH**
  - After that open the terminal and run the following commands:
    - `git clone https://github.com/litecoinz-project/zhminer.git`
    - `cd zhminer`
    - `mkdir build && cd build`
    - `cmake ../zhminer`
    - `make -j $(nproc)`
    
# Run instructions:

Parameters: 
	-h		Print this help and quit
	-l [location]	Stratum server:port
	-u [username]	Username (bitcoinaddress)
	-a [port]	Local API port (default: 0 = do not bind)
	-d [level]	Debug print level (0 = print all, 5 = fatal only, default: 2)
	-b [hashes]	Run in benchmark mode (default: 200 iterations)

CPU settings
	-t [num_thrds]	Number of CPU threads
	-e [ext]	Force CPU ext (0 = SSE2, 1 = AVX, 2 = AVX2)

NVIDIA CUDA settings
	-ci		CUDA info
	-cd [devices]	Enable CUDA mining on spec. devices
	-cb [blocks]	Number of blocks
	-ct [tpb]	Number of threads per block

Example: -cd 0 2 -cb 12 16 -ct 64 128

If run without parameters, miner will start mining with 75% of available logical CPU cores. Use parameter -h to learn about available parameters:

Example to run benchmark on your CPU:

        zhminer -b
        
Example to mine on your CPU with your own LTZ address and worker1 on LitecoinZ pool server:

        zhminer -l pool.litecoinz.org:3333 -u YOUR_LTZ_ADDRESS_HERE.worker1

Example to mine on your CPU with your own LTZ address and worker1 on LitecoinZ pool server, using 4 threads:

        zhminer -l pool.litecoinz.org:3333 -u YOUR_LTZ_ADDRESS_HERE.worker1 -t 4

<i>Note: if you have a 4-core CPU with hyper threading enabled (total 8 threads) it is best to run with only 4 threads (experimental benchmarks shows that best results are achieved with 50% threads utilized)</i>

Example to mine on your CPU as well on your CUDA GPUs with your own LTZ address and worker1 on LitecoinZ pool server, using 4 CPU threads and 2 CUDA GPUs:

        zhminer -l pool.litecoinz.org:3333 -u YOUR_LTZ_ADDRESS_HERE.worker1 -t 6 -cd 0 1
