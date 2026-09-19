#include "../include/file_dialog.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <shobjidl.h>

std::string openFileDialog(){
    std::string filePath = "";
    // Initialize COM library for the current thread
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    if (SUCCEEDED(hr)){
        IFileOpenDialog* pFileOpen;
        // Create the dialog COM object
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
        IID_PPV_ARGS(&pFileOpen));

        if (SUCCEEDED(hr)){
            hr = pFileOpen->Show(NULL);

            if (SUCCEEDED(hr)){
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);

                if (SUCCEEDED(hr)){
                    PWSTR pszPath; 
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);

                    if (SUCCEEDED(hr)){
                        int size = WideCharToMultiByte(CP_UTF8, 0, 
                            pszPath, -1, NULL, 0, NULL, NULL
                        );
                        std::string res(size, 0); 
                        WideCharToMultiByte(CP_UTF8, 0, pszPath, -1, &res[0], size, NULL, NULL);
                        filePath = res.c_str(); 
                        CoTaskMemFree(pszPath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }

    return filePath;
}
