project('encoder', 'cpp',
  default_options: ['cpp_std=c++17'],
)

cxx = meson.get_compiler('cpp')

sndfile_dep = dependency(
  'sndfile',
  version: '>= 1.0.20',
  required: true
)

lame_dep = cxx.find_library('mp3lame')

executable('encoder',
  [
    'src/encoder.cpp',
    'src/fs.cpp',
    'src/lame_wrapper.cpp',
    'src/log.cpp',
    'src/main.cpp',
    'src/wav.cpp',
  ],
  include_directories: include_directories('include'),
  dependencies: [sndfile_dep, lame_dep],
)

doxygen = find_program('doxygen', required: false)

if doxygen.found()
  doxygen_data = configuration_data({
    'INPUT': meson.source_root(),
  })

  configure_file(
    input: 'Doxyfile.in',
    output: 'Doxyfile',
    configuration: doxygen_data,
  )

  run_target('docs', command: [doxygen, 'Doxyfile'])
endif
