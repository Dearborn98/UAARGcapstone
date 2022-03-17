test_sources = [
    'sample.cpp',
    'failSample.cpp'
]

test_dep = [ 
    dependency('gtest'),
    dependency('gtest_main'),
]

# Include src dependencies as well.
test_dep += dep

e = executable(
    'test_all', test_sources, 
    dependencies: test_dep,
    include_directories: '../src')

test('gtest', e)