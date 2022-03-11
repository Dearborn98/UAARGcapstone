project('camera-opencv', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3'])

dep = dependency('opencv4')
src = ['poc.cpp', 'cameraArray.cpp']

exe = executable('poc', src,
  install : true,
  dependencies: dep)
