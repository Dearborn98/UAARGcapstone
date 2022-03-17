project('UAARGcapstone', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])

# Var Declaration
src = []

dep = [
  dependency('opencv4'), 
  dependency('threads'),
  dependency('boost', modules: ['log', 'lockfree'])
]

subdir('src')
subdir('test')

# Main Executable
executable('UAARGcapstone',
           src,
           install : true,
           dependencies: dep,
           # Boost when installed by apt provides dynamic link libs
           cpp_args: '-DBOOST_LOG_DYN_LINK')