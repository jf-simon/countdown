inc = include_directories('.', '../..', '../')

e = dependency('enlightenment')

src = [
  'countdown.h',
  'main.c',
  'config.c'
]

executable('countdown',
  src,
  dependencies: e,
  install_dir: install_dir,
  install: true,
  include_directories: inc
)
