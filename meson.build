project('UAARGcapstone', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])

# Var Declaration
src = []

dep = [
  dependency('opencv4'), 
  dependency('threads'),
  dependency('boost', modules: ['log'])
]

subdir('src')
subdir('test')

# Main Executable
executable('UAARGcapstone',
           [main, src],
           install : true,
           dependencies: dep,
           include_directories: ['src'],
           # Boost when installed by apt provides dynamic link libs
           cpp_args: '-DBOOST_LOG_DYN_LINK')