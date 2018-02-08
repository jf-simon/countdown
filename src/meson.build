inc = include_directories('.', '../..', '../')

dep_e = dependency('enlightenment')
deps = [
        dep_e,
        dependency('elementary')
]

src = [
  'countdown.h',
  'main.c',
  'config.c'
]

executable('countdown',
  src,
  dependencies: deps,
  install_dir: install_dir,
  c_args : ['-fPIE', '-fPIC'],
  link_args : ['-rdynamic', '-pie'],
  install: true,
  include_directories: inc
)
