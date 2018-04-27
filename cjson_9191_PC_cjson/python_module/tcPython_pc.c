#include <stdio.h>
#include <stdlib.h>
#include  <Python.h>
#include "tc_python.h"

int  downLoadSong()
{
    Py_Initialize();
    
    PyObject *pModule = NULL;  
    PyObject *pFunc   = NULL;  
    PyObject *pParam = NULL;
    PyObject *pDict = NULL;
    pParam = Py_BuildValue("ss","./python_module/music","./logSystem/R16_Runing.log");  
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./python_module')");
    pModule = PyImport_ImportModule("downHttp.downHttp_song");
    if(pModule == NULL)
    {
        printf("pModule NULL\n");
        return -1;
    }  
    pFunc  = PyObject_GetAttrString(pModule, "searchSongInfo");   
//    pDict = PyModule_GetDict(pModule);
//    pFunc  = PyDict_GetItemString(pDict, "searchSongInfo"); 
    if(pFunc == NULL)
    {
        printf("pFunc NULL\n");
        return -1;
    }    
    PyEval_CallObject(pFunc,pParam);     
    Py_Finalize();
    return 0;
}
