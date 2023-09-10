project('bfgamp', 'cpp', version : '1.0.0', license : 'MIT')

cc = meson.get_compiler('cpp')

dep = declare_dependency(
  dependencies : cc.find_library('bass', dirs : ['/home/dan/src/bfgamp/lib/bass']),
  include_directories : include_directories('lib/bass'),
)

exe = executable('bfg', ['amp.cpp'], dependencies : [dep])
