src += files('cameraArray.cpp', 'cameraController.cpp', 'videoStream.cpp')

subdir('utils')
subdir('imageProcessing')

uaargcameralib = library('uaargcamera', 
        src,
        install: true,
        dependencies: dep,
        # Boost when installed by apt provides dynamic link libs
        cpp_args: cpp_args)