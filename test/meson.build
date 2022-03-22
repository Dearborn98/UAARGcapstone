test_sources = [
    'pipeline.cpp'
]

test_dep = [ 
    dependency('gtest'),
    dependency('gtest_main'),
]

# Include sources as well
test_sources += src

# Include src dependencies as well.
test_dep += dep

e = executable(
    'test_all', test_sources, 
    dependencies: test_dep,
    include_directories: '../src',
    # Boost when installed by apt provides dynamic link libs
    cpp_args: '-DBOOST_LOG_DYN_LINK')

test('gtest', e)