{
  "targets": [
    {
      "target_name": "vieworks",
      "sources": [
        "inits.cc",
        "vieworks.cc",
        "imgBuffer.cpp",
        "<!(moc vieworks.h -o moc_vieworks.cpp && echo moc_vieworks.cpp)" ,
        "<!(moc imgBuffer.h -o moc_imgBuffer.cpp && echo moc_imgBuffer.cpp)" ],
      "libraries": [
        "/usr/lib/libfreeimage.so.3",
        "/usr/lib/libVwGigE.so",
        "/usr/lib/libVwTli.so.1.1.0",
        "/opt/genicam2.3.1/bin/Linux64_x64/liblog4cpp_gcc40_v2_3.so",
        "/opt/genicam2.3.1/bin/Linux64_x64/libGenApi_gcc40_v2_3.so",
        "/opt/genicam2.3.1/bin/Linux64_x64/libGCBase_gcc40_v2_3.so",
        "/usr/lib/x86_64-linux-gnu/libQtCore.so.4.8.6"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "vendor/ubuntu/FreeImage/include",
        "/usr/include/VIS-Shadow",
        "/opt/genicam2.3.1/library/CPP/include",
        "/usr/include/qt4",
        "./"
      ],
      "library_dirs": [
        "../vendor/ubuntu/FreeImage/lib",
        "../vendor/ubuntu/vieworks/lib/x64"
      ],
      'cflags_cc!': ['-fno-rtti'],
      'cflags_cc': ['-fexceptions'],
    }
  ]
}
