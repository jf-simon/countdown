efl_version = '>= 1.19.0'

proj = meson.project_name()
dir_prefix = get_option('prefix')
dir_data = join_paths(dir_prefix, get_option('datadir'))

edje = dependency('edje',       required: true,  version: efl_version)

edje_cmd = join_paths(edje.get_pkgconfig_variable('prefix'),
                      'bin', 'edje_cc')

cmd = [ edje_cmd,
        '-id', join_paths(meson.source_root(), 'data' , 'themes'),
        '-sd', join_paths(meson.source_root(), 'data' , 'themes' , 'sounds'),
        '-fd', join_paths(meson.source_root(), 'data' , 'themes' , 'fonts'),
        '@INPUT@', '@OUTPUT@'
      ]
src = 'countdown'
custom_target(src + '.edj',
              input      : src + '.edc',
              output     : src + '.edj',
              command    : cmd,
              install_dir: join_paths(dir_data, proj, 'themes'),
              install    : true
             )
meson.add_install_script('perms.sh')


install_data('sounds/bell.wav',
              install_dir: join_paths(dir_data, proj, 'sounds')
             )
             
install_data('fonts/VeraMono.ttf',
              install_dir: join_paths(dir_data, proj, 'fonts')
             )
