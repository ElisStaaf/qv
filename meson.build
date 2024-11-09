## Minimal Meson build
## for the "Vine" text
## editor, enjoy?

project( 'vine', 'c',
  version : 'NET/1',
  license : 'Apache-2.0'
)

dirs = include_directories(
  'src',
)

source = '*.c'
bin    = 'vine'

cflags = []
cflags += [
  '-Wall', '-Wextra',
  '-pedantic', '-std=c99'
]

executable(bin, src,
  install : true,
  c_args : cflags,
  include_directories : dirs
)
