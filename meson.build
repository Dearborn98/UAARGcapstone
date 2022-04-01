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

