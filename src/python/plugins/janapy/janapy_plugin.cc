//
// Copyright 2020, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#include <thread>
#include <cstdio>
#include <chrono>
using namespace std;


#include <JANA/JApplication.h>
#include <JANA/Utils/JCpuInfo.h>
#include <JANA/Services/JParameterManager.h>

#include <janapy.h>
#include <pybind11/embed.h>


void JANA_EmbeddedPythonModuleInit(JApplication *sApp);

//static bool PY_INITIALIZED = false; // See JANA_PythonModuleInit


// This is temporary and will likely be changed once the new arrow
// system is fully adopted.
//static JApplication *pyjapp = nullptr;

//#ifndef _DBG__
//#define _DBG__ std::cout<<__FILE__<<":"<<__LINE__<<std::endl
//#define _DBG_ std::cout<<__FILE__<<":"<<__LINE__<<" "
//#endif

extern "C"{
void InitPlugin(JApplication *app){
    InitJANAPlugin(app);

    // Launch thread to handle Python interpreter.
    std::thread thr(JANA_EmbeddedPythonModuleInit, app);
    thr.detach();

    // Wait for interpreter to initialize and possibly run script.
    // This allows more control from python by stalling the initialization
    // so it has a chance to modify things a bit before full JANA
    // initialization completes and data processing starts.
    while( !PY_INITIALIZED ) std::this_thread::sleep_for (std::chrono::milliseconds(100));
}
} // "C"

//.....................................................


//================================================================================

//-------------------------------------
// JANA_EmbeddedPythonModuleInit
//-------------------------------------
void JANA_EmbeddedPythonModuleInit(JApplication *sApp)
{
    /// This will initialize the embedded Python interpreter and import the
    /// JANA module (defined by the routines in this file). This gets called
    /// when the janapy plugin gets initialized. It will then automatically
    /// look for a file whose name is given by either the JANA_PYTHON_FILE
    /// config. parameter or environment variable in that order. If neither
    /// exists, it looks for a file name "jana.py" in the current working
    /// directory and if found, executes that. If you do not wish to execute
    /// a python file, then you can set the JANA_PYTHON_FILE value to an empty
    /// string (or better yet, just not attach the janapy plugin!).
    ///
    /// Note that janapy creates a dedicated thread that the python interpreter
    /// runs in.
    ///
    /// IMPORTANT: The janapy InitPlugin routine will block (resulting in the whole
    /// JANA system pausing) until either this routine finishes or the python
    /// script it invokes calls "jana.Start()". This is to give the python script
    /// a chance to modify running conditions prior to event processing starting.
    /// If the python script intends to run throughout the life of the process,
    /// then it MUST call jana.Start() at some point. If the script is small and only
    /// runs for a short time, then you don't need to call it since it will be
    /// called automatically when the script ends.

    // Start the interpreter and keep it alive.
    // NOTE: the interpreter will stop and be deleted once we leave this routine.
    // This only happens when the python script returns so there will no longer
    // be any need for it.
    py::scoped_interpreter guard{};

    // Use existing JApplication.
    pyjapp = sApp;

    // Get name of python file to execute
    string fname = "jana.py";
    try{
        fname = pyjapp->GetParameterValue<string>("JANA_PYTHON_FILE");
    }catch(...){
        auto JANA_PYTHON_FILE = getenv("JANA_PYTHON_FILE");
        if( JANA_PYTHON_FILE ) fname = JANA_PYTHON_FILE;
    }

    // Fill in the script name in sys.argv. This is needed for tkinter which
    // is hardcoded to look at sys.argv[0].
    std::vector<wchar_t*> argv;
    auto wfname = std::wstring(fname.begin(), fname.end());
    argv.push_back( (wchar_t*)wfname.c_str() );
    PySys_SetArgv(argv.size(), argv.data());
\
    // Execute python script.
    // n.b. The script may choose to run for the lifetime of the program!
    try {
        py::eval_file(fname);
    }catch(std::runtime_error &e){
        std::cerr << std::endl;
        std::cerr << string(60, '-') << std::endl;
        std::cerr << "ERROR processing python file: \"" << fname << "\"" << std::endl;
        std::cerr << e.what() << std::endl;
        std::cerr << std::endl;
        if( string(e.what()).find("could not be opened!") != string::npos) {
            std::cerr << "Please make sure one of the following is true:" << std::endl;
            std::cerr << "  - The file \"" << fname << "\" exists" << std::endl;
            std::cerr << "  - The JANA_PYTHON_FILE config. parameter points to a file that exists" << std::endl;
            std::cerr << "  - The janapy plugin is not in the configuration for the job" << std::endl;
        }
        std::cerr << string(60, '-') << std::endl;
        std::cerr << std::endl;
        pyjapp->Quit();
    }

    PY_INITIALIZED = true;
}


//================================================================================
// Module definition
// The arguments of this structure tell Python what to call the extension,
// what it's methods are and where to look for it's method definitions.
// The routines themselves are all defined in src/python/common/janapy.h
// and src/python/common/janapy.cc.
PYBIND11_EMBEDDED_MODULE(janapy, m) {

    m.doc() = "JANA2 Embedded Python Interface";\

    // (see src/python/common/janapy.h)
    JANA_MODULE_DEF
}