project('bfgamp', 'cpp', version : '1.0.0', license : 'MIT')

cc = meson.get_compiler('cpp')

if host_machine.system() == 'linux'
    if host_machine.cpu_family () == 'x86_64'
      dep = declare_dependency(
        dependencies : cc.find_library('bass', dirs : [join_paths(meson.current_source_dir(), 'lib', 'bass', 'linux')]),
        include_directories : include_directories('lib/bass'))
      endif
elif host_machine.system() == 'windows'
    if host_machine.cpu_family () == 'x86_64'
      dep = declare_dependency(
        dependencies : cc.find_library('bass', dirs : [join_paths(meson.current_source_dir(), 'lib', 'bass', 'win64')]),
        include_directories : include_directories('lib/bass'))
      endif
endif

exe = executable('bfg', ['amp.cpp'], dependencies : [dep])
