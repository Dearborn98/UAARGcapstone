project('UAARGcapstone', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])

# Var Declaration
src = []

dep = [
  dependency('opencv4'), 
  dependency('threads'),
  dependency('boost', modules: ['log', 'thread'])
]

cpp_args = '-DBOOST_LOG_DYN_LINK'

include_dir = include_directories('src')

subdir('src')
subdir('test')

# pkg_mod = import('pkgconfig')
# pkg_mod.generate(libraries : foolib,
#                  version : '1.0',
#                  name : 'libfoobar',
#                  filebase : 'foobar',
#                  description : 'A Library to barnicate your foos.')

if get_option('enable_demo')
  subdir('demo')
  executable('capstone_demo',
            [demo_src],
            dependencies: dep,
            include_directories: include_dir,
            link_with: uaargcameralib,
            # Boost when installed by apt provides dynamic link libs
            cpp_args: cpp_args)
endif
