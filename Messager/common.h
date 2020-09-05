/* C networking headers */

#include <signal.h>
/* end C networking headers */

/* C Headers*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* C++ headers */
#include <cstring>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <future>
#include <condition_variable>
#include <chrono>
/* end C++ headers */

//#define DBG

#ifdef DBG
#define DEBUG(x) std::cout << x << std::endl;
#else
#define DEBUG(x)
#endif