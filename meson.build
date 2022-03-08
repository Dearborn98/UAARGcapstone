project('UAARGcapstone', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])

# qt_include = include_directories('/usr/local/Qt-5.15.2/include/')
# qt_lib = static_library('/usr/local/Qt-5.15.2/lib/')
# dep = declare_dependency(link_with: qt_lib, include_directories : qt_include)

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules: ['Core', 'Gui'])


executable('UAARGcapstone',
           'camera.cpp',
	   dependencies: qt5_dep,
           install : true)
