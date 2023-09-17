project('bfgamp', 'cpp', version : '1.0.0', license : 'MIT',
        default_options : ['c_std=c11', 'cpp_std=c++11'])

cc = meson.get_compiler('cpp')

rtaudio = declare_dependency(
    dependencies : cc.find_library('rtaudio', dirs : [join_paths(meson.current_source_dir(), 'lib')]),
    include_directories : include_directories('lib'))
exe = executable('bfg', ['amp.cpp'], dependencies : [rtaudio])