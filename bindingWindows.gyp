{
  "variables": {
    "dll_files":["VwGigE.V7.dll"]
  },
  "targets": [
    {
      "target_name": "vieworks",
      "sources": [ "inits.cc", "vieworks.cc", "imgBuffer.cpp" ],
      "libraries": ["FreeImage.a","libVwGigE.so"],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "vendor/ubuntu/FreeImage/include",
        "vendor/ubuntu/vieworks/include"
      ],
      "copies": [{
        "destination": "build/release",
        "files": [
            "dll/TLI.V7.dll",
            "dll/TLICL.dll",
            "dll/TLICXP.dll",
            "dll/TLIGigE.dll",
            "dll/VwCXP.V7.dll",
            "dll/VwGigE.V7.dll",
            "dll/MathParser_MD_VC100_v2_4.dll",
            "dll/log4cpp_MD_VC100_v2_4.dll",
            "dll/Log_MD_VC100_v2_4.dll",
            "dll/ImageProcess.dll",
            "dll/FreeImage.dll",
            "dll/GenApi_MD_VC100_v2_4.dll",
            "dll/GCBase_MD_VC100_v2_4.dll",
            "dll/CLProtocol_MD_VC100_v2_4.dll",
            "dll/CLAllSerial_MD_VC100_v2_4.dll"
        ]
      }],
      "library_dirs": [
        "vendor/ubuntu/FreeImage/lib",
        "vendor/ubuntu/vieworks/lib"
      ]
    }
  ]
}
